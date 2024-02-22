#include <stdio.h>
#include "Gap.h"
#include "MatMultBasicKernels.h"
#include "pmsis.h"
//extern PI_L2 uint32_t perf_values[ARCHI_CLUSTER_NB_PE];
extern short int* vpunt1;
extern short int* vpunt2;
extern short int* vpunt3;
extern short int* vpunt4;


#ifndef Min
  #define Min(a, b)               (((a)<(b))?(a):(b))
#endif
#ifndef Max
  #define Max(a, b)               (((a)>(b))?(a):(b))
#endif

static int CoreCountDynamic = 1;
static int ActiveCore = gap_ncore();

static inline unsigned int __attribute__((always_inline)) ChunkSize(unsigned int X)

{
        unsigned int NCore;
        unsigned int Log2Core;
        unsigned int Chunk;

        if (CoreCountDynamic) NCore = ActiveCore; else NCore = gap_ncore(); //gap_ncore restituisce sempre 8, il numero di cores
        Log2Core = gap_fl1(NCore);
        Chunk = (X>>Log2Core) + ((X&(NCore-1))!=0);
        return Chunk;
}



void KerMatMultParallel16(KerMatMultParallel16_ArgT *Arg)

{
    short int* In1 = Arg->In1;
    unsigned int W_In1 = Arg->W_In1;
    unsigned int H_In1 = Arg->H_In1;
    short int* In2 = Arg->In2;
    unsigned int W_In2 = Arg->W_In2;
    short int* Out = Arg->Out;
    unsigned int W_Out = Arg->W_Out;
    unsigned int OutFirstCol = Arg->OutFirstCol;
    unsigned int Norm = Arg->Norm;

    unsigned int H_In2 = W_In1;
    unsigned int H_Out = H_In1;

    uint32_t CoreId = gap_coreid();
    
    /*
    //pi_perf_conf(1 << PI_PERF_ACTIVE_CYCLES);
    //pi_perf_start();
    //uint32_t core_id = pi_core_id(), cluster_id = pi_cluster_id();

    //Another parallelization algorithm
    
    short int x;
    if(H_In1 >= 8){
        int q0 = H_In1/8;
        uint32_t q = (uint32_t) q0;
        uint32_t r = H_In1 - (q*8);
        for(uint32_t i = 0; i < q; i++){
            for(uint32_t j = 0; j < W_In2; j++){
                x = 0;
                for(uint32_t k = 0; k < H_In2; k++){
                x = x + (In1[W_In1*((i*8)+CoreId) + k] * In2[k*W_In2 + j]);
                }
                Out[W_Out*((i*8)+CoreId) + j] = x;
            }
        }

        if(2*(r/2)==r){
            if(CoreId <= r){
                for(uint32_t j = 0; j < W_In2; j++){
                    x = 0;
                    for(uint32_t k = 0; k < H_In2; k++){
                    x = x + (In1[W_In1*(H_In1 - (CoreId + 1)) + k] * In2[k*W_In2 + j]);
                    }
                    Out[W_Out*(H_In1 - (CoreId+1)) + j] = x;
                }
            }
        }

        else{
            if(CoreId >= (8-r)){
                for(uint32_t j = 0; j < W_In2; j++){
                    x = 0;
                    for(uint32_t k = 0; k < H_In2; k++){
                        x = x + (In1[W_In1*(H_In1 - (8 - CoreId)) + k] * In2[k*W_In2 + j]);
                    }
                    Out[W_Out*(H_In1 - (8 - CoreId)) + j] = x;
                }
            }
        }
    }

    else{
        if(CoreId==0){
            for(uint32_t i = 0; i < H_In1; i++){
                for(uint32_t j = 0; j < W_In2; j++){
                    x = 0;
                    for(uint32_t k = 0; k < H_In2; k++){
                        x = x + (In1[W_In1*i + k] * In2[k*W_In2 + j]);
                    }
                Out[W_Out*i + j] = x;
                }
            }
        }
    }


    //pi_perf_stop();
    //perf_values[core_id] = pi_perf_read(PI_PERF_ACTIVE_CYCLES);
    
    gap_waitbarrier(0);

} */
        
    unsigned int Line, Col, i;
        
    //pi_perf_conf(1 << PI_PERF_ACTIVE_CYCLES);
    //pi_perf_start();
    //uint32_t core_id = pi_core_id(), cluster_id = pi_cluster_id();
        
    unsigned int ChunkCell = ChunkSize(W_In2);
    unsigned int First = CoreId*ChunkCell, Last  = Min(W_In2, First+ChunkCell);
    // for (Col=0; Col<W_In2; Col++) {
    for (Col=First; Col<Last; Col++) {
      for (Line=0; Line<H_In1; Line++) {
            int S = 0;
            for (i=0; i<W_In1/2; i++) {
                S = S + In1[Line*W_In1 + (2*i  )] * In2[(2*i  )*W_In2+Col];
                S = S + In1[Line*W_In1 + (2*i+1)] * In2[(2*i+1)*W_In2+Col];
            }
            if (W_In1&0x1) S += In1[Line*W_In1 + W_In1-1]*In2[(W_In1 - 1)*W_In2];
            Out[Line*W_Out+Col+OutFirstCol] = gap_clip(gap_roundnorm_reg(S, Norm), 15);
        }
    }
        //pi_perf_stop();
        //perf_values[core_id] = pi_perf_read(PI_PERF_ACTIVE_CYCLES);
        gap_waitbarrier(0);
}




