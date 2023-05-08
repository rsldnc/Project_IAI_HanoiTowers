/* 
    These functions are compulsory for tree search algorithms but they are specific
    to problems. More clearly, you must must update their blocks but do not change
    their input and output parameters.
    
    Also, if you need, you can add new functions at the end of file by declaring them in TREE_SEARCH.h
*/

#include "GRAPH_SEARCH.h"
#include "data_types.h"
#include <stdio.h>
#include <stdlib.h>

//______________________________________________________________________________
State* Create_State()
{
	State *state = (State*)malloc(sizeof(State));
	if(state==NULL)
    		Warning_Memory_Allocation();
	int i,j;
   	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 3; j++)
			state->tower_matrix[i][j] = '*';
	}
	state->tower_matrix[8][0] = 'A';
	state->tower_matrix[8][1] = 'B';
	state->tower_matrix[8][2] = 'C';
	
	do
	{ 
    		printf("Pick the num of disks(3, 5 or 7): ");
        	scanf("%d", &state->disk_num);
   	}while(state->disk_num != 3 && state->disk_num != 5 && state->disk_num != 7);
   	
	i = 7;
	int cpy_disk_num = state->disk_num;
	while (cpy_disk_num)
	{
		state->tower_matrix[i][0] = cpy_disk_num + 48;
		i--;
		cpy_disk_num--;
	}

	Print_State(state);
	printf("\n");

	return state;
}

//______________________________________________________________________________
void Print_State(const State *const state)
{
	int i = 0;
	while (i < 9)
	{
		int j = 0;
		while (j < 3)
		{
			printf("  %c", state->tower_matrix[i][j]);
			j++;
		}
		printf("\n");
		i++;
	}
}

//______________________________________________________________________________
void Print_Action(const enum ACTIONS action)
{
   switch(action){
         case  TakeA_PutB:   printf("TakeA_PutB"); break;
         case  TakeA_PutC:   printf("TakeA_PutC"); break;
         case  TakeB_PutA:   printf("TakeB_PutA"); break;
         case  TakeB_PutC:   printf("TakeB_PutC"); break;
         case  TakeC_PutA:   printf("TakeC_PutA"); break;
         case  TakeC_PutB:   printf("TakeC_PutB"); break;
    }        
}

//______________________________________________________________________________
int	ft_is_numeric(char c);
char find_topoftower(const State *const parent_state, int col_num);

//______________________________________________________________________________
int Result(const State *const parent_state, const enum ACTIONS action, Transition_Model *const trans_model)
{
    State new_state;
    int taken_from, put_into;
    switch(action)
    {
    	case  TakeA_PutB:   
			taken_from = 0; put_into = 1; break;
    	case  TakeA_PutC:   
			taken_from = 0; put_into = 2; break;
    	case  TakeB_PutA:   
			taken_from = 1; put_into = 0; break;
    	case  TakeB_PutC:   
			taken_from = 1; put_into = 2; break;
    	case  TakeC_PutA:   
			taken_from = 2; put_into = 0; break;
        case  TakeC_PutB:   
			taken_from = 2; put_into = 1; break;
    }
    
    if ((find_topoftower(parent_state, taken_from) > find_topoftower(parent_state, put_into)) || (!ft_is_numeric(find_topoftower(parent_state, taken_from)) && !ft_is_numeric(find_topoftower(parent_state, put_into)))) 
        return FALSE;
    else
    {
	int k, l;
        for (k = 0; k < 9; k++)
	{
        	for (l = 0; l < 3; l++)
		{
            		new_state.tower_matrix[k][l] = parent_state->tower_matrix[k][l];
        	}
    	}
    	
	int i = 0;
        while (parent_state->tower_matrix[i][taken_from] != find_topoftower(parent_state, taken_from))
        	i++;
        int j = 0;
        while (parent_state->tower_matrix[j + 1][put_into] != find_topoftower(parent_state, put_into))
        	j++;
        
	char c = new_state.tower_matrix[i][taken_from];
        new_state.tower_matrix[i][taken_from] = new_state.tower_matrix[j][put_into];
        new_state.tower_matrix[j][put_into] = c;
        trans_model->new_state = new_state;
        trans_model->step_cost = 1; 
    }     
    return TRUE;                                               
}

