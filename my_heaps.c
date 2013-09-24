/*
  The implementation of priority queues using heaps.
*/

#ifndef __heaps_header
#define __heaps_header

#include<stdio.h>
#include<stdlib.h>
#include "my_heaps.h"

/*
  Search a node in PQ and return its index in PQ. if the 
  node doesn't exist, return 0. Scan back towards to index 1
  Input: PQ[], PQsize, node
  Return: index of node in PQ
*/
int search(int PQ[], int *PQsize, int node)
{
	int i=1;  // index starts from 1

	while ((i < *PQsize) && (PQ[i] != node))
	{
		++i;
	}

	if (i >= *PQsize) return -1;    // not find           
	else return i;        // node's index in PQ
}

int isEmpty(int *PQsize) 
{  
   if (*PQsize <= 1 ) return 1;
   else return 0;
}

/*
  Heapify the PQ starting from index i and working
  down towards to the end of PQ
  Input: PQ[], PQsize, priority[], index i
  Return: None
*/
void heapifyDownward(int PQ[], int *PQsize, float priority[], int i)
{  
   if (*PQsize > 1 && (i >=1 && i < *PQsize)) 
   {
     int smallest = i;           // be carefull: index starts from 1. Not 0!
	
     if (2*i <= *PQsize && priority[PQ[2*i]] < priority[PQ[i]])  smallest = 2*i;
     if (2*i+1 <= *PQsize && priority[PQ[2*i+1]] < priority[PQ[smallest]])  smallest = 2*i+1;

     if (smallest != i)
     {
        int temp = PQ[i];               // do swapping
        PQ[i] = PQ[smallest];
        PQ[smallest] = temp;
        heapifyDownward(PQ, PQsize, priority, smallest);
     }

  } // end of first condition checking
} // end of heapifyDownward()

/*
  Heapify the PQ starting from index i and working
  up towards to the front of PQ. it's used when node's priority is being 
  decreased in PrimMST().
  Input: PQ[], PQsize, priority[], index i
  Return: None
*/
void heapifyUpward(int PQ[], int *PQsize, float priority[], int i)
{  
   if (*PQsize > 1 && (i >=1 && i < *PQsize)) 
   {
     int smallest = i;           // be carefull: index starts from 1. Not 0!
	
     if (i/2 > 0 &&  priority[PQ[i/2]] > priority[PQ[i]])  smallest = i/2;

     if (smallest != i)
     {
        int temp = PQ[i];               // do swapping
        PQ[i] = PQ[smallest];
        PQ[smallest] = temp;
        heapifyUpward(PQ, PQsize, priority, smallest);
     }

  } // end of first condition checking
} // end of heapifyDownward()

/*
  Remove the element at the front queue and return it.
  Input: PQ[GsizeSqr+1], index starts at 1, so the 
         size of PQ is 401
  Return: the element at index 1 of queue
*/
int extractMax(int PQ[], int *PQsize,  float priority[]) 
{
   if (*PQsize > 1) 
   {
     int root = PQ[1];
  
     PQ[1] = PQ[*PQsize -1];    // take the last node to the root

     --(*PQsize);  // the PQ size is reduced by 1. 
                // it's very important to update PQsize as MST grows!
   
     heapifyDownward(PQ, PQsize, priority, 1);
   
     return root;
   }
   else return -1;        // -1 means PQ is out of range. 
}


#endif
