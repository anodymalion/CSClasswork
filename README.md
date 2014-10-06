Selected code from the coursework for CPSC223 from Spring 2014

May add more comments here if I feel inspired.

Stack.c: basic stack functions.  Create, read, modify, destroy a Stack in a linked list data structure.
         
Quick14.c: ./Quick14 [-POS[,LEN]] [filename]*
            Implements quicksort using three stacks
RubikSq.c:  ./RubikSq [-r] [HEIGHT WIDTH] MAXLENGTH INITIAL GOAL
            Finds the shortest solution to the Rubik Square puzzle using a trie data structure.
            -r: allows shifting in one direction by possibly more than one square as a time
            HEIGHT and WIDTH of the rubik square
            MAXLENGTH: longest allowed length of the solution, in steps
            INITIAL: initial configuration of tiles (i.e. abcdefghi)
            GOAL: goal configuration of tiles
            
            Configuration 'abcdefghi' visualized as rubik square:
             +---+---+---+
		         | A | B | C |
		         +---+---+---+
		         | D | E | F |
		         +---+---+---+
		         | G | H | I |
		         +---+---+---+
