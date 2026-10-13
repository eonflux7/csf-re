#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, ResolveRwManager, 0x006B13B0, (int managerId, void* edx));
CSF_FUNC(void**, __fastcall, ReleaseManagedResource, 0x006D0970, (void* self, void* edx, void** a2));
CSF_FUNC(char, __fastcall, ScreenEffects_UpdateBase, 0x007090F0, (void* self, void* edx));

static char __fastcall C_008629a4_ReleaseManagedResources(C_008629a4* self, void* /*edx*/)
{
    if (CSF_AT(uint8_t, self, 0x8) != 0)
    {
        void** managedHandle = CSF_AT(void**, self, 0x60);
        if (managedHandle != nullptr)
        {
            // The handle is pushed before the lookup calls and consumed as the
            // stack argument of ReleaseManagedResource (ret 4).
            int globalInt = GetGlobalInt();
            int rwManager = ResolveRwManager(globalInt, nullptr);
            ReleaseManagedResource(reinterpret_cast<void*>(rwManager), nullptr, managedHandle);
            CSF_AT(void**, self, 0x60) = nullptr;
        }

        void* secondaryObject = CSF_AT(void*, self, 0x6C);
        if (secondaryObject != nullptr)
        {
            // Virtual slot +4 with argument 1.
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, secondaryObject, 0)[1])(secondaryObject, nullptr, 1);
            CSF_AT(void*, self, 0x6C) = nullptr;
        }
    }

    // Tail jump to ScreenEffects_UpdateBase with ecx = this; its result is the return value.
    return ScreenEffects_UpdateBase(self, nullptr);
}

HOOK(0x006F8020, C_008629a4_ReleaseManagedResources, "auto");
