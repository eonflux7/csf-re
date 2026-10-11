#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));

template <unsigned Slot>
static void Archive_SerializeMember(void* member, void* context)
{
    const bool canSerialize = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;

    if (canSerialize)
    {
        void* archive = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;

        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, archive, 0)[Slot])(
                    archive, nullptr, member, context);
        CSF_AT(uint8_t, context, 0) = result == -1;
    }
}

static int __fastcall Archive_SerializeRecord(void* self, void* /*edx*/, void* context)
{
    bool failed = false;
    if (CSF_AT(uint8_t, context, 0) == 0)
    {
        void* archive = CSF_AT(void*, context, 0x04);
        failed =
            !reinterpret_cast<bool(__fastcall*)(void*, void*, int, void*)>(
                CSF_AT(void**, archive, 0)[0x80 / 4])(
                    archive, nullptr, 0, context);
    }

    const uint32_t savedState = CSF_AT(uint32_t, context, 0x1C);
    CSF_AT(uint8_t, context, 0) = failed;
    CSF_AT(uint32_t, context, 0x1C) = 0;

    auto* record = static_cast<uint8_t*>(self);
    if (!failed)
    {
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        Archive_SerializeObjectRef(record + 0x04, static_cast<bool*>(context));
    }

    Archive_SerializeMember<0x54 / 4>(record + 0x08, context);
    Archive_SerializeMember<0x54 / 4>(record + 0x0C, context);
    Archive_SerializeMember<0x54 / 4>(record + 0x10, context);
    Archive_SerializeMember<0x54 / 4>(record + 0x14, context);
    Archive_SerializeMember<0x44 / 4>(record + 0x18, context);
    Archive_SerializeMember<0x54 / 4>(record + 0x1C, context);
    Archive_SerializeMember<0x44 / 4>(record + 0x20, context);
    Archive_SerializeMember<0x38 / 4>(record + 0x24, context);
    Archive_SerializeMember<0x38 / 4>(record + 0x30, context);
    Archive_SerializeMember<0x38 / 4>(record + 0x3C, context);
    Archive_SerializeMember<0x38 / 4>(record + 0x54, context);
    Archive_SerializeMember<0x38 / 4>(record + 0x48, context);

    const bool canFinish = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = savedState;

    if (canFinish)
    {
        void* archive = CSF_AT(void*, context, 0x04);
        const bool result =
            reinterpret_cast<bool(__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, archive, 0)[0x7C / 4])(
                    archive, nullptr, context);
        if (result)
        {
            CSF_AT(uint8_t, context, 0) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, context, 0) = 1;
    return 1;
}

HOOK(0x00409C40, Archive_SerializeRecord, "auto");
