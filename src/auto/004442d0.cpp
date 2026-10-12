#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Archive_SerializeEmbeddedRecord, 0x00443F80,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080,
         (int archiveAddress, S_00691860* string));

static int __fastcall Archive_SerializeRecordFields(
    void* self, void* /*edx*/, S_00691860* state)
{
    auto* record = static_cast<uint8_t*>(self);
    const uint32_t savedContext = state[1].field_0004;

    bool failed = false;
    if (CSF_AT(uint8_t, state, 0x00) == 0)
    {
        void* backend = reinterpret_cast<void*>(state->field_0004);
        failed = !((bool(__fastcall*)(void*, void*, int, S_00691860*))
                       CSF_AT(void**, backend, 0)[0x80 / 4])(
            backend, nullptr, 0, state);
    }

    CSF_AT(uint8_t, state, 0x00) = failed;
    state[1].field_0004 = 0;
    if (!failed)
    {
        void* backend = reinterpret_cast<void*>(state->field_0004);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        CSF_AT(uint8_t, state, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, S_00691860*))
                 CSF_AT(void**, backend, 0)[0x44 / 4])(
                backend, nullptr, self, state) == -1;
    }

    bool proceed = CSF_AT(uint8_t, state, 0x00) == 0;
    state[1].field_0004 = 0;
    if (proceed)
    {
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        Archive_SerializeString(reinterpret_cast<int>(record + 4), state);
    }

    proceed = CSF_AT(uint8_t, state, 0x00) == 0;
    state[1].field_0004 = 0;
    if (proceed)
    {
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        Archive_SerializeEmbeddedRecord(
            record + 16, nullptr, reinterpret_cast<int>(state));
    }

    proceed = CSF_AT(uint8_t, state, 0x00) == 0;
    state[1].field_0004 = 0;
    if (proceed)
    {
        void* backend = reinterpret_cast<void*>(state->field_0004);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        CSF_AT(uint8_t, state, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, S_00691860*))
                 CSF_AT(void**, backend, 0)[0x48 / 4])(
                backend, nullptr, record + 36, state) == -1;
    }

    state[1].field_0004 = savedContext;
    if (CSF_AT(uint8_t, state, 0x00) == 0)
    {
        void* backend = reinterpret_cast<void*>(state->field_0004);
        const int result =
            ((int(__fastcall*)(void*, void*, S_00691860*))
                 CSF_AT(void**, backend, 0)[0x7C / 4])(
                backend, nullptr, state);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, state, 0x00) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, state, 0x00) = 1;
    return 1;
}

HOOK(0x004442D0, Archive_SerializeRecordFields, "auto");
