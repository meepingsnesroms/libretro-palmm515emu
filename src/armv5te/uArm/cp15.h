#ifndef _CP15_H_
#define _CP15_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "../../pxa260/pxa260_types.h"
#include "CPU_2.h"
#include "MMU_2.h"

typedef struct{

	ArmCpu* cpu;
	ArmMmu* mmu;
	
	UInt32 control;
	UInt32 ttb;
	UInt32 FSR;	//fault sttaus register
	UInt32 FAR;	//fault address register
	UInt32 CPAR;	//coprocessor access register
	UInt32 ACP;	//auxilary control reg for xscale
}ArmCP15;

void cp15Init(ArmCP15* cp15, ArmCpu* cpu, ArmMmu* mmu);
void cp15Deinit(ArmCP15* cp15);
void cp15SetFaultStatus(ArmCP15* cp15, UInt32 addr, UInt8 faultStatus);

#ifdef __cplusplus
}
#endif

#endif
