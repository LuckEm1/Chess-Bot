#include "header\Game_Header.h"
#include "header\Node_Header.h"
#include <tuple>

//FONCTIONS
bool Control(bool start)
{
	Log log;
	board board;
	if (start)
		log.Print("0- Stop\n1- PvP\n2- PVE\n3- All Threats");
	std::string answer = str_cin("<>");
	if (answer == "0") { log.Print("Break"); return false; }									  //ENDS PROGRAM
	else if (answer == "1")	PvP(board);															      //ACTIVATES PVP
	else if (answer == "3") { show_all_danger_zone(board); }
	else if (answer == "2") { PVE(board); }
	else { log.Print("['" + answer + "' not available]"); Control(false); }					      //PREVENTS OTHER IMPUTS THANT THE ONES GIVEN
	log.Print("\n");
	return true;
}


//THE PROGRAM
int main()
{
	while (Control(true));
}