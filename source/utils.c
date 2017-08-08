#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

FS_Archive sdmcArchive;
FS_Archive ctrfsArchive;

Result AM_DeleteAllTwlUserPrograms(void) {
  u32 *cmdbuf = getThreadCommandBuffer();
  cmdbuf[0] = IPC_MakeHeader(0x1D,0,0); // 0x001D0000
  Result ret = 0;
    if(R_FAILED(ret = svcSendSyncRequest(amHandle)))
      return ret;
  return cmdbuf[1];
}

void HWCALDelete(void) {
  FSUSER_OpenArchive(&ctrfsArchive, ARCHIVE_NAND_CTR_FS, fsMakePath(PATH_EMPTY, ""));
  FSUSER_DeleteFile(ctrfsArchive, fsMakePath(PATH_ASCII, "/ro/sys/HWCAL0.dat"));
  FSUSER_DeleteFile(ctrfsArchive, fsMakePath(PATH_ASCII, "/ro/sys/HWCAL1.dat"));
  FSUSER_CloseArchive(ctrfsArchive);
}

void BackupCtrFs(void) {
  FSUSER_OpenArchive(&ctrfsArchive, ARCHIVE_NAND_CTR_FS, fsMakePath(PATH_EMPTY, ""));
  FSUSER_OpenArchive(&sdmcArchive, ARCHIVE_SDMC, fsMakePath(PATH_EMPTY, ""));
    if (R_SUMMARY(FSUSER_OpenDirectory(NULL, sdmcArchive, fsMakePath(PATH_ASCII, "/FormatMii/backup")) == RS_NOTFOUND) {
      FSUSER_CreateDirectory(sdmcArchive, fsMakePath(PATH_ASCII, "/FormatMii/backup"), FS_ATTRIBUTE_DIRECTORY);
    }
}

Result FS_InitializeCtrFileSystem(Handle handle) {
  u32 *cmdbuf = getThreadCommandBuffer();
  cmdbuf[0] = 0x08430000;
  Result ret = 0;
    if((ret = svcSendSyncRequest(handle)))
      return ret;
  ret = cmdbuf[1];
  return ret;
}
