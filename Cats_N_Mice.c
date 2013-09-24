/*
  This is the implemnentation that provides functionality to the cats 
  and mouse.Four files listed below are needed for this module:
  graph_management.h
  gameAI.h
  graph_algorithms.h
  graph_alogorithms.c
*/

#include "Cats_and_Mice.h"

// Array to store a flag indicating the Target array has been used in SoundWalk
int flagTarget[numAgents] = {0, 0, 0, 0, 0, 0, 0, 0};

// it indicates the next move is randomly selected.
int flagRamdomPos = 0;

struct path_node *kittyEyeWalk(int GamePos[numAgents][3], float A[GsizeSqr][GsizeSqr], int kitty)
{
 /*
   Kitty walk based on vision. The kitty behaviour is as follows:

   If the mouse is visible to the cat (aligned horizontally or
   vertically, *with no walls in between*), the kitty will move
   toward the mouse.

   Otherwise, the kitty will choose randomly from available
   neighbours.
   
   This function returns a *single* node path with
   the next position for the corresponding kitty.
 */

 struct path_node *path=NULL;
 struct path_node *head=NULL;

 int distance=0;  // distance between the kitty and the mouse

 // store up, down, left and right neighbor-nodes of kitty , [:][0]-x, [:][1]-y
 // initialize to -1 meaning no neibours
 int adjacency_list[4][2] = {-1, -1, -1, -1, -1, -1, -1, -1}; 

 int adj_index = 0;  // keep tracking the index of adjacency_list

 path=shortestPath(A,GamePos[kitty][0],GamePos[kitty][1],GamePos[1][0],GamePos[1][1]);

 if (path)
 {
   head = path;
   while(head->next)
   {
     distance++;
     head = head->next;
   }
   
   // the followings are based on a mathematical theorithm: the shortest
   // distance between two vertices is a straight line connecting them.
   // if two verices' horizontal distance is equal to shortest path, then it
   // means there is no walls between them horizontally. The same applys to 
   //vetical distance.

   if ((abs(GamePos[1][0] - GamePos[kitty][0]) == distance) ||
       (abs(GamePos[1][1] - GamePos[kitty][1]) == distance))
   {
	   head = newPathNode(path->next->x, path->next->y);  
       deletePath(path);      //delete node. prevent memory leak!

       flagRamdomPos = 0;  // it indicates that a ramdom position is being selected.
	   return head; 
   }
   // kitty randomly moves to one of its neigbours

   // loop up four neibours, check if their existance
   // up neighbor (x, y)
 
   if (GamePos[kitty][1]>0 &&
        (A[GamePos[kitty][1]*Gsize+GamePos[kitty][0]][(GamePos[kitty][1]-1)*Gsize+GamePos[kitty][0]])==1.0)
   {
       adjacency_list[adj_index][0] = GamePos[kitty][0];  // up -x
       adjacency_list[adj_index][1] = GamePos[kitty][1]-1;  // up -y
       adj_index++;
   } 
   // down neighbor (x, y)
   if (GamePos[kitty][1]<19 &&
        (A[GamePos[kitty][1]*Gsize+GamePos[kitty][0]][(GamePos[kitty][1]+1)*Gsize+GamePos[kitty][0]])==1.0)
   {
       adjacency_list[adj_index][0] = GamePos[kitty][0];  // down -x
       adjacency_list[adj_index][1] = GamePos[kitty][1]+1;  // down -y
       adj_index++;
   } 
   // left neighbor (x, y)
   if (GamePos[kitty][0]>0 &&
       (A[GamePos[kitty][1]*Gsize+GamePos[kitty][0]][GamePos[kitty][1]*Gsize+(GamePos[kitty][0]-1)])==1.0)
   {
       adjacency_list[adj_index][0] = GamePos[kitty][0]-1;  // left -x
       adjacency_list[adj_index][1] = GamePos[kitty][1];  // left -y
       adj_index++;
   } 
   // right neighbor (x, y)
   if (GamePos[kitty][0]<19 &&
        (A[GamePos[kitty][1]*Gsize+GamePos[kitty][0]][GamePos[kitty][1]*Gsize+GamePos[kitty][0]+1])==1.0)
   {   
       adjacency_list[adj_index][0] = GamePos[kitty][0]+1;  // right -x
       adjacency_list[adj_index][1] = GamePos[kitty][1];  // right -y
       adj_index++;
   }
     
   if (adj_index > 0) // found at least one neigbour
   {
       // generator a random number between [0, adj_index-1] inclusive
       adj_index = (int)(drand48()*(adj_index-1+0.99));
       path = newPathNode(adjacency_list[adj_index][0], adjacency_list[adj_index][1]); 

       flagRamdomPos = 1;  // it indicates that a ramdom position is being selected.
	   return path;
   }

 } // end of fist if, (no path found).

