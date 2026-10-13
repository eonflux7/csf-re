#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int archiveAddress, S_00691860* string));

static int __fastcall Archive_SerializeRecord(void* self, void* /*edx*/,
                                              S_00691860* context)
{
    const uint32_t savedContext = context[1].field_0004;
    bool failed = false;

    if (!context->pad_0000[0])
    {
        void* serializer = reinterpret_cast<void*>(context->field_0004);
        failed = !reinterpret_cast<bool(__fastcall*)(void*, void*, int, S_00691860*)>(
            CSF_AT(void**, serializer, 0)[0x80 / 4])(
                serializer, nullptr, 0, context);
    }

    context->pad_0000[0] = failed;
    context[1].field_0004 = 0;

    if (!failed)
    {
        void* serializer = reinterpret_cast<void*>(context->field_0004);
        reinterpret_cast<uint8_t*>(&context->field_0014)[1] = 0;
        reinterpret_cast<uint8_t*>(&context->field_0014)[2] = 0;
        context->pad_0000[0] =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, S_00691860*)>(
                CSF_AT(void**, serializer, 0)[0x58 / 4])(
                    serializer, nullptr, self, context) == -1;
    }

    const bool serializeName = context->pad_0000[0] == 0;
    context[1].field_0004 = 0;
    if (serializeName)
    {
        reinterpret_cast<uint8_t*>(&context->field_0014)[1] = 0;
        reinterpret_cast<uint8_t*>(&context->field_0014)[2] = 0;
        Archive_SerializeString(
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 4),
            context);
    }

    const bool serializeValue = context->pad_0000[0] == 0;
    context[1].field_0004 = 0;
    if (serializeValue)
    {
        void* serializer = reinterpret_cast<void*>(context->field_0004);
        reinterpret_cast<uint8_t*>(&context->field_0014)[1] = 0;
        reinterpret_cast<uint8_t*>(&context->field_0014)[2] = 0;
        context->pad_0000[0] =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, S_00691860*)>(
                CSF_AT(void**, serializer, 0)[0x38 / 4])(
                    serializer, nullptr,
                    reinterpret_cast<uint8_t*>(self) + 0x10, context) == -1;
    }

    context[1].field_0004 = savedContext;
    if (!context->pad_0000[0])
    {
        void* serializer = reinterpret_cast<void*>(context->field_0004);
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, S_00691860*)>(
                CSF_AT(void**, serializer, 0)[0x7C / 4])(
                    serializer, nullptr, context);
        if (static_cast<uint8_t>(result) != 0)
        {
            context->pad_0000[0] = 0;
            return result;
        }
    }

    context->pad_0000[0] = 1;
    return 1;
}

HOOK(0x004DCE80, Archive_SerializeRecord, "auto");
