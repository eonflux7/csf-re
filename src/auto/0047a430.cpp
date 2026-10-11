#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, SceneLoader_SerializeReference, 0x00442E70, (void* a3, bool* a2));
CSF_FUNC(int, __fastcall, WeaponFirearms_SerializeBaseState, 0x00479A50, (void* self, void* edx, int a2));
CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));

static int __fastcall C_008318c8_Serialize(C_008318c8* self, void* /*edx*/, int a2)
{
    auto* archive = reinterpret_cast<uint8_t*>(a2);

    bool failed = false;
    if (CSF_AT(uint8_t, archive, 0) == 0)
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        failed = !((bool(__fastcall*)(void*, void*, int, int))
            CSF_AT(void**, backend, 0)[0x80 / 4])(backend, nullptr, 0, a2);
    }

    const uint32_t savedContext = CSF_AT(uint32_t, archive, 0x1C);
    CSF_AT(uint8_t, archive, 0) = failed;
    WeaponFirearms_SerializeBaseState(self, nullptr, a2);

    failed = CSF_AT(uint8_t, archive, 0) != 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (!failed)
    {
        CSF_AT(uint8_t, archive, 0x15) = 1;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        SceneLoader_SerializeReference(&self->field_0078,
                                       reinterpret_cast<bool*>(archive));
    }

    failed = CSF_AT(uint8_t, archive, 0) != 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (!failed)
    {
        CSF_AT(uint8_t, archive, 0x15) = 1;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        Archive_SerializeObjectRef(&self->field_0088,
                                  reinterpret_cast<bool*>(archive));
    }

    failed = CSF_AT(uint8_t, archive, 0) != 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (!failed)
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        const int result = ((int(__fastcall*)(void*, void*, uint8_t*, int))
            CSF_AT(void**, backend, 0)[0x44 / 4])(
                backend, nullptr, &self->field_007c, a2);
        CSF_AT(uint8_t, archive, 0) = result == -1;
    }

    failed = CSF_AT(uint8_t, archive, 0) != 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (!failed)
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        const int result = ((int(__fastcall*)(void*, void*, uint32_t*, int))
            CSF_AT(void**, backend, 0)[0x58 / 4])(
                backend, nullptr, &self->field_0080, a2);
        CSF_AT(uint8_t, archive, 0) = result == -1;
    }

    CSF_AT(uint32_t, archive, 0x1C) = savedContext;
    if (CSF_AT(uint8_t, archive, 0) == 0)
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        const int result = ((int(__fastcall*)(void*, void*, int))
            CSF_AT(void**, backend, 0)[0x7C / 4])(backend, nullptr, a2);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, archive, 0) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, archive, 0) = 1;
    return 1;
}

HOOK(0x0047A430, C_008318c8_Serialize, "auto");
