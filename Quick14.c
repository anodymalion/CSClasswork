/* Rebecca Nickerson

Quick14

Method:
     The program determines if there were flags entered from the
   command line.  If so, it records POS and LEN, if available.
     For each line in each valid file, remove the trailing newline
   and push it onto the stack.
     Call recursive() on the stack, which will recursively call 
   itself to implement QuickSort.  It does this by using the top
   value on the stack as a pivot, and comparing each subsequent
   line on the stack to it, splitting the stack into two other
   stacks depending on whether their keys are larger or smaller
   than the pivot key.  The function then sorts each stack and
   combines each with the splitter to yield the fully sorted stack.
 */

#include "/c/cs223/Hwk3/getLine.h"
#include "/c/cs223/Hwk4/Stack.h"
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

void recursive(Stack *stack1, Stack *stack2, Stack *stack3,
	       int pos, int len, char *splitter, int num,
	       int *posi, bool eq);
void popFullStack(Stack *stk);


int main(int numargs, char *args[]){
  
  int pos = 0; //position
  int len = -1; //length
  int count = 1; //keep track of the current character in the flag
  int fullLen = 0; //total number of lines put into the stack
  char c;  //current character
  char *line; //line of the file
  char *split; //the first splitter to the recursive call
  int flag = 0; //1 if a flag is given, else 0
  int v = 2;
  int *position = &v;
  bool toPos = true;  //is the current character part of the POS?
  FILE *fp; //point to the file
  Stack stack1, stack2, stack3;

  if(numargs > 1){
    if(*(*(args+1)) == '-'){
      flag = 1;
     
      while(*(*(args+1) + count) != '\0'){
	if(isdigit(*(*(args+1) + count))){
	  //if the next character is a digit 
	  c = *(*(args+1) + count) - '0';

	  if(toPos){//if it is part of pos, adjust pos
	    pos *= 10;
	    pos += c;
	  }
	  else{//otherwise it is part of len, adjust len
	    if(len == -1)
	      len = 0;
	    len *= 10;
	    len += c;
	  }
	}
	else if(*(*(args+1) + count) == ','){
	  toPos = false;
	}
	else{
	  //print an error if the input is not a valid flag
	  fprintf(stderr, "Invalid flag\n");
	  exit(1);
	}	
	count++;
      }
    }
  }
      
  if(createS(&stack1) && createS(&stack2) && createS(&stack3)){}
  //stacks created without problem
  else{ //if a problem, quit
    fprintf(stderr, "Memory error\n");
    exit(1);
  }
  
  for(int i = 1 + flag; i < numargs; i++){ //for all valid files given
   
    if((fp = fopen(args[i], "r")) != NULL){
            
      while((line = getLine(fp)) != NULL){ //for each line
  
	if(pos < strlen(line)){
	  //if valid input (pos is an index of the line)
	  
	  if(*(line + strlen(line) - 1) == '\n'){
	    *(line + strlen(line) - 1) = '\0';
	  }// if it ends in a newline, get rid of the newline
	  
	  if(pushS(&stack1, line)) //push each line onto stack1
	    fullLen++;  //keep track of the number of lines
	  else{
	    fprintf(stderr, "Memory error\n");
	    exit(1);
	  }
	}
	else
	  free(line);
      }    
      fclose(fp);
    }
    else{
       fprintf(stderr, "Invalid file\n");
       exit(2);
    }
  }

  //now all the lines in the files are on stack1
  //must now use recursive call to sort them
  if(popS(&stack1, &split)){
    recursive(&stack1, &stack2, &stack3, pos, len, split, fullLen - 1, position, true); 
  }
  else{
    fprintf(stderr, "Memory error\n");
    exit(1);
  }
  
  //recursive() will place the sorted stack on stack2
  popFullStack(&stack2);
  
  destroyS(&stack1);
  destroyS(&stack2);
  destroyS(&stack3);

  return 0;
}



/* void recursive
arguments:
  Stack *s1 - the stack values are popped of
  Stack *s2 - the 'left' stack, larger keys than splitter go here
  Stack *s3 - the 'right' stack, smaller keys than splitter go here
  int pos - the POS from the flag
  int len - the LEN from the flag
  char *splitter - the splitter for this iteration
  int num - how many elements of s1 to remove
  int *posi - shows which of the stacks holds the sorted stack
  bool eq - used to determine if equal keys shoudl go into s2 or s3

method:
     num elements of s1 are popped off and each key compared to that of 
  the splitter.  Elements with larger keys are pushed onto s2, smaller
  keys are pushed onto s3.  Identical keys will go onto s2 or s3 depending
  on eq, which flips each iteration.  
     If elements were placed into s2, sort them by calling recursive(s2, ...)
  After this, place the splitter onto the sorted stack (determined by posi).
  After this, if elements were placed into s3, sort them by calling recursive(s3...)
     The base case occurs when no elements were placed in s2.  Then the splitter is 
  just added to the sorted stack.  In this way, each splitter is placed onto the
  sorted stack in the correct order.

If any of the stack methods (popS, pushS) fail, the program exits with an error message.

 */

