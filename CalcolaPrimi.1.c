/*
 * http://www.yolinux.com/TUTORIALS/LinuxTutorialPosixThreads.html
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>

/* Compiling with: -Wall -pedantic -lpthread*/

#define N 500*1000 /*limite numeri primi*/
#define nThread 2
#define deb 1
#define DIM 500

/*global vars*/
/* inizilizzando k=3, viene automaticamente inserito il 2 tra i numeri
 * primi, perciò count è inizializzato a 1*/
 
int count=1;
int k=3;

/*locking vars*/
pthread_mutex_t MUTEX_COUNT=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t MOUTEX_K=PTHREAD_MUTEX_INITIALIZER;


void threads(int nThreads);
void * CalcolaPrimi (void * ptr);

int main(int argc,char * argv[])
{
	int nThreads;	/*numero di threads da creare*/
	clockid_t clk;
	struct timespec tpi,tpf;
	long nseci,nsecf;
	
	if(deb>=1) 
		printf("Argc= %d \n",argc);

	if(argc>=2)
	{
		if(deb>1) 
			printf("args ::  %s %s \n",argv[0],argv[1]);
			
		nThreads=atoi(argv[1]);
		
		if(deb>1) 
			printf("in arg evaluation: NChildren= %d \n ",nThreads);
		
		if(nThreads<1)
		{
			printf("\nError retriving NC from arg. Set it at %d default value.\n",nThread);
			nThreads=nThread;
		}
		/* else if(NChildren>NChild)	NChildren=NChild; */
	
	}
	else 
	{
		nThreads=nThread;
	}
	
	clock_gettime(clk,&tpi);	
	nseci=tpi.tv_nsec;
	
	threads(nThreads);	
	 
	printf("\nFind %i prime numbers from 2 to %i. \n",count, N); 
	clock_gettime(clk,&tpf);	
	nsecf=tpf.tv_nsec;
	nsecf=nsecf-nseci+1000000000*(tpf.tv_sec-tpi.tv_sec);
	printf("nsec passati %li usec %li msec %li \n",nsecf,nsecf/1000,nsecf/1000000);
		
	exit(0);
}

void threads(int nThreads)
{
	int i,ret;
	pthread_t thread[nThreads];
	/* if (deb==1)	*/
		printf("  threads()  ::  Numero di threads= %i.\n",nThreads);
	/* Create independent threads each of which will execute function */
	for(i=0;i<nThreads;i++)
	{	ret=pthread_create(&thread[i],NULL,CalcolaPrimi,(void*) &i);

		/*controllo sul valore di ritono del thread*/
		if(ret!=0)
		{
			printf("Error - pthread_create() return code: %i\n",ret);
			exit(0);
		}
	}
	
	printf("  threads :: Waiting for babies. \n");
	/* Wait till threads are complete before main continues. Unless we  */
	/* wait we run the risk of executing an exit which will terminate   */
	/* the process and all threads before the threads have completed.   */

	/*wait -> si aspetta che tutti finiscano e terminano*/
	 for(i=0;i<nThreads;i++)
		pthread_join(thread[i],NULL);
}

void * CalcolaPrimi(void *ptr)
{
	int nt; /*n thread*/
	int i,j=2, q;
	bool primo=true;
	
	if(ptr==NULL)
		printf("Error!!! ptr == NULL!!!!\n");
	else 
		nt= * (int *) ptr; 
		
	i=1;
	if (deb==1) printf("Baby %i: i=%i \n",nt,i);
	
	/*ricerca primi*/
	pthread_mutex_lock(&MOUTEX_K);
	while(k<N)
	{	
		/*il controllo avviene solamente sui dispari*/
		q=k;
		k+=2;
		/*è necessario bloccare i threads in modo che entrino nel ciclo 
			solamente dopo che il precedente ha aumentato il valore di k*/
		pthread_mutex_unlock(&MOUTEX_K);
		primo=true;
		
		for(;j<q;j++)
		{
			if(q%j==0)			
			{					/*se primo=!false --> k è primo*/
				primo=false;    /*se primo=false  --> k non è primo*/
				break;
			}		
		}
		
		j=2;
	
		if(primo==true)
		{	
			//inizio zona critica, limitato l'acceso alla variabile count
			pthread_mutex_lock(&MUTEX_COUNT);
			
			count++;
			
			//fine zona critica
			pthread_mutex_unlock(&MUTEX_COUNT);
			
		}
		
		pthread_mutex_lock(&MOUTEX_K);
		
		//k+=2;
	}
	
	pthread_mutex_unlock(&MOUTEX_K);
	
	while (i<DIM)
	{
		i++;	
	}
	if (deb==1) 
		printf("Baby %i: Ended\n",nt);
	
	return 0;
}

	
