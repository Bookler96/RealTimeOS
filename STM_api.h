int IncludeTask( void * Task);
void Delay(int time);


void PrepareLED();
void OffLED(int type);
void InverseLED(int type);

typedef struct{
	
	int owner;

	
} mutex;

void CRITICAL_START();
void CRITICAL_END();

int mutex_init(mutex *mutex);
int mutex_lock(mutex *mutex);
int mutex_trylock(mutex *mutex);
int mutex_unlock(mutex *mutex);
int getpid(void);


int print(char * output);