#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Indirect kernel32 import: the game stores the GetFileSize entry point in the
// pointer slot at 0x82D094 and calls it through memory (`call dword ptr [0x82d094]`).
using GetFileSizeFn = uint32_t(__stdcall*)(void* hFile, uint32_t* lpFileSizeHigh);
CSF_GLOBAL(GetFileSizeFn, g_pfnGetFileSize, 0x82D094);

// Returns the size (low DWORD) of the file whose handle is held in the object.
static uint32_t __fastcall C_0085ec74_GetFileSize(C_0085ec74* self, void* /*edx*/)
{
    return g_pfnGetFileSize(reinterpret_cast<void*>(self->field_0004), nullptr);
}

HOOK(0x6A04A0, C_0085ec74_GetFileSize, "auto");
