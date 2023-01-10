#include "..\header\Game_Header.h"
#include "..\header\Node_Header.h"
#include <tuple>

Simulation::Simulation(int old_type[64], int old_color[64], float old_value, int color_step)
{
	change_from = 0;
	change_to = 0;
	for (int i = 0; i < 64; i++)
	{
		type[i] = old_type[i];
		color[i] = old_color[i];
	}
	value = old_value;
	couleur = color_step;
}

Simulation Simulation::SubMove(int loc, int new_loc, int color_step, float move_value)
{
	Simulation obj(type, color, move_value, color_step);

	obj.Alpha = Alpha;
	obj.Beta = Beta;

	obj.change_from = loc;
	obj.change_to = new_loc;

	//Update la nouvelle case
	obj.type[new_loc] = obj.type[loc];
	obj.color[new_loc] = obj.color[loc];

	//Update la vieille case
	obj.type[loc] = 0;
	obj.color[loc] = 0;

	return obj;
}
Simulation Simulation::SubSwitch(int loc, int new_loc, int color_step, float move_value)
{
	Simulation obj(type, color, move_value, color_step);

	obj.change_from = loc;
	obj.change_to = new_loc;

	obj.Alpha = Alpha;
	obj.Beta = Beta;

	//Update la nouvelle case
	int temp = obj.type[new_loc];
	obj.type[new_loc] = obj.type[loc];
	obj.type[loc] = temp;

	temp = obj.color[new_loc];
	obj.color[new_loc] = obj.color[loc];
	obj.color[loc] = temp;

	return obj;
}


// commencant en haut
float strategic_value(int type, int newLoc, bool flipTable = false) {
	static float king[64] = { 
		2,  3,  1,  0,  0,  1,  3,  2,
		2,  2,  0,  0,  0,  0,  2,  2,
	   -1, -2, -2, -2, -2, -2, -2, -1,
	   -2, -3, -3, -4, -4, -3, -3, -2,
	   -3, -4, -4, -5, -5, -4, -4, -3,
	   -3, -4, -4, -5, -5, -4, -4, -3,
	   -3, -4, -4, -5, -5, -4, -4, -3,
	   -3, -4, -4, -5, -5, -4, -4, -3
	};

	static float queen[64] = {
		-2, -1, -1, -.5, -.5, -1, -1, -2,
		-1,  0, .5, 0, 0, 0, 0, -1,
		-1, .5, 0.5, 0.5, 0.5, 0.5, 0, -1,
		0, 0, .5, .5, .5, .5, 0, -.5,
		-.5, 0, .5,.5,.5,.5,0, -.5,
		-1, 0, .5, .5, .5, .5, 0, -1,
		-1,  0,  0,   0,   0,  0,  0, -1,
		-2, -1, -1, -.5, -.5, -1, -1, -2
	};

	static float rook[64] = {
		0, 0, 0, .5, .5, 0, 0, 0,
		-.5, 0, 0, 0, 0, 0, 0, -.5,
		-.5, 0, 0, 0, 0, 0, 0, -.5,
		-.5, 0, 0, 0, 0, 0, 0, -.5,
		-.5, 0, 0, 0, 0, 0, 0, -.5,
		-.5, 0, 0, 0, 0, 0, 0, -.5,
		.5, 1, 1, 1, 1, 1, 1, .5, 
		0, 0, 0, 0, 0, 0, 0, 0
	};

	static float pawn[64] = {
		0, 0, 0, 0, 0, 0, 0, 0,
		.5, 1, 1, -2, -2, 1, 1, .5,
		.5, -.5, -1, 0, 0, -1, -.5, .5,
		0, 0, 0, 2, 2, 0, 0, 0, 
		.5, .5, 1, 2.5, 2.5, 1, .5, .5,
		1, 1, 2, 3, 3, 2, 1, 1,
		.5, .5, .5, .5, .5, .5, .5, .5,
		0, 0, 0, 0, 0, 0, 0, 0
	};

	static float knight[64] = {
		-5, -4, -3, -3, -3, -3, -4, -5,
		-4, -2, 0, .5, .5, 0, -2, -4,
		-3, .5, 1, 1.5, 1.5, 1, .5, -3,
		-3, 0, 1.5, 2, 2, 1.5, 0, -3,
		-3, .5, 1.5, 2, 2, 1.5, .5, -3,
		-3, 0, 1, 1.5, 1.5, 1, 0, -3,
		-4, -2, 0, 0, 0, 0, -2, -4,
		-5, -4, -3, -3, -3, -3, -4, -5
	};

	static float bishop[64] = {
		-2, -1, -1, -1, -1, -1, -1, -2,
		-1, .5, 0, 0, 0, 0, .5, -1,
		-1, 1, 1, 1, 1, 1, 1, -1,
		-1, 0, 1, 1, 1, 1, 0, -1,
		-1, .5, .5, 1, 1, .5, .5, -1,
		-1, 0, .5, 1, 1, .5, 0, -1,
		-1, 0, 0, 0, 0, 0, 0, -1,
		-2, -1, -1, -1, -1, -1, -1, -2
	};

	float value = 0;


	if (flipTable) {
		int mirror = 63 - newLoc;
		newLoc = mirror - 2 * (mirror % 8) + 7;
	}

	if (type == 1)
		value = pawn[newLoc];
	else if (type == 2)
		value = rook[newLoc];
	else if (type == 3)
		value = knight[newLoc];
	else if (type == 4)
		value = bishop[newLoc];
	else if (type == 5)
		value = queen[newLoc];
	else if (type == 6)
		value = king[newLoc];

	return value;
}

