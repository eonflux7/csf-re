#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, Campaign_SerializeRecordBase, 0x0043FAF0, (C_0082d410* record, int stateAddress));

static int __fastcall C_0083f130_Serialize(
    C_0083f130* self, void* /*edx*/, int stateAddress)
{
    void* state = reinterpret_cast<void*>(stateAddress);

    bool failed = false;
    if (CSF_AT(uint8_t, state, 0x00) == 0)
    {
        void* stream = CSF_AT(void*, state, 0x04);
        failed = !reinterpret_cast<bool(__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, stream, 0)[0x80 / 4])(
                stream, nullptr, 0, stateAddress);
    }

    const uint32_t savedContext = CSF_AT(uint32_t, state, 0x1C);
    CSF_AT(uint8_t, state, 0x00) = static_cast<uint8_t>(failed);
    CSF_AT(uint32_t, state, 0x1C) = 0;

    if (!failed)
    {
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        Campaign_SerializeRecordBase(
            reinterpret_cast<C_0082d410*>(&self->field_0004),
            stateAddress);
    }

    const bool serializeMember = CSF_AT(uint8_t, state, 0x00) == 0;
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (serializeMember)
    {
        void* stream = CSF_AT(void*, state, 0x04);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;

        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, int)>(
                CSF_AT(void**, stream, 0)[0x54 / 4])(
                    stream, nullptr,
                    &CSF_AT(uint32_t, self, 0x48), stateAddress);
        CSF_AT(uint8_t, state, 0x00) = static_cast<uint8_t>(result == -1);
    }

    const bool finish = CSF_AT(uint8_t, state, 0x00) == 0;
    CSF_AT(uint32_t, state, 0x1C) = savedContext;
    if (finish)
    {
        void* stream = CSF_AT(void*, state, 0x04);
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, stream, 0)[0x7C / 4])(
                    stream, nullptr, stateAddress);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, state, 0x00) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, state, 0x00) = 1;
    return 1;
}

HOOK(0x004F4880, C_0083f130_Serialize, "auto");
