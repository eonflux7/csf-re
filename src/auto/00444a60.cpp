#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, SceneLoader_SerializeReference, 0x00442E70, (void* a3, bool* a2));

static int __fastcall C_008310b8_Serialize(
    C_008310b8* self, void* /*edx*/, bool* state)
{
    bool failed = false;
    if (CSF_AT(uint8_t, state, 0x00) == 0)
    {
        void* provider = CSF_AT(void*, state, 0x04);
        failed = !reinterpret_cast<bool(__fastcall*)(void*, void*, int, bool*)>(
            CSF_AT(void**, provider, 0)[0x80 / 4])(
                provider, nullptr, 0, state);
    }

    const uint32_t savedContext = CSF_AT(uint32_t, state, 0x1C);
    CSF_AT(uint8_t, state, 0x00) = failed;
    *reinterpret_cast<uint8_t*>(&self->field_0008) = 1;

    const bool serializeReference = CSF_AT(uint8_t, state, 0x00) == 0;
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (serializeReference)
    {
        CSF_AT(uint8_t, state, 0x15) = 1;
        CSF_AT(uint8_t, state, 0x16) = 0;
        SceneLoader_SerializeReference(&self->field_0004, state);
    }

    const bool serializeValue = CSF_AT(uint8_t, state, 0x00) == 0;
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (serializeValue)
    {
        void* provider = CSF_AT(void*, state, 0x04);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, char*, bool*)>(
                CSF_AT(void**, provider, 0)[0x44 / 4])(
                    provider, nullptr,
                    &CSF_AT(char, self, 0x09), state);
        CSF_AT(uint8_t, state, 0x00) = result == -1;
    }

    *reinterpret_cast<uint8_t*>(&self->field_0008) = 0;
    CSF_AT(uint32_t, state, 0x1C) = savedContext;

    if (CSF_AT(uint8_t, state, 0x00) == 0)
    {
        void* provider = CSF_AT(void*, state, 0x04);
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, bool*)>(
                CSF_AT(void**, provider, 0)[0x7C / 4])(
                    provider, nullptr, state);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, state, 0x00) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, state, 0x00) = 1;
    return 1;
}

HOOK(0x00444A60, C_008310b8_Serialize, "auto");