std::string get_str(Simulation& sim, int origin)
{
	std::string str_noir[7] = {"p", "t", "c", "f", "r" , "k"};
	std::string str_blanc[7] = {"P", "T", "C", "F", "R" , "K"};
	if (!sim.color[origin])
		return " ";
	if (sim.color[origin] == 1)
		return str_blanc[sim.type[origin] - 1];
	return str_noir[sim.type[origin] - 1];
}

void present_simulation(Simulation sim)
{
	board board;
	Log log;
	log.Print(board.top);
	for (int x = 0; x < 8; x++)
	{
		log.Print("    " + its(8 - x) + "    |   " + get_str(sim, x * 8) + "   |   " + get_str(sim, x * 8 + 1) + "   |   " + get_str(sim, x * 8 + 2) + "   |   " + get_str(sim, x * 8 + 3) + "   |   " + get_str(sim, x * 8 + 4) + "   |   " + get_str(sim, x * 8 + 5) + "   |   " + get_str(sim, x * 8 + 6) + "   |   " + get_str(sim, x * 8 + 7) + "   |");
		if (x != 7)
			log.Print(board.mid);
	}
	log.Print(board.bot);
}

bool kingLives(Simulation sim, int color)
{
	int king_loc = -1;
	for (int origin = 0; origin < 64; origin++) //Traverse le board
		if ((sim.color[origin] == color) && (sim.type[origin] == 6))
			king_loc = origin;

	int_node* threat = new int_node, * threat_head = threat;
	bool reponse = true;
	int possible_move, add_value;
	for (int_node* move = danger_zone(king_loc); move->next != nullptr; move = move->next) //Traverse le board
		if (sim.color[move->data] == color % 2 + 1) //Fait alterner les couleures
		{
			std::tie(possible_move, add_value) = sim.Move(move->data, king_loc, false);
			if (possible_move)
				return false;
		}

	return true;
}

bool King_Safety(Simulation sim, bool player_turn, int color, int change_from, int change_to, int movement_type)
{
	Simulation obj(sim.type, sim.color, 0, 0); 

	if (movement_type == 1)
		obj = obj.SubMove(change_from, change_to ,0 ,0);
	else if (movement_type == 2)
		obj = obj.SubSwitch(change_from, change_to, 0, 0); 
	else if (movement_type == 3)
	{
		obj = obj.SubMove(change_from, change_to, 0, 0);
		obj.type[change_to] = 5;
	}

	int king_loc;
	for (int origin = 0; origin < 64; origin++) //Traverse le board
		if ((obj.color[origin]== color) && (obj.type[origin]==6))
			king_loc = origin;

	int_node* threat = new int_node, *threat_head = threat;
	bool reponse = true;
	int possible_move, add_value;
	for (int_node* move = danger_zone(king_loc); move->next != nullptr; move = move->next) //Traverse le board
		if (obj.color[move->data] == color % 2 + 1) //Fait alterner les couleures
		{
			std::tie(possible_move, add_value) = obj.Move(move->data, king_loc, false);
			if (possible_move)
			{
				threat = add_node_int(move->data, threat);
				reponse = false;
			}
		}
	if (player_turn && (!reponse))
	{
		Log log;
		board board;
		log.Print("Vous ne pouvez faire cela, sinon vous serez en échec par:");
		for (int_node* threat = threat_head; threat -> next != nullptr; threat = threat -> next)
			log.Print(board.block[threat->data]);
	}
	return reponse;
}

