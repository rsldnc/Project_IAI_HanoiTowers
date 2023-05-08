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
		printf("\n");
		int j = 0;
		while (j < 3)
		{
			printf("  %c", state->tower_matrix[i][j]);
			j++;
		}
		i++;
	}
	printf("\n");
}

//______________________________________________________________________________
void Print_Action(const enum ACTIONS action)
{
	switch(action)
	{
		case  TakeA_PutB:   
			printf("TakeA_PutB"); break;
		case  TakeA_PutC:   
			printf("TakeA_PutC"); break;
		case  TakeB_PutA:   
			printf("TakeB_PutA"); break;
		case  TakeB_PutC:   
			printf("TakeB_PutC"); break;
		case  TakeC_PutA:   
			printf("TakeC_PutA"); break;
		case  TakeC_PutB:   
			printf("TakeC_PutB"); break;
	}
}

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
	int i = 7;
	int count = 0;
	while (ft_is_numeric(goal->tower_matrix[i][2]))
	{
		if (state->tower_matrix[i][2] != goal->tower_matrix[i][2])
			count++;
		i--;
	}

	return count;   
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
