#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, Campaign_SerializeRecordBase, 0x0043FAF0, (C_0082d410* record, int stateAddress));

static int __fastcall C_00831304_Serialize(
    C_00831304* self, void* /*edx*/, int stateAddress)
{
    void* state = reinterpret_cast<void*>(stateAddress);

    bool failed = false;
    if (CSF_AT(uint8_t, state, 0) == 0)
    {
        void* serializer = CSF_AT(void*, state, 0x04);
        failed = !reinterpret_cast<bool (__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, serializer, 0)[0x80 / 4])(
                serializer, nullptr, 0, stateAddress);
    }

    const uint32_t savedContext = CSF_AT(uint32_t, state, 0x1C);
    CSF_AT(uint8_t, state, 0) = failed;
    CSF_AT(uint32_t, state, 0x1C) = 0;

    if (!failed)
    {
        void* serializer = CSF_AT(void*, state, 0x04);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        CSF_AT(uint8_t, state, 0) =
            reinterpret_cast<int (__fastcall*)(void*, void*, uint32_t*, int)>(
                CSF_AT(void**, serializer, 0)[0x48 / 4])(
                    serializer, nullptr, &self->field_0004, stateAddress) == -1;
    }

    bool serializeNext = CSF_AT(uint8_t, state, 0) == 0;
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (serializeNext)
    {
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        Campaign_SerializeRecordBase(
            reinterpret_cast<C_0082d410*>(
                reinterpret_cast<uint8_t*>(self) + 0x08),
            stateAddress);
    }

    serializeNext = CSF_AT(uint8_t, state, 0) == 0;
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (serializeNext)
    {
        void* serializer = CSF_AT(void*, state, 0x04);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        CSF_AT(uint8_t, state, 0) =
            reinterpret_cast<int (__fastcall*)(void*, void*, uint32_t*, int)>(
                CSF_AT(void**, serializer, 0)[0x48 / 4])(
                    serializer, nullptr, &self->field_0030, stateAddress) == -1;
    }

    serializeNext = CSF_AT(uint8_t, state, 0) == 0;
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (serializeNext)
    {
        void* serializer = CSF_AT(void*, state, 0x04);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        CSF_AT(uint8_t, state, 0) =
            reinterpret_cast<int (__fastcall*)(void*, void*, uint32_t*, int)>(
                CSF_AT(void**, serializer, 0)[0x48 / 4])(
                    serializer, nullptr, &self->field_0034, stateAddress) == -1;
    }

    serializeNext = CSF_AT(uint8_t, state, 0) == 0;
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (serializeNext)
    {
        void* serializer = CSF_AT(void*, state, 0x04);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        CSF_AT(uint8_t, state, 0) =
            reinterpret_cast<int (__fastcall*)(void*, void*, void*, int)>(
                CSF_AT(void**, serializer, 0)[0x58 / 4])(
                    serializer, nullptr,
                    reinterpret_cast<uint8_t*>(self) + 0x38,
                    stateAddress) == -1;
    }

    CSF_AT(uint32_t, state, 0x1C) = savedContext;
    if (CSF_AT(uint8_t, state, 0) == 0)
    {
        void* serializer = CSF_AT(void*, state, 0x04);
        // Only the low byte determines success; the full result is returned.
        const int result =
            reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, serializer, 0)[0x7C / 4])(
                    serializer, nullptr, stateAddress);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, state, 0) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, state, 0) = 1;
    return 1;
}

HOOK(0x00457970, C_00831304_Serialize, "auto");
