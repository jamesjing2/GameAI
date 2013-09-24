/*
  The header file for the module that provides
  functionality to the cats and mouse in Game maze.
*/

#ifndef __kittymice_header
#define __kittymice_header

#include "graph_management.h"
#include "graph_algorithms.h"
#include "GameAI.h"

struct path_node *kittyEyeWalk(int GamePos[numAgents][3], float A[GsizeSqr][GsizeSqr], int kitty);
struct path_node *kittySmellWalk(int GamePos[numAgents][3], float A[GsizeSqr][GsizeSqr], int kitty);
struct path_node *kittySmartSmell(int GamePos[numAgents][3], float A[GsizeSqr][GsizeSqr], int Target[numAgents][2], int kitty);
struct path_node *kittySoundWalk(int GamePos[numAgents][3], float A[GsizeSqr][GsizeSqr], int Target[numAgents][2], int kitty);
struct path_node *EvilKittiesOfDoom(int GamePos[numAgents][3], float A[GsizeSqr][GsizeSqr], int kitty);
struct path_node *superKitty(int GamePos[numAgents][3], float A[GsizeSqr][GsizeSqr], int Target[numAgents][2], int kitty);
struct path_node *mouseWalk(int GamePos[numAgents][3],float A[GsizeSqr][GsizeSqr]);
struct path_node *mouseEscape(int GamePos[numAgents][3],float A[GsizeSqr][GsizeSqr]);
int main(int argc, char *argv[]);	// Note that main() lives here!

#endif
