#include "..\header\Game_Header.h"
#include "..\header\Node_Header.h"
#include <fstream>

//renvoi true si la valeur est dans un board sinon false
bool border(int value)
{
	if ((value > -1) && (value < 64))
		return true;
	return false;
}

//affiche le board
void info_to_board(std::string info)
{
	board board;
	Log log;
	log.Print(board.top);
	for (int x = 0; x < 8; x++)
	{
		log.Print("    " + its(8 - x) + "    |   " + info.at(x * 8) + "   |   " + info.at(x * 8 + 1) + "   |   " + info.at(x * 8 + 2) + "   |   " + info.at(x * 8 + 3) + "   |   " + info.at(x * 8 + 4) + "   |   " + info.at(x * 8 + 5) + "   |   " + info.at(x * 8 + 6) + "   |   " + info.at(x * 8 + 7) + "   |");
		if (x != 7) log.Print(board.mid);
	}
	log.Print(board.bot + "\n");
}

//enregistre les string de board dans info.txt
void StoreInfo(board& board)
{
	Log log;
	std::ofstream file("info.txt", std::fstream::app);
	if (file.is_open())
	{
		std::string str;
		for (int i = 0; i < 64; i++)
			str += board.string[i];
		file << str << std::endl;
		file.close();
	}
	else log.Error("Unable to open file");
}

//va chercher toutes les info de board dans le fichier jusqu'a <range> exemplaires et les enregistres dans des nodes
str_node* extract(int range)
{
	str_node* n, * head;
	n = new str_node;
	head = n;

	std::ifstream file("info.txt");
	if (file.is_open())
	{
		std::string line;
		while (!file.eof() && (range != 0))
		{
			std::getline(file, line);
			n = add_node_str(line, n);
			range--;
		}
		file.close();
	}
	else { Log log; log.Error("Unable to open file"); }
	return head;
}

//affiche toutes les boards d'écheques dans info.txt jusqu'a <amount> exemplaires
void visualise(int amount)
{
	str_node* mobile_node = extract(amount);
	while (mobile_node->data != "")
	{
		info_to_board(mobile_node->data);
		mobile_node = mobile_node->next;
	}
}