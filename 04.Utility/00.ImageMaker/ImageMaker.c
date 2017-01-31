#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#define BYTESOFSECTOR 512

int AdjustInSectorSize(int iFd, int iSourceSize);
void WriteKernelInformation(int iTargetFd, int iTotalKernelSectorCount, 
    int iKernel32SectorCount);
int CopyFile(int iSourceFd, int iTargetFd);

int main(int argc, char* argv[])
{
  int iSourceFd, iTargetFd;
  int iBootLoaderSize, iSourceSize;
  int iKernel32SectorCount;
  int iKernel64SectorCount;

  if(argc < 4)
  {
    fprintf(stderr, "[ERROR] %s BootLoader.bin Kernel32.bin Kernel64.bin\n", argv[0]);
    return -1;
  }

  /* Open target DISK image. */
  if((iTargetFd = open("Disk.img", O_RDWR | O_CREAT | O_TRUNC, S_IREAD | S_IWRITE)) == -1)
  {
    fprintf(stderr, "[ERROR] Disk.img open fail\n");
    return -1;
  }

  /* Copy BootLoader Image and Adjust in sector size. */
  printf("[INFO] Copy boot loader to image file\n");
  if((iSourceFd = open(argv[1], O_RDONLY)) == -1)
  {
    fprintf(stderr, "[ERROR] %s open fail\n", argv[1]);
    return -1;
  }
  iSourceSize = CopyFile(iSourceFd, iTargetFd);
  close(iSourceFd);
  iBootLoaderSize = AdjustInSectorSize(iTargetFd, iSourceSize);
  printf("[INFO] %s size = [%d] and sector count = [%d]\n",
      argv[1], iSourceSize, iBootLoaderSize);


  /* Copy 32-bit Protected Mode Kernel Image and Adjust in sector size. */
  printf("[INFO] Copy protected mode kernel to image file\n");
  if((iSourceFd = open(argv[2], O_RDONLY)) == -1)
  {
    fprintf(stderr, "[ERROR] %s open fail\n", argv[2]);
    return -1;
  }
  iSourceSize = CopyFile(iSourceFd, iTargetFd);
  close(iSourceFd);
  iKernel32SectorCount = AdjustInSectorSize(iTargetFd, iSourceSize);
  printf("[INFO] %s size = [%d] and sector count = [%d]\n", 
      argv[2], iSourceSize, iKernel32SectorCount);


  /* Copy 64-bit IA-32e Mode Kernel Image and Adjust in sector size. */
  printf("[INFO] Copy IA-32e mode kernel to image file\n");
  if((iSourceFd = open(argv[3], O_RDONLY)) == -1)
  {
    fprintf(stderr, "[ERROR] %s open fail\n", argv[3]);
    return -1;
  }
  iSourceSize = CopyFile(iSourceFd, iTargetFd);
  close(iSourceFd);
  iKernel64SectorCount = AdjustInSectorSize(iTargetFd, iSourceSize);
  printf("[INFO] %s size = [%d] and sector count = [%d]\n",
      argv[3], iSourceSize, iKernel64SectorCount);

  /* Update sector count in DISK image.  */
  printf("[INFO] Start to write kernel information\n");
  WriteKernelInformation(iTargetFd, iKernel32SectorCount + iKernel64SectorCount,
      iKernel32SectorCount);
  
  /* Final DISK image is ready. */
  printf("[INFO] Image file create complete\n");
  close(iTargetFd);

  return 0;
}

int AdjustInSectorSize(int iFd, int iSourceSize)
{
  int i;
  int iAdjustSizeToSector;
  int iSectorCount;
  char cCh;

  iAdjustSizeToSector = iSourceSize % BYTESOFSECTOR;
  cCh = 0x00;

  if(iAdjustSizeToSector != 0)
  {
    iAdjustSizeToSector = 512 - iAdjustSizeToSector;
    printf("[INFO] File size [%d] and fill [%d] byte\n", 
        iSourceSize, iAdjustSizeToSector);
    for(i=0; i<iAdjustSizeToSector; ++i)
      write(iFd, &cCh, 1);
  }
  else
    printf("[INFO] File size is aligned 512 byte\n");

  iSectorCount = (iSourceSize + iAdjustSizeToSector) / BYTESOFSECTOR;
  return iSectorCount;
}

void WriteKernelInformation(int iTargetFd, int iTotalKernelSectorCount, 
    int iKernel32SectorCount)
{
  unsigned short usData;
  long lPosition;

  lPosition = lseek(iTargetFd, 5, SEEK_SET);
  if(lPosition == -1)
  {
    fprintf(stderr, "lseek fail. Return value = %ld, errno = %d, %d\n", 
        lPosition, errno, SEEK_SET);
    exit(-1);
  }

  usData = (unsigned short) iTotalKernelSectorCount;
  write(iTargetFd, &usData, 2);
  usData = (unsigned short) iKernel32SectorCount;
  write(iTargetFd, &usData, 2);

  printf("[INFO] Total sector count except boot loader [%d]\n", iTotalKernelSectorCount);
  printf("[INFO] Total sector count of protected mode kernel [%d]\n", 
      iKernel32SectorCount);
}

int CopyFile(int iSourceFd, int iTargetFd)
{
  int iSourceFileSize;
  int iRead;
  int iWrite;
  char vcBuffer[BYTESOFSECTOR];

  iSourceFileSize = 0;
  while (1)
  {
    iRead = read(iSourceFd, vcBuffer, sizeof(vcBuffer));
    iWrite = write(iTargetFd, vcBuffer, iRead);

    if(iRead != iWrite)
    {
      fprintf(stderr, "[ERROR] iRead != iWrite\n");
      exit(-1);
    }

    iSourceFileSize += iRead;
    if(iRead != sizeof(vcBuffer))
      break;
  }

  return iSourceFileSize;
}
