/* 
    These functions are standard for tree search algorithms and you do not need to
    change them for different search problems.
	YOU SHOULD COMPLETE : void Insert_Priority_Queue_A_Star(Node *const child, Queue **frontier)
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GRAPH_SEARCH.h"
#include "data_types.h"
#include "hashTable.h"

//______________________________________________________________________________
Node* First_InsertFrontier_Search_TREE(const enum METHODS method, Node *const root, State *const goal_state)
{
    int Number_Searched_Nodes  = 0;  // The number of nodes passing goal test
	int Number_Generated_Nodes = 1;  // The number of generated nodes (The first one is the root)  
	int Number_Allocated_Nodes = 1;  // The number of nodes in memory (The first one is the root)
	enum ACTIONS action;
    Node *node, *child, *temp_node; 
    Queue *frontier;
    Hash_Table *explorer_set;
	
	// a priority queue ordered by PATH-COST, with node as the only element		  
    frontier = Start_Frontier(root);  
	Print_Frontier(frontier);	
    
    explorer_set =  New_Hash_Table(HASH_TABLE_BASED_SIZE);
	Show_Hash_Table(explorer_set);
	
    while(Number_Searched_Nodes<MAX_SEARCHED_NODE) 
    {
        if(Empty(frontier))
            return FAILURE;  
                                   
        node = Pop(&frontier); 
        
		// GOAL-TEST
		Number_Searched_Nodes++;
		if(Goal_Test(&(node->state), goal_state) ){
			printf("\nThe number of searched nodes is : %d\n", Number_Searched_Nodes);
			printf("\nThe number of generated nodes is : %d\n", Number_Generated_Nodes);
			printf("\nThe number of generated nodes in memory is : %d\n", Number_Allocated_Nodes);
			Delete_Hash_Table(explorer_set);
			return node;
		}
		
		ht_insert(explorer_set, &(node->state));       
		Show_Hash_Table(explorer_set);
				                                
        for(action=0; action<ACTIONS_NUMBER; action++)
		{
			child = Child_Node(node, action);
		    
			if(child!=NULL){
				Number_Generated_Nodes++;
				Number_Allocated_Nodes++;
				
				if(ht_search(explorer_set, &(child->state))) 
					continue; // child.STATE is in explored set 
					
				temp_node = Frontier_search(frontier, &(child->state));	
					
				switch(method){
                    case UniformCostSearch:
					    if(temp_node!=NULL){
							if(child->path_cost < temp_node->path_cost) // child.STATE has been in frontier with higher cost
								Remove_Node_From_Frontier(temp_node, &frontier);
							else // child.STATE has already been in frontier with lower cost 
							    continue;		
						}  
                        Insert_Priority_Queue_UniformSearch(child, &frontier); break;
                    case AStarSearch:
                    	if(temp_node!=NULL){
							if(child->path_cost + child->state.h_n < temp_node->path_cost + temp_node->state.h_n) // child.STATE has been in frontier with higher cost
								Remove_Node_From_Frontier(temp_node, &frontier);	
							else // child.STATE has already been in frontier with lower cost 
							    continue;	
						} 
                        child->state.h_n = Compute_Heuristic_Function(&(child->state), goal_state); 
                        Insert_Priority_Queue_A_Star(child, &frontier); break;     
                    default:
                        printf("ERROR: Unknown method in First_InsertFrontier_Search_TREE.\n");
						Delete_Hash_Table(explorer_set);  
                        exit(-1);     	   
                }
				Print_Frontier(frontier);	
			}	            
		}    
    } 
    
    printf("Maximum number of searched nodes is exceeded. %d nodes are searched, but the goal could not found.\n", Number_Searched_Nodes);
    Delete_Hash_Table(explorer_set);
    return FAILURE;
}

//______________________________________________________________________________
Node* First_GoalTest_Search_TREE(const enum METHODS method, Node *const root, State *const goal_state)
{
    int Number_Searched_Nodes  = 0;  // The number of nodes passing goal test
	int Number_Generated_Nodes = 1;  // The number of generated nodes (The first one is the root)  
	int Number_Allocated_Nodes = 1;  // The number of nodes in memory (The first one is the root)
	enum ACTIONS action;
    Node *node, *child; 
    Queue *frontier;
    Hash_Table *explorer_set; 
	
	// GOAL-TEST
	Number_Searched_Nodes++;
    if(Goal_Test(&(root->state), goal_state) ){
        printf("\nThe number of searched nodes is : %d\n", Number_Searched_Nodes);
		printf("\nThe number of generated nodes is : %d\n", Number_Generated_Nodes);
		printf("\nThe number of generated nodes in memory is : %d\n", Number_Allocated_Nodes);
        return root;
    }
	
	frontier = Start_Frontier(root);
	Print_Frontier(frontier);
	
	explorer_set =  New_Hash_Table(HASH_TABLE_BASED_SIZE);
	Show_Hash_Table(explorer_set);
	  
    while(Number_Searched_Nodes<MAX_SEARCHED_NODE) 
    {
        if(Empty(frontier))
            return FAILURE;  
                                   
        node = Pop(&frontier);
         
		ht_insert(explorer_set, &(node->state));       
		Show_Hash_Table(explorer_set);
                                        
        for(action=0; action<ACTIONS_NUMBER; action++)
		{
			child = Child_Node(node, action);
			
			if(child!=NULL){
				Number_Generated_Nodes++;
				Number_Allocated_Nodes++;
						
				if(ht_search(explorer_set, &(child->state)) || Frontier_search(frontier, &(child->state))!=NULL)
					continue; // child.STATE is in explored set or frontier		
	
            	// GOAL-TEST 	
				Number_Searched_Nodes++;
			    if(Goal_Test(&(child->state), goal_state) ){
			        printf("\nThe number of searched nodes is : %d\n", Number_Searched_Nodes);
			        printf("\nThe number of generated nodes is : %d\n", Number_Generated_Nodes);
			        printf("\nThe number of generated nodes in memory is : %d\n", Number_Allocated_Nodes);
			        Delete_Hash_Table(explorer_set);
			        return child;
			    }

			    switch(method){
                    case BreastFirstSearch:      
                        Insert_FIFO(child, &frontier); break;   
                    case GreedySearch:
                        child->state.h_n = Compute_Heuristic_Function(&(child->state), goal_state);
                        Insert_Priority_Queue_GreedySearch(child, &frontier); break;   
                    default:
                        printf("ERROR: Unknown method in First_GoalTest_Search_TREE.\n");  
                        exit(-1);                  
                }
                Print_Frontier(frontier);			   
            }	            
		}    
    } 
    
    printf("Maximum number of searched nodes is exceeded. %d nodes are searched, but the goal could not found.\n", Number_Searched_Nodes);
    Delete_Hash_Table(explorer_set);
    return FAILURE;
}

//______________________________________________________________________________
Node* DepthType_Search_TREE(const enum METHODS method, Node *const root, State *const goal_state, const int Max_Level)
{
    static int Number_Searched_Nodes  = 0;  // The number of nodes passing goal test
	static int Number_Generated_Nodes = 1;  // The number of generated nodes (The first one is the root) 
	static int Number_Allocated_Nodes = 1;  // The number of nodes in memory (The first one is the root)   
	enum ACTIONS action;
    Node *node, *child; 
    Queue *frontier;
    Hash_Table *explorer_set; 
	
	// GOAL-TEST
	Number_Searched_Nodes++;
    if(Goal_Test(&(root->state), goal_state) ){
        printf("\nThe number of searched nodes is : %d\n", Number_Searched_Nodes);
		printf("\nThe number of generated nodes is : %d\n", Number_Generated_Nodes);
		printf("\nThe number of generated nodes in memory is : %d\n", Number_Allocated_Nodes);
        return root;
    }
	
	frontier = Start_Frontier(root);
	Print_Frontier(frontier);
	
	explorer_set =  New_Hash_Table(HASH_TABLE_BASED_SIZE);
	Show_Hash_Table(explorer_set);
	    
    while(Number_Searched_Nodes<MAX_SEARCHED_NODE) 
    {
        if(Empty(frontier))
            return FAILURE;  
                                   
        node = Pop(&frontier);
        
        ht_insert(explorer_set, &(node->state));       
		Show_Hash_Table(explorer_set);
		
		
		if(method==DepthLimitedSearch || method==IterativeDeepeningSearch)
			if(Level_of_Node(node)==Max_Level){
				Clear_All_Branch(node, &Number_Allocated_Nodes);
				continue; 
			} 
                
                                        
        for(action=0; action<ACTIONS_NUMBER; action++)
		{
			child = Child_Node(node, action);
		    
			if(child!=NULL){
			    Number_Generated_Nodes++;
			    Number_Allocated_Nodes++;
				
				if(ht_search(explorer_set, &(child->state)) || Frontier_search(frontier, &(child->state))!=NULL){
					Clear_Single_Branch(child, &Number_Allocated_Nodes); // if child.STATE is in explored set or frontier
				} 				
				else{
					// GOAL-TEST
					Number_Searched_Nodes++;
				    if(Goal_Test(&(child->state), goal_state) ){
				        printf("\nThe number of searched nodes is : %d\n", Number_Searched_Nodes);
				        printf("\nThe number of generated nodes is : %d\n", Number_Generated_Nodes);
				        printf("\nThe number of generated nodes in memory is : %d\n", Number_Allocated_Nodes);
				        Delete_Hash_Table(explorer_set);
				        return child;
				    }

				    Insert_LIFO(child, &frontier);	
				    Print_Frontier(frontier);
				}										   
            }
			
			if(action==ACTIONS_NUMBER-1 && node->Number_of_Child==0) // If node has not child, clear it
				Clear_All_Branch(node, &Number_Allocated_Nodes);	            
		}
		    
    } 
    
    printf("%d nodes are searched, but the goal could not found.\n", Number_Searched_Nodes);
    Delete_Hash_Table(explorer_set);
    return FAILURE;
}


//______________________________________________________________________________
Node* Child_Node(Node *const parent, const enum ACTIONS action)
{
      Node *child = NULL;
      Transition_Model trans_model;
      if( Result(&(parent->state), action, &trans_model) ){
          child = (Node*)malloc(sizeof(Node));
          if(child==NULL)
               Warning_Memory_Allocation(); 
           
          child->state     = trans_model.new_state;
          child->path_cost = parent->path_cost + trans_model.step_cost;
          child->parent    = parent;
          child->action    = action;
          child->Number_of_Child = 0;
          child->parent->Number_of_Child++;
      } 
      return child;
}

//______________________________________________________________________________
Queue* Start_Frontier(Node *const root)
{
    Queue *frontier = (Queue*)malloc(sizeof(Queue));
	    
	if(frontier==NULL)
    	Warning_Memory_Allocation(); 
    	
    frontier->node  = root;      
    frontier->next  = NULL;
	 
    return frontier; 
}    
    
//______________________________________________________________________________
int Empty(const Queue *const frontier)
{  
   return (frontier==NULL);
}

//______________________________________________________________________________
Node* Pop(Queue **frontier)
{
    Node *node = NULL;
    Queue *temp_queue; 
     
	if(!Empty(*frontier)){
	 	node = (*frontier)->node;
        temp_queue = *frontier;	 	
	 	*frontier = (*frontier)->next;
	 	free(temp_queue);
	} 
	 
	printf("\nPOP: ");
    Print_Node(node);
    printf("\n");

    return node; 
}

//______________________________________________________________________________
void Insert_FIFO(Node *const child, Queue **frontier) 
{  
    Queue *temp_queue;  
    Queue *new_queue = (Queue*)malloc(sizeof(Queue));
    if(new_queue==NULL)
    	Warning_Memory_Allocation(); 

	new_queue->node = child;
	new_queue->next = NULL;
	 
    if(Empty(*frontier))
	 	*frontier = new_queue; 
	else{ // If frontier is not empty, find the last element of the queue.  
		for(temp_queue = *frontier; temp_queue->next!= NULL; temp_queue = temp_queue->next);
	    temp_queue->next = new_queue;		
	}   
}

//______________________________________________________________________________
void Insert_LIFO(Node *const child, Queue **frontier) 
{  
    Queue *new_queue = (Queue*)malloc(sizeof(Queue));
    if(new_queue==NULL)
        Warning_Memory_Allocation(); 

	new_queue->node = child;
	new_queue->next = *frontier;
	*frontier = new_queue;   
}


//______________________________________________________________________________
void Insert_Priority_Queue_UniformSearch(Node *const child, Queue **frontier) 
{  
    Queue *temp_queue;  
    Queue *new_queue = (Queue*)malloc(sizeof(Queue));
    if(new_queue==NULL)
        Warning_Memory_Allocation(); 
        
	new_queue->node = child;
	 
    if(Empty(*frontier)){
         new_queue->next = NULL;                 
	 	*frontier = new_queue; 
    }
	else{ // If frontier is not empty, find appropriate element according to ordered cost. 
	    if(child->path_cost<(*frontier)->node->path_cost){ // Child has lowest cost
	        new_queue->next = *frontier;
            *frontier = new_queue; 
        }
        else{
            for(temp_queue = *frontier; temp_queue->next != NULL; temp_queue = temp_queue->next){
                if(child->path_cost<temp_queue->next->node->path_cost){ 
                    new_queue->next = temp_queue->next;   
                    temp_queue->next = new_queue;
                    return;
                }                                              
            } //If child has highest cost
            temp_queue->next = new_queue;  
            new_queue->next = NULL;                       
        } 		
	}   
}

//______________________________________________________________________________
void Insert_Priority_Queue_GreedySearch(Node *const child, Queue **frontier) 
{  
    Queue *temp_queue;  
    Queue *new_queue = (Queue*)malloc(sizeof(Queue));
    if(new_queue==NULL)
        Warning_Memory_Allocation(); 
        
	new_queue->node = child;
	 
    if(Empty(*frontier)){
         new_queue->next = NULL;                 
	 	*frontier = new_queue; 
    }
	else{ // If frontier is not empty, find appropriate element according to ordered cost. 
	    if(child->state.h_n < (*frontier)->node->state.h_n){ // Child has lowest cost
	        new_queue->next = *frontier;
            *frontier = new_queue; 
        }
        else{
            for(temp_queue = *frontier; temp_queue->next != NULL; temp_queue = temp_queue->next){
                if(child->state.h_n < temp_queue->next->node->state.h_n){ 
                     new_queue->next = temp_queue->next;   
                     temp_queue->next = new_queue;
                     return;
                }                                              
            } //If child has highest cost
            temp_queue->next = new_queue;  
            new_queue->next = NULL;                       
        } 		
	}   
}

//______________________________________________________________________________
void Insert_Priority_Queue_A_Star(Node *const child, Queue **frontier) 
{  
    if (*frontier == NULL) 
    { 
        *frontier = (Queue*)malloc(sizeof(Queue)); 
        (*frontier)->node = child; 
        (*frontier)->next = NULL; 
    } 
    else if ((child->path_cost + child->state.h_n) <= ((*frontier)->node->path_cost + (*frontier)->node->state.h_n)) 
    { 
        Queue *new_node = (Queue*)malloc(sizeof(Queue)); 
        new_node->node = child; 
        new_node->next = *frontier; 
        *frontier = new_node; 
    } 
    else 
    { 
        Queue *temp = *frontier; 
        while (temp->next != NULL && (temp->next->node->path_cost + temp->next->node->state.h_n) < (child->path_cost + child->state.h_n)) 
            temp = temp->next; 

        Queue *new_node = (Queue*)malloc(sizeof(Queue)); 
        new_node->node = child; 
        new_node->next = temp->next; 
        temp->next = new_node; 
    } 
}
//______________________________________________________________________________
void Print_Frontier(Queue *const frontier)
{
	Queue *temp_queue; 
	
	printf("\nQUEUE: [ ");
	for(temp_queue = frontier; temp_queue!= NULL; temp_queue = temp_queue->next){
		Print_Node(temp_queue->node);
		if(temp_queue->next!= NULL)
		printf(" ,");
	}
	printf(" ]\n");   			
}

//______________________________________________________________________________
void Remove_Node_From_Frontier(Node *const old_child, Queue **const frontier) 
{  
    Queue *curr_queue, *prev_queue;  
 
	for(curr_queue = *frontier; curr_queue!= NULL; curr_queue = curr_queue->next){
		if(curr_queue->node == old_child){
	    	//Remove the old child
	    	if(curr_queue==*frontier)  // for the first node
	    	   	*frontier = curr_queue->next;
			else
				prev_queue->next = curr_queue->next; 
		}
		prev_queue = curr_queue;		
	}
}

//______________________________________________________________________________
Node* Frontier_search(Queue *const frontier, const State *const state)
{
	Queue *temp_queue; 
		
	for(temp_queue = frontier; temp_queue!= NULL; temp_queue = temp_queue->next){
		if(Compare_States(&(temp_queue->node->state), state))
			return temp_queue->node;
	}
	return NULL;   			
}

//______________________________________________________________________________
int Frontier_update(Queue *const frontier, const State *const state)
{
	Queue *temp_queue; 
		
	for(temp_queue = frontier; temp_queue!= NULL; temp_queue = temp_queue->next){
		if(Compare_States(&(temp_queue->node->state), state))
			return TRUE;
	}
	return FALSE;   			
}

//______________________________________________________________________________
void Print_Node(const Node *const node)
{
	if(node!=NULL){
		printf("NODE(");
		Print_State(&(node->state));
		if(node->parent){
			printf(", parent:");
			Print_State(&(node->parent->state));
			printf(", action:");
			Print_Action(node->action);	
			printf(", path_cost: %.1f )", node->path_cost);	
		}
		else
			printf(":root)");
	}
	else
	   printf("NODE:NULL"); 	
}

//______________________________________________________________________________
void Show_Solution_Path(Node *const goal)
{   
    Node *temp;
	if(goal==FAILURE)
		printf("THE SOLUTION CAN NOT BE FOUND.\n");
	else{
		printf("\nTHE COST PATH IS %.2f.\n", goal->path_cost);
		printf("\nTHE SOLUTION PATH IS:\n");
		for(temp = goal; temp!= NULL; temp = temp->parent)
		{
			Print_State(&(temp->state));
			if(temp->parent!= NULL){
				printf("\n\taction(");
                Print_Action(temp->action);
                printf(")\n");
            }			
		}
	}		
}

//______________________________________________________________________________
int Level_of_Node(Node *const node)
{
    int counter = 0;
    Node *temp = node;
    while(temp->parent!=NULL){
        temp = temp->parent;
        counter++;                     
    }
    return counter;
}

//______________________________________________________________________________
void Clear_All_Branch(Node *node, int *Number_Allocated_Nodes)
{
    Node *parent = node->parent;
    if(Level_of_Node(node)==0)
    	return;
    	
    Clear_Single_Branch(node, Number_Allocated_Nodes);
    
    if(parent->Number_of_Child==0) // Clear nodes having no child. 
    	Clear_All_Branch(parent, Number_Allocated_Nodes);
}

void Clear_Single_Branch(Node *node, int *Number_Allocated_Nodes)
{
    if(Level_of_Node(node)==0)
    	return;
	
	printf("\nCLEARING: ");
	Print_Node(node);
	printf("\n"); 
	node->parent->Number_of_Child--;	  
    free(node);
    (*Number_Allocated_Nodes)--; 
}

//______________________________________________________________________________
void Warning_Memory_Allocation()
{
    printf("The memory Error in alloacation process! Press a key to exit.\n");
    exit(-1);
}

//______________________________________________________________________________
int Goal_Test(const State *const state, const State *const goal_state)
{	
	return Compare_States(state, goal_state); 
}

//______________________________________________________________________________
int Compare_States(const State *const state1, const State *const state2)
{
	unsigned char key1[MAX_KEY_SIZE], key2[MAX_KEY_SIZE];
	Generate_HashTable_Key(state1, key1);
	Generate_HashTable_Key(state2, key2);	
	return !strcmp(key1, key2); 
}


