/*
  The implementation of graph algorithms module.
  
  PrimMST - computes an MST using Prim's method
  shortestPath - computes (and returns) the shortest
                 path between two nodes using Dijkstra's
                 method
*/

#include <math.h>
#include "graph_algorithms.h"
#include "my_heaps.h"

void PrimMST(float A[GsizeSqr][GsizeSqr])
{
 /*
   This function carries out Prim's algorithm
   and produce an MST from the graph represented by the adjacency
   matrix A by using priority heaps. Update A[][] to
   contain the MST, and also make sure that all edges in the
   MST have weight 1. In other words, after we compute the MST
   we will no longer care about random weights since the cost
   of moving from one space in the maze to the next should
   always be one.
 */

  int p[GsizeSqr];  // p[u] stores the index of node v that is linked to u
                  //  and w(p[u], u) is minimized! This is the MST!
  float priority[GsizeSqr]; // store priority, minimum weight of edge between u,v

  int PQ[GsizeSqr+1];  // index starts at 1

  PQ[0] = -1;        // we don't use index 0 in PQ. PQ[0]is set to -1

  int PQsize = GsizeSqr+1;   // initially PQ stores all nodes plus -1 in index 0
                             // it'd be updated as MST grows

  int abitraryRoot;         // an arbitrary node we will pick as the root of PQ
  
  int u;                  // the first node with max priority in PQ
  int v;                  // the neighbor node of u
  
  int adjacency_list[4] = {-1, -1, -1, -1};  // store up, down, left and right
                                             // neighbor-nodes of u
                                             // initialized to -1
  int adj_index = 0;           // keep tracking the index of adjacency_list

  int visitedFlag[GsizeSqr];
 
 
  // initialize visitedFlag with 0, marking all nodes unvisited! 
  memset(visitedFlag, 0,(GsizeSqr)*sizeof(int));
 
  for (int i= 0; i<GsizeSqr; i++) // initialization
  {
     priority[i] = 1.0e10;  // make it really big number!
     p[i] = -1;            // -1 is equivalent to NIL
     PQ[i+1] = i;        // store each node starting from index 1
                       // it's also considered as initializing PQ, since each
                       // node contains the same priority value.
  }

  abitraryRoot = 1;          // pick an arbitray node.

  priority[abitraryRoot] = 0;       // set priority to max!
  
   // do swap between the index 1 and arbitrary root, i.e., updating PQ
  int tempIndex = search(PQ, &PQsize, abitraryRoot);                     
  PQ[tempIndex] = PQ[1];
  PQ[1] = abitraryRoot;
 
  while (!(isEmpty(&PQsize)))
  {
     u = extractMax(PQ, &PQsize, priority);  // get the first top priority node

     visitedFlag[u] = 1;    // mark u as visited
  
    if ((u-Gsize) >= 0)       adjacency_list[adj_index++] = u-Gsize;  // up 
    if ((u+Gsize) <= 399)     adjacency_list[adj_index++] = u+Gsize;  // down
    if ((u % Gsize) != 0)     adjacency_list[adj_index++] = u-1;  // left
    if (((u+1) % Gsize) != 0) adjacency_list[adj_index++] = u+1;  // right

    for (int i=0; i<4; ++i)
    {
       v = adjacency_list[i];     // obtain a neighbor node

       if (v == -1) break;   // if it's not a neighbor, jump out of for loop
       if (visitedFlag[v]) continue; // if visisted, skip!

       int k = search(PQ, &PQsize, v);   // find the index of v in PQ

       if (k != -1)         // found v in PQ
       {
          if (A[u][v] < priority[v])
          { 
              p[v] = u;
              priority[v] = A[u][v];        // decreasing priority
              
              // it's important to update PQ whenever priority is modified!
              heapifyUpward(PQ, &PQsize, priority, k);  
          }
	   }
    } // end of for loop checking neighbors

   for(int f = 0; f < 4; ++f) adjacency_list[f]=-1;       // reset neigbour list!
   adj_index = 0;     // reset index of adjacent_list. reuse it again!      

  } // end of while loop, don't delete!
 
  // finally we are here! We have MST now. The following steps are updating A[][] 

  // Zero out adjacency matrix, and be ready to update it
  memset(A,0,GsizeSqr*GsizeSqr*sizeof(float));

  for (int j=0; j<GsizeSqr; j++) {         // go through each node in p[]
	  if (j == abitraryRoot) continue;     // do not update the first node we have chosen
	  A[p[j]][j] = 1.0;                    // weight is set to 1.0
	  A[j][p[j]] = 1.0;                    // update symentric node as well!
  }

} // end of PrimST(), do not delete!

struct path_node *shortestPath(float A[GsizeSqr][GsizeSqr], int xi, int yi, int xf, int yf)
{
 /*
   Solves for the shortest path between grid point (xi,yi) and (xf,yf)
   on the graph encoded by A using Dijkstra's shortest path method using priority
   heaps.   Produces a linked list of nodes describing the path from (xi,yi) to 
   (xf,yf) including the endpoints. The list describes the path to be followed in
   the correct order and will be used by the main function to update agent positions.

   The shortest path is returned as a linked list of nodes to be visited.

   In general, we need to keep track of visited nodes, and the predecessor
   for each node that has been explored while computing the shortest path.

 */

