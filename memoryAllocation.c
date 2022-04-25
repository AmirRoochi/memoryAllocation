

#include <stdio.h>
#include <stdlib.h>

int physicalSize = 0;
int algorithm = -1;
int freeIndex = -1;

typedef struct block{
    int start;
    int size;
} block;

block * list = NULL;


void release(){
    if(physicalSize > 0){
        physicalSize = 0;
        algorithm = -1;
        freeIndex = -1;
        free(list);
    }
}//release

void enter(){
    int i;
    release();
    printf("Enter size of physical memory: ");
    scanf("%d", &physicalSize);
    if (physicalSize <= 0){
        printf("Invalid physical memory size. \n");
        return;
    }
    printf("Enter hole-fitting algorithm (0 = first fit, 1 =  best fit): ");
    scanf("%d", &algorithm);
    if (algorithm != 0 && algorithm != 1){
        printf("Invalid algorithm selection. \n");
        physicalSize = 0;
        return;
    }
    list = (block *)malloc(physicalSize * sizeof(block));
    for(i = 0; i < physicalSize; i++){
        list[i].start = -1;
        //list[i].size = 0; updated.
        list[i].size = -1;
    }
    freeIndex = 0;
    
    
}//enter

void printList(){
    int i;
    printf("\n");
    printf("Index\tStart\tOpening\n");
    printf("-----------------------------\n");
    for(i = 0; i < freeIndex; i++){
        int end = list[i].start + list[i].size;
        printf("%d\t%d\t%d\n", i, list[i].start, end);
    }
    printf("Size: %d\n", physicalSize);
    
}//printList

void allocate(){
    int i, blockSize = 0;
    int openLocation = 0;
    int openSize;
    int selection = -1;
    int smallestSize = physicalSize + 1;
    int start;
    
    if (physicalSize <= 0){
        printf("Invalid physical memory size. \n");
        return;
    }else if(freeIndex == physicalSize){
        printf("Memory is full. Dallocate first. \n");
        return;
    }
    printf("Enter block Size: ");
    scanf("%d", &blockSize);
    if(blockSize <= 0){
        printf("Invalid block size. \n");
        return;
    }
    //check openings between physical address 0 and block 0's 
    for(i = 0; i < freeIndex; i++){
        openSize = list[i].start - openLocation;
        if(blockSize <= openSize){
            if(algorithm == 0 && selection == -1){
                selection = i;
                start = openLocation;
                
            }else if(algorithm == 1 && smallestSize > openSize){
                selection = i;
                start = openLocation;
                smallestSize = openSize;
            }
        }
        openLocation = list[i].start + list[i].size;
    }
    
    //check openings between physical end memory location and last block location
    openSize = physicalSize - openLocation;
    if(blockSize <= openSize){
            if(algorithm == 0 && selection == -1){
                selection = freeIndex;
                start = openLocation;
                
            }else if(algorithm == 1 && smallestSize > openSize){
                selection = freeIndex;
                start = openLocation;
            }
        }
    //perform insertion
    if(selection >= 0){
        for(i = freeIndex; i > selection; i--){
            list[i].start = list[i-1].start;
            list[i].size = list[i-1].size;
        }
        list[selection].start = start;
        list[selection].size = blockSize;
        freeIndex++;
        int end = start + blockSize;
        printf("New Block inserted, starting at %d and ending before %d\n",start, end);
    }else{
        printf("Unable to insert new block\n");
        return;
    }
    
    
    printList();
}//allocate

void deallocate(){
    int index = -1;
    int i;
    if (physicalSize <= 0){
        printf("Invalid physical memory size. \n");
        return;
    }else if(freeIndex == 0){
        printf("Memory is empty allocate first. \n");
        return;
    }
    //perofrm deletion at selected Index
    printf("Enter block index: ");
    scanf("%d", &index);
    if(index < 0){
        printf("Invalid block index, too low.\n");
        return;
    }else if(index >= freeIndex){
        printf("Invalid block index, too high.\n");
        return;
    }
    freeIndex--;
    for(i = index; i < freeIndex; i++){
        list[i].start = list[i+1].start;
        list[i].size = list[i+1].size;
    }
    list[freeIndex].start = -1;
    list[freeIndex].size = -1;
    printf("Block successfully de-allocated.\n");
    //print
    printList();
}//deallocate

void defragment(){
    int i;
    if (physicalSize <= 0){
        printf("Invalid physical memory size. \n");
        return;
    }else if(freeIndex == 0){
        printf("Memory is empty allocate first. \n");
        return;
    }
    //change every blocks starting address into previous block ending address
    list[0].start = 0;
    
    for(i = 1; i < freeIndex; i++){
        list[i].start = list[i-1].start + list[i-1].size;
    }
    printf("Memory successfully defragmented.\n");
    //print
    printList();
    
}//defragment







int main () {
  int option = 0;
  while (option != 5) {
      //used for program safety and avoid infinite loop
      option = 5;
      printf ("Memory Allocation: \n");
      printf ("---------------------\n");
      printf ("1) Enter Parameters\n");
      printf ("2) Allocate memory for block\n");
      printf ("3) Deallocate memory for block\n");
      printf ("4) Defragment memory\n");
      printf ("5) Quit Program and free memory\n");
      printf ("\n");
      printf ("Enter Selection: ");
      scanf ("%d", &option);
      printf ("\n");
      switch (option) {
	case 1:
	  enter();
	  break;
	case 2:
	  allocate();
	  break;
	case 3:
	  deallocate();
	  break;
	case 4:
	  defragment();
	  break;
	case 5:
	  printf ("Goodbye");
	  release();
	  break;
	default:
	  printf ("Invalid option, try again. \n");
	}
      printf ("\n");
    }

  return 0;
}

