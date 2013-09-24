/*
  This is the header file for the implementation of
  priority queues using heaps.
*/

#ifndef __heaps_header
#define __heaps_header

#include<stdio.h>
#include<stdlib.h>
#include "GameAI.h"

/*
  Define new type of data: Bool, increasing readability 
*/
tyepdef enum {NO=0, YES} Bool; 

/*
  Search a node in PQ and return its index in PQ. if the 
  node doesn't exist, return 0.
  back towards to index 1
  Input: PQ[GsizeSqr+1], int node
  Return: index of node in PQ
*/
int search(int PQ[], int *PQsize, int node );

/*
  Heapify the PQ starting from index i and working
  down towards to the end of PQ
  Input: PQ[], PQsize, priority[], index i
  Return: None
*/
void heapifyDownward(int PQ[], int *PQsize, float priority[], int i);

/*
  Heapify the PQ starting from index i and working
  up towards to the front of PQ. it's used when node's priority is being 
  decreased in PrimMST().
  Input: PQ[], PQsize, priority[], index i
  Return: None
*/
void heapifyUpward(int PQ[], int *PQsize, float priority[], int i);

/*
  Remove the element at the front queue and return it.
  Input: PQ[GsizeSqr+1], index starts at 1, so the 
         size of PQ is 401, priority[]
  Return: the element at index 1 of queue
*/
int extractMax(int PQ[], int *PQsize, float priority[]);

/*
  Check if PQ is empty, return FALSE if it's empty

*/
int isEmpty(int *PQsize); 



#endif
