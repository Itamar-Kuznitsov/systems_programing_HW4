#include <stdio.h>
#include <stdlib.h>
#include "algo.h"

// consts:
int node_size;
int min = INF; 

//  function that build graph:
char build_graph_cmd(pnode head){
    int i = 0;
    int id = 0;
    char c;

    //inital the head:
    head->id = 0;
    head->next = NULL;
    head->cost = INF;      // for dijkstra
    head->visited = FALSE; // for dijkstra
    head->edges = NULL;
    head->edges = (pedge)malloc(sizeof(edge)); // set list
    if (head->edges == NULL){
        printf("Unable to allocate memory to curr node: %d", i);
    }

    head->edges->endpoint = head;
    head->edges->weight = 0; // zero = no edge. (other wise dijkstra will be messed up)
    head->edges->next = NULL;
    
    //initial over nodes:
    pnode currNode = head;
    for (i = 1; i < node_size; ++i){
        // allocating memory for the nodes
        pnode newNode = (struct GRAPH_NODE_ *)malloc(sizeof(struct GRAPH_NODE_));
        newNode->id = i;
        newNode->next = NULL;
        newNode->edges = NULL;
        newNode->edges = (pedge)malloc(sizeof(edge)); // set list
        if (newNode->edges == NULL){
            printf("Unable to allocate memory to curr node: %d", i);
        }

        newNode->edges->endpoint = newNode;
        newNode->edges->weight = 0; // zero = no edge (other wise dijkstra is messed up)
        newNode->edges->next = NULL;
        newNode->cost = INF;      // dijkstra
        newNode->visited = FALSE; // dijksrta
        currNode->next = newNode;
        currNode = newNode;
    }

    // get 'n'
    scanf(" %c", &c);
    while (c == 'n'){
        pnode tmp = head;
        // gets the number of the node
        scanf("%d", &id); 
        while (tmp->id != id){
            tmp = tmp->next;
        }
        // adds the edges, and returns the next n from the edges if one exists.
        c = add_edges(tmp->edges, head); 
    }
    return c;
}

// function that print the graph: 
void printGraph_cmd(pnode head){
    pnode tmp = head;
    while (tmp != NULL){
        printf("-------node number: %d -------\n", tmp->id);
        pedge curr_edge = tmp->edges;
        
        while (curr_edge != NULL){
            if (curr_edge->weight != 0){
                printf("src: %d dest: %d weight: %d\n", tmp->id, curr_edge->endpoint->id, curr_edge->weight);
            }
            curr_edge = curr_edge->next;
        }
        tmp = tmp->next;
    }
}

// dijkstra algo for calculating the shortest path:
int dijkstra(pnode head, int src, int dest, int flag){
    pnode pp, set_src;
    int visitednum = 0; // checker if visited all nodes
    set_src = head;

    while (set_src->id != src){
        set_src = set_src->next;
    }
    // set src value to 0, because we want to start from it and increase while checking the other nodes.
    set_src->cost = 0; 
    while (TRUE){
        int min = INF;
        
        // Keeping node which has minimum cost
        pnode targetnode = NULL;
        pp = head;
        while (pp != NULL){
            if (pp->cost < min && pp->visited == FALSE){
                min = pp->cost;
                targetnode = pp;
            }
            pp = pp->next;
        }

        if (targetnode == NULL){
            pp = head;
            while (TRUE){
                if (pp->visited == FALSE)
                {
                    targetnode = pp;
                    break;
                }
                pp = pp->next;
            }
        }

        
        //  calculate cost of each node which has in edge relaxation proccess
        if (targetnode->edges != NULL)
        {
            if (targetnode->edges->endpoint != NULL)
            {
                if (targetnode->edges->endpoint->cost > targetnode->cost + targetnode->edges->weight)
                {
                    targetnode->edges->endpoint->cost = targetnode->cost + targetnode->edges->weight;
                }
            }
            
            //  If there's more than one edges connected to node.
            pedge pptemp;
            pptemp = targetnode->edges->next;
            while (pptemp != NULL)
            {
                if (pptemp->endpoint->cost > targetnode->cost + pptemp->weight)
                {
                    pptemp->endpoint->cost = targetnode->cost + pptemp->weight;
                }
                pptemp = pptemp->next;
            }
        }
    
        //  change visited flag
        targetnode->visited = TRUE;

        visitednum++;
        if (visitednum == node_size)
        {
            break;
        }
    }

    //  find distination node
    pnode get_dest = head;
    int totalcost = 0;
    while (get_dest != NULL)
    {
        if (get_dest->id == dest)
        {
            totalcost = get_dest->cost;
            break;
        }
        get_dest = get_dest->next;
    }
    if (flag == 1){ 
        // dijkstra case.
        if (totalcost == INF)
            totalcost = -1;
        printf("Dijsktra shortest path: %d \n", totalcost);
    }

    // resets the data after each dijkstra usage.
    pnode reset = head;
    while (reset != NULL)
    {
        reset->cost = INF;
        reset->visited = FALSE;
        reset = reset->next;
    }
    return totalcost; // used for tsp.
}

