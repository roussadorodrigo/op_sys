



int sot_barrier_init(sot_barrier_t * barrier, int numberOfThreads){
	
	//INTI MUTEX
	pthread_mutex_init(barrier->mutex, NULL);
	
	//INIT COND
	pthread_cond_init(barrier->cond, NULL);
	
	barrier->max_threads = numberOfThreads;
	barrier->waiting_threads = 0;
	
	
}

int sot_barrier_destroy(sot_barrier_t * barrier){
	
	//DESTROY MUTEX
	pthread_mutex_destroy(barrier->mutex);
	
	//DESTROY COND
	pthread_cond_destroy(barrier->cond);
	
	
}

int sot_barrier_wait(sot_barrier_t * barrier){ 
	
	pthread_mutex_lock(barrier->mutex);
	
	barrier->waiting_threads++;
	
	if(barrier->waiting_threads == barrier->max_threads){
		
		barrier->waiting_threads = 0;
		pthread_cond_broadcast(barrier->cond);
		
	}else{
		while(barrier->waiting_threads > 0){
			pthread_cond_wait(barrier->cond);
			
			
		}
	}
	
	
	
	pthread_mutex_unlock(barrier->mutex);
	
}