 return NULL;    // no shortestPath found or no valid neibours
} // end of kittyEyeWalk()

struct path_node *kittySmellWalk(int GamePos[numAgents][3], float A[GsizeSqr][GsizeSqr], int kitty)
{
 /*
   Kitty walk based on smell. Expected behaviour:

   The kitties don't know where the mouse is, but they can use 'smell' to determine whether
   neighboring positions in the grid are closer or farther away from the mouse.

   The kitties will move in whichever valid direction minimizes distance to the mouse.
   If two directions result in the same distance, choose either.

   All distances are Manhattan distances.

   Kitties can get stuck... to un-stick them, see kittySmartSmell()
 */

 struct path_node *path=NULL;
 int dx, dy;     // store distance in x-axis, and y-axis 

 dx = GamePos[kitty][0] - GamePos[1][0]; // get x-axis distance 
 dy = GamePos[kitty][1] - GamePos[1][1];  // get y-axis distance

 // if we consider the game grid layou as a x-y quardrants, and the kitty is vertex
 // there are totally eight cases that a mouse may fall into. 

 if(dx > 0 && dy > 0)  // case 1: the mouse is in 2nd quardrant
 {
	// if the mouse is in 2nd quardrant, the kitty can only move to
	//left or up in order to be closer to the mouse
	if(A[GamePos[kitty][1]*Gsize+GamePos[kitty][0]][(GamePos[kitty][1]-1)*Gsize+GamePos[kitty][0]]==1.0)
	{	// if there is no wall to up, then move to up
		path = newPathNode(GamePos[kitty][0], GamePos[kitty][1]-1);
		return path;
	}
    if(A[GamePos[kitty][1]*Gsize+GamePos[kitty][0]][GamePos[kitty][1]*Gsize+(GamePos[kitty][0]-1)]==1.0)  
	{	// if there is no wall to left, then move to left
	   path = newPathNode(GamePos[kitty][0]-1, GamePos[kitty][1]); 
	   return path;
	}
 }
 if(dx > 0 && dy < 0)  // case 2: the mouse is in 3rd quardrant
 {
	if(A[GamePos[kitty][1]*Gsize+GamePos[kitty][0]][(GamePos[kitty][1]+1)*Gsize+GamePos[kitty][0]]==1.0)
	{    
		path = newPathNode(GamePos[kitty][0], GamePos[kitty][1]+1);  // move down
		return path;
	}
    if(A[GamePos[kitty][1]*Gsize+GamePos[kitty][0]][GamePos[kitty][1]*Gsize+(GamePos[kitty][0]-1)]==1.0)  
	{	
		path = newPathNode(GamePos[kitty][0]-1, GamePos[kitty][1]); // move left
		return path;
	}
 }
 if(dx < 0 && dy > 0)  // case 3: the mouse is in 1st quardrant
 {
	if(A[GamePos[kitty][1]*Gsize+GamePos[kitty][0]][(GamePos[kitty][1]-1)*Gsize+GamePos[kitty][0]]==1.0)
	{	
		path = newPathNode(GamePos[kitty][0], GamePos[kitty][1]-1); // move up
		return path;
	}
    if(A[GamePos[kitty][1]*Gsize+GamePos[kitty][0]][GamePos[kitty][1]*Gsize+(GamePos[kitty][0]+1)]==1.0)  
	{	
		path = newPathNode(GamePos[kitty][0]+1, GamePos[kitty][1]); // move right
		return path;
	}
 }
 if(dx < 0 && dy < 0)  // case 4: the mouse is in 4th quardrant
 {
	if(A[GamePos[kitty][1]*Gsize+GamePos[kitty][0]][(GamePos[kitty][1]+1)*Gsize+GamePos[kitty][0]]==1.0)
	{
		path = newPathNode(GamePos[kitty][0], GamePos[kitty][1]+1);  // move down
		return path;
	}
    if(A[GamePos[kitty][1]*Gsize+GamePos[kitty][0]][GamePos[kitty][1]*Gsize+(GamePos[kitty][0]+1)]==1.0)  
	{	
		path = newPathNode(GamePos[kitty][0]+1, GamePos[kitty][1]); // move right
		return path;
	}
 }
 if(dx < 0 && dy == 0)  // case 5: the mouse is in +x axis
 {
    if(A[GamePos[kitty][1]*Gsize+GamePos[kitty][0]][GamePos[kitty][1]*Gsize+(GamePos[kitty][0]+1)]==1.0)  
	{	
		path = newPathNode(GamePos[kitty][0]+1, GamePos[kitty][1]); // move right
		return path;
	}
 }
 if(dx > 0 && dy == 0)  // case 6 the mouse is in -x axis
 {
    if(A[GamePos[kitty][1]*Gsize+GamePos[kitty][0]][GamePos[kitty][1]*Gsize+(GamePos[kitty][0]-1)]==1.0)  
	{
		path = newPathNode(GamePos[kitty][0]-1, GamePos[kitty][1]); // move left
		return path;
	}
 }
 if(dx == 0 && dy > 0)  // case 7: the mouse is in +y axis
 {
	if(A[GamePos[kitty][1]*Gsize+GamePos[kitty][0]][(GamePos[kitty][1]-1)*Gsize+GamePos[kitty][0]]==1.0)
	{
		path = newPathNode(GamePos[kitty][0], GamePos[kitty][1]-1); // move up
		return path;
	}
 }
 if(dx ==0 && dy < 0)  // case 8 the mouse is in -y axis
 {
	if(A[GamePos[kitty][1]*Gsize+GamePos[kitty][0]][(GamePos[kitty][1]+1)*Gsize+GamePos[kitty][0]]==1.0)
	{
		path = newPathNode(GamePos[kitty][0], GamePos[kitty][1]+1);  // move down
		return path;
	}
 }

