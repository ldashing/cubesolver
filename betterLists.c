#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


#define ORIGEN 10
#define BOUNDS 4
#define VOLOUM BOUNDS*BOUNDS*BOUNDS


int space[20][20][20]={0};

int inputSnake[100] = {0};
int inputIndex = 0;

int solve();
int globLength=0;

unsigned long visitedNodes=0;

int max(int num1, int num2){
    return (num1 > num2 ) ? num1 : num2;
}

int min(int num1, int num2) {
    return (num1 > num2 ) ? num2 : num1;
}

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

struct node{
   int curX;
   int curY;
   int curZ;
   
   int minSizeX;
   int minSizeY;
   int minSizeZ;

   int maxSizeX;
   int maxSizeY;
   int maxSizeZ;
   
   int diffX;
   int diffY;
   int diffZ;

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
      //printf("Index: %d    X: %d  Y: %d  Z: %d  Type: %d  Facing: %d Max#MinSizeX: %d#%d Max#MinSizeX: %d#%d Max#MinSizeX: %d#%d diffx#y#z %d#%d#%d\n",ptr->index, ptr->curX, ptr-> curY, ptr->curZ, ptr->type, ptr->facing, ptr->maxSizeX, ptr->minSizeX, ptr->maxSizeY, ptr->minSizeY, ptr->maxSizeZ, ptr->minSizeZ, ptr->diffX, ptr->diffY, ptr->diffZ);
      printf("Index: %d    X: %d  Y: %d  Z: %d  Type: %d  Facing: %d diffx#y#z %d#%d#%d\n",ptr->index, ptr->curX, ptr-> curY, ptr->curZ, ptr->type, ptr->facing, ptr->diffX, ptr->diffY, ptr->diffZ);
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

void pushX(int offset, int newType, int newFacing) {
   visitedNodes++;
   globLength++;
   inputIndex++;
   struct node *link = (struct node*) malloc(sizeof(struct node));
	
   link->index = head->index+1;

   link->curX = head->curX+offset;
   link->curY = head->curY;
   link->curZ = head->curZ;
      
   link->minSizeX=min(link->curX,head->minSizeX);
   link->maxSizeX=max(link->curX,head->maxSizeX);

   link->minSizeY=head->minSizeY;
   link->maxSizeY=head->maxSizeY;

   link->minSizeZ=head->minSizeZ;
   link->maxSizeZ=head->maxSizeZ;
      

   link->diffX=abs(link->minSizeX-link->maxSizeX)+1;
   link->diffY=head->diffY;
   link->diffZ=head->diffZ;
    
   space[head->curX+offset][head->curY][head->curZ]=1;
   
   link->type = newType;
   link->facing = newFacing;

   link->next = head;

   head = link;
}

void pushY(int offset, int newType, int newFacing) {
   visitedNodes++;
   globLength++;
   inputIndex++;
   struct node *link = (struct node*) malloc(sizeof(struct node));

   link->index = head->index+1;

   link->curX = head->curX;
   link->curY = head->curY+offset;
   link->curZ = head->curZ;
      
   link->minSizeX=head->minSizeX;
   link->maxSizeX=head->maxSizeX;

   link->minSizeY=min(link->curY,head->minSizeY);
   link->maxSizeY=max(link->curY,head->maxSizeY);

   link->minSizeZ=head->minSizeZ;
   link->maxSizeZ=head->maxSizeZ;
      

   link->diffX=head->diffX;
   link->diffY=abs(link->minSizeY-link->maxSizeY)+1;
   link->diffZ=head->diffZ;
    
   space[head->curX][head->curY+offset][head->curZ]=1;

   link->type = newType;
   link->facing = newFacing;

   link->next = head;

   head = link;
}

void pushZ(int offset, int newType, int newFacing) {
   visitedNodes++;
   globLength++;
   inputIndex++;
   struct node *link = (struct node*) malloc(sizeof(struct node));

   link->index = head->index+1;

   link->curX = head->curX;
   link->curY = head->curY;
   link->curZ = head->curZ+offset;
      
   link->minSizeX=head->minSizeX;
   link->maxSizeX=head->maxSizeX;

   link->minSizeY=head->minSizeY;
   link->maxSizeY=head->maxSizeY;

   link->minSizeZ=min(link->curZ,head->minSizeZ);
   link->maxSizeZ=max(link->curZ,head->maxSizeZ);
      

   link->diffX=head->diffX;
   link->diffY=head->diffY;
   link->diffZ=abs(link->minSizeZ-link->maxSizeZ)+1;
    
   space[head->curX][head->curY][head->curZ+offset]=1;

   link->type = newType;
   link->facing = newFacing;

   link->next = head;

   head = link;
}

void push(int offsetX, int offsetY, int offsetZ, int newType, int newFacing) {
   globLength++;
   inputIndex++;
   struct node *link = (struct node*) malloc(sizeof(struct node));
	
   if(head == NULL){
      link->index = 1;
      link->curX = ORIGEN;
      link->curY = ORIGEN;
      link->curZ = ORIGEN;

      link->diffX=0;
      link->diffY=0;
      link->diffZ=0;
      
      link->maxSizeX = ORIGEN;
      link->maxSizeY = ORIGEN;
      link->maxSizeZ = ORIGEN;

      link->minSizeX = ORIGEN;
      link->minSizeY = ORIGEN;
      link->minSizeZ = ORIGEN;
      
      space[ORIGEN][ORIGEN][ORIGEN]=1;
   }else{
      link->index = head->index+1;

      link->curX = head->curX+offsetX;
      link->curY = head->curY+offsetY;
      link->curZ = head->curZ+offsetZ;
      
      link->minSizeX=min(link->curX,head->minSizeX);
      link->maxSizeX=max(link->curX,head->maxSizeX);

      link->minSizeY=min(link->curY,head->minSizeY);
      link->maxSizeY=max(link->curY,head->maxSizeY);

      link->minSizeZ=min(link->curZ,head->minSizeZ);
      link->maxSizeZ=max(link->curZ,head->maxSizeZ);
      

      link->diffX=abs(link->minSizeX-link->maxSizeX)+1;
      link->diffY=abs(link->minSizeY-link->maxSizeY)+1;
      link->diffZ=abs(link->minSizeZ-link->maxSizeZ)+1;
    
      space[head->curX+offsetX][head->curY+offsetY][head->curZ+offsetZ]=1;
   }
   link->type = newType;
   link->facing = newFacing;


	
   //point it to old first node
   link->next = head;
	
   //point first to new first node
   head = link;
}

struct node* deleteFirst() {
   space[head->curX][head->curY][head->curZ]=0;

