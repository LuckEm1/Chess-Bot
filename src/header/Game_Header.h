#pragma once
#include <string>

/*			LOG			*/
//----------------------------------------------------------
struct Log
{
	//std::string int_to_str(int variable);
	void Info(std::string message);
	void Print(std::string message);
	void Error(std::string message);
	void Warning(std::string message);
	void Int(int message);
	void Char(char message);
};
//----------------------------------------------------------


/*        BOARD CONTROLE        */
//----------------------------------------------------------
struct board
{
	//Représente le board avec ses 64 cases
	bool force_break = false;
	int type[64];
	char string[64];
	int color[64];
	std::string block[64];

	int nbr_roi = 0;



	/*			INITIALISES THE BOARD			*/
	//----------------------------------------------------------
	board();
	void SetPiece(int loc, char string, int type, int color);
	bool GameCheck();
	void PrintBoard();
	void Set_Board();
	//----------------------------------------------------------



	/*			VEREFIES AND ACTS UPON THE INTERACTION WITH USERS			*/
	//----------------------------------------------------------
	int FindBlock(std::string Block);
	bool Check(std::string Block, bool check_empty_space);
	int Location(bool check_empty_space);
	void Act(bool tour, bool error);
	//----------------------------------------------------------



	/*        MOUVEMENTS        */
	//----------------------------------------------------------
	void SubMove(int loc, int new_loc);
	void SubSwitch(int loc, int new_loc);

	bool Cavalier(int loc, int new_loc);
	bool CheckUpDown(int loc, int new_loc);
	bool CheckLeftRight(int loc, int new_loc);
	bool Diagonal(int loc, int new_loc);

	int Move(int loc, int new_loc, bool error);
	//----------------------------------------------------------


	


	std::string top = "\n	     A 	     B 	     C 	     D 	     E 	     F 	     G	     H\n\n	 |-------|-------|-------|-------|-------|-------|-------|-------|\n	 |	 |       |	 |       |	 |       |	 |       |";
	std::string mid = "	 |       |       |       |       |       |       |       |       |\n	 |-------|-------|-------|-------|-------|-------|-------|-------|\n	 |       |       |       |       |       |       |       |       |";
	std::string bot = "	 |       |       |       |       |       |       |       |       |\n	 |-------|-------|-------|-------|-------|-------|-------|-------|";
};
//----------------------------------------------------------


/*        TESTS        */
//----------------------------------------------------------	
bool Control(bool start);
void PvP(board& board);
void visualise(int amount);
/**/void StoreInfo(board& board);
//----------------------------------------------------------


/*        IOSTREAM EXTERNAL ACCESS        */
//----------------------------------------------------------
std::string its(int variable);
std::string str_cin(std::string message);
int int_cin(std::string message);
//----------------------------------------------------------

bool border(int value);
void all_threats(board& board);
