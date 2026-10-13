#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int archiveAddress, S_00691860* string));

static int __fastcall Archive_SerializeRecord(
    void* self, void* /*edx*/, void* context)
{
    auto* state = static_cast<S_00691860*>(context);
    const uint32_t savedContext = CSF_AT(uint32_t, state, 0x1C);

    bool failed = false;
    if (!CSF_AT(uint8_t, state, 0))
    {
        void* archive = reinterpret_cast<void*>(state->field_0004);
        failed = !((bool(__fastcall*)(void*, void*, int, S_00691860*))
                       CSF_AT(void**, archive, 0)[128 / 4])(
                           archive, nullptr, 0, state);
    }

    CSF_AT(uint8_t, state, 0) = failed;
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (!failed)
    {
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        Archive_SerializeString(reinterpret_cast<int>(self), state);
    }

    bool canSerialize = CSF_AT(uint8_t, state, 0) == 0;
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (canSerialize)
    {
        void* archive = reinterpret_cast<void*>(state->field_0004);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        const int result =
            ((int(__fastcall*)(void*, void*, void*, S_00691860*))
                 CSF_AT(void**, archive, 0)[88 / 4])(
                     archive, nullptr,
                     static_cast<uint8_t*>(self) + 12, state);
        CSF_AT(uint8_t, state, 0) = result == -1;
    }

    canSerialize = CSF_AT(uint8_t, state, 0) == 0;
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (canSerialize)
    {
        void* archive = reinterpret_cast<void*>(state->field_0004);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        const int result =
            ((int(__fastcall*)(void*, void*, void*, S_00691860*))
                 CSF_AT(void**, archive, 0)[88 / 4])(
                     archive, nullptr,
                     static_cast<uint8_t*>(self) + 16, state);
        CSF_AT(uint8_t, state, 0) = result == -1;
    }

    canSerialize = CSF_AT(uint8_t, state, 0) == 0;
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (canSerialize)
    {
        void* archive = reinterpret_cast<void*>(state->field_0004);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        const int result =
            ((int(__fastcall*)(void*, void*, void*, S_00691860*))
                 CSF_AT(void**, archive, 0)[88 / 4])(
                     archive, nullptr,
                     static_cast<uint8_t*>(self) + 20, state);
        CSF_AT(uint8_t, state, 0) = result == -1;
    }

    canSerialize = CSF_AT(uint8_t, state, 0) == 0;
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (canSerialize)
    {
        void* archive = reinterpret_cast<void*>(state->field_0004);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        const int result =
            ((int(__fastcall*)(void*, void*, void*, S_00691860*))
                 CSF_AT(void**, archive, 0)[32 / 4])(
                     archive, nullptr,
                     static_cast<uint8_t*>(self) + 24, state);
        CSF_AT(uint8_t, state, 0) = result == -1;
    }

    canSerialize = CSF_AT(uint8_t, state, 0) == 0;
    CSF_AT(uint32_t, state, 0x1C) = savedContext;
    if (canSerialize)
    {
        void* archive = reinterpret_cast<void*>(state->field_0004);
        const int result =
            ((int(__fastcall*)(void*, void*, S_00691860*))
                 CSF_AT(void**, archive, 0)[124 / 4])(
                     archive, nullptr, state);
        if (static_cast<uint8_t>(result))
        {
            CSF_AT(uint8_t, state, 0) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, state, 0) = 1;
    return 1;
}

HOOK(0x0049F600, Archive_SerializeRecord, "auto");