 return NULL ; // if it doesn't fall in those 8 cases, then return NULL
               //the kitty gets stuck unfortunately.
}

struct path_node *kittySmartSmell(int GamePos[numAgents][3], float A[GsizeSqr][GsizeSqr], int Target[numAgents][2], int kitty)
{
 /*
   Kitty walk using smart smell. Expected behaviour:

   As long as the kitty is not stuck, it uses smell just like the kittySmellWalk()
   function (in fact, you're allowed to call that function from here if you
   like).

   A kitty that has reached a dead end is allowed to use the shortest path
   algorithm to find the best route to go to the other side of whichever
   wall is blocking it.

   Once the cat switches to shortest-path, it MUST reach the intended target
   destination.

   After the kitty has arrived at its intended destination, it reverts to
   using smell to track the mouse.
 */
    
 struct path_node *path=NULL;
 struct path_node *shortestpath=NULL;
 int dx, dy;     // store distance in x-axis, and y-axis

 // there are two mode here, one is shortestpath mode where target position is set
 // another is smellWalk mode where target position is not set ( the kitty is not
 // stuck
 if (GamePos[kitty][0]==Target[kitty][0] && GamePos[kitty][1]==Target[kitty][1] ||
	 Target[kitty][0]==-1)
 {   
	 // the mouse changed its position, or the kitty reached intended target postion
	 //it's in smellWalk mode

	 Target[kitty][0] = -1;  // it's very important to reset target in smelWalk mode
	 Target[kitty][1] = -1;  

	 path = kittySmellWalk(GamePos,A,kitty);  // smell the mouse, find next grid to move
	 
	 if(path) // if the kitty is not stuck
	 {
		 if (kitty==7)   // update the mouse's postion only after all kitties complete smartSmellWalk!
		 {
			Target[1][0] = GamePos[1][0]; // keep record of the mouse's location, since 
			Target[1][1] = GamePos[1][1]; // the mouse always moves first
		 }
		 return path; // as long as the kitty is not stuck, keep smelling and moving!
	 }
	 // if we consider the game grid layou as a x-y quardrants, and the kitty is vertex
	 // the mouse may fall into 4 quardrants, x-axis or y axis 
	 dx = GamePos[kitty][0] - GamePos[1][0]; // get x-axis distance 
	 dy = GamePos[kitty][1] - GamePos[1][1];  // get y-axis distance

	 //set up the target. there are four scenarios for the kitty getting stuck.
	 if(dy > 0)  // scenario 1: 2nd and 1 quardrant and +y axis
	 {
		 // if the mouse is in 2nd quardrant or +y axis , the kitty can only move to
		 //left or up in order to be closer to the mouse
		 Target[kitty][0] = GamePos[kitty][0]; 
		 Target[kitty][1] = GamePos[kitty][1]-1;  // up one grid behind the wall 
	 }
	 if(dy < 0)  // scenario 2: 3rd, 4th quardrant and -y axis
	 {
		 Target[kitty][0] = GamePos[kitty][0]; 
		 Target[kitty][1] = GamePos[kitty][1]+1;  // down one grid behind the wall 
	 }

	 if(dx < 0 && dy == 0)  // scenario 3: +x axis
	 {
 		 Target[kitty][0] = GamePos[kitty][0]+1;   // move right one grid behind the wall 
		 Target[kitty][1] = GamePos[kitty][1];
	 }
	 if(dx > 0 && dy == 0)  // scenario 4: -x axis
	 {
		 Target[kitty][0] = GamePos[kitty][0]-1;   // move left one grid behind the wall 
		 Target[kitty][1] = GamePos[kitty][1];
	 }
 }

