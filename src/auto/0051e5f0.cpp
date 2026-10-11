#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
static CSF_GLOBAL(const char*, g_psz_PFX_VEHICLE_HUMO_DAMAGE, 0x008A8C98);

static int __fastcall C_00840f48_ResolveVehicleDamageSmoke(C_00840f48* self, void* /*edx*/)
{
    void* manager = CSF_AT(void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FCC);
    self->field_0008 =
        ((int (__fastcall*)(void*, void*, const char*))
            CSF_AT(void**, manager, 0)[0xC / 4])(
                manager, nullptr, g_psz_PFX_VEHICLE_HUMO_DAMAGE);

    manager = CSF_AT(void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FCC);
    int result = static_cast<int>(self->field_0008);
    if (result != -1)
    {
        void* entry =
            ((void* (__fastcall*)(void*, void*, uint32_t))
                CSF_AT(void**, manager, 0)[0x8 / 4])(
                    manager, nullptr, self->field_0008);
        uint32_t state = CSF_AT(uint32_t, entry, 0x4);
        result = reinterpret_cast<int>(static_cast<uint8_t*>(entry) + 4);
        if (state)
            self->field_0008 = -1;
    }
    return result;
}

HOOK(0x0051E5F0, C_00840f48_ResolveVehicleDamageSmoke, "auto");
