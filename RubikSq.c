/*Rebecca Nickerson

RubikSq.c

RubikSq finds the shortest solution to a puzzle that
shifts letter tiles around from an INITIAL to a GOAL
pattern.  It generates all the possible moves that can
result from a given pattern, and adds these to a queue.
Everything pushed on the queue is also added to a dictionary
that is a trie.  If any of these generated moves are the
GOAL position, RubikSq prints out the sequence of steps by 
finding the which string the current move came from and printing
it out, moving backwards until it reaches the GOAL position.
If there are no ways to reach the GOAL within the given limit,
RubikSq prints nothing.  If the command line arguments are
invalid, it exits immediately.

*/


#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "/c/cs223/Hwk4/Stack.h"

typedef struct trie{
  char key;
  char *from;
  int len;
  struct trie *children[12];
}Trie;

int checkTiles(char *a, char *b);
int checkNum(char *a);
int checkFlag(char *a);
void popQ(Stack *q1, Stack *q2, char **s);
char* shiftR(char *s, int a, int b, int c, int d);
char* shiftC(char *s, int a, int b, int c, int d);
char* findInTrie(Trie *tr, char *str);
int lenInTrie(Trie *tr, char *str);
int insert(Trie *tr, char *str, char *from, int leng);
void printSteps(Trie a, char *s);



