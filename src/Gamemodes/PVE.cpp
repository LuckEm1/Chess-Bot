#include "..\header\Game_Header.h"
#include "..\header\Node_Header.h"
#include <tuple>


void Act_ai(Simulation& sim, board& board, Log& log, bool tour, int change_from, int change_to, bool error, int_node* danger_zones) {
	std::string color_error_str;

	//Indique c'est au tour de qui et si une erreur de couleur est en jeu
	if (error) color_error_str = "[MAUVAISE COULEURE]: ";

	//Intéragis avec l'utilisateur afin de pouvoir agir sur le board avec les information recues (et vérifiées dans le board)
	int Ancienne_Case = board.Location(true);

	//Si le mauvais joueur joue
	if ((board.color[Ancienne_Case] == sim.ai_color) != tour) {
		Act_ai(sim, board, log, tour, change_from, change_to, true, danger_zones);
		return;
	}
	
	int Nouvelle_Case = board.Location(false);

	int move = board.Move(Ancienne_Case, Nouvelle_Case, true);
	if (King_Safety(sim, true, (sim.ai_color % 2) + 1, Ancienne_Case, Nouvelle_Case, move) && (bool)move)	{
		if (move == 1)
			board.SubMove(Ancienne_Case, Nouvelle_Case);
		else if (move == 2)
			board.SubSwitch(change_from, change_to);
		else if (move == 3)
		{
			board.type[change_from] = 5;
			board.SubMove(change_from, change_to);
		}
	}
	else
		Act_ai(sim, board, log, tour, change_from, change_to, false, danger_zones);
}

std::tuple<int, int> get_random_move(Simulation sim, int color, int_node* danger_zones)
{
	final_move_node* random_move = new final_move_node, * head = random_move;
	int total = 0;
	int possible_move, add_value;
	for (int origin = 0; origin < 64; origin++) //Traverse le board
		if (sim.color[origin] == color) //Fait alterner les couleures
			for (int_node* move = danger_zones + origin; move->next != nullptr; move = move->next)
			{
				std::tie(possible_move, add_value) = sim.Move(origin, move->data, false);
				if (possible_move && origin < move->data)
				{
					total++;
					random_move = add_final_move_node(random_move, NULL, origin, move->data);
				}
			}
	srand((unsigned int)time(NULL));
	for (int i = 0; i < rand() % (total + 1); i++)
		head = head->next;
	return { head->change_from, head->change_to };
}

void PVE(board& board)
{
	Log log;
	bool tour = true;
	int change_from = 0; int change_to = 0;
	static int depth = 6;

	//Sets board
	board.Set_Board();

	//prints board
	board.PrintBoard();

	//PVP loop
	while (board.GameCheck() && !board.force_break)
	{
		Simulation sim(board.type, board.color, 0, 0);
		if (tour)
		{
			log.Info("FINDING MOVE...");
			unsigned long long int count = 0;
			Simulation* obj = sim.Get_Best_Move(depth, change_from, change_to, save_all_danger_zones(), depth, count);
			change_from = obj->change_from; change_to = obj->change_to;

			//log.Int(count);
			if (board.type[change_from] == 1 && board.color[change_from] == obj->ai_color && change_to <= 63 && change_to >= 56)
			{
				board.type[change_from] = 5;
				board.SubMove(change_from, change_to);
			}
			else if (change_to == 4 && board.color[4] == 2 && board.color[change_from] == 2 && board.type[4] == 6 && ((change_from == 0 && board.type[1] == 0 && board.type[2] == 0 && board.type[3] == 0) || (change_from == 7 && board.type[6] == 0 && board.type[5] == 0)))
				board.SubSwitch(change_from, change_to);
			else
				board.SubMove(change_from, change_to);
			log.Info("...DONE");
		}
		else
			Act_ai(sim, board, log, tour, change_from, change_to, false, save_all_danger_zones());
		tour = !tour;
		//Affiche le Board
		board.PrintBoard();
		//StoreInfo(board);
		//present(threats(board.FindBlock(str_cin("Threat radar origin: ")), board));
	}
}
