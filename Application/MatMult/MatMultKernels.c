#include "MatMultKernels.h"
L1_CL_MEM AT_L1_POINTER L1_Memory;
L2_MEM AT_L2_POINTER L2_Memory;
void ParMatMult(
		short int* M1,
		short int* M2,
		short int* Out,
		uint32_t H_M1,
		uint32_t H_M2,
		uint32_t W_M1,
		uint32_t W_M2,
		unsigned int Norm)
{
	KerMatMultParallel16_ArgT S_KerArg0, *KerArg0 = &S_KerArg0;
	KerArg0->W_In1 = (unsigned int) (W_M1);
	KerArg0->W_Out = (unsigned int) (W_M2);
	KerArg0->Norm = (unsigned int) (Norm);
	KerArg0->In1 = M1;
	KerArg0->H_In1 = (unsigned int) (H_M1);
	KerArg0->In2 = M2;
	KerArg0->W_In2 = (unsigned int) (W_M2);
	KerArg0->Out = Out;
	KerArg0->OutFirstCol = (unsigned int) (0);
	AT_FORK(gap_ncore(), (void *) KerMatMultParallel16, (void *) KerArg0);
	__CALL(KerMatMultParallel16, KerArg0);
}