int main(int numargs, char *args[]){
  //should have either 4, 5, 6, or 7 arguments
  //if 7, [1] must be -r, [2,3,4] must be ints, initial and goal are letters
  //if 6, must not include -r, include all else
  //if 5 must include -r and not height OR width
  //if 4, must have int for maxlength and letters for other, and nothing else

  //height and width are ints  between 2 and 5 (Default is 3)
  //maxlength is non-negative int (can be 0)
  //INITIAL and GOAL have the necessary number of letters for the size of the tray
  //letters between only A and L (duplicates ok) 

  int width = 3; //width of the tray, default is 3
  int height = 3; //height of the tray, default is 3
  int maxlength = -1; //maxlength of the pattern
  int tilelen; //number of tiles
  char *temp; 
  bool flag; //true is -r is specified
  bool used;
  char *initial; //initial position
  char *goal; //goal position

  //if all the command-line args follow the rules for 4 args
  if((numargs == 4) && checkNum(args[1]) &&
     checkTiles(args[2], args[3])){
    
    maxlength = (int)strtol(args[1], &temp, 10);
    tilelen = strlen(args[2]);
    flag = false; //no flag present   
    initial = args[2];
    goal = args[3]; 
  }
  //if all the command-line args follow the rules for 5 args
  else if((numargs == 5) && checkFlag(args[1]) &&
	  checkNum(args[2]) && checkTiles(args[3], args[4])){
    
    maxlength = (int)strtol(args[2], &temp, 10);
    tilelen = strlen(args[3]);
    flag = true; //flag present
    initial = args[3];
    goal = args[4];
  }
  //if all the command-line args follow the rules for 6 args
  else if((numargs == 6) && checkNum(args[1]) &&
	  checkNum(args[2]) && checkNum(args[3]) &&
	  checkTiles(args[4], args[5])){

    height = (int)strtol(args[1], &temp, 10);
    width = (int)strtol(args[2], &temp, 10);
    maxlength = (int)strtol(args[3], &temp, 10);
    tilelen = strlen(args[4]);
    flag = false;   //no flag present
    initial = args[4];
    goal = args[5];
  }
  //if all the command-line args follow the rules for 7 args
  else if((numargs == 7) && checkFlag(args[1]) &&
	  checkNum(args[2]) && checkNum(args[3]) &&
	  checkNum(args[4]) && checkTiles(args[5], args[6])){
  
    height = (int)strtol(args[2], &temp, 10);
    width = (int)strtol(args[3], &temp, 10);
    maxlength = (int)strtol(args[4], &temp, 10);
    tilelen = strlen(args[5]);
    flag = true;  //flag present
    initial = args[5];
    goal = args[6];
  }
  else{//invalid command line args
    fprintf(stderr, "Invalid command line arguments\n");
    exit(1);
  }
  
  if((height >= 2) && (height <= 5) && 
     (width >= 2) && (width <= 5) &&
     (tilelen == height*width)){}
  else{//if the height and width aren't right, then quit
    fprintf(stderr, "Invalid command line arguments\n");
    exit(1);
  }
   
  Stack q1; //create the two stacks for the queue
  Stack q2;
  createS(&q1);
  createS(&q2);

  char *nextStr; //holds what is popped off queue 'P'
  int nextLen; //holds the length of P

  Trie root; //root of the trie dictionary
  root.len = -1;
  root.from = NULL;
 
  for(int k = 0; k < 12; k++)
    root.children[k] = NULL;
  
  insert(&root, goal, NULL, 0); //insert root in queue & dictionary
  pushS(&q1, goal);
 
  if(!(strcmp(goal, initial))){ //if goal is initial
    printf("%s\n", initial);
    destroyS(&q1);
    destroyS(&q2);
    return 0;
  }

  while(!(isEmptyS(&q1) && isEmptyS(&q2))){ //while queue not empty
    popQ(&q1, &q2, &nextStr);
    nextLen = lenInTrie(&root, nextStr);
    
    used = false;

    if(nextLen < maxlength){

      //generate all the possible tiles
      for(int c = 0; c < width; c++){ //for all column shifts
	if(flag){//-r, then do it for all possible shifts
	  for(int sh = 1; sh < width; sh++){
	    char *nextTiles = shiftC(nextStr, height, width, c, height - sh);
	    //next tile pattern generated by a shift

	    if(!strcmp(nextTiles, initial)){
	      insert(&root, nextTiles, nextStr, nextLen + 1);
	      printSteps(root, nextTiles);
	      destroyS(&q1);
	      destroyS(&q2);
	      return 0;
	    }
	    if(insert(&root, nextTiles, nextStr, nextLen + 1)){
	      pushS(&q1, nextTiles); //add it to the queue and dictionary
	      used = true;
	    }
	    else
	      free(nextTiles);
	  }
	}
	else{//else do it for shift is one step only
	  char *nextTiles = shiftC(nextStr, height, width, c, height - 1);
	  //next tile pattern generated by a shift

	  if(!strcmp(nextTiles, initial)){
	    insert(&root, nextTiles, nextStr, nextLen + 1);
	    printSteps(root, nextTiles);
	    destroyS(&q1);
	    destroyS(&q2);
	    return 0;
	  }

	  if(insert(&root, nextTiles, nextStr, nextLen + 1)){
	    pushS(&q1, nextTiles);  //add it to the queue and dictionary
	    used = true;
	  }
	  else
	    free(nextTiles);
	}
      }
      for(int r = 0; r < height; r++){//for all possible row shifts
	if(flag){//-r, then do it for all possible shifts
	  for(int sh = 1; sh < height; sh++){
	    char *nextTiles = shiftR(nextStr, height, width, r, width - sh);
	    //next tile pattern generated by a shift	   
	    
	    if(!strcmp(nextTiles, initial)){
	      insert(&root, nextTiles, nextStr, nextLen + 1);
	      printSteps(root, nextTiles);
	      destroyS(&q1);
	      destroyS(&q2);
	      return 0;
	    }
	   
	    if(insert(&root, nextTiles, nextStr, nextLen + 1)){
	      pushS(&q1, nextTiles);  //add it to the queue and dictionary
	      used = true;
	    }
	    else
	      free(nextTiles);
	  }
	}
	else{//else do it for shift is one step only
	  char *nextTiles = shiftR(nextStr, height, width, r, width - 1);
	  //next tile pattern generated by a shift

	  if(!strcmp(nextTiles, initial)){

	    insert(&root, nextTiles, nextStr, nextLen + 1);
	    printSteps(root, nextTiles);
	    destroyS(&q1);
	    destroyS(&q2);
	    return 0;
	  }
	  
	  if(insert(&root, nextTiles, nextStr, nextLen + 1)){
	    pushS(&q1, nextTiles);  //add it to the queue and dictionary
	    used = true;
	  }
	  else
	    free(nextTiles);
	}
      }
    } 
    
    if(!used)
      free(nextStr);

  }//end of while queue is not empty
  
  destroyS(&q1);
  destroyS(&q2);

  return 0;
}


