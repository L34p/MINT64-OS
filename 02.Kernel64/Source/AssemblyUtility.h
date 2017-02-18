#ifndef __ASSEMBLY_UTILITY_H__
#define __ASSEMBLY_UTILITY_H__

#include "Types.h"

BYTE kInPortByte(WORD wPort);
void kOutPortBYTE(WORD wPort, BYTE bData);
void kLoadGDTR(QWORD qwGDTRAddress);
void kLoadTR(WORD wTSSSegmentOffset);
void kLoadIDTR(QWORD qwIDTRAddress);

#endif /* __ASSEMBLY_UTILITY_H__ */
