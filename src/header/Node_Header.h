#pragma once
#include <string>
#include <random>
#include <cstdlib>
#include <ctime>
#include <limits>

//INT NODES
//----------------------------------------------------------
struct int_node {
	int data = 0;
	int_node* next = nullptr;
};
inline int_node* add_node_int(int value, int_node* pre_node)
{
	int_node* new_node = new int_node;
	pre_node->next = new_node;
	pre_node->data = value;
	return new_node;
}


//STR NODES
//----------------------------------------------------------
struct str_node {
	std::string data;
	str_node* next = nullptr;
};
inline str_node* add_node_str(std::string value, str_node* pre_node)
{
	str_node* new_node = new str_node;
	pre_node->next = new_node;
	pre_node->data = value;
	return new_node;
}


//MOVES NODES
//----------------------------------------------------------
struct moves_node {
	float value = NULL;
	moves_node* next = nullptr;
};
inline moves_node* add_moves_node(moves_node* pre_node, float value)
{
	pre_node->value = value;
	moves_node* new_node = new moves_node;
	pre_node->next = new_node;
	return new_node;
}

//FIANL MOVES NODES
//----------------------------------------------------------
struct final_move_node {
	float value = NULL;
	int change_from = NULL;
	int change_to = NULL;
	final_move_node* next = nullptr;
};

inline final_move_node* add_final_move_node(final_move_node* pre_node, float value, int change_from, int change_to)
{
	pre_node->value = value;
	pre_node->change_from = change_from;
	pre_node->change_to = change_to;
	final_move_node* new_node = new final_move_node;
	pre_node->next = new_node;
	return new_node;
}





str_node* extract(int range);

void present(int_node* node);
int_node* threats(int origin, board& board);
int_node* danger_zone(int origin);
void show_all_danger_zone(board& board);
int_node* save_all_danger_zones();

struct Simulation {

	//2 = ai noir, 1 = ai blanc 
	int ai_color = 2;
	int couleur = ai_color;

	int change_from = 8; int change_to = 16;

	float Alpha = -214748364.f;
	float Beta  =  214748364.f;

	int type[64];
	int color[64];
	float value = 0;

	Simulation(int old_type[64], int old_color[64], float old_value, int color_step);

	bool Cavalier(int loc, int new_loc);
	bool CheckUpDown(int loc, int new_loc);
	bool CheckLeftRight(int loc, int new_loc);
	bool Diagonal(int loc, int new_loc);
	std::tuple<int, int> Move(int loc, int new_loc, bool error);

	Simulation * Get_Best_Move(int profondeur, int pre_cf, int pre_ct, int_node* danger_zones, int& depth, unsigned long long int& count);
	Simulation SubMove(int loc, int new_loc, int color_step, float move_value);
	Simulation SubSwitch(int loc, int new_loc, int color_step, float move_value);

	bool updatedValues(int profondeur, float nbr, Simulation* sim = nullptr);
};


bool King_Safety(Simulation sim, bool player_turn, int color, int change_from, int change_to, int movement_type);
void Act_ai(Simulation& sim, board& board, Log& log, bool tour, int change_from, int change_to, bool error, int_node* danger_zones);
void PVE(board& board);
void present_simulation(Simulation sim);
int pawn_value(Simulation sim, int origin);
int get_min_info(moves_node* node);
