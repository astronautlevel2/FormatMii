#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>

#pragma once

FS_Archive sdmcArchive;
FS_Archive ctrfsArchive;
Result AM_DeleteAllTwlUserPrograms(void);
void BackupCtrFs(void);
void HWCALDelete(void);
Result FS_InitializeCtrFileSystem(Handle handle);
u32 kDown = hidKeysDown();
u32 kHeld = hidKeysHeld();
