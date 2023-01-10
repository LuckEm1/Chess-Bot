#include "..\header\Game_Header.h"
#include "..\header\Node_Header.h"

//Renvoi le numero associé à la case
int board::FindBlock(std::string Block)
{
	if (Block == "0")
		return 0;
	Log log;
	for (int i = 0; i < 64; i++)
		if (block[i] == Block)
			return i;
	//Si il y a un probleme dans la première phase du check, 
	log.Warning("Please correct the syntax/grammar of the case.");
	return 0;
}


//Confire que l'information reçu par l'utilisateur est conforme au jeu
bool board::Check(std::string Block, bool check_empty_space)
{
	//Block est la case donnée tels que Block = Lettre + Chiffre
	//On assume que Block n'est pas dans block[]
	bool block_bool = false;

	//Confirme que la case donnée par l'utilisateur est dans block[]
	for (int y = 0; y < 64; y++)
		if (block[y] == Block)
			block_bool = true;

	//Si ce statement est confirmé par check_empty_space, renvoi false si la case sélectionné est vide
	if ((type[FindBlock(Block)] == 0) && check_empty_space)
		return false;

	//Renvoi true si l'information est valide et false si elles ne le sont pas
	return block_bool;
}


//Intéragi avec l'utilisateur, vérifie l'information, renvoi le numero associé à la case
int board::Location(bool check_empty_space)
{
	Log log;
	std::string location_block;

	//Demande la case à affecter à l'utilisateur
	if (check_empty_space)
		log.Print("L'ancienne case: ");
	else
		log.Print("La nouvelle case: ");

	//Demande la case à trouver
	location_block = str_cin("");
	if (location_block == "x") force_break = true;

	//Vérifie que les informations recues par l'utilisateur sont conformes au Board 
	if (not Check(location_block, check_empty_space))
		//et redemande les informations à l'utilisateur si elles sont fausses et relance "Location"
		Location(check_empty_space);

	//Trouve le numero associé à la case => location
	int location = FindBlock(location_block);

	//Imprime le pion de la case en question tant que la case en question n'est pas vide
	if (not check_empty_space)
		log.Print(its(type[location]));

	//renvoi le numero associé à la case
	return location;
}


//Déclanche et gère l'action des utilisateurs
void board::Act(bool tour, bool error) {
	Log log;
	std::string color_error_str;

	//Indique c'est au tour de qui et si une erreur de couleur est en jeu
	if (error) color_error_str = "[MAUVAISE COULEURE]: ";
	if (tour)  log.Print("\n" + color_error_str + "C'est au tour des Blancs.");
	else	   log.Print("\n" + color_error_str + "C'est au tour des Noirs.");

	//Intéragis avec l'utilisateur afin de pouvoir agir sur le board avec les information recues (et vérifiées dans le board)
	int Ancienne_Case = Location(true);

	if ((color[Ancienne_Case] == 1) != tour) {
		Act(tour, true); 
		return;
	}

	int Nouvelle_Case = Location(false);

	//Confirm si le mouvement demandé est conforme au règles du jeu et agis sur le board

	int move = Move(Ancienne_Case, Nouvelle_Case, true);
	if (King_Safety(Simulation(type, color, 0, 0), true, (int)(!tour) + 1, Ancienne_Case, Nouvelle_Case, move) && (bool)move) {
		if (move == 1)
			SubMove(Ancienne_Case, Nouvelle_Case);
		else if (move == 2)
			SubSwitch(Ancienne_Case, Nouvelle_Case);
	}
	else
		Act(tour, false);
}


//PVP
void PvP(board& board) {
	bool tour = false;

	//Sets board
	board.Set_Board();

	//prints board
	board.PrintBoard();

	//PVP loop
	while (board.GameCheck() && !board.force_break)
	{
		board.Act(tour, false);
		tour = !tour;

		//Affiche le Board
		board.PrintBoard();
		//StoreInfo(board);
		//present(threats(board.FindBlock(str_cin("Threat radar origin: ")), board));
	}
}
