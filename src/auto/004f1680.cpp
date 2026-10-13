#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int archiveAddress, S_00691860* string));

static int __fastcall SceneLightsObjects_SerializeRecord(void* self, void* /*edx*/, S_00691860* context)
{
    auto* record = static_cast<uint8_t*>(self);
    const uint32_t savedState = CSF_AT(uint32_t, context, 0x1C);

    bool failed = false;
    if (CSF_AT(uint8_t, context, 0) == 0)
    {
        void* archive = reinterpret_cast<void*>(context->field_0004);
        failed = !reinterpret_cast<bool(__fastcall*)(void*, void*, int, S_00691860*)>(
            CSF_AT(void**, archive, 0)[0x80 / 4])(archive, nullptr, 0, context);
    }
    CSF_AT(uint8_t, context, 0) = failed;
    CSF_AT(uint32_t, context, 0x1C) = 0;

    if (!failed)
    {
        void* archive = reinterpret_cast<void*>(context->field_0004);
        reinterpret_cast<uint8_t*>(&context->field_0014)[1] = 0;
        reinterpret_cast<uint8_t*>(&context->field_0014)[2] = 0;
        CSF_AT(uint8_t, context, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, S_00691860*)>(
                CSF_AT(void**, archive, 0)[0x58 / 4])(archive, nullptr, self, context) == -1;
    }

    failed = CSF_AT(uint8_t, context, 0) != 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (!failed)
    {
        void* archive = reinterpret_cast<void*>(context->field_0004);
        reinterpret_cast<uint8_t*>(&context->field_0014)[1] = 0;
        reinterpret_cast<uint8_t*>(&context->field_0014)[2] = 0;
        CSF_AT(uint8_t, context, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, S_00691860*)>(
                CSF_AT(void**, archive, 0)[0x58 / 4])(archive, nullptr, record + 4, context) == -1;
    }

    failed = CSF_AT(uint8_t, context, 0) != 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (!failed)
    {
        reinterpret_cast<uint8_t*>(&context->field_0014)[1] = 0;
        reinterpret_cast<uint8_t*>(&context->field_0014)[2] = 0;
        Archive_SerializeString(reinterpret_cast<int>(record + 8), context);
    }

    failed = CSF_AT(uint8_t, context, 0) != 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (!failed)
    {
        void* archive = reinterpret_cast<void*>(context->field_0004);
        reinterpret_cast<uint8_t*>(&context->field_0014)[1] = 0;
        reinterpret_cast<uint8_t*>(&context->field_0014)[2] = 0;
        CSF_AT(uint8_t, context, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, S_00691860*)>(
                CSF_AT(void**, archive, 0)[0x38 / 4])(archive, nullptr, record + 20, context) == -1;
    }

    failed = CSF_AT(uint8_t, context, 0) != 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (!failed)
    {
        void* archive = reinterpret_cast<void*>(context->field_0004);
        reinterpret_cast<uint8_t*>(&context->field_0014)[1] = 0;
        reinterpret_cast<uint8_t*>(&context->field_0014)[2] = 0;
        CSF_AT(uint8_t, context, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, S_00691860*)>(
                CSF_AT(void**, archive, 0)[0x54 / 4])(archive, nullptr, record + 32, context) == -1;
    }

    failed = CSF_AT(uint8_t, context, 0) != 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (!failed)
    {
        void* archive = reinterpret_cast<void*>(context->field_0004);
        reinterpret_cast<uint8_t*>(&context->field_0014)[1] = 0;
        reinterpret_cast<uint8_t*>(&context->field_0014)[2] = 0;
        CSF_AT(uint8_t, context, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, S_00691860*)>(
                CSF_AT(void**, archive, 0)[0x54 / 4])(archive, nullptr, record + 36, context) == -1;
    }

    CSF_AT(uint32_t, context, 0x1C) = savedState;
    if (CSF_AT(uint8_t, context, 0) != 0)
    {
        CSF_AT(uint8_t, context, 0) = 1;
        return 1;
    }

    void* archive = reinterpret_cast<void*>(context->field_0004);
    const int result = reinterpret_cast<int(__fastcall*)(void*, void*, S_00691860*)>(
        CSF_AT(void**, archive, 0)[0x7C / 4])(archive, nullptr, context);
    if (static_cast<uint8_t>(result) == 0)
    {
        CSF_AT(uint8_t, context, 0) = 1;
        return 1;
    }

    CSF_AT(uint8_t, context, 0) = 0;
    return result;
}

HOOK(0x004F1680, SceneLightsObjects_SerializeRecord, "auto");
