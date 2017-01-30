#include "Types.h"
#include "Page.h"

void kPrintString(int iX, int iY, const char* pcString);
BOOL kInitializeKernel64Area(void);
BOOL kIsMemoryEnough(void);

/* 
 * 32-bit protected mode C language kernel entry point.
 * This function must be located at the first.
 */
void Main(void)
{
  kPrintString(0, 3, "C Language Kernel Start.....................[PASS]");
  kPrintString(0, 4, "Minimum Memory Size Check...................[    ]");
  if(kIsMemoryEnough() == FALSE)
  {
    kPrintString(45, 4, "FAIL");
    kPrintString(0, 5, "Not Enough Memory. MINT64 OS requires over 64M memory.");
    while (1);
  }
  else
    kPrintString(45, 4, "PASS");

  kPrintString(0, 5, "IA-32e Kernel Area Initialize...............[    ]");
  if(kInitializeKernel64Area() == FALSE)
  {
    kPrintString(45, 5, "FAIL");
    kPrintString(0, 6, "IA-32e Kernel Area Initialize Fail.");
    while (1);
  }
  else 
    kPrintString(45, 5, "PASS");

  kPrintString(0, 6, "IA-32e Page Tables Initialize...............[    ]");
  kInitializePageTables();
  kPrintString(45, 6, "PASS");

  while (1);
}

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

/*
 * Initialize IA-32e Kernel Area (1MB ~ 6MB)
 */
BOOL kInitializeKernel64Area(void)
{
  DWORD* pdwCurrentAddress = (DWORD*) 0x100000;

  while((DWORD) pdwCurrentAddress < 0x600000)
  {
    *pdwCurrentAddress = 0x00;
    if(*pdwCurrentAddress != 0)
      return FALSE;

    pdwCurrentAddress++;
  }

  return TRUE;
}

/*
 * Check whether memory is enough or not.
 * MINT64 OS requires 64MB memory.
 */
BOOL kIsMemoryEnough(void)
{
  const DWORD step = (0x100000 / 4);
  DWORD* pdwCurrentAddress = (DWORD*) 0x100000;

  while((DWORD) pdwCurrentAddress < 0x4000000)
  {
    *pdwCurrentAddress = 0xdeadbeef;
    if(*pdwCurrentAddress != 0xdeadbeef)
      return FALSE;

    pdwCurrentAddress += step;
  }

  return TRUE;
} 


