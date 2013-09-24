/*
  The header file for the graph algorithms
  module to implement two graph manipulation algorithms:

  PrimMST - computes an MST using Prim's method
  shortestPath - computes (and returns) the shortest
                 path between two nodes using Dijkstra's
                 method
*/

#ifndef __graph_algorithms_header
#define __graph_algorithms_header

#include "graph_management.h"

void PrimMST(float A[GsizeSqr][GsizeSqr]);
struct path_node *shortestPath(float A[GsizeSqr][GsizeSqr], int xi, int yi, int xf, int yf);

#endif
