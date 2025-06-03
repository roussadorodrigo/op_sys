void norm_min_max_and_classify_parallel(int v[], size_t v_sz, int nThreads){
    pthread_t threads[N_THREADS];
    thArg arg[N_THREADS];

    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);

    sot_barrier_t barrier;
    sot_barrier_init(&barrier, N_THREADS);

    int somatorio = 0;

    int ref = 0;

    int global_min = v[0];
    int global_max = v[0];

    size_t nTermosThread = v_sz / N_THREADS;
    size_t nTermosResto = v_sz % N_THREADS;

    #ifdef DEBUG
    printf("Termos por thread: %ld\nTermos resto: %ld\n", nTermosThread, nTermosResto);
    #endif

    for(int i = 0; i < N_THREADS; i++){
        #ifdef DEBUG
        printf("Inicialização dos args de %d\n", i);
        #endif

        arg[i].values = v;
        arg[i].lim_inf = i*nTermosThread;
        arg[i].lim_sup = (arg[i].lim_inf) + nTermosThread - 1;
        arg[i].id = i;
        arg[i].mutex = &mutex;
        arg[i].global_min = &global_min;
        arg[i].global_max = &global_max;
        arg[i].barrier = &barrier;
        arg[i].somatorio = &somatorio;
        arg[i].values_size = v_sz;
        

        if(i == N_THREADS - 1)
            arg[i].lim_sup += nTermosResto;

        #ifdef DEBUG
        printf("Thread na posição %d, com limite inferior %ld e limite superior %ld\n", i, arg[i].lim_inf, arg[i].lim_sup);
        #endif
    }

    for(int i = 0; i < N_THREADS; i++){
        pthread_create(&threads[i], NULL, thread_func, &arg[i]);
    }

    for(int i = 0; i < N_THREADS; i++){
        pthread_join(threads[i], NULL);
    }

    #ifdef DEBUG
    printf("Mínimo global: %d e máximo global: %d\n", global_min, global_max);
    #endif

    #ifdef DEBUG
    printf("Vetor values: ");
    for(size_t i = 0; i < v_sz; i++)
        printf("%d ",v[i]);
    printf("\n");
    #endif

    pthread_mutex_destroy(arg->mutex);
    sot_barrier_destroy(arg->barrier);
}
