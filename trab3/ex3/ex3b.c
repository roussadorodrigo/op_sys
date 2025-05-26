typedef struct {
// a definir com os atributos e mecanismos de sincronismo
// necessários à sua implementação
} sot_barrier_t;
int sot_barrier_init (sot_barrier_t *barrier, int numberOfThreads);
int sot_barrier_destroy (sot_barrier_t *barrier);
int sot_barrier_wait (sot_barrier_t *barrier);
