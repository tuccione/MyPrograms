/*Test git*/
#include <stdio.h>
#include <stdbool.h>

#define MAX 50*1000*1000

int main(int argc,char ** argv)
{
	int i=3;
	int j=2;
	
	bool primo=true;
	int count=1;
	
	/*numero limite tra cui cercare i primi, (inserire da linea di comando)*/	
	int N=atoi(argv[1]); 
	
	/*ricerca primi*/
	for(;i<N;)
	{	
		primo=true;
		
		for(;j<i;j++)
		{
			if(i%j==0)			//se primo=!false --> i è primo
			primo=false;		//se primo=false  --> i non è primo
		}
		
		j=2;
	
		if(primo==true)
			count++;
			
		i=i+2;
	}
	
	printf("\nTrovati %i numeri primi fra 2 e %i. \n",count, N);
	
	return 0;
	
}