int pawn_value(Simulation sim, int origin)
{
	//vide  pion 10, tour50, cavalier30, fou30, roi900, reine90
	int value[7] = {0, 10, 50, 30, 30, 90, 900 };
	int moveValue = value[sim.type[origin]];
	//positif si le pion mangé est de l'autre couleur, donc target positif
	return sim.color[origin] != sim.ai_color ? moveValue : -moveValue;
}

//CONFIRME MOUVEMENTS
bool Simulation::Cavalier(int loc, int new_loc)
{
	int diff = new_loc - loc;
	bool mod_loc_0 = loc % 8 != 0, mod_loc_1 = (loc + 1) % 8 != 0, mod_loc_2 = (loc + 2) % 8 != 0, mod_loc_m1 = (loc - 1) % 8 != 0;
	bool loc_sup_15 = loc > 15, loc_sup_7 = loc > 7, loc_inf_56 = loc < 56, loc_inf_48 = loc < 48;

	bool haut_droit = (diff == -15) && mod_loc_1 && loc_sup_15;
	bool haut_gauche = (diff == -17) && mod_loc_0 && loc_sup_15;
	bool droite_haut = (diff == -6) && mod_loc_1 && mod_loc_2 && loc_sup_7;
	bool droite_bas = (diff == 10) && mod_loc_1 && mod_loc_2 && loc_inf_56;
	bool gauche_haut = (diff == -10) && mod_loc_0 && mod_loc_m1 && loc_sup_7;
	bool gauche_bas = (diff == 6) && mod_loc_0 && (mod_loc_m1) && loc_inf_56;
	bool bas_droite = (diff == 17) && mod_loc_1 && loc_inf_48;
	bool bas_gauche = (diff == 15) && mod_loc_0 && loc_inf_48;
	if ((haut_droit || haut_gauche || droite_haut || droite_bas || gauche_haut || gauche_bas || bas_droite || bas_gauche) && (color[new_loc] != color[loc]))
		return 1;
	return 0;
}
bool Simulation::CheckUpDown(int loc, int new_loc)
{
	if ((loc - new_loc) % 8 == 0)
	{
		if (new_loc - loc > 0) //Si loc est en haut de new_loc et que les frontières sont respectées
			for (int x = 1; x < int((new_loc - loc) / 8); x++) //x est la différence entre loc et new_loc
			{
				if (color[loc + x * 8] == 0) //S'assure que le chemin entre loc et new_loc est vide
					continue;
				else return false;
			}

		else if (new_loc - loc < 0) //Si loc est en haut de new_loc et que les frontières sont respectées
			for (int x = 1; x < int((loc - new_loc) / 8); x++) //x est la différence entre loc et new_loc
			{
				if (color[loc - x * 8] == 0) //S'assure que le chemin entre loc et new_loc est vide
					continue;
				else return false;
			}

		else return false;

		return true;
	}
	return false;
}
bool Simulation::CheckLeftRight(int loc, int new_loc)
{
	if ((loc - (loc % 8) - 1 < new_loc) && (loc - (loc % 8) + 8 > new_loc)) //vérifie si une brèche des frontières a lieu
	{
		if (new_loc > loc) //Si loc est à la gauche de new_loc et que les frontières sont respectées
			for (int x = loc + 1; x != new_loc; x++) //x est la différence entre loc et new_loc
			{
				if (color[x] == 0)
					continue;
				else return false;
			}

		else if (new_loc < loc) //Si loc est à la droite de new_loc et que les frontières sont respectées
			for (int x = new_loc + 1; x < loc; x++) //x est la différence entre loc et new_loc 
			{
				if (color[x] == 0)
					continue;
				else return false;
			}

		else return false;

		return true;
	}
	return false;
}
bool Simulation::Diagonal(int loc, int new_loc)
{
	if (loc - new_loc > 0)			//Haut
	{
		if ((loc - new_loc) % 7 == 0 && (new_loc % 8 > loc % 8))			//Droite
			for (int step = loc - 7; (step >= 7) && (step % 8) && (step >= new_loc); step -= 7)
			{
				if ((color[step] == 0) || (step == new_loc))
					continue;
				else return false;
			}

		else if ((loc - new_loc) % 9 == 0 && (new_loc % 8 < loc % 8))		//Gauche
			for (int step = loc - 9; (step >= 9) && ((step + 9) % 8) && (step >= new_loc); step -= 9)
			{
				if ((color[step] == 0) || (step == new_loc))
					continue;
				else return false;
			}

		else return false;
	}

	else if (loc - new_loc < 0)		 //Bas
	{
		if (((new_loc - loc) % 7 == 0) && (new_loc % 8 < loc % 8))			//Gauche
			for (int step = loc + 7; (step <= 56) && ((step - 7) % 8) && (step <= new_loc); step += 7)
			{
				if ((color[step] == 0) || (step == (new_loc)))
					continue;
				else return false;
			}

		else if (((new_loc - loc) % 9 == 0) && (new_loc % 8 > loc % 8))		//Droite
			for (int step = loc + 9; (step <= 54) && (step % 8) && (step <= new_loc); step += 9)
			{
				if ((color[step] == 0) || (step == new_loc))
					continue;
				else return false;
			}

		else return false;
	}
	else return false;

	return true;
}

