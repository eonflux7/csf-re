#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Converts a 32-bit game pointer returned in eax into a typed pointer.
static uint32_t* GamePtrFromInt(int value)
{
    return reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(static_cast<uint32_t>(value)));
}

// Clears the byte flag at owner+1 (owner = *(a1+4)), then, when the entry's
// handle resolves to an object of type 26 with a non-negative scene index,
// copies the "bit 2 set" state of the object's sub-record into that flag.
// Returns the last pointer value the original leaves in eax.
static uint32_t* __stdcall UpdateFlagFromResolvedHandleBit2(int a1, int a2, int a3)
{
    (void)a2;
    void* self = reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(a1)));

    void* ownerObj = reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, self, 4)));
    CSF_AT(uint8_t, ownerObj, 1) = 0;

    void* refHolder = reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(a3)));
    void* refObj = reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, refHolder, 0)));
    uint32_t entryAddr = CSF_AT(uint32_t, refObj, 4);
    void* entry = reinterpret_cast<void*>(static_cast<uintptr_t>(entryAddr));

    int32_t sceneIndex = CSF_AT(int32_t, entry, 12);
    uint32_t* result = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(entryAddr + 4));

    if (sceneIndex >= 0)
    {
        result = GamePtrFromInt(CHandleTable_Resolve(g_pCurrentScene, nullptr,
                                                     static_cast<int>(entryAddr + 4)));
        if (result != nullptr)
        {
            void* resolved = reinterpret_cast<void*>(result);
            if (CSF_AT(uint32_t, resolved, 0x50) == 26)
            {
                uint32_t linkedAddr = CSF_AT(uint32_t, resolved, 0x34);
                void* linked = reinterpret_cast<void*>(static_cast<uintptr_t>(linkedAddr));
                result = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(linkedAddr));

                uint32_t linkedFlags = CSF_AT(uint32_t, linked, 0x44);
                void* ownerAgain = reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, self, 4)));
                CSF_AT(uint8_t, ownerAgain, 1) = static_cast<uint8_t>((linkedFlags & 4) != 0);
            }
        }
    }
    return result;
}

HOOK(0x0045D3E0, UpdateFlagFromResolvedHandleBit2, "auto");
