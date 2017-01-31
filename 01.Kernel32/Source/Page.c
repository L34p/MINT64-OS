#include "Page.h"

/*
 * Initialize Kernel Page Tables.
 * Default PAGE SIZE is 2MB. (CR3 -> PML4TE -> PDPTE -> PDE)
 * Support up to 64GB. (Use 64 Page Directories)
 * Therefore, we use only one PML4T and one PDPT.
 * Also, we use 1:1 mapping between linear and physical address for simplicity.
 *
 * PML4T -> 0x100000 ~ 0x101000, 4KB
 * PDPT  -> 0x101000 ~ 0x102000, 4KB
 * PD    -> 0x102000 ~ 0x142000, 256KB
 */
void kInitializePageTables(void)
{
  PML4TENTRY* pstPML4TEntry = (PML4TENTRY*) 0x100000;
  PDPTENTRY* pstPDPTEntry = (PDPTENTRY*) 0x101000;
  PDENTRY* pstPDEntry = (PDENTRY*) 0x102000;
  DWORD dwMappingAddress = 0;
  int i;

  /* Initialize PML4T. */
  kSetPageEntryData(&(pstPML4TEntry[0]), 0x00, 0x101000, PAGE_FLAGS_DEFAULT, 0);
  for(i=1; i<PAGE_MAXENTRYCOUNT; ++i)
    kSetPageEntryData(&(pstPML4TEntry[i]), 0, 0, 0, 0);

  /* Initialize PDPT. */
  for(i=0; i<64; ++i)
    kSetPageEntryData(&(pstPDPTEntry[i]), 0, 0x102000 + (i * PAGE_TABLESIZE), PAGE_FLAGS_DEFAULT, 0);
  
  for(i=64; i<PAGE_MAXENTRYCOUNT; ++i)
    kSetPageEntryData(&(pstPDPTEntry[i]), 0, 0, 0, 0);

  /* Initialize PD. */
  for(i=0; i<PAGE_MAXENTRYCOUNT*64; ++i)
  {
    kSetPageEntryData(&(pstPDEntry[i]), (i * (PAGE_DEFAULTSIZE >> 20)) >> 12, dwMappingAddress, 
        PAGE_FLAGS_DEFAULT | PAGE_FLAGS_PS, 0);
    dwMappingAddress += PAGE_DEFAULTSIZE;
  }
}

/*
 * Page Table helper function.
 * Set page entry data properly.
 */
void kSetPageEntryData(PTENTRY* pstEntry, DWORD dwUpperBaseAddress, DWORD dwLowerBaseAddress,
    DWORD dwLowerFlags, DWORD dwUpperFlags)
{
  pstEntry->dwAttributeAndLowerBaseAddress = dwLowerBaseAddress | dwLowerFlags;
  pstEntry->dwUpperBaseAddressAndEXB = (dwUpperBaseAddress & 0xFF) | dwUpperFlags;
}