   //save reference to first link
   struct node *tempLink = head;
	
   //mark next to first link as first 
   head = head->next;
	free(tempLink);
   //return the deleted link
   return tempLink;
}

int length() {	
   return globLength;
}

void pop(){
   inputIndex--;
   globLength--;
   struct node *temp = deleteFirst();
}

int checkIfOccupied(int x, int y, int z){
   return space[x][y][z];
}

int checkIfOutOfBounds(int boundX, int boundY, int boundZ){
   struct node *ptr = head;

   if(ptr->diffX > boundX || ptr->diffY > boundY || ptr->diffZ > boundZ){
      return 1;
   }
   return 0;
}

int tryToPush(struct node* stack, int pushX, int pushY, int pushZ, int heading, int blockType){
   if(!(checkIfOccupied(stack->curX+pushX, stack->curY+pushY, stack->curZ+pushZ))){
      push(pushX,pushY,pushZ,blockType,heading); //Neuer Block 
      solve();
   }
}

int tryToPushX(struct node* stack, int push, int heading, int blockType){
   if(checkIfOccupied(stack->curX+push, stack->curY, stack->curZ)){
      return 0;
   }
   if(head->diffX==BOUNDS){
      if(head->curX+push > head->maxSizeX || head->curX+push < head->minSizeX){
         //printf("wouldb \n");
         return 0;
      }
      /*
      if(blockType==1){
         if(head->curX+push+push > head->maxSizeX || head->curX+push+push < head->minSizeX){
           //printf("wouldb \n");
            return 0;
         }
      }
      */
   }
   pushX(push,blockType,heading); //Neuer Block 
   solve();
}

int tryToPushY(struct node* stack, int push, int heading, int blockType){
   if(checkIfOccupied(stack->curX, stack->curY+push, stack->curZ)){
      return 0;
   }
   if(head->diffY==BOUNDS){
      if(head->curY+push > head->maxSizeY || head->curY+push < head->minSizeY){
         //printf("wouldb \n");
         return 0;
      }
      /*
      if(blockType==1){
         if(head->curY+push+push > head->maxSizeY || head->curY+push+push < head->minSizeY){
           //printf("wouldb \n");
            return 0;
         }
      }
      */
   }
   pushY(push,blockType,heading); //Neuer Block 
   solve();
}

int tryToPushZ(struct node* stack, int push, int heading, int blockType){
   if(checkIfOccupied(stack->curX, stack->curY, stack->curZ+push)){
      return 0;
   }
   if(head->diffZ==BOUNDS){
      if(head->curZ+push > head->maxSizeZ || head->curZ+push < head->minSizeZ){
         //printf("wouldb \n");
         return 0;
      }
      /*
      if(blockType==1){
         if(head->curZ+push+push > head->maxSizeZ || head->curZ+push+push < head->minSizeZ){
           //printf("wouldb \n");
            return 0;
         }
      }
      */
   }
   pushZ(push,blockType,heading); //Neuer Block 
   solve();
}

int solve(){
   
   if(globLength==VOLOUM){
      return 0;
   }

   struct node *ptr = head;
   int currentBlock = inputSnake[inputIndex];

   if(head->type==1){//striaght block
      switch(head->facing){
         case 1:
            tryToPushZ(ptr, 1, head->facing, currentBlock);
         break;
         case 2:
            tryToPushY(ptr, 1, head->facing, currentBlock);
         break;
         case 3:
            tryToPushX(ptr, 1, head->facing, currentBlock);
         break;
         case 4:
            tryToPushZ(ptr, -1, head->facing, currentBlock);
         break;
         case 5:
            tryToPushY(ptr, -1, head->facing, currentBlock);
         break;
         case 6:
            tryToPushX(ptr, -1, head->facing, currentBlock);
         break;
      }
   }else if(head->type==2){
      switch(head->facing){
         case 1:
            tryToPushY(ptr, 1, 2, currentBlock);
            tryToPushX(ptr, 1, 3, currentBlock);
            tryToPushY(ptr, -1, 5, currentBlock);
            tryToPushX(ptr, -1, 6, currentBlock);
         break;
         case 2:
            tryToPushZ(ptr, 1, 1, currentBlock);
            tryToPushX(ptr, 1, 3, currentBlock);
            tryToPushZ(ptr, -1, 4, currentBlock);
            tryToPushX(ptr, -1, 6, currentBlock);
         break;
         case 3:
            tryToPushZ(ptr, 1, 1, currentBlock);
            tryToPushY(ptr, 1, 2, currentBlock);
            tryToPushZ(ptr, -1, 4, currentBlock);
            tryToPushY(ptr, -1, 5, currentBlock);
         break;
         case 4:
            tryToPushY(ptr, 1, 2, currentBlock);
            tryToPushX(ptr, 1, 3, currentBlock);
            tryToPushY(ptr, -1, 5, currentBlock);
            tryToPushX(ptr, -1, 6, currentBlock);
         break;
         case 5:
            tryToPushZ(ptr, 1, 1, currentBlock);
            tryToPushX(ptr, 1, 3, currentBlock);
            tryToPushZ(ptr, -1, 4, currentBlock);
            tryToPushX(ptr, -1, 6, currentBlock);
         break;
         case 6:
            tryToPushZ(ptr, 1, 1, currentBlock);
            tryToPushY(ptr, 1, 2, currentBlock);
            tryToPushZ(ptr, -1, 4, currentBlock);
            tryToPushY(ptr, -1, 5, currentBlock);
         break;
      }
   }
   
   if(globLength!=VOLOUM){
      //inputIndex--;
      pop();
      #ifdef DEBUG
      printf("\b\b\b");
      #endif
      return 1;
   }
   return 0;
}

void printMovesInteractiv(){
   printf("\nPress enter for next move\n");
   reverse(&head);
   struct node *ptr = head;
   while(ptr->next != NULL) {
      int xdiff = ptr->next->curX - ptr->curX;
      int ydiff = ptr->next->curY - ptr->curY;
      int zdiff = ptr->next->curZ - ptr->curZ;

      if(xdiff==1){printf("x+ ");}
      else if(xdiff==-1){printf("x- ");}
      else if(ydiff==1){printf("y+ ");}
      else if(ydiff==-1){printf("y- ");}
      else if(zdiff==1){printf("z+ ");}
      else if(zdiff==-1){printf("z- ");}
      ptr = ptr->next;

      getchar();
      printf("\n\n\n");
   }
   reverse(&head);
   printf("\n");
}

void printMoves(){
   printf("\n");
   reverse(&head);
   struct node *ptr = head;
   while(ptr->next != NULL) {
      int xdiff = ptr->next->curX - ptr->curX;
      int ydiff = ptr->next->curY - ptr->curY;
      int zdiff = ptr->next->curZ - ptr->curZ;

      if(xdiff==1){printf("x+ ");}
      else if(xdiff==-1){printf("x- ");}
      else if(ydiff==1){printf("y+ ");}
      else if(ydiff==-1){printf("y- ");}
      else if(zdiff==1){printf("z+ ");}
      else if(zdiff==-1){printf("z- ");}
      //printf("xdiff: %d\n",xdiff);

      //printf("Index: %d    X: %d  Y: %d  Z: %d  Type: %d  Facing: %d Max#MinSizeX: %d#%d Max#MinSizeX: %d#%d Max#MinSizeX: %d#%d diffx#y#z %d#%d#%d\n",ptr->index, ptr->curX, ptr-> curY, ptr->curZ, ptr->type, ptr->facing, ptr->maxSizeX, ptr->minSizeX, ptr->maxSizeY, ptr->minSizeY, ptr->maxSizeZ, ptr->minSizeZ, ptr->diffX, ptr->diffY, ptr->diffZ);
      //printf("Index: %d    X: %d  Y: %d  Z: %d  Type: %d  Facing: %d diffx#y#z %d#%d#%d\n",ptr->index, ptr->curX, ptr-> curY, ptr->curZ, ptr->type, ptr->facing, ptr->diffX, ptr->diffY, ptr->diffZ);
      ptr = ptr->next;
   }
   reverse(&head);
   printf("\n");


}

int main() {
   readInput();
   push(0,0,0,1,1);

   inputIndex=1;
   printf("\nSolving...\n");

   solve();
   printf("\nDone\n");
   printf("visitedNodes: %lu",visitedNodes);

   printListRev();
   //printf("bounds: %d\n",checkIfOutOfBounds(BOUNDS,BOUNDS,BOUNDS));
   //printf("indexCounter: %d",inputIndex);
   printMoves();  
   return 0;
}