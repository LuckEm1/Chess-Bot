#include "..\header\Game_Header.h"

//CONSTRUSTOR
board::board()
{
	int str_upd = 0;
	int int_upd = 56;
	std::string Case;
	for (int x = 0; x < 64; x++)
	{
		string[x] = ' ';
		type[x] = 0;
		color[x] = 0;

		//Génère les noms de cases traditionnelles
		// => block[]
		if ((x % 8 == 0) && (x != 0))
		{
			str_upd = 0;
			int_upd--;
		}
		Case = str_upd + 65;
		Case += char(int_upd);
		str_upd++;
		block[x] = Case;
	}
	Set_Board();
}



//INITIALISE UNE PIÈCE
void board::SetPiece(int loc, char info_string, int info_type, int info_color)
{
	if ((0 < loc) && (loc < 65))
	{
		loc -= 1;
		type[loc] = info_type;
		string[loc] = info_string;
		color[loc] = info_color;
	}
	else {
		//Informe en cas de brèche des frontières
		Log log1;
		log1.Error("Piece location out of bounderies");
	}
}



//VÉRIFIE SI UN JOUEUR A GAGNÉ
bool board::GameCheck()
{
	int i;
	for (int x = 0; x < 64; x++)
	{
		if (type[x] == 6)
		{
			i = x;
			nbr_roi++;
			if (nbr_roi == 2)
			{
				nbr_roi = 0;
				return true;
				//Continu la partie si tout les deux rois sont présents
			}
		}
	}
	//La couleur du roi restant gagne et la bouble principale est terminée, mettant donc fin au jeu
	Log log;
	log.Print(its(color[i]) + " wins!");
	return false;
}



//AFFICHE LE BOARD
void board::PrintBoard()
{
	Log log;
	log.Print(top);
	for (int x = 0; x < 8; x++)
	{
		log.Print("    " + its(8 - x) + "    |   " + string[x * 8] + "   |   " + string[x * 8 + 1] + "   |   " + string[x * 8 + 2] + "   |   " + string[x * 8 + 3] + "   |   " + string[x * 8 + 4] + "   |   " + string[x * 8 + 5] + "   |   " + string[x * 8 + 6] + "   |   " + string[x * 8 + 7] + "   |");
		if (x != 7)
			log.Print(mid);
	}
	log.Print(bot);
}



//Sets and resets the board
void board::Set_Board()
{
	//Set Pieces

	//WHITE
	SetPiece(64, 'T', 2, 1);
	SetPiece(63, 'C', 3, 1);
	SetPiece(62, 'F', 4, 1);
	SetPiece(60, 'R', 5, 1);
	SetPiece(61, 'K', 6, 1);
	SetPiece(59, 'F', 4, 1);
	SetPiece(58, 'C', 3, 1);
	SetPiece(57, 'T', 2, 1);

	SetPiece(49, 'P', 1, 1);
	SetPiece(50, 'P', 1, 1);
	SetPiece(51, 'P', 1, 1);
	SetPiece(52, 'P', 1, 1);
	SetPiece(53, 'P', 1, 1);
	SetPiece(54, 'P', 1, 1);
	SetPiece(55, 'P', 1, 1);
	SetPiece(56, 'P', 1, 1);

	//BLACK
	SetPiece(8, 't', 2, 2);
	SetPiece(7, 'c', 3, 2);
	SetPiece(6, 'f', 4, 2);
	SetPiece(5, 'k', 6, 2);
	SetPiece(4, 'r', 5, 2);
	SetPiece(3, 'f', 4, 2);
	SetPiece(2, 'c', 3, 2);
	SetPiece(1, 't', 2, 2);

	SetPiece(9, 'p', 1, 2);
	SetPiece(10, 'p', 1, 2);
	SetPiece(11, 'p', 1, 2);
	SetPiece(12, 'p', 1, 2);
	SetPiece(13, 'p', 1, 2);
	SetPiece(14, 'p', 1, 2);
	SetPiece(15, 'p', 1, 2);
	SetPiece(16, 'p', 1, 2);
}



//Vérifie Si un joueur est en échec