/*int checkTiles
arguments
  char *a - first string
  char *b - second string

  checks to make sure that each character in a 
  appears in b the exact same number of time.
  this means a and b are the same if true.  returns
  1 if true and 0 if false.  It will return 0 also
  if any of the chars are outside the allowed range

 */

int checkTiles(char *a, char *b){
  int success = 0;
  int aLen = strlen(a); //length of a
  int bLen = strlen(b); //length of b
  int avals[12]; //array of the allowed characters, for a
  int bvals[12]; //array of the allowed characters, for b
  
  if(aLen != bLen)
    return success;
  
  for(int i = 0; i < 12; i++){
    avals[i] = 0;
    bvals[i] = 0;
  }

  for(int i = 0; i < aLen; i++){//must fall within 'A' to 'L'
    if((!(*(a + i) >= 65 && *(a + i) <= 76)) && (!(*(b + i) >= 65 && *(b + i) <= 76)))
      return success;
    else{
      avals[(*(a + i) - 65)] += 1;
      bvals[(*(b + i) - 65)] += 1;
    } 
  }//if the character is allowed, increase the value of the element
    //in the array with that index by one
 
  //I now realize I could have just done [- 'A']
  //but I don't want to change all this

  for(int i = 0; i < 12; i++){
    if(avals[i] != bvals[i])
      return success;
  }//if all the chars appear the same number of times

  success = 1;
  return success;
}


/*int checkNum
arguments
  char *a - string to check

  if every character in a is a digit, returns
  1, otherwise returns 0.
 */

int checkNum(char *a){
  int success = 1;
  int len = strlen(a);

  for(int i = 0; i < len; i++){
    if(!isdigit(*(a+i))){
      success = 0;
      break;
    }
  }
  return success;
} 




/*int checkFlag
arguments
  char *a - string to be checked

  if the first character is '-' and the second is 'r',
  and there are no more characters, returns true.  
  otherwise returns false.
 */

int checkFlag(char *a){
  int success = 0;
  int len = strlen(a);
  
  if(len == 2)
    if((*a == '-') && (*(a+1) == 'r'))
      success = 1;
  
  return success;
}



/*void popQ
arguments
  Stack *s1 - pointer to the first stack
  Stack *s2 - pointer to the second stack
  char **c - the top value in the queue is stored here

  popQ removed the first item from s2 and stores it in c.
  If s2 is empty, it removes each item from s1 and places 
  it in s2.

 */

void popQ(Stack *s1, Stack *s2, char **c){
  
  char *t;

  if(isEmptyS(s2)){
    while(!(isEmptyS(s1))){
      popS(s1, &t);
      pushS(s2, t);
    }
  }

  popS(s2, c);
}



/*char* shiftR
arguments
  char *str - string that is being altered
  int height - height of the board
  int width - width of the board
  int row - row to be shifted
  int shift - number of tiles the board is shifted

  shiftR finds the indicies of str that will be shifted.
  For indicies outside this range, it copies the char in
  str at that index into newstr.  For indicies inside this
  range, it calculates which char in str holds the value
  newstr needs, and copies this into newstr.

 */

char* shiftR(char *str, int height, int width, int row, int shift){
   
  int len = strlen(str);
  char *newstr = malloc(len + 1);
  *(newstr + len) = '\0';
  int ind = row * width;
  int tInd; //index of the string with the char copied
  
  for(int i = 0; i < len; i++){//if the char is not in the portion
    if((i < ind) || (i >= ind + width))//that is shifted, leave it
      *(newstr + i) = *(str + i);       //in the same place  
    else{ //if it is shifted, calculate the new value for that index
      tInd = i + shift; 
      if(tInd < ind + width)
	*(newstr + i) = *(str + tInd);
      else //if it overflows, subtract width
	*(newstr + i) = *(str + tInd - width);
    }
  }
  return newstr;
}



/*char* shiftC
arguments
  char *str - string that is being altered
  int height - height of the board
  int width - width of the board
  int row - row to be shifted
  int shift - number of tiles the board is shifted

  shiftC finds the indicies of str that will be shifted.
  For indicies outside this range, it copies the char in
  str at that index into newstr.  For indicies inside this
  range, it calculates which char in str holds the value
  newstr needs, and copies this into newstr.
 */