float Compute_Heuristic_Function(const State *const state, const State *const goal)
{
      const float SLD[CITY_NUMBER][CITY_NUMBER] =   // CALCULATED ROUGHLY!!!
        {   {  0, 366, 300, 220, 590, 235, 430, 535, 420, 168, 225, 355, 110, 290, 185, 130, 105, 435, 470,  67},  // Arad
            {366,   0, 160, 242, 161, 176,  77, 151, 226, 244, 241, 234, 380, 100, 193, 253, 329,  80, 199, 374},  // Bucharest
			{300, 160,   0, 102, 325, 210, 142, 325, 368, 145, 110, 388, 355, 133, 138, 190, 220, 220, 385, 330},  // Craiova
			{220, 242, 102,   0, 390, 245, 240, 380, 400, 100,  70, 460, 310, 190, 155, 205, 150, 302, 410, 265},  // Drobeta
			{590, 161, 325, 390,   0, 325, 205,  80, 245, 400, 375, 350, 595, 265, 340, 395, 555, 130, 260, 593},  // Eforie
			{235, 176, 210, 245, 325,   0, 200, 251, 180, 175, 185, 135, 220, 102,  85,  95, 255, 195, 200, 227},  // F 
			{430,  77, 142, 240, 205, 200,   0, 213, 297, 245, 280, 285, 410, 125, 225, 285, 330, 120, 230, 520},  // G
			{535, 151, 325, 380,  80, 251, 213,   0, 205, 370, 370, 260, 535, 220, 345, 365, 495,  92, 130, 535},  // H
			{440, 226, 368, 400, 245, 180, 297, 205,   0, 330, 350,  80, 365, 250, 260, 280, 450, 190,  85, 390},  // I
			{168, 244, 145, 100, 400, 175, 245, 370, 330,   0,  66, 290, 240, 140,  95, 135, 100, 285, 360, 205},  // L 
			{225, 241, 110,  70, 375, 185, 280, 370, 350,  66,   0, 305, 300, 140, 120, 175, 130, 280, 370, 260},  // M
			{355, 234, 388, 460, 350, 135, 285, 260,  80, 290, 305,   0, 273, 220, 260, 250, 330, 235, 155, 310},  // N
			{110, 380, 355, 310, 595, 220, 410, 535, 365, 240, 300, 273,   0, 305, 200, 140, 213, 415, 435,  66},  // O
			{290, 100, 133, 190, 265, 102, 125, 220, 250, 140, 140, 220, 305,   0, 115, 150, 230, 140, 185, 297},  // P
			{185, 193, 138, 155, 340,  85, 225, 345, 260,  95, 120, 260, 200, 115,   0,  75, 150, 230, 290, 193},  // R
			{130, 253, 190, 205, 395,  95, 285, 365, 280, 135, 175, 250, 140, 150,  75,   0, 137, 300, 330, 135},  // S
			{105, 329, 220, 150, 555, 255, 330, 495, 450, 100, 130, 330, 213, 230, 150, 137,   0, 380, 455, 155},  // T
			{435,  80, 220, 302, 130, 195, 120,  92, 190, 285, 280, 235, 415, 140, 230, 300, 380,   0, 132, 425},  // U
			{470, 199, 385, 410, 260, 200, 230, 130,  85, 360, 370, 155, 435, 185, 290, 330, 455, 132,   0, 452},  // V
			{ 67, 374, 330, 265, 593, 227, 520, 535, 390, 205, 260, 310,  66, 297, 193, 135, 155, 425, 452,   0}   // Z
		};
	     //    A    B    C    D    E    F    G    H    I    L    M    N    O    P    R    S    T    U    V    Z   
         
        return SLD[state->city][goal->city];   
}

// ==================== WRITE YOUR OPTIONAL FUNCTIONS ==========================

int	ft_is_numeric(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	else
		return (0);
}

char find_topoftower(const State *const parent_state, int col_num)
{
	int i = 0;
	while (!ft_is_numeric(parent_state->tower_matrix[i][col_num]) && i < 8)
	{
		i++;
	}
	return parent_state->tower_matrix[i][col_num];
}