//Vérifie si le déplacement d'une pièce de loc à new_loc est possible
std::tuple<int, int> Simulation::Move(int loc, int new_loc, bool error)
{
	int type_loc = type[loc], new_loc_color = color[new_loc], loc_color = color[loc];

	//TOUR - ROI
	if (type_loc == 2 && loc_color == new_loc_color) {
		bool tour_roi_noir = new_loc == 4 && color[4] == 2 && loc_color == 2 && type[4] == 6 && ((loc == 0 && type[1] == 0 && type[2] == 0 && type[3] == 0) || (loc == 7 && type[6] == 0 && type[5] == 0));
		bool tour_roi_blanc = new_loc == 60 && color[60] == 1 && loc_color == 1 && type[60] == 6 && ((loc == 56 && type[57] == 0 && type[58] == 0 && type[59] == 0) || (loc == 63 && type[61] == 0 && type[62] == 0));
		if (tour_roi_noir || tour_roi_blanc)
			return { 2 , 0 };
	}

	if ((type_loc == 0) || (new_loc_color == loc_color) || (new_loc==loc)) { return { 0, 0 }; }

	//PION
	if (type_loc == 1) {
		bool new_loc_type_null = (type[new_loc] == 0), new_loc_white = (new_loc_color == 1), new_loc_black = (new_loc_color == 2);
		bool mod_loc_0 = loc % 8 != 0, mod_loc_1 = (loc + 1) % 8 != 0;

		bool act_pion_avance_w = (new_loc == loc - 8) && new_loc_type_null;
		bool act_pion_avance_b = (new_loc == loc + 8) && new_loc_type_null;
		bool act_pion_gauche_w = ((new_loc == loc - 9) && new_loc_black && mod_loc_0);
		bool act_pion_gauche_b = ((new_loc == loc + 7) && (new_loc_white) && mod_loc_0);
		bool act_pion_droite_w = ((new_loc == loc - 7) && new_loc_black && mod_loc_1);
		bool act_pion_droite_b = ((new_loc == loc + 9) && (new_loc_white) && mod_loc_1);
		bool start_b = (new_loc == loc + 16) && new_loc_type_null && (type[new_loc - 8] == 0) && ((7 < loc) && (loc < 16));
		bool start_w = (new_loc == loc - 16) && new_loc_type_null && (type[new_loc + 8] == 0) && ((47 < loc) && (loc < 56));
		//Vérifie la couleur du pion  (	WHITE    )
		if (loc_color == 1) {
			//Vérifie si le mouvement demandé est valide
			if (act_pion_avance_w || act_pion_gauche_w || act_pion_droite_w || start_w)		//Si OUI
			{
				if ((0 <= new_loc) && (new_loc <= 7))
				{
					if (color[loc] == ai_color)
						return { 3, 80 };
					return { 3, -80 };
				}
					
				return { 1, pawn_value(*this, new_loc) };
			}
		}
		//Vérifie la couleur du pion (	BLACK    )
		else if ((loc_color == 2) && (new_loc_color != loc_color)) {
			//Vérifie si le mouvement demandé est valide
			if (act_pion_avance_b || act_pion_gauche_b || act_pion_droite_b || start_b)		//Si OUI
			{
				if ((56 <= new_loc) && (new_loc <= 63))
					return { 3, 80 };
				return { 1, pawn_value(*this, new_loc) };
			}
		}
	}

	//TOUR
	else if (type_loc == 2) {
		if (CheckLeftRight(loc, new_loc) || CheckUpDown(loc, new_loc))		//Si OUI
			return { 1, pawn_value(*this, new_loc) };
	}

	//CAVALIER
	else if (type_loc == 3) {
		//Vérifie si le mouvement demandé est valide
		if (Cavalier(loc, new_loc))		//Si OUI
			return { 1, pawn_value(*this, new_loc) };
	}

	//FOU
	else if (type_loc == 4) {
		//Vérifie si le mouvement demandé est valide
		if (Diagonal(loc, new_loc))		//Si OUI
			return { 1, pawn_value(*this, new_loc) };
	}

	//ROI
	else if (type_loc == 6) {
		bool recule = new_loc == loc - 8;
		bool avance = new_loc == loc + 8;
		bool haut_gauche = ((new_loc == loc - 9) && (loc % 8 != 0));
		bool bas_droite = ((new_loc == loc + 9) && ((loc + 1) % 8 != 0));
		bool bas_gauche = ((new_loc == loc - 7) && ((loc + 1) % 8 != 0));
		bool haut_droit = ((new_loc == loc + 7) && (loc % 8 != 0));
		bool gauche = new_loc == loc - 1;
		bool droite = new_loc == loc + 1;
		//Vérifie si le mouvement demandé est valide
		if (recule || avance || haut_gauche || bas_droite || bas_gauche || haut_droit || gauche || droite)		//Si OUI
			return { 1, pawn_value(*this, new_loc) };
	}
	
	//REINE
	else if (type_loc == 5) {
		//Vérifie si le mouvement demandé est valide
		if (CheckLeftRight(loc, new_loc) || CheckUpDown(loc, new_loc) || Diagonal(loc, new_loc))		//Si OUI
			return { 1, pawn_value(*this, new_loc) };
	}
	return { 0, 0 };
}


