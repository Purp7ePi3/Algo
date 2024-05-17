#include <stdio.h>
#include <stdlib.h>
int count = 0;
/*C recursive function to solve tower of hanoi puzzle*/
void towerOfHanoi(int n, char from_rod, char to_rod, char aux_rod)
{
	count ++;
	if (n == 1)
	{
		//printf("Move disk 1 from rod %c to rod %c\n", from_rod, to_rod);
		return;
	}
	towerOfHanoi(n-1, from_rod, aux_rod, to_rod);
	//printf("Move disk %d from rod %c to rod %c\n", n, from_rod, to_rod);
	towerOfHanoi(n-1, aux_rod, to_rod, from_rod);
}

int main(int argc,char *argv[])
{   
    int n = atoi(argv[1]);
	
	if(argc<=1){
		printf("Metti un argomento in input");
		return 1;
	}
	/*int n = 4; // Number of disks*/
	towerOfHanoi(n, 'A', 'C', 'B');
	printf("%d",count);
	return 0;
}