//function to check the array minimum cost.
void tsp(pnode head, int cities[], int size){
    int tmp_min = 0;  // temporary weight 
    int curr_id = cities[0]; 
    int next_id = 0;         
    
    // check other connected nodes weights.
    for (int i = 1; i < size; ++i)
    {
        next_id = cities[i];                            // dst
        tmp_min += dijkstra(head, curr_id, next_id, 0); // using dijsktra sums the shortest path to all.
        curr_id = cities[i];                            // src
    }
    if (min > tmp_min)
    {
        min = tmp_min;
    }
}

//function to swap the variables
void swap(int *a, int *b){
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

//permutation function
void permutation(pnode head,int *arr, int start, int end){
    if (start == end)
    {
        tsp(head, arr, end + 1);
        return;
    }
    int i;
    for (i = start; i <= end; i++)
    {
        // swapping numbers
        swap((arr + i), (arr + start));
        // fixing one first digit
        // and calling permutation on
        // the rest of the digits
        permutation(head, arr, start + 1, end);
        swap((arr + i), (arr + start));
    }
}

// function that free the graph memory 
void freeGraph(pnode *graph)
{
    if ((*graph)->next == NULL)
    {
        remove_edges((*graph)->edges);
        free((*graph)->edges);
        free((*graph));
        return;
    }
    pnode currNode = (*graph);
    pnode temp;
    while (currNode != NULL)
    {
        remove_edges(currNode->edges);
        free(currNode->edges);
        temp = currNode->next;
        free(currNode);
        currNode = temp;
    }
}

// main function
int main(){
  char c;
  pnode *graph;
  struct GRAPH_NODE_ *head = NULL;
  graph = &head;
  // what the user choose to do.
  scanf("%c", &c); 
  
  while (c != '\n'){
    if (c == ' '){
            scanf("%c", &c);
    }

    // A case, creating a new graph.
    if (c == 'A'){
            if (*graph != NULL){
                freeGraph(graph);
            }

            // how many nodes int the graph
            scanf("%d", &node_size); 
            // allocating memory for the nodes
            head = (struct GRAPH_NODE_ *)malloc(node_size * sizeof(struct GRAPH_NODE_));
            graph = &head;
            if (head == NULL){
                printf("Unable to allocate memory to head");
                exit(1);
            }
            c = build_graph_cmd(head); // builds a graph of N nodes & all its edges.
    }

    // P case, print the graph.
    if (c == 'P'){
        // printing for self test.
        printGraph_cmd(head);
        scanf("%c", &c);
        printf("\n");
    }

    // E case, exit state.
    if (c == 'E'){ 
        break;
    }

    // B case, adding new node.
    if (c == 'B'){
        c = add_node(*graph);
    }

    // D case, delede spesific node.
    if (c == 'D'){
        deleteNode(graph);
        head = *graph;
        scanf("%c", &c);
    }

    // S case, shortest path beatween two nodes.    
    if (c == 'S'){
        int src, dst;
        scanf("%d", &src);
        scanf("%d", &dst);
        // calling dijkstra algo, 1 wether to print the path or not.
        dijkstra(head, src, dst, 1); 
        scanf("%c", &c);
    }

    // T case, finding TSP shortest path.
    if (c == 'T'){
        // save how much nodes nead to pass 
        int size;
        scanf("%d", &size);
        int i;
        int cities[size];

        // getting cities.
        for (i = 0; i < size; i++) 
            scanf("%d", &cities[i]);
        
        // calling permutation function
        permutation(*graph, cities, 0, size - 1);
        if (min == INF)
            min = -1;
        
        printf("TSP shortest path: %d \n", min);
        min = INF; // reset min to inf after altered it in the program.
        scanf("%c", &c);
    }
  }

  freeGraph(graph);
}
