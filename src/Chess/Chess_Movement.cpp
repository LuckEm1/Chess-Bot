#include "..\header\Game_Header.h"

//Intéragi avec les listes afin 
void board::SubMove(int loc, int new_loc)
{
	if ((-1 < loc) && (loc < 64) && (-1 < new_loc) && (new_loc < 64)) {
		//Update le board
			//Update la nouvelle case
		string[new_loc] = string[loc];
		type[new_loc] = type[loc];
		color[new_loc] = color[loc];

		//Update la vieille case
		string[loc] = ' ';
		type[loc] = 0;
		color[loc] = 0;
	}
	else {
		//Informe en cas de brèche des frontières
		Log log;
		log.Error("Piece location out of bounderies");
	}
}

void board::SubSwitch(int loc, int new_loc)
{
	int temp = type[new_loc];
	type[new_loc] = type[loc];
	type[loc] = temp;

	temp = color[new_loc];
	color[new_loc] = color[loc];
	color[loc] = temp;

	char temp2 = string[new_loc];
	string[new_loc] = string[loc];
	string[loc] = temp2;
}

bool board::Cavalier(int loc, int new_loc)
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

bool board::CheckUpDown(int loc, int new_loc)
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

bool board::CheckLeftRight(int loc, int new_loc)
{
	if ((loc - (loc % 8) - 1 < new_loc) && (loc - (loc % 8) + 8 > new_loc)) //vérifie si une brèche des frontières a lieu
	{
		if (new_loc > loc) //Si loc est à la gauche de new_loc et que les frontières sont respectées
			for (int x = loc+1; x != new_loc; x++) //x est la différence entre loc et new_loc
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

bool board::Diagonal(int loc, int new_loc)
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

char string_type(int type, int couleur)
{
	char pieces_n[7] = {' ' , 'p' , 't' , 'c' , 'f' , 'r' , 'k' };
	char pieces_b[7] = { ' ' , 'P' , 'T' , 'C' , 'F' , 'R' , 'K' };
	if (couleur == 1)
		return pieces_b[type];
	else if (couleur == 2)
		return pieces_n[type];
	Log log;
	log.Error("string_type => mauvaise couleur");
	return ' ';
}

//Vérifie si le déplacement d'une pièce de loc à new_loc est possible
int board::Move(int loc, int new_loc, bool error)
{
	int type_loc = type[loc], new_loc_color = color[new_loc], loc_color = color[loc];
	Log log;

	//TOUR - ROI
	if (type_loc == 2 && loc_color == new_loc_color) {
		bool tour_roi_noir = new_loc == 4 && color[4] == 2 && loc_color == 2 && type[4] == 6 && ((loc == 0  && type[1] == 0 && type[2] == 0 && type[3] == 0) || (loc == 7 && type[6] == 0 && type[5] == 0));
		bool tour_roi_blanc = new_loc == 60 && color[60] == 1 && loc_color == 1 && type[60] == 6 && ((loc == 56 && type[57] == 0 && type[58] == 0 && type[59] == 0) || (loc == 63 && type[61] == 0 && type[62] == 0));
		if (tour_roi_noir || tour_roi_blanc)
			return 2;
	}

	else if ((type_loc == 0) || (loc == new_loc) || (color[loc] == color[new_loc])) { return 0; }

	//PION
	else if (type_loc == 1) {
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
				if ((0<=new_loc) && (new_loc<=7))
				{
					log.Print("1 = tour ; 2 = cavalier ; 3 = fou ; 4 - reine");
					int new_type = 0;
					while ((new_type < 1) || (new_type>4))
						new_type = int_cin("Choisit le type que vas prendre le pion en "+block[loc]);
					string[loc] = string_type(new_type + 1, 1);
					type[loc] = new_type + 1;
				}
				return 1;
			}
		}
		//Vérifie la couleur du pion (	BLACK    )
		else if ((loc_color == 2) && (new_loc_color != loc_color)) {
			//Vérifie si le mouvement demandé est valide
			if (act_pion_avance_b || act_pion_gauche_b || act_pion_droite_b || start_b)		//Si OUI
			{
				if ((56 <= new_loc) && (new_loc <= 63))
				{
					log.Print("1 = tour ; 2 = cavalier ; 3 = fou ; 4 - reine");
					int new_type = 0;
					while ((new_type < 1) || (new_type > 4))
						new_type = int_cin("Choisit le type que vas prendre le pion en " + block[loc]);
					string[loc] = string_type(new_type + 1, 2);
					type[loc] = new_type + 1;
				}
				return 1;
			}
		}
	}

	//TOUR
	else if (type_loc == 2) {
		//Vérifie si le mouvement demandé est valide
		if (CheckLeftRight(loc, new_loc) || CheckUpDown(loc, new_loc))		//Si OUI
			return 1;
	}

	//CAVALIER
	else if (type_loc == 3) {
		//Vérifie si le mouvement demandé est valide
		if (Cavalier(loc, new_loc))		//Si OUI
			return 1;
	}

	//FOU
	else if (type_loc == 4) {
		//Vérifie si le mouvement demandé est valide
		if (Diagonal(loc, new_loc))		//Si OUI
			return 1;
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
			return 1;
	}

	//REINE
	else if (type_loc == 5) {
		//Vérifie si le mouvement demandé est valide
		if (CheckLeftRight(loc, new_loc) || CheckUpDown(loc, new_loc) || Diagonal(loc, new_loc))		//Si OUI
			return 1;
	}

	if (error) { log.Print("Move impossible"); }
	return 0;
}
