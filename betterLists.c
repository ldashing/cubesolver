#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define ORIGEN 0
#define BOUNDS 4
#define VOLOUM BOUNDS*BOUNDS*BOUNDS

int inputSnake[100] = {0};
int inputIndex = 0;

void readInput(){
    char ch, file_name[25];
    FILE *fp;

    fp = fopen("cube", "r"); 

    if (fp == NULL){
      perror("Error while opening the file.\n");
      exit(EXIT_FAILURE);
    }
    int inputCounter=0;
    while((ch = fgetc(fp)) != EOF){
        printf("%c", ch);
        if(ch=='s'){
            inputSnake[inputCounter]=1;
            inputCounter++;
        }        
        if(ch=='k'){
            inputSnake[inputCounter]=2;
            inputCounter++;
        }
   }
   printf("      ");
   inputSnake[inputCounter]=3;
   for(int i=0;i<100;i++){
       printf("%d",inputSnake[i]);
   } 
   printf("\n");
   fclose(fp);
}

void delay(int number_of_seconds){
    // Converting time into milli_seconds
    int milli_seconds = 100 * number_of_seconds;
  
    // Storing start time
    clock_t start_time = clock();
  
    // looping till required time is not achieved
    while (clock() < start_time + milli_seconds);
}

struct node{
   int curX;
   int curY;
   int curZ;
   int index;
   int type; //1=straigt 2=corner
   int facing;
   struct node *next;
};

struct node *head = NULL;
struct node *current = NULL;

void printList() {
   struct node *ptr = head;
   while(ptr != NULL) {
      printf("Index: %d    X: %d  Y: %d  Z: %d  Type: %d  Facing: %d \n",ptr->index, ptr->curX, ptr-> curY, ptr->curZ, ptr->type, ptr->facing);
      ptr = ptr->next;
   }
}

void reverse(struct node** head_ref) {
   struct node* prev   = NULL;
   struct node* current = *head_ref;
   struct node* next;
	
   while (current != NULL) {
      next  = current->next;
      current->next = prev;   
      prev = current;
      current = next;
   }
	
   *head_ref = prev;
}

void printListRev(){
   reverse(&head);
   printList();
   reverse(&head);
}

void push(int offsetX, int offsetY, int offsetZ, int newType, int newFacing) {
   //create a link
   inputIndex++;
   struct node *link = (struct node*) malloc(sizeof(struct node));
	
   if(head == NULL){
      link->index = 1;
      link->curX = ORIGEN;
      link->curY = ORIGEN;
      link->curZ = ORIGEN;
   }else{
      link->index = head->index+1;
      link->curX = head->curX+offsetX;
      link->curY = head->curY+offsetY;
      link->curZ = head->curZ+offsetZ;

   }
   link->type = newType;
   link->facing = newFacing;

	
   //point it to old first node
   link->next = head;
	
   //point first to new first node
   head = link;
}

struct node* deleteFirst() {

   //save reference to first link
   struct node *tempLink = head;
	
   //mark next to first link as first 
   head = head->next;
	free(tempLink);
   //return the deleted link
   return tempLink;
}

bool isEmpty() {
   return head == NULL;
}

int length() {
   int length = 0;
   struct node *current;
	
   for(current = head; current != NULL; current = current->next) {
      length++;
   }
	
   return length;
}

void pop(){
   inputIndex--;
   struct node *temp = deleteFirst();
}

int checkIfOccupied(int x, int y, int z){
   struct node *ptr = head;
   while(ptr != NULL) {
      if(x == ptr->curX && y == ptr->curY && z == ptr->curZ){
        //printf("oc\n");
        return 1;
      }
      ptr = ptr->next;
   }
   return 0;
}

