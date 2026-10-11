#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, SceneLoader_SerializeRecordMember, 0x00445620, (void* a1, int a2));
CSF_FUNC(int, __stdcall, SceneLoader_SerializeMemberGroup, 0x004E67B0, (int* a1, int a2));

static int __fastcall S_004e6b70_SerializeRecord(
    S_004e6b70* self, void* /*edx*/, int contextAddress)
{
    auto* context = reinterpret_cast<uint8_t*>(contextAddress);
    auto* record = reinterpret_cast<uint8_t*>(self);

    bool failed = false;
    if (CSF_AT(uint8_t, context, 0x00) == 0)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        failed = !reinterpret_cast<bool(__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, serializer, 0x00)[0x80 / 4])(
                serializer, nullptr, 0, contextAddress);
    }

    int savedMemberState = CSF_AT(int, context, 0x1C);
    CSF_AT(uint8_t, context, 0x00) = failed;
    CSF_AT(int, context, 0x1C) = 0;
    if (!failed)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, int)>(
                CSF_AT(void**, serializer, 0x00)[0x44 / 4])(
                    serializer, nullptr, record + 0x48, contextAddress) == -1;
    }

    bool active = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(int, context, 0x1C) = 0;
    if (active)
    {
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        SceneLoader_SerializeRecordMember(record + 0x08, contextAddress);
    }

    active = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(int, context, 0x1C) = 0;
    if (active)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, int)>(
                CSF_AT(void**, serializer, 0x00)[0x58 / 4])(
                    serializer, nullptr, record + 0x2C, contextAddress) == -1;
    }

    active = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(int, context, 0x1C) = 0;
    if (active)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, int)>(
                CSF_AT(void**, serializer, 0x00)[0x44 / 4])(
                    serializer, nullptr, record + 0x49, contextAddress) == -1;
    }

    active = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(int, context, 0x1C) = 0;
    if (active)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, int)>(
                CSF_AT(void**, serializer, 0x00)[0x54 / 4])(
                    serializer, nullptr, record + 0x30, contextAddress) == -1;
    }

    active = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(int, context, 0x1C) = 0;
    if (active)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, int)>(
                CSF_AT(void**, serializer, 0x00)[0x54 / 4])(
                    serializer, nullptr, record + 0x34, contextAddress) == -1;
    }

    active = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(int, context, 0x1C) = 0;
    if (active)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, int)>(
                CSF_AT(void**, serializer, 0x00)[0x54 / 4])(
                    serializer, nullptr, record + 0x38, contextAddress) == -1;
    }

    active = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(int, context, 0x1C) = 0;
    if (active)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, int)>(
                CSF_AT(void**, serializer, 0x00)[0x54 / 4])(
                    serializer, nullptr, record + 0x3C, contextAddress) == -1;
    }

    active = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(int, context, 0x1C) = 0;
    if (active)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, int)>(
                CSF_AT(void**, serializer, 0x00)[0x48 / 4])(
                    serializer, nullptr, record + 0x40, contextAddress) == -1;
    }

    active = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(int, context, 0x1C) = 0;
    if (active)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, int)>(
                CSF_AT(void**, serializer, 0x00)[0x54 / 4])(
                    serializer, nullptr, record + 0x44, contextAddress) == -1;
    }

    active = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(int, context, 0x1C) = 0;
    if (active)
    {
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        SceneLoader_SerializeMemberGroup(
            reinterpret_cast<int*>(record + 0x1C), contextAddress);
    }

    active = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(int, context, 0x1C) = savedMemberState;
    if (active)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        int result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, serializer, 0x00)[0x7C / 4])(
                serializer, nullptr, contextAddress);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, context, 0x00) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, context, 0x00) = 1;
    return 1;
}

HOOK(0x004E6B70, S_004e6b70_SerializeRecord, "auto");
