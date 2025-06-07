int sot_barrier_init(sot_barrier_t * barrier, int numberOfThreads){
	
	//INIT MUTEX
	if(pthread_mutex_init(&barrier->mutex, NULL) != 0)
        	return -1;
	
	//INIT COND
	if(pthread_cond_init(&barrier->cond, NULL) != 0)
        	return -1;
    

	barrier->max_threads = numberOfThreads;
	barrier->waiting_threads = 0;
    	barrier->ref = 0;
	
   	return 0;
	
}

int sot_barrier_destroy(sot_barrier_t * barrier){
	
	
	if(pthread_mutex_destroy(&barrier->mutex) != 0)
        return -1;
	
	
	if(pthread_cond_destroy(&barrier->cond))
        return -1;

	return 0;
	
}

int sot_barrier_wait(sot_barrier_t * barrier){ 

    int current_ref = barrier->ref;

	if(pthread_mutex_lock(&barrier->mutex) != 0)
        return -1;
	
	barrier->waiting_threads++;
	
	if(barrier->waiting_threads == barrier->max_threads){
		
		barrier->waiting_threads = 0;
        	barrier->ref++;
		if(pthread_cond_broadcast(&barrier->cond) != 0)
            		return -1;
		
	}else
		while(barrier->ref == current_ref)
			if(pthread_cond_wait(&barrier->cond, &barrier->mutex) != 0)
               			return -1;
			
	
	pthread_mutex_unlock(&barrier->mutex);
	
    return 0;
}
