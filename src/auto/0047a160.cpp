#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, IsWeaponSubObjectReady, 0x00478280, (void* self, void* edx));
CSF_FUNC(int, __fastcall, InitializeWeaponState, 0x004794C0, (void* self, void* edx, char a3));

static int __fastcall C_008318c8_UpdateShellEffectState(
    C_008318c8* self, void* /*edx*/, char mode)
{
    InitializeWeaponState(self, nullptr, mode);
    void* subObject = reinterpret_cast<void*>(IsWeaponSubObjectReady(self, nullptr));

    const int enabled =
        CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_0004), 0x228) ? 0 : 1;

    void* controller =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, subObject, 0)[0x28 / 4])(subObject, nullptr);

    return reinterpret_cast<int (__fastcall*)(void*, void*, int, int)>(
        CSF_AT(void**, controller, 0)[0x24 / 4])(controller, nullptr, 8, enabled);
}

HOOK(0x0047A160, C_008318c8_UpdateShellEffectState, "auto");
