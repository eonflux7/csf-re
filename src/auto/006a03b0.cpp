#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Win32 WriteFile, reached through its import-address-table slot (stdcall, 5 args).
using WriteFileFn = int(__stdcall*)(void*, const void*, uint32_t, uint32_t*, void*);
CSF_GLOBAL(WriteFileFn, g_pfnWriteFile, 0x0082D234);

// Writes nNumberOfBytesToWrite bytes from lpBuffer to the file handle stored at this->field_0004.
// Returns the byte count reported by WriteFile on success, or nullptr when the buffer is null or the write fails.
static void* __fastcall C_0085ec74_WriteBytesToFile(C_0085ec74* self, void* /*edx*/, const void* lpBuffer, uint32_t nNumberOfBytesToWrite)
{
    if (lpBuffer == nullptr) {
        return nullptr;
    }

    uint32_t bytesWritten = 0;
    void* fileHandle = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0004));
    int writeSucceeded = g_pfnWriteFile(fileHandle, lpBuffer, nNumberOfBytesToWrite, &bytesWritten, nullptr);

    if (writeSucceeded != 0) {
        return reinterpret_cast<void*>(static_cast<uintptr_t>(bytesWritten));
    }
    return nullptr;
}

HOOK(0x006A03B0, C_0085ec74_WriteBytesToFile, "auto");
