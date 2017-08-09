#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

FS_Archive sdmcArchive;
FS_Archive ctrfsArchive;

void init_services(void)
{
    fsInit();
    sdmcInit();
    amInit();
    aptInit();
    ptmSysmInit();
    FSUSER_OpenArchive(&sdmcArchive, ARCHIVE_SDMC, fsMakePath(PATH_EMPTY, ""));
    if (R_SUMMARY(FSUSER_OpenDirectory(NULL, sdmcArchive, fsMakePath(PATH_ASCII, "/FormatMii")) == RS_NOTFOUND))
        FSUSER_CreateDirectory(sdmcArchive, fsMakePath(PATH_ASCII, "/FormatMii"), FS_ATTRIBUTE_DIRECTORY);
    FSUSER_OpenArchive(&ctrfsArchive, ARCHIVE_NAND_CTR_FS, fsMakePath(PATH_EMPTY, ""));
}

void deinit_services(void)
{
    amExit();
    aptExit();
    fsExit();
    sdmcExit();
    gfxExit();
    ptmSysmExit();
}

Result AM_DeleteAllTwlUserPrograms(void)
{
    Handle amHandle = *amGetSessionHandle();
    u32 *cmdbuf = getThreadCommandBuffer();
    cmdbuf[0] = IPC_MakeHeader(0x1D,0,0); // 0x001D0000
    Result ret = 0;
    if(R_FAILED(ret = svcSendSyncRequest(amHandle)))
        return ret;
    return cmdbuf[1];
}

void HWCALDelete(void) {
    FSUSER_DeleteFile(ctrfsArchive, fsMakePath(PATH_ASCII, "/ro/sys/HWCAL0.dat"));
    FSUSER_DeleteFile(ctrfsArchive, fsMakePath(PATH_ASCII, "/ro/sys/HWCAL1.dat"));
}

void BackupCtrFs(void) 
{
    if (R_SUMMARY(FSUSER_OpenDirectory(NULL, sdmcArchive, fsMakePath(PATH_ASCII, "/FormatMii/backup")) == RS_NOTFOUND))
        FSUSER_CreateDirectory(sdmcArchive, fsMakePath(PATH_ASCII, "/FormatMii/backup"), FS_ATTRIBUTE_DIRECTORY);
}

Result copy_directory(const char *source_path, const FS_Archive source_archive, const char *dest_path, const FS_Archive dest_archive)
{
    Result res;
    Handle dir_handle;
    Handle file_handle;

    res = FSUSER_OpenDirectory(&dir_handle, source_archive, fsMakePath(PATH_ASCII, source_path));
    while (true)
    {
        FS_DirectoryEntry *entry = malloc(sizeof(FS_DirectoryEntry));
        u32 entries_read;
        FSDIR_Read(dir_handle, &entries_read, 1, entry);
        if (entries_read)
        {
            char *full_source_path = calloc(1, 0x106);
            strcpy(full_source_path, source_path);
            utf16_to_utf8(&full_source_path[strlen(source_path)], entry->name, 0x106 - strlen(source_path));
            char *full_dest_path = calloc(1, 0x106);
            strcpy(full_dest_path, dest_path);
            utf16_to_utf8(&full_dest_path[strlen(dest_path)], entry->name, 0x106 - strlen(dest_path));
        }
    }
}