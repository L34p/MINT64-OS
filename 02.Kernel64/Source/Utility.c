#include "Utility.h"

/* 
 * Print string on the screen at (X, Y)
 */
void kPrintString( int iX, int iY, const char* pcString)
{
  CHARACTER* pstScreen = (CHARACTER*) 0xB8000;
  int i;

  pstScreen += (iY * 80) + iX;
  for (i=0; pcString[i] != 0; ++i)
    pstScreen[i].bCharacter = pcString[i];
}

void kMemSet(void* pvDestination, BYTE bData, int iSize)
{
  int i;
  for(i=0; i<iSize; ++i)
    ((char*) pvDestination)[i] = bData;
}

int kMemCpy(void* pvDestination, const void* pvSource, int iSize)
{
  int i;
  for(i=0; i<iSize; ++i)
    ((char*) pvDestination)[i] = ((char*) pvSource)[i];

  return iSize;
}

int kMemCmp(const void* pvDestination, const void* pvSource, int iSize)
{
  int i;
  char cTemp;
  for(i=0; i<iSize; ++i)
  {
    cTemp = ((char*) pvDestination)[i] - ((char*) pvSource)[i];
    if(cTemp != 0)
      return (int) cTemp;
  }

  return 0;
}
