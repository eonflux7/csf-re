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

// Win32 scalar aliases (no <windows.h> in this build).
using HandleValue = void*;
using DwordValue = uint32_t;
using LongValue = int32_t;

// SetFilePointer(HANDLE, LONG, PLONG, DWORD) is imported; the original calls it
// indirectly through the import slot at 0x82D230, so bind that slot by address.
CSF_GLOBAL(void*, g_pSetFilePointerImport, 0x0082D230);

using SetFilePointerFn = DwordValue(__stdcall*)(HandleValue, LongValue, LongValue*, DwordValue);

// Maps the caller's seek origin (0 begin, 1 current, 2 end) to the Win32 move method.
// Anything else falls back to FILE_BEGIN.
static DwordValue SeekOriginToMoveMethod(int seekOrigin) {
    switch (seekOrigin) {
        case 1:
            return 1;  // FILE_CURRENT
        case 2:
            return 2;  // FILE_END
        default:
            return 0;  // FILE_BEGIN
    }
}

static bool __fastcall C_0085ec74_SeekFile(C_0085ec74* self, void* /*edx*/, int seekOrigin, LongValue distanceToMove) {
    DwordValue moveMethod = SeekOriginToMoveMethod(seekOrigin);
    HandleValue fileHandle = reinterpret_cast<HandleValue>(static_cast<uintptr_t>(self->field_0004));
    auto setFilePointer = reinterpret_cast<SetFilePointerFn>(g_pSetFilePointerImport);
    DwordValue newPosition = setFilePointer(fileHandle, distanceToMove, nullptr, moveMethod);
    return newPosition != 0xFFFFFFFFu;  // INVALID_SET_FILE_POINTER
}

HOOK(0x006A03F0, C_0085ec74_SeekFile, "auto");
