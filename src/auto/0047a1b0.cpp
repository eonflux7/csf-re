#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, IsWeaponSubObjectReady, 0x00478280, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_479420, 0x00479420, (void* self, void* edx));

static int __fastcall C_008318c8_ResetSubObjectAndUpdate(C_008318c8* self, void* /*edx*/)
{
    int subObjectAddress = IsWeaponSubObjectReady(self, nullptr);
    if (subObjectAddress)
    {
        void* subObject = reinterpret_cast<void*>(subObjectAddress);
        void* state = ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, subObject, 0)[0x28 / 4])(subObject, nullptr);

        ((void (__fastcall*)(void*, void*, int, int))
            CSF_AT(void**, state, 0)[0x24 / 4])(state, nullptr, 8, 0);
    }
    return sub_479420(self, nullptr);
}

HOOK(0x0047A1B0, C_008318c8_ResetSubObjectAndUpdate, "auto");
