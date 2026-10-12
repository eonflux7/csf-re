#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, C_00833908_CheckSlot28TimerAndForward, 0x00495160, (void* self, void* edx));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Returns (linked object value - 4), or nullptr when the linked value is zero.
static C_00833908* Estatico_GetLinkedObjectBase(int a1)
{
    int linkedValue = GetLinkedObjectValue_4DB5C0(reinterpret_cast<void*>(static_cast<uintptr_t>(a1)), nullptr);
    if (linkedValue)
        return reinterpret_cast<C_00833908*>(static_cast<uintptr_t>(linkedValue - 4));
    return nullptr;
}

static C_00833908* __stdcall Estatico_ForwardToLinkedHandleWhenState3(int a1)
{
    C_00833908* result = Estatico_GetLinkedObjectBase(a1);

    if (CSF_AT(int, result, 0x608) == 3)
    {
        C_00833908* linkedBase = Estatico_GetLinkedObjectBase(a1);
        uint8_t* base = reinterpret_cast<uint8_t*>(linkedBase);

        // Negative value at +0x630 selects the +0x610 slot, otherwise +0x628.
        uintptr_t slotOffset = (CSF_AT(int, base, 0x630) < 0) ? 0x610u : 0x628u;
        int handle = static_cast<int>(reinterpret_cast<uintptr_t>(base) + slotOffset);

        int target = CHandleTable_Resolve(g_pCurrentScene, nullptr, handle);
        if (target)
        {
            int forwarded = C_00833908_CheckSlot28TimerAndForward(
                reinterpret_cast<void*>(static_cast<uintptr_t>(target)), nullptr);
            return reinterpret_cast<C_00833908*>(static_cast<uintptr_t>(forwarded));
        }
        return nullptr;
    }
    return result;
}

HOOK(0x4AFBB0, Estatico_ForwardToLinkedHandleWhenState3, "auto");