float get_max_value(moves_node* node)
{
	float max = node->value;
	for (moves_node* mobile_node = node->next; mobile_node->next != nullptr; mobile_node = mobile_node->next)
		if (mobile_node->value > max)
			max = mobile_node->value;
	return max; 
}
float get_min_value(moves_node* node)
{
	float min = node->value;
	for (moves_node* mobile_node = node->next; mobile_node->next != nullptr; mobile_node = mobile_node->next)
		if (mobile_node->value < min)
			min = mobile_node->value;
	return min;
}
/*
Simulation Simulation::Get_Best_Move_ab(int profondeur, int pre_cf, int pre_ct, int_node* danger_zones, int ab_pruning)
{
	int possible_move;
	int add_value;
	bool first = true;
	moves_node* move_node = new moves_node, * move_head = move_node;
	moves_node* temp_end = nullptr;
	final_move_node* final_set = new final_move_node, * final_head = final_set;


	//cherche toutes les possibilitées de mouvement et les mets dans des nodes avec la tête étant <head>
	for (int origin = 0; origin < 64; origin++) //Traverse le board
		if (color[origin] == 2 - (couleur % 2)) //Fait alterner les couleures
			for (int_node* move = danger_zones + origin; move->next != nullptr; move = move->next) //Renvoi toutes les position possibles de déplacement du pion sur <origine>
			{
				//Vérifie si le déplacement demandé est possible et enregistre le changement de valeur associé aux points
				std::tie(possible_move, add_value) = Move(origin, move->data, false);

				//if (King_Safety(*this, false, ai_color, origin, move->data, possible_move, danger_zones) & (bool)possible_move)
				//{
				if (possible_move == 1) //déplacement normal
					{
						if (profondeur == 1)
							move_node = add_moves_node(move_node, value + add_value);
						else if (profondeur == 5 && King_Safety(*this, false, ai_color, origin, move->data, 1))
							final_set = add_final_move_node(final_set, SubMove(origin, move->data, couleur + 1, add_value).Get_Best_Move_ab(profondeur - 1, NULL, NULL, danger_zones, ab_pruning).value, origin, move->data);
						else
							move_node = add_moves_node(move_node, SubMove(origin, move->data, couleur + 1, value + add_value).Get_Best_Move_ab(profondeur - 1, NULL, NULL, danger_zones, ab_pruning).value);
					}
				else if (possible_move == 2)
					{
						if (profondeur == 1) //échange roi tourre
							move_node = add_moves_node(move_node, value + add_value);
						else if (profondeur == 5 && King_Safety(*this, false, ai_color, origin, move->data, 2))
							final_set = add_final_move_node(final_set, SubSwitch(origin, move->data, couleur + 1, add_value).Get_Best_Move_ab(profondeur - 1, NULL, NULL, danger_zones, ab_pruning).value, origin, move->data);
						else
							move_node = add_moves_node(move_node, SubSwitch(origin, move->data, couleur + 1, value + add_value).Get_Best_Move_ab(profondeur - 1, NULL, NULL, danger_zones, ab_pruning).value);
					}
				else if (possible_move == 3) //promotion d'un pion
					{
						if (profondeur == 1)
							move_node = add_moves_node(move_node, value + add_value + 80);
						else if (profondeur == 5 && King_Safety(*this, false, ai_color, origin, move->data, 3))
						{
							type[origin] = 5;
							final_set = add_final_move_node(final_set, SubMove(origin, move->data, couleur + 1, add_value).Get_Best_Move_ab(profondeur - 1, NULL, NULL, danger_zones, ab_pruning).value, origin, move->data);
							type[origin] = 1;
						}
						else
						{
							type[origin] = 5;
							move_node = add_moves_node(move_node, SubMove(origin, move->data, couleur + 1, value + add_value).Get_Best_Move_ab(profondeur - 1, NULL, NULL, danger_zones, ab_pruning).value);
							type[origin] = 1;
						}
					}
				//}
			}
	//Trouve le meilleur chemin possible en comparant leurs maximums ainsi que le nombre de ceux-ci
	if (profondeur == 5)
	{
		Log log;
		final_move_node* final = final_head;

		for (final_move_node* test = final_head; test->change_to==pre_cf ; test = test->next)
			final_move_node* final = test;
		
		if (final->change_from==NULL && final->change_to == NULL)
		{
			log.Print("ECHEC ET MATH");
			std::tie(change_from, change_to) = get_random_move(*this, ai_color, danger_zones);
		}
		else
		{
			int total = 0;
			int max = final->value;
			for (final_move_node* node = final->next; node->next != nullptr; node = node->next)
			{
				total++;
				if ((node->value > max) && (node->change_to != pre_cf))
				{
					max = node->value;
					final = node;
				}
			}
			log.Int((int)pres_king_safety(*this, ai_color));
			if (!max && pres_king_safety(*this, ai_color))
				std::tie(change_from, change_to) = get_random_move(*this, ai_color, danger_zones);
			else
			{
				change_from = final->change_from;
				change_to = final->change_to;
			}
		}
	}
	//Ajoute l'ensemble final à l'objet 
	else if ((profondeur == 1) || !(couleur % 2))
	{
		value = get_max_info(move_head); 
		//_ab_pruning = value;
	}
	else //si il n'est pas noir
	{
		value = get_min_info(move_head);
		//_ab_pruning = -value;
	}
	return *this;
}*/