char* shiftC(char *str, int height, int width, int col, int shift){
  int len = strlen(str);
  char *newstr = malloc(len + 1);
  *(newstr + len) = '\0';
  double j;

  for(int i = 0; i < len; i++){
    j = (double)(i - col)/(double)width;
   
    if((int)j == j){//will be shifted
      if(i + shift*width >= len)
	*(newstr + i) = *(str + i + shift*width - len);
      else //in columns, so add shift*width to get next char
	*(newstr + i) = *(str + i + shift*width);
    }
    else
      *(newstr + i) = *(str + i);
  }
  return newstr;
}



/*char findInTrie
arguments
  Trie *tr - pointer to the root of the trie
  char *str - string to be found in the trie

  findInTrie finds the string that str is one step
  away from. The function assumes
  that str is in tr.  It follows each char key in 
  tr until it reaches the end of str, then returns
  the value of from in the final node.
 */

char* findInTrie(Trie *tr, char *str){
  int i = 0;
  int len = strlen(str);
  Trie *level = tr;

  while(i < len){
    for(int j = 0; j < 12; j++){
      if(level->children[j]->key == *(str + i)){
	i++;
	level = level->children[j];
	break;
      }
    }
  }

  return level->from;
}



/*int lenInTrie
arguments
  Trie *tr - pointer to the root of the trie
  char *str - string to be found in the trie

  lenInTrie finds the len in the Trie corresponding
  to the string str. len represents the number of 
  steps necessary to get to str. The function assumes
  that str is in tr.  It follows each char key in 
  tr until it reaches the end of str, then returns
  the value of len in the final node.
 */

int lenInTrie(Trie *tr, char *str){
  int i = 0;
  int len = strlen(str);
  Trie *level = tr;

  while(i < len){
    for(int j = 0; j < 12; j++){
      if(level->children[j]->key == *(str + i)){
	i++;
	level = level->children[j];
	break;
      }
    }
  }
  return level->len;
}



/* int insert
arguments
  Trie *tr - pointer to the root of the trie to be searched
  char *str - string to be inserted into tr
  char *fr - string that represents FROM
  int leng - # steps to get to str

  insert searches through the children of a node in the trie
  to find a key that matches the next char in str. If the key
  is found, it moves on to find the next char.  If the key is
  not found, it inserts the key as a child of the appropriate
  node and continues until all chars are inserted.  If any
  keys are inserted, it returns 1.  If no keys are inserted, 
  it returns 0, indicating that str was already in the trie.
 */

int insert(Trie *tr, char *str, char *fr, int leng){

  int i = 0; //index of str
  int len = strlen(str); //length of str
  Trie *level = tr; //level of the trie, starts at root
  bool found = false; //was the current key found?
  int nextI = 0; //index of children to insert the next key at
  int inserted = 0; //was a key inserted? if no, str was already in tr

  while(i < len){
    found = false;
    nextI = 0;
   
    for(int j = 0; level->children[j] != NULL && j < 12; j++){
      if(level->children[j]->key == *(str + i)){//key is present
	level = level->children[j]; //move on to the next key
	found = true;
	break;
      }
      nextI = j + 1;
      }
    
     if(!found){//key is not present //!found
       inserted = 1;
       
       Trie *t = malloc(sizeof(Trie)); //create new node
       t->key = *(str + i);     
       t->len = leng;
       t->from = fr; 
       
       for(int k = 0; k < 12; k++)
	 t->children[k] = NULL;
 
       level->children[nextI] = t; //new node is child of level

       level = level->children[nextI]; //go to that node
       
     }	
     i++;
  }
  return inserted;
}



/*void printSteps
arguments
  Trie tr - root of the trie
  char *str - string being searched for

  printSteps prints the str, then finds the string
  that str is one step away from by calling findInTrie().
  It prints that string and continues until len = 0.
  Every step from str to the goal position will be printed
 */

void printSteps(Trie tr, char *str){
  printf("%s\n", str);
  int len = lenInTrie(&tr, str);
  char *next = str;

  while(len > 0){
    next = findInTrie(&tr, next);
    len = lenInTrie(&tr, next);
    printf("%s\n", next);
  }
}