 // shortest path mode

 // as soon as we get the target postion, we compute the shortest path for the kitty
 shortestpath = shortestPath(A,GamePos[kitty][0],GamePos[kitty][1],Target[kitty][0],Target[kitty][1]);
 if(shortestpath) path = newPathNode(shortestpath->next->x, shortestpath->next->y); // obtain next move for the kitty
 if(shortestpath) deletePath(shortestpath);   // to prevent memory leak, remove the path every time

 return path;

}// end of smartSmellWalk(), don't delete



struct path_node *kittySoundWalk(int GamePos[numAgents][3], float A[GsizeSqr][GsizeSqr], int Target[numAgents][2], int kitty)
{
 /*
   Kitty sound walk: Expected behaviour:

   In space no one can hear you scream, unfortunately, we're not in space, so the
   mouse's little steps can be heard by hungry kitties.

   This function works by checking whether the mouse is closer than 'hearingRadius'
   away from a kitty. If so, the cat can hear the mouse and will use the shortest
   path to reach the mouse's location.

   ** As long as the mouse remains within hearing radius, the shortest path is
   ** updated at each turn (unlike kittySmartSmell() which computed a destination
   ** once and then had to wait for the cat to arrive there)

   If the mouse steps outside the hearing radius, the cat still has to complete
   the trip to the last known mouse location.

   As long as the mouse has not stepped within hearing radius, or if it stepped
   out and the cat reached the last known mouse location, the cat reverts to
   smell based mouse finding (you should use kittySmellWalk() for that - why
   can't you use kittySmartSmell()? )

   All distances are Manhattan distances.

   Depending on the value of hearingRadius (see GameAI.h), this could make the
   kitties pretty tough to beat, so don't make hearingRadius too large or the
   poor mouse doesn't stand a chance!

   Grid locations over which the mouse can be heard by one of the kitties
   will be shown in dark red on each frame.
 */

 struct path_node *path=NULL;
 struct path_node *shortestpath=NULL;
 int dx, dy;     // store distance in x-axis, and y-axis

 dx = GamePos[kitty][0] - GamePos[1][0]; // get x-axis distance 
 dy = GamePos[kitty][1] - GamePos[1][1];  // get y-axis distance

 //printf("dx=%d, dy=%d\n", dx, dy); // debug......................................................

 if ((abs(dx)+abs(dy)) <= hearingRadius)
 {
    Target[kitty][0] = GamePos[1][0];  // record the position of the mouse 
    Target[kitty][1] = GamePos[1][1];
    shortestpath = shortestPath(A,GamePos[kitty][0],GamePos[kitty][1],Target[kitty][0],Target[kitty][1]);
    if(shortestpath) path = newPathNode(shortestpath->next->x, shortestpath->next->y); // obtain next move for the kitty
    if(shortestpath) deletePath(shortestpath);   // to prevent memory leak, remove the path every time
    
	flagTarget[kitty] = 1;  // flag indicates the Target has been used
	return path;
 }
 //printf("k=%d, kx=%d, ky=%d, tkx=%d, kty=%d\n",kitty, GamePos[kitty][0],
	// GamePos[kitty][1], Target[kitty][0], Target[kitty][1]);                    // debug...............
 
