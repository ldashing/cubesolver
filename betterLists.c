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
   globLength++;
   inputIndex++;
   struct node *link = (struct node*) malloc(sizeof(struct node));
	
   if(head == NULL){
      link->index = 1;
      link->curX = ORIGEN;
      link->curY = ORIGEN;
      link->curZ = ORIGEN;
      space[ORIGEN][ORIGEN][ORIGEN]=1;
   }else{
      link->index = head->index+1;
      link->curX = head->curX+offsetX;
      link->curY = head->curY+offsetY;
      link->curZ = head->curZ+offsetZ;
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
   #ifdef DEBUG
   /*
   printf("minX: %d   maxX: %d   size: %d   maxSize: %d\n",minX, maxX, diffX, boundX);
   printf("minY: %d   maxY: %d   size: %d   maxSize: %d\n",minY, maxY, diffY, boundY);
   printf("minZ: %d   maxZ: %d   size: %d   maxSize: %d\n\n",minZ, maxZ, diffZ, boundZ);
   */ 
   #endif
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

int tryToPush(struct node* stack, int pushX, int pushY, int pushZ, int heading, int blockType){
   if(!(checkIfOccupied(stack->curX+pushX, stack->curY+pushY, stack->curZ+pushZ) || checkIfWouldOutOfBounds(pushX,pushY,pushZ))){
      push(pushX,pushY,pushZ,blockType,heading); //Neuer Block 
      solve();
   }
}

int solve(){
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