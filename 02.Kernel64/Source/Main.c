#include "Types.h"
#include "Keyboard.h"
#include "Descriptor.h"
#include "Utility.h"

/* 
 * 64-bit protected mode C Language kernel entry point.
 * This function must be located at the first.
 */
void Main(void)
{
  int x, y;
  BYTE bTemp;
  BYTE bFlags;
  char vcTemp[2] = {0,};

  kPrintString(0, 10, "Switch To IA-32e Mode.......................[PASS]");
  kPrintString(0, 11, "IA-32e C Language Kernel Start..............[PASS]");

  kPrintString(0, 12, "GDT Initialize And Switch For IA-32e Mode...[    ]");
  kInitializeGDTTableAndTSS();
  kLoadGDTR(GDTR_STARTADDRESS);
  kPrintString(45, 12, "PASS");

  kPrintString(0, 13, "TSS Segment Load............................[    ]");
  kLoadTR(GDT_TSSSEGMENT);
  kPrintString(45, 13, "PASS");

  kPrintString(0, 14, "IDT Initialize..............................[    ]");
  kInitializeIDTTables();
  kLoadIDTR(IDTR_STARTADDRESS);
  kPrintString(45, 14, "PASS");

  kPrintString(0, 15, "Activate Keyboard...........................[    ]");
  if(kActivateKeyboard() == TRUE)
  {
    kPrintString(45, 15, "PASS");
  }
  else
  {
    kPrintString(45, 15, "FAIL");
    while(1);
  }

  /* Simple echo shell to test keyboard driver. */
  kPrintString(0, 17, "MINT64-OS ~$ ");
  x = 13;
  y = 17;
  while(1)
  {
    if(kIsOutputBufferFull() == TRUE)
    {
      bTemp = kGetKeyboardScanCode();
      if(kConvertScanCodeToASCIICode(bTemp, vcTemp, &bFlags) == TRUE)
      {
        if(vcTemp[0] == KEY_ENTER)
        {
          if(bFlags & KEY_FLAGS_DOWN)
          {
            x = 0;
            y += 1;
          }
          continue;
        }
        else if(vcTemp[0] == KEY_BACKSPACE)
        {
          if(bFlags & KEY_FLAGS_DOWN)
            kPrintString(--x, y, " ");
          continue;
        }

        if((bFlags & KEY_FLAGS_DOWN) &&
           (kIsPrintableScanCode(bTemp) == TRUE))
          kPrintString(x++, y, vcTemp);

        /* Trigger Divide By Zero exception to test interrupt handler. */
        if(vcTemp[0] == '0')
        {
          bTemp = bTemp / 0;
        }
      }
    }
  }

}
