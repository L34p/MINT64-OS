#include "Types.h"
#include "Keyboard.h"

static void kPrintString(int iX, int iY, const char* pcString);

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
  kPrintString(0, 12, "Activate Keyboard...........................[    ]");

  if(kActivateKeyboard() == TRUE)
  {
    kPrintString(45, 12, "PASS");
  }
  else
  {
    kPrintString(45, 12, "FAIL");
    while(1);
  }

  /* Simple echo shell to test keyboard driver. */
  kPrintString(0, 14, "MINT64-OS ~$ ");
  x = 13;
  y = 14;
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
      }
    }
  }

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
