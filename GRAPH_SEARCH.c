#include <stdio.h>
#include <stdlib.h>
#include "GRAPH_SEARCH.h"
#include "data_types.h"

State* Create_Goal_State(int disk_num);
// THIS PROGRAM WORKS WHEN THE GOAL STATE CAN BE DEFINED BEFORE SEARCHING

int main()
{	
    Node root, *goal;
    State *goal_state = NULL;
    enum METHODS method;
    int Max_Level, level;
   
    // This part must be updated if a new algorithm is added. 
    printf("1 --> Breast-First Search\n");
    printf("2 --> Uniform-Cost Search\n");
    printf("3 --> Depth-First Search\n");
    printf("4 --> Depth-Limited Search\n");
    printf("5 --> Iterative Deepening Search\n");
    printf("6 --> Greedy Search\n");
    printf("7 --> A* Search\n");
    printf("Select a method to solve the problem: ");
    scanf("%d", &method);
    if(method==DepthLimitedSearch)
    {
	    printf("Enter maximum level for depth-limited search : ");                         
	    scanf("%d", &Max_Level);                  
    }     
    
    // Creating the root node ... 
    root.parent    = NULL;
    root.path_cost = 0;
    root.action    = NO_ACTION;
    root.Number_of_Child = 0;
    	
    printf("======== SELECTION OF INITIAL STATE =============== \n");
    root.state     = *(Create_State());
    
    if(PREDETERMINED_GOAL_STATE)  // User will determine the goal state if it is true
    {
	    printf("======== GOAL STATE =============== \n"); 
	    goal_state = Create_Goal_State(root.state.disk_num);
	    Print_State(goal_state);
	    printf("\n");
    }
    
    if(method==GreedySearch || method==AStarSearch){
        root.state.h_n  = Compute_Heuristic_Function(&(root.state), goal_state);
        goal_state->h_n = 0;                 
	}  
    	
    switch(method) 
    {
        case BreastFirstSearch: 
        case GreedySearch:               
            goal = First_GoalTest_Search_TREE(method, &root, goal_state);  break; 
		case DepthFirstSearch: 	
		case DepthLimitedSearch: 
			goal = DepthType_Search_TREE(method, &root, goal_state, Max_Level);  break;  
        case IterativeDeepeningSearch:
            for(level=0; TRUE ;level++){
            	goal = DepthType_Search_TREE(method, &root, goal_state, level);
            	if(goal!=FAILURE){
            		printf("The goal is found in level %d.\n", level); 
            		break;
				}		
			}
            break;     
        case UniformCostSearch: 
        case AStarSearch:     
            goal = First_InsertFrontier_Search_TREE(method, &root, goal_state);  break;      
     
        default: 
            printf("ERROR: Unknown method.\n");  
            exit(-1);    
    }

    Show_Solution_Path(goal);
  	
    return 0;
}

State* Create_Goal_State(int disk_num)
{
	State *state = (State*)malloc(sizeof(State));
	if(state==NULL)
    		Warning_Memory_Allocation(); 
	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 3; j++)
			state->tower_matrix[i][j] = '*';
	}
	state->tower_matrix[8][0] = 'A';
	state->tower_matrix[8][1] = 'B';
	state->tower_matrix[8][2] = 'C';
   	
	int i = 7;
	int cpy_disk_num = disk_num;
	while (cpy_disk_num)
	{
		state->tower_matrix[i][2] = cpy_disk_num + 48;
		i--;
		cpy_disk_num--;
	}
	return state;
}
