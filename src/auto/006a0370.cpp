#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Win32 ReadFile, reached through its import-address-table slot (stdcall, 5 args).
using ReadFileFn = int(__stdcall*)(void*, void*, uint32_t, uint32_t*, void*);
CSF_GLOBAL(ReadFileFn, g_pfnReadFile, 0x0082D238);

// Reads up to nNumberOfBytesToRead bytes from the file handle stored at this->field_0004 into lpBuffer.
// Returns the byte count reported by ReadFile on success, or nullptr when the buffer is null or the read fails.
static void* __fastcall C_0085ec74_ReadBytesFromFile(C_0085ec74* self, void* /*edx*/, void* lpBuffer, uint32_t nNumberOfBytesToRead)
{
    if (lpBuffer == nullptr) {
        return lpBuffer;
    }

    uint32_t bytesRead = 0;
    void* fileHandle = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0004));
    int readSucceeded = g_pfnReadFile(fileHandle, lpBuffer, nNumberOfBytesToRead, &bytesRead, nullptr);

    if (readSucceeded != 0) {
        return reinterpret_cast<void*>(static_cast<uintptr_t>(bytesRead));
    }
    return nullptr;
}

HOOK(0x006A0370, C_0085ec74_ReadBytesFromFile, "auto");
