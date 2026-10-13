#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, Campaign_SerializeRecordBase, 0x0043FAF0, (C_0082d410* record, int stateAddress));
CSF_FUNC(int, __fastcall, Campaign_SerializeRecordDetails, 0x0044BED0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int archiveAddress, S_00691860* string));

static int __fastcall Campaign_SerializeExtendedRecord(
    void* self, void* /*edx*/, S_00691860* state)
{
    auto* record = static_cast<uint8_t*>(self);
    const uint32_t savedContext = CSF_AT(uint32_t, state, 0x1C);

    bool failed = false;
    if (CSF_AT(uint8_t, state, 0) == 0)
    {
        void* archive = reinterpret_cast<void*>(state->field_0004);
        failed = !reinterpret_cast<bool(__fastcall*)(
            void*, void*, int, S_00691860*)>(
                CSF_AT(void**, archive, 0)[128 / 4])(
                    archive, nullptr, 0, state);
    }

    CSF_AT(uint8_t, state, 0) = failed;
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (!failed)
    {
        void* archive = reinterpret_cast<void*>(state->field_0004);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        CSF_AT(uint8_t, state, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, S_00691860*)>(
                CSF_AT(void**, archive, 0)[88 / 4])(
                    archive, nullptr, record, state) == -1;
    }

    bool proceed = CSF_AT(uint8_t, state, 0) == 0;
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (proceed)
    {
        void* archive = reinterpret_cast<void*>(state->field_0004);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        CSF_AT(uint8_t, state, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, S_00691860*)>(
                CSF_AT(void**, archive, 0)[56 / 4])(
                    archive, nullptr, record + 4, state) == -1;
    }

    proceed = CSF_AT(uint8_t, state, 0) == 0;
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (proceed)
    {
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        Archive_SerializeString(reinterpret_cast<int>(record + 16), state);
    }

    proceed = CSF_AT(uint8_t, state, 0) == 0;
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (proceed)
    {
        void* archive = reinterpret_cast<void*>(state->field_0004);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        CSF_AT(uint8_t, state, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, S_00691860*)>(
                CSF_AT(void**, archive, 0)[72 / 4])(
                    archive, nullptr, record + 28, state) == -1;
    }

    proceed = CSF_AT(uint8_t, state, 0) == 0;
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (proceed)
    {
        void* archive = reinterpret_cast<void*>(state->field_0004);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        CSF_AT(uint8_t, state, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, S_00691860*)>(
                CSF_AT(void**, archive, 0)[88 / 4])(
                    archive, nullptr, record + 32, state) == -1;
    }

    proceed = CSF_AT(uint8_t, state, 0) == 0;
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (proceed)
    {
        void* archive = reinterpret_cast<void*>(state->field_0004);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        CSF_AT(uint8_t, state, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, S_00691860*)>(
                CSF_AT(void**, archive, 0)[88 / 4])(
                    archive, nullptr, record + 36, state) == -1;
    }

    proceed = CSF_AT(uint8_t, state, 0) == 0;
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (proceed)
    {
        void* archive = reinterpret_cast<void*>(state->field_0004);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        CSF_AT(uint8_t, state, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, S_00691860*)>(
                CSF_AT(void**, archive, 0)[68 / 4])(
                    archive, nullptr, record + 40, state) == -1;
    }

    proceed = CSF_AT(uint8_t, state, 0) == 0;
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (proceed)
    {
        void* archive = reinterpret_cast<void*>(state->field_0004);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        CSF_AT(uint8_t, state, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, S_00691860*)>(
                CSF_AT(void**, archive, 0)[56 / 4])(
                    archive, nullptr, record + 44, state) == -1;
    }

    proceed = CSF_AT(uint8_t, state, 0) == 0;
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (proceed)
    {
        void* archive = reinterpret_cast<void*>(state->field_0004);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        CSF_AT(uint8_t, state, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, S_00691860*)>(
                CSF_AT(void**, archive, 0)[88 / 4])(
                    archive, nullptr, record + 56, state) == -1;
    }

    proceed = CSF_AT(uint8_t, state, 0) == 0;
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (proceed)
    {
        void* archive = reinterpret_cast<void*>(state->field_0004);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        CSF_AT(uint8_t, state, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, S_00691860*)>(
                CSF_AT(void**, archive, 0)[88 / 4])(
                    archive, nullptr, record + 60, state) == -1;
    }

    proceed = CSF_AT(uint8_t, state, 0) == 0;
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (proceed)
    {
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        Campaign_SerializeRecordBase(
            reinterpret_cast<C_0082d410*>(record + 64),
            reinterpret_cast<int>(state));
    }

    proceed = CSF_AT(uint8_t, state, 0) == 0;
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (proceed)
    {
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        Campaign_SerializeRecordDetails(
            record + 92, nullptr, reinterpret_cast<int>(state));
    }

    proceed = CSF_AT(uint8_t, state, 0) == 0;
    CSF_AT(uint32_t, state, 0x1C) = savedContext;
    if (proceed)
    {
        void* archive = reinterpret_cast<void*>(state->field_0004);
        const bool result =
            reinterpret_cast<bool(__fastcall*)(void*, void*, S_00691860*)>(
                CSF_AT(void**, archive, 0)[124 / 4])(
                    archive, nullptr, state);
        if (result)
        {
            CSF_AT(uint8_t, state, 0) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, state, 0) = 1;
    return 1;
}

HOOK(0x0044C600, Campaign_SerializeExtendedRecord, "auto");