int checkIfOutOfBounds(int boundX, int boundY, int boundZ){

   int minX=ORIGEN;
   int maxX=ORIGEN;

   int minY=ORIGEN;
   int maxY=ORIGEN;
    
   int minZ=ORIGEN;
   int maxZ=ORIGEN;

   struct node *ptr = head;
   while(ptr != NULL) {
      if(ptr->curX < minX){minX = ptr->curX;}
      if(ptr->curX > maxX){maxX = ptr->curX;}

      if(ptr->curY < minY){minY = ptr->curY;}
      if(ptr->curY > maxY){maxY = ptr->curY;}

      if(ptr->curZ < minZ){minZ = ptr->curZ;}
      if(ptr->curZ > maxZ){maxZ = ptr->curZ;}

      ptr = ptr->next;
   }
    
   int diffX=0;
   if(minX>maxX){diffX=minX-maxX+1;}else{diffX=maxX-minX+1;}

   int diffY=0;
   if(minY>maxY){diffY=minY-maxY+1;}else{diffY=maxY-minY+1;}

   int diffZ=0;
   if(minZ>maxZ){diffZ=minZ-maxZ+1;}else{diffZ=maxZ-minZ+1;}
   /*
   printf("minX: %d   maxX: %d   size: %d   maxSize: %d\n",minX, maxX, diffX, boundX);
   printf("minY: %d   maxY: %d   size: %d   maxSize: %d\n",minY, maxY, diffY, boundY);
   printf("minZ: %d   maxZ: %d   size: %d   maxSize: %d\n\n",minZ, maxZ, diffZ, boundZ);
   */
   if(diffX > boundX || diffY > boundY || diffZ > boundZ){
      //printf("Out of bounds\n\n\n");
      return 1;
   }

   return 0;
}

int checkIfWouldOutOfBounds(int extraX, int extraY, int extraZ){
   push(extraX,extraY,extraZ,66,66);
   int tmp = checkIfOutOfBounds(BOUNDS,BOUNDS,BOUNDS);
   pop();
   if(tmp){
     // printf("wouldbeout\n");
   }
   return tmp;
}

