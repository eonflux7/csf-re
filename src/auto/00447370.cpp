#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int archiveAddress, S_00691860* string));

static int __fastcall Archive_SerializeNamedRecord(
    void* self, void* /*edx*/, S_00691860* context)
{
    const uint32_t savedState = context[1].field_0004;
    auto* record = static_cast<uint8_t*>(self);

    bool failed = CSF_AT(uint8_t, context, 0) == 0;
    if (failed)
    {
        void* serializer = reinterpret_cast<void*>(context->field_0004);
        failed = !((bool(__fastcall*)(void*, void*, int, S_00691860*))
            CSF_AT(void**, serializer, 0)[128 / 4])(
                serializer, nullptr, 0, context);
    }

    CSF_AT(uint8_t, context, 0) = failed;
    context[1].field_0004 = 0;
    if (!failed)
    {
        void* serializer = reinterpret_cast<void*>(context->field_0004);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            ((int(__fastcall*)(void*, void*, void*, S_00691860*))
                CSF_AT(void**, serializer, 0)[88 / 4])(
                    serializer, nullptr, self, context) == -1;
    }

    failed = CSF_AT(uint8_t, context, 0) != 0;
    context[1].field_0004 = 0;
    if (!failed)
    {
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        Archive_SerializeString(reinterpret_cast<int>(record + 4), context);
    }

    failed = CSF_AT(uint8_t, context, 0) != 0;
    context[1].field_0004 = 0;
    if (!failed)
    {
        void* serializer = reinterpret_cast<void*>(context->field_0004);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            ((int(__fastcall*)(void*, void*, void*, S_00691860*))
                CSF_AT(void**, serializer, 0)[56 / 4])(
                    serializer, nullptr, record + 16, context) == -1;
    }

    failed = CSF_AT(uint8_t, context, 0) != 0;
    context[1].field_0004 = 0;
    if (!failed)
    {
        void* serializer = reinterpret_cast<void*>(context->field_0004);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            ((int(__fastcall*)(void*, void*, void*, S_00691860*))
                CSF_AT(void**, serializer, 0)[84 / 4])(
                    serializer, nullptr, record + 32, context) == -1;
    }

    failed = CSF_AT(uint8_t, context, 0) != 0;
    context[1].field_0004 = 0;
    if (!failed)
    {
        void* serializer = reinterpret_cast<void*>(context->field_0004);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            ((int(__fastcall*)(void*, void*, void*, S_00691860*))
                CSF_AT(void**, serializer, 0)[84 / 4])(
                    serializer, nullptr, record + 28, context) == -1;
    }

    context[1].field_0004 = savedState;
    if (CSF_AT(uint8_t, context, 0) == 0)
    {
        void* serializer = reinterpret_cast<void*>(context->field_0004);
        const int result =
            ((int(__fastcall*)(void*, void*, S_00691860*))
                CSF_AT(void**, serializer, 0)[124 / 4])(
                    serializer, nullptr, context);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, context, 0) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, context, 0) = 1;
    return 1;
}

HOOK(0x00447370, Archive_SerializeNamedRecord, "auto");
