#ifndef __MATMULTKERNEL_H__
#define __MATMULTKERNEL_H__

#include "AutoTilerLibTypes.h"
#include "MatMultBasicKernels.h"
#define _L1_Memory_SIZE 44640
#define _L2_Memory_SIZE 0
extern char *L1_Memory; /* Size given for generation: 45000 bytes, used: 44640 bytes */
extern char *L2_Memory; /* Size used for generation: 0 bytes */
extern void ParMatMult(
		short int* M1,
		short int* M2,
		short int* Out,
		uint32_t H_M1,
		uint32_t H_M2,
		uint32_t W_M1,
		uint32_t W_M2,
		unsigned int Norm);
#endif
