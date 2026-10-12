#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(int, __cdecl, MissionScript_FreeSlotRange, 0x00468AF0,
         (int* first, int* end, int , int));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __fastcall SceneLoader_ResetNextSceneManager(int* self, void* /*edx*/)
{
    int result = CSF_AT(int, self, 0x14);
    auto slotBaseAddress = static_cast<int>(reinterpret_cast<std::intptr_t>(
        reinterpret_cast<std::uint8_t*>(self) + 0x10));
    auto thisAddress = static_cast<int>(reinterpret_cast<std::intptr_t>(self));

    if (result != 0) {
        MissionScript_FreeSlotRange(
            reinterpret_cast<int*>(static_cast<std::intptr_t>(result)),
            reinterpret_cast<int*>(static_cast<std::intptr_t>(CSF_AT(int, self, 0x18))),
            slotBaseAddress,
            thisAddress);

        result = CSF_AT(int, self, 0x14);
        if (result != 0) {
            result = Mem_FreeDebug(
                reinterpret_cast<void*>(static_cast<std::intptr_t>(result)));
        }
    }

    CSF_AT(int, self, 0x14) = 0;
    CSF_AT(int, self, 0x18) = 0;
    CSF_AT(int, self, 0x1C) = 0;
    return result;
}

HOOK(0x00442200, SceneLoader_ResetNextSceneManager, "auto");
