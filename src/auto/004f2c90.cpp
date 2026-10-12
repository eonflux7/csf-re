#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, Campaign_SerializeRecordBase, 0x0043FAF0, (C_0082d410* record, int stateAddress));
CSF_FUNC(int, __stdcall, ProfilesSavegames_SerializeBase, 0x004F1E60, (void* a1, int arg4));

static bool HudStringShow_BeginSerializationStep(void* state)
{
    const bool ready = CSF_AT(uint8_t, state, 0) == 0;
    CSF_AT(int32_t, state, 0x1C) = 0;
    return ready;
}

static int __fastcall HudStringShow_SerializeRecord(
    void* self, void* /*edx*/, void* state)
{
    auto* record = static_cast<uint8_t*>(self);
    const int32_t savedContext = CSF_AT(int32_t, state, 0x1C);

    bool failed = true;
    if (!CSF_AT(uint8_t, state, 0))
    {
        void* serializer = CSF_AT(void*, state, 4);
        failed = !((bool(__fastcall*)(void*, void*, int, void*))
            CSF_AT(void**, serializer, 0)[128 / 4])(
                serializer, nullptr, 0, state);
    }

    CSF_AT(uint8_t, state, 0) = failed;
    CSF_AT(int32_t, state, 0x1C) = 0;
    if (!failed)
    {
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        Campaign_SerializeRecordBase(
            reinterpret_cast<C_0082d410*>(self),
            reinterpret_cast<int>(state));
    }

    if (HudStringShow_BeginSerializationStep(state))
    {
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        ProfilesSavegames_SerializeBase(record + 28, reinterpret_cast<int>(state));
    }

    if (HudStringShow_BeginSerializationStep(state))
    {
        void* serializer = CSF_AT(void*, state, 4);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        CSF_AT(uint8_t, state, 0) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, serializer, 0)[88 / 4])(
                    serializer, nullptr, record + 56, state) == -1;
    }

    if (HudStringShow_BeginSerializationStep(state))
    {
        void* serializer = CSF_AT(void*, state, 4);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        CSF_AT(uint8_t, state, 0) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, serializer, 0)[84 / 4])(
                    serializer, nullptr, record + 60, state) == -1;
    }

    if (HudStringShow_BeginSerializationStep(state))
    {
        void* serializer = CSF_AT(void*, state, 4);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        CSF_AT(uint8_t, state, 0) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, serializer, 0)[84 / 4])(
                    serializer, nullptr, record + 64, state) == -1;
    }

    if (HudStringShow_BeginSerializationStep(state))
    {
        void* serializer = CSF_AT(void*, state, 4);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        CSF_AT(uint8_t, state, 0) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, serializer, 0)[84 / 4])(
                    serializer, nullptr, record + 68, state) == -1;
    }

    if (HudStringShow_BeginSerializationStep(state))
    {
        void* serializer = CSF_AT(void*, state, 4);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        CSF_AT(uint8_t, state, 0) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, serializer, 0)[84 / 4])(
                    serializer, nullptr, record + 72, state) == -1;
    }

    if (HudStringShow_BeginSerializationStep(state))
    {
        void* serializer = CSF_AT(void*, state, 4);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        CSF_AT(uint8_t, state, 0) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, serializer, 0)[64 / 4])(
                    serializer, nullptr, record + 84, state) == -1;
    }

    if (HudStringShow_BeginSerializationStep(state))
    {
        void* serializer = CSF_AT(void*, state, 4);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        CSF_AT(uint8_t, state, 0) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, serializer, 0)[64 / 4])(
                    serializer, nullptr, record + 92, state) == -1;
    }

    if (HudStringShow_BeginSerializationStep(state))
    {
        void* serializer = CSF_AT(void*, state, 4);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        CSF_AT(uint8_t, state, 0) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, serializer, 0)[64 / 4])(
                    serializer, nullptr, record + 100, state) == -1;
    }

    if (HudStringShow_BeginSerializationStep(state))
    {
        void* serializer = CSF_AT(void*, state, 4);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        CSF_AT(uint8_t, state, 0) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, serializer, 0)[60 / 4])(
                    serializer, nullptr, record + 76, state) == -1;
    }

    if (HudStringShow_BeginSerializationStep(state))
    {
        void* serializer = CSF_AT(void*, state, 4);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        CSF_AT(uint8_t, state, 0) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, serializer, 0)[64 / 4])(
                    serializer, nullptr, record + 108, state) == -1;
    }

    if (HudStringShow_BeginSerializationStep(state))
    {
        void* serializer = CSF_AT(void*, state, 4);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        CSF_AT(uint8_t, state, 0) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, serializer, 0)[68 / 4])(
                    serializer, nullptr, record + 116, state) == -1;
    }

    CSF_AT(int32_t, state, 0x1C) = savedContext;
    if (CSF_AT(uint8_t, state, 0))
    {
        CSF_AT(uint8_t, state, 0) = 1;
        return 1;
    }

    void* serializer = CSF_AT(void*, state, 4);
    const int result =
        ((int(__fastcall*)(void*, void*, void*))
            CSF_AT(void**, serializer, 0)[124 / 4])(
                serializer, nullptr, state);

    if (!static_cast<uint8_t>(result))
    {
        CSF_AT(uint8_t, state, 0) = 1;
        return 1;
    }

    CSF_AT(uint8_t, state, 0) = 0;
    return result;
}

HOOK(0x004F2C90, HudStringShow_SerializeRecord, "auto");
