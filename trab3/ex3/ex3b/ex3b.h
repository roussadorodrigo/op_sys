typedef struct{
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	int max_threads; //N
	int waiting_threads; //n
    int ref;
	
}sot_barrier_t;

