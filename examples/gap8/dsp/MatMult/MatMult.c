/*
 * Copyright (C) 2017 GreenWaves Technologies
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 *
 */

#include <stdio.h>
#include "pmsis.h"
#include "Gap.h"
#include "MatMultKernels.h"

#define STACK_SIZE ( 1024 * 2 ) //questo lo lascerei così a meno che non funzioni
#define CID        ( 0 )
extern char *L1_Memory;

struct Matrices{
        short int *m[3];
        uint32_t d[4];
    };

 
static void cluster_main(void *arg){
    printf("Executing cluster_main.\n");
    struct Matrices *mat = (struct Matrices *)arg;
    printf("W_M2 check in cluster_main:%d\n", mat->d[3]);
    ParMatMult(mat->m[0], mat->m[1], mat->m[2], mat->d[0], mat->d[1], mat->d[2], mat->d[3], 0);
    printf("Exiting cluster_main.\n");
}


void run_MatMult(short int *M1, short int *M2, short int *Out, uint32_t H_M1, uint32_t H_M2, uint32_t W_M1, uint32_t W_M2) //La run_MatMult nel main si chiama con gli identificatori
//delle matrici da moltiplicare e della matrice in cui mettere il risultato. Vanno indicate anche le dimensioni delle matrici da moltiplicare
{
    printf("Checking W_M2 start of run_MatMult:%d\n", W_M2);
    printf("Executing run_MatMult.\n");
    if (H_M2 != W_M1)
        printf("Incompatible dimensions.\n");
    
    uint32_t H_Out = H_M1;
    uint32_t W_Out = W_M2;
    M1 = (short int *) pi_l2_malloc(W_M1 * H_M1 * sizeof(short int));
    M2 = (short int *) pi_l2_malloc(W_M2 * H_M2 * sizeof(short int));
    Out = (short int *) pi_l2_malloc(W_Out * H_Out * sizeof(short int));
    if ((M1 == NULL) || (M2 == NULL) || (Out == NULL))
    {
        printf("Failed to allocate memory for matrix.\n");
        pmsis_exit(-1);
    }

    struct pi_device cluster_dev;
    struct pi_cluster_conf conf;
    /* Init cluster configuration structure. */
    pi_cluster_conf_init(&conf);
    conf.id = (uint32_t) CID;   /* Cluster ID. */
    conf.cc_stack_size = STACK_SIZE;
    /* Configure & open cluster. */
    pi_open_from_conf(&cluster_dev, (void *) &conf);
    if (pi_cluster_open(&cluster_dev))
    {
        printf("Cluster open failed !\n");
        pmsis_exit(-2);
    }

    /* Allocate the predetermined memory size in the shared L1 memory that the cluster can act on. */
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
    printf("W_M2 check in runMatMult:%d\n", argument->d[3]);

    /* Prepare cluster task and send it to cluster. */
    struct pi_cluster_task task;
    pi_cluster_task(&task, cluster_main, (void *)argument);

    /* Offloading Task to cluster. */
    pi_cluster_send_task(&cluster_dev, &task);

    pi_l1_free(&cluster_dev, L1_Memory, _L1_Memory_SIZE);

    pi_cluster_close(&cluster_dev);

    pi_l2_free(M1, W_M1 * H_M1 * sizeof(short int));
    pi_l2_free(M2, W_M2 * H_M2 * sizeof(short int));
    pi_l2_free(Out, W_Out * H_Out * sizeof(short int));
    printf("Exiting run_MatMult.\n");
}

//ESEMPIO

int main(void)
{
    short int m1[2*3], m2[3*3], out[2*3]; //per ora lascio la robaccia che c'è dentro tanto voglio solo vedere se funziona
    printf("\n\n\t *** MatMult ***\n\n");
    run_MatMult(m1,m2,out,2,3,3,3); 
    printf("Operation completed with success!\n");
}
