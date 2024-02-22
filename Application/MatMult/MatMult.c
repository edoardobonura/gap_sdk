#include <stdio.h>
#include "pmsis.h"
#include "Gap.h"
#include "MatMultKernels.h"

#define STACK_SIZE ( 1024 * 2 )
#define CID        ( 0 )
extern char *L1_Memory;

//struct pi_device uart;
//PI_L2 uint32_t perf_values[ARCHI_CLUSTER_NB_PE];



struct Matrices{
        short int *m[3];
        uint32_t d[4];
    };

 
static void cluster_main(void *arg){
    struct Matrices *mat = (struct Matrices *)arg;
    ParMatMult(mat->m[0], mat->m[1], mat->m[2], mat->d[0], mat->d[1], mat->d[2], mat->d[3], 0);
}


void run_MatMult(short int *M1, short int *M2, short int *Out, uint32_t H_M1, uint32_t H_M2, uint32_t W_M1, uint32_t W_M2)

{
    if (H_M2 != W_M1){
        printf("Incompatible dimensions.\n");
        pmsis_exit(-1);
    }

    //pi_perf_conf(1 << PI_PERF_CYCLES | 1 << PI_PERF_ACTIVE_CYCLES);
    //pi_perf_start();
    //uint32_t core_id = pi_core_id(), cluster_id = pi_cluster_id();
    //uint32_t fc_perf = pi_perf_read(PI_PERF_ACTIVE_CYCLES);
    
    uint32_t H_Out = H_M1;
    uint32_t W_Out = W_M2;

    struct pi_device cluster_dev;
    struct pi_cluster_conf conf;
    pi_cluster_conf_init(&conf);
    conf.id = (uint32_t) CID; 
    conf.cc_stack_size = STACK_SIZE;
    pi_open_from_conf(&cluster_dev, (void *) &conf);
    if (pi_cluster_open(&cluster_dev))
    {
        printf("Cluster open failed !\n");
        pmsis_exit(-2);
    }

    L1_Memory = (char *) pi_l1_malloc(&cluster_dev, _L1_Memory_SIZE);
    if (L1_Memory == NULL )
    {
        printf("Memory Allocation Error! Quit...\n");
        pmsis_exit(-3);
    }

    short int *matrices[3] = {M1, M2, Out};
    struct Matrices M;
    struct Matrices *argument = &M;
    argument->m[0] = matrices[0];
    argument->m[1] = matrices[1];
    argument->m[2] = matrices[2];
    argument->d[0]=H_M1;
    argument->d[1]=H_M2;
    argument->d[2]=W_M1;
    argument->d[3]=W_M2;

    
    struct pi_cluster_task task;
    pi_cluster_task(&task, cluster_main, (void *)argument);

   
    pi_cluster_send_task(&cluster_dev, &task);

    pi_l1_free(&cluster_dev, L1_Memory, _L1_Memory_SIZE);
    pi_cluster_close(&cluster_dev);

    
    //pi_perf_stop();
    //uint32_t cycles = pi_perf_read(PI_PERF_ACTIVE_CYCLES);
    //uint32_t tim_cycles = pi_perf_read(PI_PERF_CYCLES);
    //printf("Perf : %d cycles Timer : %d cycles\n", cycles, tim_cycles);
    //printf("[%d %d] Perf : %d cycles\n", cluster_id, core_id, fc_perf);
        //for (uint32_t i = 0; i < (uint32_t) ARCHI_CLUSTER_NB_PE; i++)
        //{
        //printf("%d\n", perf_values[i]);
        //}
}

/*EXAMPLE*/

int main(void){


    short int* prova;
    
    uint32_t dim = 150;
    short int *m1 = NULL;
    short int *m2 = NULL;
    short int *out = NULL;
    uint32_t w_m1 = dim;
    uint32_t h_m1 = dim;
    uint32_t w_m2 = dim;
    uint32_t h_m2 = dim;
    m1 = (short int *) pmsis_l2_malloc(w_m1 * h_m1 * sizeof(short int));
    m2 = (short int *) pmsis_l2_malloc(w_m2 * h_m2 * sizeof(short int));
    out = (short int *) pmsis_l2_malloc(h_m1 * w_m2 * sizeof(short int));
    if ((m1 == NULL) || (m2 == NULL) || (out == NULL)){
       printf("Failed to allocate memory for matrix. \n");
    }
    for (uint32_t i = 0; i < w_m1*h_m1; i++)
        m1[i]=1;
    for (uint32_t i = 0; i < w_m2*h_m2; i++)
        m2[i]=1;

    run_MatMult(m1,m2,out,h_m1,h_m2,w_m1,w_m2);

    pi_l2_free(m1, w_m1 * h_m1 * sizeof(short int));
    pi_l2_free(m2, w_m2 * h_m2 * sizeof(short int));
    pi_l2_free(out, h_m1 * w_m2 * sizeof(short int));
}
