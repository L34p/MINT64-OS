#include "Types.h"
#include "Page.h"
#include "ModeSwitch.h"

static void kPrintString(int iX, int iY, const char* pcString);
static BOOL kInitializeKernel64Area(void);
static BOOL kIsMemoryEnough(void);
static void kCopyKernel64ImageTo2Mbyte(void);

/* 
 * 32-bit protected mode C Language kernel entry point.
 * This function must be located at the first.
 */
void Main(void)
{
  DWORD dwEAX, dwEBX, dwECX, dwEDX;
  char vcVendorString[13] = {0,};
  kPrintString(0, 3, "Protected mode C Language Kernel Start......[PASS]");

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

  kReadCPUID(0x00, &dwEAX, &dwEBX, &dwECX, &dwEDX);
  *((DWORD*) vcVendorString + 0) = dwEBX;
  *((DWORD*) vcVendorString + 1) = dwEDX;
  *((DWORD*) vcVendorString + 2) = dwECX;
  kPrintString(0, 7, "Processor Vendor String.....................[            ]");
  kPrintString(45, 7, vcVendorString);

  kReadCPUID(0x80000001, &dwEAX, &dwEBX, &dwECX, &dwEDX);
  kPrintString(0, 8, "64bit Mode Support Check....................[    ]");
  if(dwEDX & (1 << 29))
    kPrintString(45, 8, "PASS");
  else
  {
    kPrintString(45, 8, "FAIL");
    kPrintString(0, 9, "This processor does not support 64bit mode.");
    while(1);
  }

  kPrintString(0, 9, "Copy IA-32e Kernel To 2M Address............[    ]");
  kCopyKernel64ImageTo2Mbyte();
  kPrintString(45, 9, "PASS");

  kPrintString(0, 10,"Switch To IA-32e Mode.......................[    ]");
  kSwitchAndExecute64bitKernel();
}

/* 
 * Print string on the screen at (X, Y)
 */
static void kPrintString( int iX, int iY, const char* pcString)
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
static BOOL kInitializeKernel64Area(void)
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
static BOOL kIsMemoryEnough(void)
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

static void kCopyKernel64ImageTo2Mbyte(void)
{
  WORD wKernel32SectorCount, wTotalKernelSectorCount;
  DWORD* pdwSourceAddress;
  DWORD* pdwDestinationAddress;
  int i;

  wTotalKernelSectorCount = *((WORD*) 0x7c05);
  wKernel32SectorCount = *((WORD*) 0x7c07);

  pdwSourceAddress = (DWORD*) (0x10000 + (wKernel32SectorCount * 512));
  pdwDestinationAddress = (DWORD*) 0x200000;

  for(i=0; i< 512 * (wTotalKernelSectorCount - wKernel32SectorCount) / 4; ++i)
  {
    *pdwDestinationAddress = *pdwSourceAddress;
    pdwDestinationAddress++;
    pdwSourceAddress++;
  }
}

