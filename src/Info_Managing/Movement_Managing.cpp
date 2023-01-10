#include "..\header\Game_Header.h"
#include "..\header\Node_Header.h"

//renvoi les zones en croix par rapport a un point origin a partir de <start> cases
int_node* get_cross(int origin, int start, int_node* node)
{
	int diff = origin % 8;
	for (int i = origin - diff; i < origin - diff + 8; i++)
		if ((i < origin - start) || (i > origin + start))
			node = add_node_int(i, node);

	for (int i = diff; i < 64; i += 8)
		if ((i < origin - start * 8) || (i > origin + start * 8))
			node = add_node_int(i, node);

	return node;
}
//renvoi les zones en diagonal par rapport a un point origin a partir de <start> cases
int_node* get_diagonal(int origin, int start, int_node* node)
{
	//Haut-Gauche
	for (int step = origin; step >= 9 && (step % 8); step -= 9)
		if (step <= origin - start * 9)
			node = add_node_int(step - 9, node);

	//Haut-Droite
	for (int step = origin; step >= 7 && ((step + 1) % 8); step -= 7)
		if (step <= origin - start * 7)
			node = add_node_int(step - 7, node);

	//Bas-Droite
	for (int step = origin; step <= 54 && ((step + 1) % 8); step += 9)
		if (step >= origin + start * 9)
			node = add_node_int(step + 9, node);

	//Bas-Gauche
	for (int step = origin; step <= 56 && (step % 8); step += 7)
		if (step >= origin + start * 7)
			node = add_node_int(step + 7, node);

	return node;
}
//informe get_square_info par rapport aux bordures voulues pour la carré
void get_square_info(int origin, int radious, int* array)
{
	if ((radious < 8) && (origin > -1) && (origin < 64))
	{
		int diff = origin % 8;
		bool in_left = ((origin + 1) % 8 > radious) || !((origin + 1) % 8);
		bool in_right = (8 - diff > radious) || !diff;
		bool in_top = origin - 8 * radious >= 0;
		bool in_down = origin + 8 * radious < 64;

		int max_left = radious; int max_right = radious; int max_top = radious; int max_down = radious;
		if (!in_left)    max_left = diff;
		if (!in_right)   max_right = 7 - diff;
		if (!in_top)     max_top = origin / 8;
		if (!in_down)    max_down = (63 - origin) / 8;

		//OPTIMISABLE EN ENLEVANT LE in_max
		int in_max[8] = { in_left, in_right, in_top, in_down, max_left, max_right, max_top, max_down };
		for (int i = 0; i < 8; i++)
			*(array + i) = in_max[i];

		//for (int i = 0; i < 8; i++) { Log log; log.Print(its(i) + " : " + its(in_max[i])); }
		return;
	}
	Log log;  log.Error("Square Radious - origin or radious out of bounds");
}
//renvoi les zones en carré par rapport a un point origin a partir de <start> cases
int_node* get_square(int origin, int radious, int_node* node)
{
	int radious_info[8];
	get_square_info(origin, radious, radious_info);

	//Rajoute les cases à/en ... de l'origine
	//Gauche
	if (radious_info[0])
	{
		int start = origin - radious * 9;

		if (radious_info[2] && radious_info[3])
			for (int i = 0; i < radious * 2 + 1; i++)
				node = add_node_int(start + 8 * i, node);

		else if (radious_info[3])
			for (int i = radious - radious_info[6]; i < radious * 2 + 1; i++)
				node = add_node_int(start + 8 * i, node);

		else
			for (int i = 0; i < radious + 1 + radious_info[7]; i++)
				node = add_node_int(start + 8 * i, node);
	}
	//Droite
	if (radious_info[1])
	{
		int start = origin - radious * 7;

		if (radious_info[2] && radious_info[3])
			for (int i = 0; i < radious * 2 + 1; i++)
				node = add_node_int(start + 8 * i, node);

		else if (radious_info[2])
			for (int i = 0; i < radious + 1 + radious_info[7]; i++)
				node = add_node_int(start + 8 * i, node);

		else
			for (int i = radious - radious_info[6]; i < radious * 2 + 1; i++)
				node = add_node_int(start + 8 * i, node);
	}
	//Haut
	if (radious_info[2])
	{
		int start = origin - radious * 9;

		if (radious_info[0] && radious_info[1])
			for (int i = 1; i < radious * 2; i++)
				node = add_node_int(start + i, node);

		else if (radious_info[0])
			for (int i = 1; i < radious + radious_info[5] + 1; i++)
				node = add_node_int(start + i, node);

		else
			for (int i = radious - radious_info[4]; i < radious * 2; i++)
				node = add_node_int(start + i, node);
	}
	//Bas
	if (radious_info[3])
	{
		int start = origin + radious * 7;

		if (radious_info[0] && radious_info[1])
			for (int i = 1; i < radious * 2; i++)
				node = add_node_int(start + i, node);

		else if (radious_info[0])
			for (int i = 1; i < radious + radious_info[5] + 1; i++)
				node = add_node_int(start + i, node);

		else
			for (int i = radious - radious_info[4]; i < radious * 2; i++)
				node = add_node_int(start + i, node);
	}

	return node;
}

