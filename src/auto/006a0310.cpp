// Module: core_zlib_log
// Opens a file with CreateFileA using a mode selector (0 = read, 1 = write, 2 = read/write)
// and stores the resulting handle in this->field_0004. Returns true when the handle is valid.

#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Win32 types used here (windows.h is not in the allowed include list).
using FileHandle = void*;
using CreateFileAFn = FileHandle(__stdcall*)(const char* fileName, uint32_t desiredAccess,
                                             uint32_t shareMode, void* securityAttributes,
                                             uint32_t creationDisposition, uint32_t flagsAndAttributes,
                                             FileHandle templateFile);

// Import slot for CreateFileA (the original calls through the IAT: call dword ptr [0x82d09c]).
CSF_GLOBAL(CreateFileAFn, g_CreateFileAImport, 0x82D09C);

static bool __fastcall OpenFileForAccess(C_0085ec74* self, void* /*edx*/,
                                         const char* lpFileName, int a3, int a4)
{
    (void)a4;

    uint32_t desiredAccess = 0x80000000u;   // GENERIC_READ
    uint32_t creationDisposition = 3;       // OPEN_EXISTING
    if (a3 == 1)
    {
        desiredAccess = 0x40000000u;        // GENERIC_WRITE
        creationDisposition = 2;            // CREATE_ALWAYS
    }
    else if (a3 == 2)
    {
        desiredAccess = 0xC0000000u;        // GENERIC_READ | GENERIC_WRITE
        creationDisposition = 4;            // OPEN_ALWAYS
    }

    FileHandle fileHandle = g_CreateFileAImport(lpFileName, desiredAccess, 1u /*FILE_SHARE_READ*/,
                                                nullptr, creationDisposition, 0x10000080u, nullptr);
    self->field_0004 = (uint32_t)(uintptr_t)fileHandle;

    // INVALID_HANDLE_VALUE is (HANDLE)-1
    return fileHandle != (FileHandle)(uintptr_t)0xFFFFFFFFu;
}

HOOK(0x6A0310, OpenFileForAccess, "auto");