 // the mouse is away form sound range, and the kitty still move toward to target position!
 if (!(GamePos[kitty][0]==Target[kitty][0]&&GamePos[kitty][1]==Target[kitty][1])&&
	 Target[kitty][0] != -1)
 {
    shortestpath = shortestPath(A,GamePos[kitty][0],GamePos[kitty][1],Target[kitty][0],Target[kitty][1]);
    if(shortestpath) path = newPathNode(shortestpath->next->x, shortestpath->next->y);
    if(shortestpath) deletePath(shortestpath);

	flagTarget[kitty] = 1;  // flag indicates the Target has been used
    return path;
 }
 Target[kitty][0] = -1;  // reset the last know mouse location
 Target[kitty][1] = -1;

 path = kittySmellWalk(GamePos,A,kitty);  // smell the mouse, find next grid to move
 
 flagTarget[kitty] = 1;  // flag indicates the Target has been used
 return path;
}

struct path_node *superKitty(int GamePos[numAgents][3], float A[GsizeSqr][GsizeSqr], int Target[numAgents][2], int kitty)
{
 /*
   Implement this function to create the ultimate kitty mouse hunter.
   
   A super-kitty uses all possible means to capture the mouse: Sound, smell, and
   vision. Different modes of perception have different precedence:

   - Vision is first: If the cat can see the mouse it will move toward the mouse
                      regardless of any previously computed path.
   - Sound: As you saw, sound is pretty effective so if the mouse is not visible,
            kitties will try to use sound to locate the mouse and find a shortest
            path toward it.
   - Smell: If the mouse can't be seen or heard, smell is used to move in the
            general direction of the mouse.
 */

  // define all local variables here:
  struct path_node *path=NULL;
  int dx, dy;     // store distance in x-axis, and y-axis


/********************************************************************************
 *                                                                              *
 *   Here is my strategy of building a super kitty:                             *
 *                                                                              *
 *   First of all, checking if the mouse is stepping in hearing radius of       *
 *   if so, then the ketty uses sound to locate the mouse. Since the ketty      *
 *   find a shortest path within hearing range, it'd be an effective way to     *
 *   to catch the kitty.                                                        *
 *                                                                              *
 *   Next, as long as the mouse steps out of the hearing range, the kitty       *
 *   check if a mouse in the kitty's sight, if so, then the kitty use           *
 *   kittyEyeWalk to locate the mouse.                                          *
 *                                                                              *   
 *   if the kity cann't see the mouse, the kitty uses smart Smell Walk to get   * 
 *   the mouse.                                                                 *
 *                                                                              *
 *   Since Target array is used in a different way in sould walk and smartsmell *
 *   walk, we will reset the Target right after the mouse is away from hearing  * 
 *   radious                                                                    *
 *                                                                              *
 ********************************************************************************/

 dx = GamePos[kitty][0] - GamePos[1][0]; // get x-axis distance 
 dy = GamePos[kitty][1] - GamePos[1][1];  // get y-axis distance

 if ((abs(dx)+abs(dy)) <= hearingRadius)
 {
    path=kittySoundWalk(GamePos,A,Target,kitty);
    return path;
 }

 // use vision
 path = kittyEyeWalk(GamePos,A,kitty);

 if(!path) return NULL;    // Kitty has nowhere to go

 //printf("From EyeWalk, k=%d,  pathx=%d, y=%d\n", kitty, path->x, path->y);        //debug.......................

 if (!flagRamdomPos)
	 return path;   // It's the path, such that the kitty can see the mouse!
 else  flagRamdomPos=0;  // reset flag , and smart smell walk!
 
 // use smart smell

// here is the tricky part: reset the Target[][] array!
 
 //printf("flagTarget[%d]=%d\n", kitty, flagTarget[kitty]);              //debug...................

 if (flagTarget[kitty])  
 {   
	 Target[kitty][0] = -1;  // reset the Target, since it' been used in SoundWalk 
	 Target[kitty][1] = -1;

	 flagTarget[kitty] = 0;  // reset flag
 }

 path=kittySmartSmell(GamePos,A,Target,kitty);
 //if(path) printf("From SmartSmell, k=%d,  pathx=%d, y=%d\n", kitty, path->x, path->y);    //debug................

 return path;
	 
}

struct path_node *mouseEscape(int GamePos[numAgents][3],float A[GsizeSqr][GsizeSqr])
{
 /*
   Implement this function to create the smartest mouse possible

   a smart mouse can update function designed to elude even the smartest cat.

    Note that: The mouse can see the kitties (since the mouse would usually be
               user controlled, we assume the mouse sees where the kitties are).
               The mouse can also measure distance to the kitties.
               Of course, the mouse knows where the cheese and gate are.

    Design a maze traveling approach that gives the mouse a chance to escape even
    kitties using kittyAIcheat() or superKitty(). This will involve carefully
    managing the graph to account for cat position and proximity while optimizing
    the path to the cheese and the output.
 */