 if (xi<0||xi>=Gsize&&yi<0&&yi>=Gsize||xf<0||xf>=Gsize||yf<0||yf>=Gsize)
 {
  fprintf(stderr,"shortestPath(): Endpoint(s) outside of the graph!\n");
  return(NULL);
 }

  int p[GsizeSqr];  // predecessor list. p[u] stores the index node v that is linked to u
                  //  and w(p[u], u) is minimizd.
  float d[GsizeSqr]; // store distance. d[u] store the distance from start node to u

  int PQ[GsizeSqr+1];  // index starts at 1

  PQ[0] = -1;        // we don't use index 0 in PQ. PQ[0]is set to -1

  int PQsize = GsizeSqr+1;   // initially PQ stores all nodes plus -1 in index 0
                             // it'd be updated as MST grows
  
  int u;                  // the current node we are working on 
  int v;                  // the neighbor node
  
  int adjacency_list[4] = {-1, -1, -1, -1};  // store up, down, left and right
                                             // neighbor-nodes
                                             // initialized to -1
  int adj_index = 0;           // keep tracking the index of adjacency_list

  int visitedFlag[GsizeSqr]; 
 
  // initialize visitedFlag with 0, marking all nodes unvisited! 
  memset(visitedFlag, 0,(GsizeSqr)*sizeof(int));
 
  for (int i= 0; i<GsizeSqr; i++) // initialization
  {
     d[i] = 1.0e10;  // make it really big number!
     p[i] = -1;            // -1 is equivalent to NIL
     PQ[i+1] = i;        // store each node starting from index 1
                       // it's also considered as initializing PQ, since each
                       // node contains the same priority value.
  }
  
  /* there is a mathmatical relationship between x, y and node */
  int startnode = yi * Gsize + xi;          // start node in the path
  int finalnode = yf * Gsize + xf;          // end node in the path

  d[startnode] = 0;       // set start node distance with 0
  
   //swap. This is an alternative way to update PQ! since all with same distance.
  int tempIndex = search(PQ, &PQsize, startnode);                     
  PQ[tempIndex] = PQ[1];
  PQ[1] = startnode;
  
  //if (xi==xf && yi==yf) return newPathNode(xi, yi);
  if (xi==xf && yi==yf) return NULL;


  //key part of Djkstra's algoritm starts here!
  while (!(isEmpty(&PQsize)))
  {
     //printf("while loop - PQsize=%d\n", PQsize);         // debug

     u = extractMax(PQ, &PQsize, d);  // get the first node with minimum distance from PQ

     visitedFlag[u] = 1;    // mark u as visited

     if (u == finalnode) break;          // get to the final node
  
     if ((u-Gsize) >= 0)       adjacency_list[adj_index++] = u-Gsize;  // up 
     if ((u+Gsize) <= 399)     adjacency_list[adj_index++] = u+Gsize;  // down
     if ((u % Gsize) != 0)     adjacency_list[adj_index++] = u-1;  // left
     if (((u+1) % Gsize) != 0) adjacency_list[adj_index++] = u+1;  // right

     for (int i=0; i<4; ++i)
     {

       v = adjacency_list[i];     // obtain a neighbor node

       if (v == -1) break;   // if it's not a neighbor, jump out of for loop
       if (visitedFlag[v]) continue; // if visited, skip

	   if (A[u][v] == 0) continue;   // no path between u and v. skip

       int k = search(PQ, &PQsize, v);   // find the index of v in PQ

       if (k != -1)         // found v in PQ
       {
	     
          if (d[v] > (d[u] + A[u][v]))
          { 
              d[v] = d[u] + A[u][v];       // update distance of v
              p[v] = u;        // v's predecessor setting to u
              
              // it's important to update PQ whenever distance is modified!
              heapifyUpward(PQ, &PQsize, d, k);  
          }
	   }
    } // end of for loop checking neighbors

   for(int f = 0; f < 4; ++f) adjacency_list[f]=-1;       // reset neigbour list!
   adj_index = 0;     // reset index of adjacent_list. reuse it again!      

  } // end of while loop, don't delete!
 
  // finally we are here! We have predecessor list now. And we build a linked list tracing 
  // back from finalnode to startnode

  u = finalnode;
  struct path_node *unode= newPathNode(u%Gsize, u/Gsize);
  while (p[u] != -1)
  {
    /* math: x = node % Gsize; y= node / Gsize */
    /* create new node for u's predecessor */  
	struct path_node *prevnode = newPathNode((p[u])%Gsize, (p[u])/Gsize);
	prevnode->next = unode;   

    unode = prevnode;
	u = p[u];  // classical! tracing back.
  }

  return unode;   // it must be start node. since p[u] = -1
   
} // end of shortestPath. Don't delete!