//présente une serie de nodes sous forme d'un board d'écheques
void present(int_node* node)
{
	Log log;
	std::string str[64];
	for (int i = 0; i < 64; i++)
		str[i] = " ";
	while (node->next != nullptr)
	{
		str[node->data] = "0";
		node = node->next;
	}
	std::string top = "\n	     A 	     B 	     C 	     D 	     E 	     F 	     G	     H\n\n	 |-------|-------|-------|-------|-------|-------|-------|-------|\n	 |	 |       |	 |       |	 |       |	 |       |";
	std::string mid = "	 |       |       |       |       |       |       |       |       |\n	 |-------|-------|-------|-------|-------|-------|-------|-------|\n	 |       |       |       |       |       |       |       |       |";
	std::string bot = "	 |       |       |       |       |       |       |       |       |\n	 |-------|-------|-------|-------|-------|-------|-------|-------|";
	log.Print(top);
	for (int x = 0; x < 8; x++)
	{
		log.Print("    " + its(8 - x) + "    |   " + str[x * 8] + "   |   " + str[x * 8 + 1] + "   |   " + str[x * 8 + 2] + "   |   " + str[x * 8 + 3] + "   |   " + str[x * 8 + 4] + "   |   " + str[x * 8 + 5] + "   |   " + str[x * 8 + 6] + "   |   " + str[x * 8 + 7] + "   |");
		if (x != 7) log.Print(mid);
	}
	log.Print(bot + "\n");
}
//Détecte les menaces pour une case et renvoi la tête du node contenant cette information
int_node* threats(int  origin, board& board)
{
	int_node* node = new int_node, * head = node;

	for (int_node* threat = danger_zone(origin); threat->next != nullptr; threat = threat->next)
		if (board.Move(threat->data, origin, false))
			node = add_node_int(threat->data, node);

	return head;
}
//Détecte les menaces pour toutes les cases  et les affiches
void all_threats(board& board)
{
	Log log;
	for (int i = 0; i < 64; i++)
	{
		log.Info(board.block[i] + " :");
		present(threats(i, board));
	}
}

//Détecte les cases pouvant poser une menace pour une case <origing>
int_node* danger_zone(int origin)
{
	int_node* node = new int_node, * head = node;

	node = get_diagonal(origin, 2, node);
	node = get_square(origin, 1, node);
	node = get_square(origin, 2, node);
	node = get_cross(origin, 2, node);

	return head;
}

//Détecte et affiche les cases pouvant poser une menace pour toutes les cases
void show_all_danger_zone(board& board)
{
	Log log;
	for (int i = 0; i < 64; i++)
	{
		log.Info(board.block[i] + " :");
		present(danger_zone(i));
	}
}
//Saves all danger zones (possible threats for a unit) in an array
int_node* save_all_danger_zones()
{
	int_node* danger_zones = new int_node[64];
	for (int i = 0; i < 64; i++)
		danger_zones[i] = *danger_zone(i);
	return danger_zones;
}

