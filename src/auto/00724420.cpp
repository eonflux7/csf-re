#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// 0x007241E0 - already reimplemented in the project. Resets the element range
// (begin/end pointers) stored in the record array.
CSF_FUNC(int*, __cdecl, ResetRecordRange, 0x007241E0, (int* begin, int* end));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __fastcall ClearRecordStorage(int* self, void* /*edx*/)
{
    int result = CSF_AT(int, self, 0x04);
    if (result != 0)
    {
        ResetRecordRange(
            reinterpret_cast<int*>(static_cast<std::uintptr_t>(result)),
            reinterpret_cast<int*>(static_cast<std::uintptr_t>(CSF_AT(int, self, 0x08))));

        result = CSF_AT(int, self, 0x04);
        if (result != 0)
            result = Mem_FreeDebug(reinterpret_cast<void*>(static_cast<std::uintptr_t>(result)));
    }

    CSF_AT(int, self, 0x04) = 0;
    CSF_AT(int, self, 0x08) = 0;
    CSF_AT(int, self, 0x0C) = 0;
    return result;
}

HOOK(0x00724420, ClearRecordStorage, "auto");