  struct path_node *path=NULL;
  struct path_node *head=NULL;  // it is used in travesal in linked list- path-node
  int flagCatInPath = 0;    // it indicates if there is a cat in the mouse's shortest path
                           // 0 means none, 1 means there is a hungry cat waiting there!
  int nearestCatInpath = -1;  // it stores which cat is in mouse's way
  int dx, dy;     // store distance in x-axis, and y-axis


   // Check if cheese was being eaten
  if (!GamePos[2][2])   // cheese has not been eaten
     path=shortestPath(A,GamePos[1][0],GamePos[1][1],GamePos[2][0],GamePos[2][1]); 
     //path=shortestPath(A,GamePos[1][0],GamePos[1][1], 19, 19);        // debug....................
  else
	 // done with chees, move to exit gate!
     path=shortestPath(A,GamePos[1][0],GamePos[1][1],GamePos[0][0],GamePos[0][1]);
    
     //GamePos[1][0] = 19;          //debug, let the mouse stuck somewhere......................
	 //GamePos[1][1] = 0;
	 //path=NULL;

  if(!path) return NULL;
    
  // the while is to find if there is a cat in mouse' path
  head = path->next;
  while(head)
  {
	  for (int i=3;i<numAgents;i++)
	  {
		  if ((head->x == GamePos[i][0]) &&
			  (head->y == GamePos[i][1]))
		  {
				flagCatInPath = 1;     //1 means there is a hungry cat waiting there!
		        nearestCatInpath = i;  // keep a record of which cat is waiting for the mouse!
				break;
		  }
	  }
      if (flagCatInPath) break;
	  head = head->next;
  }

  // check there is a cat near the mouse
  for (int i=3;i<numAgents;i++)
  {
	  dx = GamePos[i][0] - GamePos[1][0]; // get x-axis distance 
      dy = GamePos[i][1] - GamePos[1][1];  // get y-axis distance
	  if ((abs(dx)+abs(dy))<=2)
	  {
			flagCatInPath = 1;     //1 means there is a hungry cat waiting there!
	        nearestCatInpath = i;  // keep a record of which cat is waiting for the mouse!
	  }
  }

  if (!flagCatInPath) return path;    // it's safe to go, no cats in my way!


  //set the destination one grid different from cat's position
  // check four conor grids around the cat, if it's valid postion, move there tempararily
  if(((GamePos[nearestCatInpath][0]-2)>0)&&((GamePos[nearestCatInpath][1]-2)>0))
  {
	path=shortestPath(A,GamePos[1][0],GamePos[1][1],
		GamePos[nearestCatInpath][0]-2,GamePos[nearestCatInpath][1]-2);
	flagCatInPath=0; 
	return path;   // get the shortest destinated at upper left cornor grid of the cat.
	              // avoiding be seen by cat
  }
  if(((GamePos[nearestCatInpath][0]+2)>0)&&((GamePos[nearestCatInpath][1]-2)>0))
  {
	path=shortestPath(A,GamePos[1][0],GamePos[1][1],
		GamePos[nearestCatInpath][0]+2,GamePos[nearestCatInpath][1]-2);
	flagCatInPath=0; 
	return path;   // get the shortest destinated at upper right cornor grid of the cat.
	              // avoiding be seen by cat
  }
  if(((GamePos[nearestCatInpath][0]-2)>0)&&((GamePos[nearestCatInpath][1]+2)>0)) 
  {
	path=shortestPath(A,GamePos[1][0],GamePos[1][1],
		GamePos[nearestCatInpath][0]-2,GamePos[nearestCatInpath][1]+2);
	flagCatInPath=0; 
	return path;  // get the shortest destinated at lower left cornor grid of the cat.
	              // avoiding be seen by cat
  }
  if(((GamePos[nearestCatInpath][0]+2)>0)&&((GamePos[nearestCatInpath][1]+2)>0))
  {
	path=shortestPath(A,GamePos[1][0],GamePos[1][1],
		GamePos[nearestCatInpath][0]+2,GamePos[nearestCatInpath][1]-2);
	flagCatInPath=0; 
	return path;   // get the shortest destinated at lower right cornor grid of the cat.
	              // avoiding be seen by cat
  }

  //return path; 
}