int solve(int incr){
   struct node *ptr = head;

   int currentBlock = inputSnake[inputIndex];

   if(inputSnake[inputIndex]==3){
      printf("end of input###############################################################");
      return 1;
   }
   
   //printf("head facing: %d current index %d current block: %d\n", head->facing,inputIndex,inputSnake[inputIndex]);
   //inputIndex++;
   if(head->type==1){//striaght block
      switch(head->facing){
         case 1:
            if(!(checkIfOccupied(ptr->curX, ptr->curY, ptr->curZ+1) || checkIfWouldOutOfBounds(0,0,1))){ //falls frei ist und nicht zu groß wird
            push(0,0,1,currentBlock,head->facing); //Neuer Block nach oben
            printf("z+ ");
            solve(incr);
            }
         break;
         case 2:
            if(!(checkIfOccupied(ptr->curX, ptr->curY+1, ptr->curZ) || checkIfWouldOutOfBounds(0,1,0))){
            push(0,1,0,currentBlock,head->facing); //Neuer Block nach vorne
            printf("y+ ");
            solve(incr);
            }
         break;
         case 3:
            if(!(checkIfOccupied(ptr->curX+1, ptr->curY, ptr->curZ) || checkIfWouldOutOfBounds(1,0,0))){
            push(1,0,0,currentBlock,head->facing); //Neuer Block nach rechts
            printf("x+ ");
            solve(incr);
            }
         break;
         case 4:
            if(!(checkIfOccupied(ptr->curX, ptr->curY, ptr->curZ-1) || checkIfWouldOutOfBounds(0,0,-1))){
            push(0,0,-1,currentBlock,head->facing); //Neuer Block nach unten
            printf("z- ");
            solve(incr);
            }
         break;
         case 5:
            if(!(checkIfOccupied(ptr->curX, ptr->curY-1, ptr->curZ) || checkIfWouldOutOfBounds(0,-1,0))){
            push(0,-1,0,currentBlock,head->facing); //Neuer Block nach hinten
            printf("y- ");
            solve(incr);
            }
         break;
         case 6:
            if(!(checkIfOccupied(ptr->curX-1, ptr->curY, ptr->curZ) || checkIfWouldOutOfBounds(-1,0,0))){
            push(-1,0,0,currentBlock,head->facing); //Neuer Block nach links
            printf("x- ");
            solve(incr);
            }
         break;
         
      }
   }else if(head->type==2){
      switch(head->facing){
         case 1:
            if(!(checkIfOccupied(ptr->curX, ptr->curY+1, ptr->curZ) || checkIfWouldOutOfBounds(0,1,0))){ //2
               push(0,1,0,currentBlock,2); //Neuer Block nach vorne
               printf("y+ ");
               solve(incr);
            }
    
            if(!(checkIfOccupied(ptr->curX+1, ptr->curY, ptr->curZ) || checkIfWouldOutOfBounds(1,0,0))){ //3
               push(1,0,0,currentBlock,3); //Neuer Block nach rechts
               printf("x+ ");
               solve(incr);
            }
     
            if(!(checkIfOccupied(ptr->curX, ptr->curY-1, ptr->curZ) || checkIfWouldOutOfBounds(0,-1,0))){ //5
               push(0,-1,0,currentBlock,5); //Neuer Block nach hinten
               printf("y- ");
               solve(incr);
            }
     
            if(!(checkIfOccupied(ptr->curX-1, ptr->curY, ptr->curZ) || checkIfWouldOutOfBounds(-1,0,0))){ //6
               push(-1,0,0,currentBlock,6); //Neuer Block nach links
               printf("x- ");
               solve(incr);
            }
         break;
         case 2:
            if(!(checkIfOccupied(ptr->curX, ptr->curY, ptr->curZ+1) || checkIfWouldOutOfBounds(0,0,1))){ //1
               push(0,0,1,currentBlock,1); //Neuer Block nach oben
               printf("z+ ");
               solve(incr);}
     
            if(!(checkIfOccupied(ptr->curX+1, ptr->curY, ptr->curZ) || checkIfWouldOutOfBounds(1,0,0))){ //3
               push(1,0,0,currentBlock,3); //Neuer Block nach rechts
               printf("x+ ");
               solve(incr);}
     
            if(!(checkIfOccupied(ptr->curX, ptr->curY, ptr->curZ-1) || checkIfWouldOutOfBounds(0,0,-1))){ //4
               push(0,0,-1,currentBlock,4); //Neuer Block nach unten
               printf("z- ");
               solve(incr);}
     
            if(!(checkIfOccupied(ptr->curX-1, ptr->curY, ptr->curZ) || checkIfWouldOutOfBounds(-1,0,0))){ //6
               push(-1,0,0,currentBlock,6); //Neuer Block nach links
               printf("x- ");
               solve(incr);}
         break;
         case 3:
            if(!(checkIfOccupied(ptr->curX, ptr->curY, ptr->curZ+1) || checkIfWouldOutOfBounds(0,0,1))){ //1
               push(0,0,1,currentBlock,1); //Neuer Block nach oben
               printf("z+ ");
               solve(incr);}
     
            if(!(checkIfOccupied(ptr->curX, ptr->curY+1, ptr->curZ) || checkIfWouldOutOfBounds(0,1,0))){ //2
               push(0,1,0,currentBlock,2); //Neuer Block nach vorne
               printf("y+ ");
               solve(incr);}
    

            if(!(checkIfOccupied(ptr->curX, ptr->curY, ptr->curZ-1) || checkIfWouldOutOfBounds(0,0,-1))){ //4
               push(0,0,-1,currentBlock,4); //Neuer Block nach unten
               printf("z- ");
               solve(incr);}
     
            if(!(checkIfOccupied(ptr->curX, ptr->curY-1, ptr->curZ) || checkIfWouldOutOfBounds(0,-1,0))){ //5
               push(0,-1,0,currentBlock,5); //Neuer Block nach hinten
               printf("y- ");
               solve(incr);}
     

         break;
         case 4:
            if(!(checkIfOccupied(ptr->curX, ptr->curY+1, ptr->curZ) || checkIfWouldOutOfBounds(0,1,0))){ //2
               push(0,1,0,currentBlock,2); //Neuer Block nach vorne
               printf("y+ ");
               solve(incr);}
    
            if(!(checkIfOccupied(ptr->curX+1, ptr->curY, ptr->curZ) || checkIfWouldOutOfBounds(1,0,0))){ //3
               push(1,0,0,currentBlock,3); //Neuer Block nach rechts
               printf("x+ ");
               solve(incr);}
     

            if(!(checkIfOccupied(ptr->curX, ptr->curY-1, ptr->curZ) || checkIfWouldOutOfBounds(0,-1,0))){ //5
               push(0,-1,0,currentBlock,5); //Neuer Block nach hinten
               printf("y- ");
               solve(incr);}
     
            if(!(checkIfOccupied(ptr->curX-1, ptr->curY, ptr->curZ) || checkIfWouldOutOfBounds(-1,0,0))){ //6
               push(-1,0,0,currentBlock,6); //Neuer Block nach links
               printf("x- ");
               solve(incr);}
         break;
         case 5:
            if(!(checkIfOccupied(ptr->curX, ptr->curY, ptr->curZ+1) || checkIfWouldOutOfBounds(0,0,1))){ //1
               push(0,0,1,currentBlock,1); //Neuer Block nach oben
               printf("z+ ");
               solve(incr);}
     

            if(!(checkIfOccupied(ptr->curX+1, ptr->curY, ptr->curZ) || checkIfWouldOutOfBounds(1,0,0))){ //3
               push(1,0,0,currentBlock,3); //Neuer Block nach rechts
               printf("x+ ");
               solve(incr);}
     
            if(!(checkIfOccupied(ptr->curX, ptr->curY, ptr->curZ-1) || checkIfWouldOutOfBounds(0,0,-1))){ //4
               push(0,0,-1,currentBlock,4); //Neuer Block nach unten
               printf("z- ");
               solve(incr);}
     

            if(!(checkIfOccupied(ptr->curX-1, ptr->curY, ptr->curZ) || checkIfWouldOutOfBounds(-1,0,0))){ //6
               push(-1,0,0,currentBlock,6); //Neuer Block nach links
               printf("x- ");
               solve(incr);}
         break;
         case 6:
            if(!(checkIfOccupied(ptr->curX, ptr->curY, ptr->curZ+1) || checkIfWouldOutOfBounds(0,0,1))){ //1
               push(0,0,1,currentBlock,1); //Neuer Block nach oben
               printf("z+ ");
               solve(incr);}
     
            if(!(checkIfOccupied(ptr->curX, ptr->curY+1, ptr->curZ) || checkIfWouldOutOfBounds(0,1,0))){ //2
               push(0,1,0,currentBlock,2); //Neuer Block nach vorne
               printf("y+ ");
               solve(incr);}
    

            if(!(checkIfOccupied(ptr->curX, ptr->curY, ptr->curZ-1) || checkIfWouldOutOfBounds(0,0,-1))){ //4
               push(0,0,-1,currentBlock,4); //Neuer Block nach unten
               printf("z- ");
               solve(incr);}
     
            if(!(checkIfOccupied(ptr->curX, ptr->curY-1, ptr->curZ) || checkIfWouldOutOfBounds(0,-1,0))){ //5
               push(0,-1,0,currentBlock,5); //Neuer Block nach hinten
               printf("y- ");
               solve(incr);}
     

         break;
      }

   }
   
   if(length()!=VOLOUM){
      //inputIndex--;
      pop();
      printf("\b\b\b");
      return 1;
   }

   return 0;

        
}


void main() {
   readInput();
   push(0,0,0,1,1);
   inputIndex=1;
   solve(1);
   printf("\n\n\n");
   printListRev();
   printf("bounds: %d\n",checkIfOutOfBounds(BOUNDS,BOUNDS,BOUNDS));
   printf("indexCounter: %d",inputIndex);
  
   
}