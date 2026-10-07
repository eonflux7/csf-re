#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, ResetWideStringsInRecordRange, 0x007233D0,
         (int begin, int end));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __fastcall ClearWideStringRecordStorage(int* self, void* /*edx*/)
{
    int result = CSF_AT(int, self, 0x04);
    if (result != 0)
    {
        ResetWideStringsInRecordRange(result, CSF_AT(int, self, 0x08));

        result = CSF_AT(int, self, 0x04);
        if (result != 0)
        {
            result = Mem_FreeDebug(
                reinterpret_cast<void*>(static_cast<std::uintptr_t>(result)));
        }
    }

    CSF_AT(int, self, 0x04) = 0;
    CSF_AT(int, self, 0x08) = 0;
    CSF_AT(int, self, 0x0C) = 0;
    return result;
}

HOOK(0x00723750, ClearWideStringRecordStorage, "auto");
