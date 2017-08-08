#include <3ds.h>
#include <3ds/types.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

int main(int argc, char **argv) {
  gfxInitDefault();
  consoleInit(GFX_TOP, NULL);
  printf("FormattMii\nby Yuuki Aiko\n\n");
  printf("Initializing necessary services...\n");
  fsInit();
  sdmcInit();
  FSUSER_OpenArchive(&sdmcArchive, ARCHIVE_SDMC, fsMakePath(PATH_EMPTY, ""));
    if (R_SUMMARY(FSUSER_OpenDirectory(NULL, sdmcArchive, fsMakePath(PATH_ASCII, "/FormatMii")) == RS_NOTFOUND) {
      FSUSER_CreateDirectory(sdmcArchive, fsMakePath(PATH_ASCII, "/FormatMii"), FS_ATTRIBUTE_DIRECTORY);
    }
  FSUSER_CloseArchive(sdmcArchive);
  amInit();
  aptInit();
  printf("Press A to begin the System Format.\n");
  printf("Press START to exit.\n");
  while (aptMainLoop()) {
    gspWaitForVBlank();
    hidScanInput();
      if (kDown & KEY_A) {
        printf("Your system will be formatted and all data deleted (excluding your SD card)!\n")
        printf("Press L+R+X to confirm. Press START to exit.\n");
          if ((kHeld & KEY_L) && (kHeld & KEY_R) && (kHeld & KEY_X)) {
            printf("Backing up files that will be deleted to /FormatMii/backup...\n");
            BackupCtrFs();
            sdmcExit();
            printf("Formatting your system...\n");
            printf("Deleting HWCAL0.dat and HWCAL1.dat...\n");
            HWCALDelete();
            printf("Calling AM_DeleteAllTwlUserPrograms...\n");
            AM_DeleteAllTwlUserPrograms();
            printf("Calling FS_InitializeCtrFileSystem...\n");
            FS_InitializeCtrFileSystem(*(fsGetSessionHandle()));
            printf("Done! Rebooting to Initial Setup...\n");
            svcSleepThread(500000000);
            aptOpenSession();
            APT_HardwareResetAsync(NULL);
            aptCloseSession();
          }
      }
          else if (kDown & KEY_START) {
            printf("Format aborted, exiting...\n");
            break;
          }
      else if (kDown & KEY_START) {
        printf("Format aborted, exiting...\n");
        break;
      }
  }
  amExit();
  aptExit();
  fsExit();
  sdmcExit();
  gfxFlushBuffers();
  gfxSwapBuffers();
  gspWaitForVBlank();
  gfxExit();
  return 0;
}