void recursive(Stack *s1, Stack *s2, Stack *s3,
	       int pos, int len, char *splitter, int num,
	       int *posi, bool eq){ //pass pointers to the stacks?
   
  char *cLine; //current line taken off the stack
  char *newSplit;//, *splitKey, *lineKey; //splitter, keys to compare
  int lNum = 0; //number of elements pushed onto the 'left', smaller stack
  int rNum = 0; //number of elements pushed onto the 'right', larger stack
  bool swap = false;
  
  for(int i = 0; i < num; i++){ //for all elements in stack that should be removed
    if(popS(s1, &cLine)){//pop everything off
          
      int cmp = strncmp((splitter + pos), (cLine + pos), len);

      if(cmp < 0){ //if key for line is larger than splitter
	
	if(pushS(s2, cLine)) //push it onto s2
	  lNum++;
	else{
	  fprintf(stderr, "Memory error\n");
	  exit(1);
	}
      }
      else if(cmp == 0){
	if(eq){
	  if(pushS(s3, cLine))
	    rNum++;
	  else{
	    fprintf(stderr, "Memory error\n");
	    exit(1);
	  }
	}
	else{
	  if(pushS(s2, cLine))
	    lNum++;
	  else{
	    fprintf(stderr, "Memory error\n");
	    exit(1);
	  }
	}
      }
      else{ //otherwise push it onto s3
	if(pushS(s3, cLine))
	  rNum++;
	else{
	  fprintf(stderr, "Memory error\n");
	  exit(1);
	}
      }      
    }
    else{
      fprintf(stderr, "Memory error\n");
      exit(1);
    }
  }

  //now, all lines with keys larger than that of the splitter are in
  //s2, and all smaller are in s3

  if(lNum > 0){//if there were lines put into s2
    //need to sort the new things in s2 and push onto the sorted stack
    if(popS(s2, &newSplit)){ //get the new splitter
      
      if(*posi == 1)
	*posi = 2;
      else if(*posi == 2)
	*posi = 1;
      
      recursive(&(*s2), &(*s1), &(*s3), pos, len, newSplit, lNum-1, posi, !eq);
      //sort the new lines pushed onto s2

      if(*posi == 1)
	*posi = 2;
      else if(*posi == 2)
	*posi = 1;
    }
    else{
      fprintf(stderr, "Memory error\n");
      exit(1);
    }
  }

  //after the left-stack is sorted, push the splitter onto the stack
  if(*posi == 1)
    if(pushS(s1, splitter)){}
    else{
      fprintf(stderr, "Memory error\n");
      exit(1);
    }
  else if(*posi == 2)
    if(pushS(s2, splitter)){}
    else{
      fprintf(stderr, "Memory error\n");
      exit(1);
    }
  else
    if(pushS(s3, splitter)){}
    else{
      fprintf(stderr, "Memory error\n");
      exit(1);
    }
  
  //finally sort the right-stack onto the stack
  if(rNum > 0){        
    if(*posi == 2){
      if(popS(s3, &newSplit))
	recursive(&(*s3), &(*s2), &(*s1), pos, len, newSplit, rNum-1, posi, !eq);
     else{
       fprintf(stderr, "Memory error\n");
       exit(1);
     }
    }
    else if(*posi == 1){
      *posi = 2;
      swap = true;
      if(popS(s3, &newSplit))
	recursive(&(*s3), &(*s1), &(*s2), pos, len, newSplit, rNum-1, posi, !eq);
      else{
	fprintf(stderr, "Memory error\n");
	exit(1);
      }
    }
    
    if(swap && (*posi == 2)){
      *posi = 1;
    } 
  }
}


/*void popFullStack
arguments: Stack *stk - a pointer to a Stack

for all of the non-header nodes in stk, the function pops
them off the stack and prints them out on a new line, then
frees the line from memory.
 */
 
void popFullStack(Stack *stk){
  
  char *nextline;
  
  while(popS(&(*stk), &nextline)){
     
    printf("%s\n", nextline);
    free(nextline);
  }
}

 



