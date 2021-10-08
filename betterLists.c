#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define ORIGEN 0
#define BOUNDS 4
#define VOLOUM BOUNDS*BOUNDS*BOUNDS

int space[BOUNDS][BOUNDS][BOUNDS]={0};

int inputSnake[100] = {0};
int inputIndex = 0;

int solve();
int globLength=0;

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

void push(int offsetX, int offsetY, int offsetZ, int newType, int newFacing) {
   //create a link
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
      
      link->maxSizeX = 0;
      link->maxSizeY = 0;
      link->maxSizeZ = 0;

      link->minSizeX = 0;
      link->minSizeY = 0;
      link->minSizeZ = 0;
      

      space[ORIGEN][ORIGEN][ORIGEN]=1;
   }else{
      link->index = head->index+1;

      link->curX = head->curX+offsetX;
      link->curY = head->curY+offsetY;
      link->curZ = head->curZ+offsetZ;


      if(link->curX < head->minSizeX){link->minSizeX = link->curX;}else{link->minSizeX = head->minSizeX;}
      if(link->curX > head->maxSizeX){link->maxSizeX = link->curX;}else{link->maxSizeX = head->maxSizeX;}

      if(link->curY < head->minSizeY){link->minSizeY = link->curY;}else{link->minSizeY = head->minSizeY;}
      if(link->curY > head->maxSizeY){link->maxSizeY = link->curY;}else{link->maxSizeY = head->maxSizeY;}

      if(link->curZ < head->minSizeZ){link->minSizeZ = link->curZ;}else{link->minSizeZ = head->minSizeZ;}
      if(link->curZ > head->maxSizeZ){link->maxSizeZ = link->curZ;}else{link->maxSizeZ = head->maxSizeZ;}

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

bool isEmpty() {
   return head == NULL;
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
      //printf("Out of bounds\n\n\n");
      return 1;
   }
   return 0;
}

int checkIfWouldOutOfBounds(int extraX, int extraY, int extraZ){
/*
   struct node *ptr = head;

   if(ptr->diffX+extraX > BOUNDS || ptr->diffY+extraY > BOUNDS || ptr->diffZ+extraZ > BOUNDS){
      printf("Out of bounds\n\n\n");
      return 1;
   }
   return 0;
   */
   
   push(extraX,extraY,extraZ,66,66);
   int tmp = checkIfOutOfBounds(BOUNDS,BOUNDS,BOUNDS);
   pop();
   if(tmp){
     // printf("wouldbeout\n");
   }
   return tmp;
   
}

int tryToPush(struct node* stack, int pushX, int pushY, int pushZ, int heading, int blockType){
   if(!(checkIfOccupied(stack->curX+pushX, stack->curY+pushY, stack->curZ+pushZ) || checkIfWouldOutOfBounds(pushX,pushY,pushZ))){
      push(pushX,pushY,pushZ,blockType,heading); //Neuer Block 
      solve();
   }
}

int solve(){
   struct node *ptr = head;
         //printf("Index: %d    X: %d  Y: %d  Z: %d  Type: %d  Facing: %d diffx#y#z %d#%d#%d\n",ptr->index, ptr->curX, ptr-> curY, ptr->curZ, ptr->type, ptr->facing, ptr->diffX, ptr->diffY, ptr->diffZ);


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
            tryToPush(ptr, 0, 0, 1, head->facing, currentBlock);
         break;
         case 2:
            tryToPush(ptr, 0, 1, 0, head->facing, currentBlock);
         break;
         case 3:
            tryToPush(ptr, 1, 0, 0, head->facing, currentBlock);
         break;
         case 4:
            tryToPush(ptr, 0, 0, -1, head->facing, currentBlock);
         break;
         case 5:
            tryToPush(ptr, 0, -1, 0, head->facing, currentBlock);
         break;
         case 6:
            tryToPush(ptr, -1, 0, 0, head->facing, currentBlock);
         break;
      }
   }else if(head->type==2){
      switch(head->facing){
         case 1:
            tryToPush(ptr, 0, 1, 0, 2, currentBlock);
            tryToPush(ptr, 1, 0, 0, 3, currentBlock);
            tryToPush(ptr, 0, -1, 0, 5, currentBlock);
            tryToPush(ptr, -1, 0, 0, 6, currentBlock);
         break;
         case 2:
            tryToPush(ptr, 0, 0, 1, 1, currentBlock);
            tryToPush(ptr, 1, 0, 0, 3, currentBlock);
            tryToPush(ptr, 0, 0, -1, 4, currentBlock);
            tryToPush(ptr, -1, 0, 0, 6, currentBlock);
         break;
         case 3:
            tryToPush(ptr, 0, 0, 1, 1, currentBlock);
            tryToPush(ptr, 0, 1, 0, 2, currentBlock);
            tryToPush(ptr, 0, 0, -1, 4, currentBlock);
            tryToPush(ptr, 0, -1, 0, 5, currentBlock);
         break;
         case 4:
            tryToPush(ptr, 0, 1, 0, 2, currentBlock);
            tryToPush(ptr, 1, 0, 0, 3, currentBlock);
            tryToPush(ptr, 0, -1, 0, 5, currentBlock);
            tryToPush(ptr, -1, 0, 0, 6, currentBlock);
         break;
         case 5:
            tryToPush(ptr, 0, 0, 1, 1, currentBlock);
            tryToPush(ptr, 1, 0, 0, 3, currentBlock);
            tryToPush(ptr, 0, 0, -1, 4, currentBlock);
            tryToPush(ptr, -1, 0, 0, 6, currentBlock);
         break;
         case 6:
            tryToPush(ptr, 0, 0, 1, 1, currentBlock);
            tryToPush(ptr, 0, 1, 0, 2, currentBlock);
            tryToPush(ptr, 0, 0, -1, 4, currentBlock);
            tryToPush(ptr, 0, -1, 0, 5, currentBlock);
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


int main() {
   readInput();
   push(0,0,0,1,1);

   inputIndex=1;
   solve(1);
   printf("\n\n\n");
   printListRev();
   printf("bounds: %d\n",checkIfOutOfBounds(BOUNDS,BOUNDS,BOUNDS));
   printf("indexCounter: %d",inputIndex);
  
   return 0;
}