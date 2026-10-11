#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Import slot for kernel32!SetFilePointer (stdcall, 4 args).
CSF_GLOBAL(void*, g_SetFilePointerImport, 0x82D230);

// Seeks the file by 0 bytes from the current position (FILE_CURRENT), which returns the
// current file offset. Returns 0 when no file handle is stored.
static uint32_t __fastcall C_0085ec74_GetCurrentFilePosition(C_0085ec74* self, void* /*edx*/) {
    uint32_t fileHandle = self->field_0004;
    if (fileHandle) {
        using SetFilePointerFn = uint32_t(__stdcall*)(void*, int32_t, int32_t*, uint32_t);
        auto setFilePointer = reinterpret_cast<SetFilePointerFn>(g_SetFilePointerImport);
        return setFilePointer(reinterpret_cast<void*>(static_cast<uintptr_t>(fileHandle)), 0, nullptr, 1u);
    }
    return 0;
}

HOOK(0x6A0430, C_0085ec74_GetCurrentFilePosition, "auto");
