#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int archiveAddress, S_00691860* string));
CSF_FUNC(int, __fastcall, S_0041f0a0_SerializeBaseState, 0x004D19C0, (S_0041f0a0* self, void* , S_0041f0a0* archive));

static int __fastcall S_0041f0a0_SerializeSearchOrderState(
    S_0041f0a0* self, void* /*edx*/, S_0041f0a0* archive)
{
    bool failed = false;
    if (CSF_AT(uint8_t, archive, 0x00) == 0)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        failed = !((bool(__fastcall*)(void*, void*, int, S_0041f0a0*))
            CSF_AT(void**, serializer, 0)[0x80 / 4])(
                serializer, nullptr, 0, archive);
    }

    uint32_t savedContext = CSF_AT(uint32_t, archive, 0x1C);
    CSF_AT(uint8_t, archive, 0x00) = failed;
    S_0041f0a0_SerializeBaseState(self, nullptr, archive);

    bool ready = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        int result = ((int(__fastcall*)(void*, void*, uint8_t*, S_0041f0a0*))
            CSF_AT(void**, serializer, 0)[0x58 / 4])(
                serializer, nullptr, &CSF_AT(uint8_t, self, 0x3C), archive);
        CSF_AT(uint8_t, archive, 0x00) = result == -1;
    }

    ready = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        Archive_SerializeString(
            reinterpret_cast<int>(&self->field_0040),
            reinterpret_cast<S_00691860*>(archive));
    }

    ready = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        Archive_SerializeString(
            reinterpret_cast<int>(&CSF_AT(uint8_t, self, 0x4C)),
            reinterpret_cast<S_00691860*>(archive));
    }

    ready = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = savedContext;
    if (ready)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        int result = ((int(__fastcall*)(void*, void*, S_0041f0a0*))
            CSF_AT(void**, serializer, 0)[0x7C / 4])(
                serializer, nullptr, archive);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, archive, 0x00) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, archive, 0x00) = 1;
    return 1;
}

HOOK(0x004CDC00, S_0041f0a0_SerializeSearchOrderState, "auto");
