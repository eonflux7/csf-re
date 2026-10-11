#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(C_00840e68*, __fastcall, C_00840e68_Construct, 0x0051C0C0, (C_00840e68* self, void*));
CSF_GLOBAL(const char*, g_psz_PFX_VEHICLE_HUMO_DAMAGE, 0x008A8C98);

static int __fastcall C_00840f48_Construct(C_00840f48* self, void* /*edx*/)
{
    C_00840e68_Construct(self, nullptr);
    *(void***)self = CSF_VTBL(0x00840F48);
    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0014 = 0;
    self->field_0018 = 10;
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_0028 = 10;

    void* manager = CSF_AT(void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FCC);
    self->field_0008 =
        ((int(__fastcall*)(void*, void*, const char*))
         CSF_AT(void**, manager, 0)[3])(
            manager, nullptr, g_psz_PFX_VEHICLE_HUMO_DAMAGE);

    manager = CSF_AT(void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FCC);
    if (self->field_0008 != UINT32_MAX)
    {
        void* effect =
            ((void*(__fastcall*)(void*, void*, uint32_t))
             CSF_AT(void**, manager, 0)[2])(
                manager, nullptr, self->field_0008);
        if (CSF_AT(uint32_t, effect, 0x4) != 0)
            self->field_0008 = UINT32_MAX;
    }
    return reinterpret_cast<int>(self);
}

HOOK(0x0051E810, C_00840f48_Construct, "auto");
