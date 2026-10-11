#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(int, __cdecl, MissionScript_DestroyElementRange, 0x00469760,
         (int* first, int* last, ...));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __fastcall MissionScript_ResetElementStorage(int* self, void* /*edx*/)
{
    int result = CSF_AT(int, self, 0x8);
    auto* storage = reinterpret_cast<std::uint8_t*>(self) + 0x4;

    if (result != 0) {
        MissionScript_DestroyElementRange(
            reinterpret_cast<int*>(static_cast<std::intptr_t>(result)),
            reinterpret_cast<int*>(static_cast<std::intptr_t>(CSF_AT(int, self, 0xC))),
            storage, self);

        result = CSF_AT(int, self, 0x8);
        if (result != 0) {
            result = Mem_FreeDebug(
                reinterpret_cast<void*>(static_cast<std::intptr_t>(result)));
        }
    }

    CSF_AT(int, self, 0x8) = 0;
    CSF_AT(int, self, 0xC) = 0;
    CSF_AT(int, self, 0x10) = 0;
    return result;
}

HOOK(0x00410D50, MissionScript_ResetElementStorage, "auto");
