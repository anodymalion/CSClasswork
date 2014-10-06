#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "/c/cs223/Hwk4/Stack.h"


struct stack{ 
  char *line;           //string line
  struct stack *next;   //next item in the stack
}; 

int createS(Stack *stk){
  int success = 0; //whether the stack was created
  
  if((*stk = malloc(sizeof(struct stack)))){
    success = 1;
    (*stk)->next = NULL;
    (*stk)->line = "HEADER";
  }
  return success;
}

int isEmptyS(Stack *stk){
  int success = 0;
  if((*stk)->next == NULL)
    success = 1;
  return success;
}

int pushS(Stack *stk, char *s){ 
  //push the value in s onto the top of the stk
  //insert *s after the header, make its next point to previous first element

  int success = 0; //check success

  Stack new;
  new = malloc(sizeof(struct stack));

  if(new){
    success = 1;

    new->next = (*stk)->next;
    new->line = s;
    
    (*stk)->next = new;

  }
  


  return success;
}

int popS(Stack *stk, char **s){
  //pop off the top value of stk and store it in s
  //header->next now goes to whatever the top item pointed to 

  //will need to free the storage of the item being removed

  int success = 0;

  if(!isEmptyS(stk)){
   
    success = 1;
    
    *s = (*stk)->next->line;
    Stack temp = (*stk)->next ;
    (*stk)->next = (*stk)->next->next;
    free(temp);
  }

  return success;
}

int topS (Stack *stk, char **s){//get the first line
   int success = 0; //0 if stack is empty
   
  if(!isEmptyS(stk)){ 
    success = 1;
    *s = (*stk)->next->line;
  }
  return success;
}

int destroyS(Stack *stk){ //need to free all the things
  int success = 1;
  char *dummy;

  while(popS(stk, &dummy)){}
  free(*stk);

  return success;
}



