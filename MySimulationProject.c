#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>

int InitBowls(int BowlsCount);
void CatEat(int bowl_num);
void MouseEat(int bowl_num);
void CatSleep(void);
void MouseSleep(void);

int NumCats,NumMice,NumBowls,NumLoops;

static char bowls[20];

static int EatingCatsCounter,EatingMiceCounter;

sem_t mutex;

int ce=1,me=3,cs=5,ms=3;

void CatEat(int bowl_num)
{
	if((bowl_num == 0) || (bowl_num>NumBowls))
		{
			printf("Invalid Bowl number %d While in CatsEat \n",bowl_num);
		}
	sem_wait(&mutex);
	if(bowls[bowl_num-1]=='c')
		{
			printf("CatEat is attempting to make two cats eat from the same bowl number : %d \n",bowl_num);
		}
	if(EatingMiceCounter>0)
		{
			printf("CatEat is making an attempt made by cat while mice are eating.\n");
		}

	if((bowls[bowl_num-1]=='0')&&(EatingMiceCounter==0))
		{
			EatingCatsCounter+=1;
			bowls[bowl_num-1]='c';
			printf("CatEat started bowl number %d \n",bowl_num);
		}

	sem_post(&mutex);
	sleep(ce);
	sem_wait(&mutex);

	if((EatingCatsCounter>0)&&(bowls[bowl_num-1]=='c'))
		{
			printf("CatEat Finish from number %d \n",bowl_num);
		}

	sem_post(&mutex);
	return;
}	

void CatSleep()
{
	sleep(cs);
	return;
}


void MouseEat(int bowl_num)
{
	if((bowl_num == 0) || (bowl_num>NumBowls))
		{
			printf("Invalid Bowl number %d While mouse is Eating \n",bowl_num);
		}
	sem_wait(&mutex);
	if(bowls[bowl_num-1]=='m')
		{
			printf("MouseEat is attempting to make two mice eat from the same bowl number : %d \n",bowl_num);
		}
	if(EatingCatsCounter>0)
		{
			printf("MouseEat is making an attempt made by mouse while cats are eating.\n");
		}
	if((bowls[bowl_num-1]=='0')&&(EatingCatsCounter==0))
		{	
			EatingMiceCounter+=1;
			bowls[bowl_num-1]='c';
			printf("MouseEat started bowl number %d \n",bowl_num);
		}
	sem_post(&mutex);
	sleep(me);
	sem_wait(&mutex);
	
	if((EatingMiceCounter>0)&&(bowls[bowl_num-1]=='m'))
	{
		printf("MouseEat Finish from number %d \n",bowl_num);
	}

	sem_post(&mutex);
	return;
}

void MouseSleep()
{
	sleep(ms);
	return;
}

int InitBowls(int BowlsCount)
{
	int i;
	if(BowlsCount==0)
		{
			printf("There is no food to eat for mice and cats");
			return 0;
		}
	else
		{
			bowls[BowlsCount];

			for(i=0;i<BowlsCount;i++)
				{
					bowls[i]='0';
				}
			EatingCatsCounter=EatingMiceCounter=0;
			NumBowls=BowlsCount;
			sem_init(&mutex,0,1);

			if(&mutex == NULL)
				{
					printf("initialize bowls properly");
				}
		}
	return 0;
}




void CatSimulation(void * unusedpointer,int catnumber)
{
  int i;
  int bowl;

(void) unusedpointer;
(void) catnumber;


  for(i=0;i<NumLoops;i++) 
  	{
		CatSleep();

    bowl = ((int)random() % NumBowls) + 1;
    CatEat(bowl);

  }
  sem_post(&mutex); 
}

void MouseSimulation(void * unusedpointer,int mousenumber)
{
  int i;
  int bowl;

(void) unusedpointer;
(void) mousenumber;

  for(i=0;i<NumLoops;i++) 
  	{
		MouseSleep();

    bowl = ((int)random() % NumBowls) + 1;
    MouseEat(bowl);

  }
  sem_post(&mutex); 
}

int catmouse(int nargs,int args[5])
{
  int index;
  pthread_t error;
  int i;
  

  if (nargs != 5) {
    printf("Usage: <command> NUM_BOWLS NUM_CATS NUM_MICE NUM_LOOPS\n");
    return 1;  // return failure indication
  }

  /* check the problem parameters, and set the global variables */
  NumBowls = args[1];
  if (NumBowls <= 0) {
    printf("catmouse: invalid number of bowls: %d\n",NumBowls);
    return 1;
  }
  NumCats = args[2];
  if (NumCats < 0) {
    printf("catmouse: invalid number of cats: %d\n",NumCats);
    return 1;
  }
  NumMice = args[3];
  if (NumMice < 0) {
    printf("catmouse: invalid number of mice: %d\n",NumMice);
    return 1;
  }
  NumLoops = args[4];
  if (NumLoops <= 0) {
    printf("catmouse: invalid number of loops: %d\n",NumLoops);
    return 1;
  }
  printf("Using %d bowls, %d cats, and %d mice. Looping %d times.\n",
          NumBowls,NumCats,NumMice,NumLoops);

  sem_init(&mutex,0,1);
  if (&mutex == NULL) {
    printf("catmouse: could not create semaphore\n");
  }

  if (InitBowls(NumBowls)) {
    printf("catmouse: error initializing bowls.\n");
  }

  for (index = 0; index < NumCats; index++) {
    pthread_create(&error,NULL,index,CatSimulation,NULL);
    pthread_join(error,NULL);
  }

  for (index = 0; index < NumMice; index++) {
    pthread_create(&error,NULL,MouseSimulation,NULL);
    pthread_join(error,NULL);
  }

  for(i=0;i<(NumCats+NumMice);i++) 
	{
   		sem_wait(&mutex);
  	}
  sem_destroy(&mutex);

  return 0;
}

int main()
{
int a[4]={4,4,4,4};
catmouse(5,a);
}


