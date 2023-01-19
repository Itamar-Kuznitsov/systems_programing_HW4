#include <stdio.h>
#include <stdlib.h>
#include "nodes.h"

// function that add node:
char add_node(pnode head){
  int id = 0;
  int exist = -1;
  char c;
  
  // gets the number of the new node 
  scanf(" %d", &id); 
  
  pnode existNode = head;
  while (existNode != NULL){
    // if the node does exsits, we'll remove its edges.
    if (existNode->id == id){ 
      remove_edges(existNode->edges);
      exist = id;
      break;
    }
    existNode = existNode->next;
  }

  // if the node does not exist, simply add it and add the new edges.
  if (exist == -1){
    pnode last = head;
    while (last->next != NULL){
      last = last->next;
    }
    // creating new node
    pnode newNode = (struct GRAPH_NODE_ *)malloc(sizeof(struct GRAPH_NODE_));
    newNode->id = id;
    newNode->next = NULL;
    newNode->cost = INF;      
    newNode->visited = FALSE; 
    newNode->edges = NULL;
    newNode->edges = (pedge)malloc(sizeof(edge)); // set list
    
    if (newNode->edges == NULL){
      printf("Unable to allocate memory to curr node %d", id);
    }

    newNode->edges->endpoint = newNode;
    newNode->edges->weight = 0;
    newNode->edges->next = NULL;
    last->next = newNode;
    c = add_edges(newNode->edges, head);
  }

  else{
    // after removing the edges of a an exsistent node, add the edges!
    c = add_edges(existNode->edges, head);
  }
  return c;
}

// function that getting the right edge:
void checkEdges(pedge *edge, int toRemov){
  pedge prev, temp;
  pedge currEdge = *edge;

  while (currEdge != NULL){

    if (currEdge->endpoint != NULL){
      if (currEdge->endpoint->id == toRemov){
        if (currEdge == *edge){
          temp = (*edge)->next;
          free(*edge);
          *edge = temp;
        }

        else{
          prev->next = currEdge->next;
          free(currEdge);
          currEdge = prev;
        }
      }
    }
    prev = currEdge;
    currEdge = currEdge->next;
  }
}

// function that delete given node: 
void deleteNode(pnode *graph){
  pnode head = *graph;
  char c;
  scanf(" %c", &c);
  int toRemov = c - '0';
  pnode prev;
  pnode node;
  node = *graph;

  while (node != NULL){
    checkEdges(&(node->edges), toRemov);
    node = node->next;
  }

  if (head->id == toRemov){
    *graph = head->next;
    remove_edges(head->edges);
    // free the first edge
    free(head->edges);  
    free(head);
  }

  else{
    node = head;
    while (node != NULL && node->id != toRemov){
      prev = node;
      node = node->next;
    }

    // if the node is not in the graph 
    if (node == NULL)
      return; 

    remove_edges(node->edges);
    free(node->edges); // because remove_edges doesnt free() the the first edge
    prev->next = node->next;
    free(node);
  }
}