bool UpdateAlpha(float nbr, float& alpha) {
	bool greater = nbr > alpha;
	if (greater)
		alpha = nbr;
	return greater;
}

bool UpdateBeta(float nbr, float& beta) {
	bool smaller = nbr < beta;
	if (smaller)
		beta = nbr;
	return smaller;
}

bool Simulation::updatedValues(int profondeur, float nbr, Simulation * sim) {
	bool update = false;
	if (profondeur == 1)
		return ai_color != 2 - (couleur % 2) ? UpdateBeta(nbr, Beta) : UpdateAlpha(nbr, Alpha);
	return ai_color != 2 - (couleur % 2) ? UpdateBeta(std::min(sim->Alpha, sim->Beta), Beta) : UpdateAlpha(std::max(sim->Alpha, sim->Beta), Alpha);
}




Simulation* Simulation::Get_Best_Move(int profondeur, int pre_cf, int pre_ct, int_node* danger_zones, int& depth,  unsigned long long int& count)
{
	int possible_move;
	float add_value;
	bool pruned = false;
	count += 1;


	//cherche toutes les possibilitées de mouvement et les mets dans des nodes avec la tête étant <head>
	for (int origin = 0; origin < 64 && !pruned; origin++) //Traverse le board
		if (color[origin] == 2 - (couleur % 2)) //Fait alterner les couleures
			for (int_node* move = danger_zones + origin; move->next != nullptr && !pruned; move = move->next) //Renvoi toutes les position possibles de déplacement du pion sur <origine>
			{
				//Vérifie si le déplacement demandé est possible et enregistre le changement de valeur associé aux points
				std::tie(possible_move, add_value) = Move(origin, move->data, false);
				float strategicValue = strategic_value(type[origin], move->data);
				add_value += color[origin] == ai_color ? strategicValue : -strategicValue;
				Simulation* simPtr = nullptr;

				//if (King_Safety(*this, false, ai_color, origin, move->data, possible_move, danger_zones) & (bool)possible_move)
				//{
				if (possible_move == 1) //déplacement normal
				{
					if (profondeur == depth && King_Safety(*this, false, ai_color, origin, move->data, 1))
						simPtr = SubMove(origin, move->data, couleur + 1, add_value).Get_Best_Move(profondeur - 1, NULL, NULL, danger_zones, depth, count);
					else if (profondeur != 1)
						simPtr = SubMove(origin, move->data, couleur + 1, value + add_value).Get_Best_Move(profondeur - 1, NULL, NULL, danger_zones, depth, count);
				}
				else if (possible_move == 2)
				{
					if (profondeur == depth && King_Safety(*this, false, ai_color, origin, move->data, 2))
						simPtr = SubSwitch(origin, move->data, couleur + 1, add_value).Get_Best_Move(profondeur - 1, NULL, NULL, danger_zones, depth, count);
					else if (profondeur != 1)
						simPtr = SubSwitch(origin, move->data, couleur + 1, value + add_value).Get_Best_Move(profondeur - 1, NULL, NULL, danger_zones, depth, count);
				}
				else if (possible_move == 3) //promotion d'un pion
				{
					type[origin] = 5;
					if (profondeur == depth && King_Safety(*this, false, ai_color, origin, move->data, 3))
						simPtr = SubMove(origin, move->data, couleur + 1, add_value).Get_Best_Move(profondeur - 1, NULL, NULL, danger_zones, depth, count);
					else if (profondeur != 1)
						simPtr = SubMove(origin, move->data, couleur + 1, value + add_value).Get_Best_Move(profondeur - 1, NULL, NULL, danger_zones, depth, count);
					type[origin] = 1;
				}

				if (possible_move != 0 )
				{
					if (updatedValues(profondeur, value + add_value, simPtr)) {
						if (Alpha >= Beta)
							pruned = true;
						if (profondeur == depth && simPtr != nullptr)
						{
							change_from = simPtr->change_from;
							change_to = simPtr->change_to;
						}
					}
				}
				//}
			}
	//Trouve le meilleur chemin possible en comparant leurs maximums ainsi que le nombre de ceux-ci
	//if (profondeur == 5)
		/*{
		Log log;
		final_move_node* final = final_head;

		for (final_move_node* test = final_head; test->change_to == pre_cf; test = test->next)
			final_move_node* final = test;

		if (final->change_from == NULL && final->change_to == NULL)
		{
			log.Print("ECHEC ET MATH");
			std::tie(change_from, change_to) = get_random_move(*this, ai_color, danger_zones);
		}
		else
		{
			int total = 0;
			float max = final->value;
			for (final_move_node* node = final->next; node->next != nullptr; node = node->next)
			{
				total++;
				if ((node->value > max) && (node->change_to != pre_cf))
				{
					max = node->value;
					final = node;
				}
			}
			log.Int((int)pres_king_safety(*this, ai_color));
			if (!max && pres_king_safety(*this, ai_color))
				std::tie(change_from, change_to) = get_random_move(*this, ai_color, danger_zones);
			else
			{
				change_from = final->change_from;
				change_to = final->change_to;
			}
		}
	}*/
	return this;
}


