#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall C_0083380c_SerializeFields(
    C_0083380c* self, void* /*edx*/, void* context)
{
    bool failed = false;
    if (CSF_AT(uint8_t, context, 0) == 0)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        failed = !reinterpret_cast<bool (__fastcall*)(void*, void*, int, void*)>(
            CSF_AT(void**, serializer, 0)[0x80 / 4])(
                serializer, nullptr, 0, context);
    }

    const uint32_t savedState = CSF_AT(uint32_t, context, 0x1C);
    CSF_AT(uint8_t, context, 0) = failed;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (!failed)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            reinterpret_cast<int (__fastcall*)(void*, void*, uint32_t*, void*)>(
                CSF_AT(void**, serializer, 0)[0x54 / 4])(
                    serializer, nullptr, &self->field_0004, context) == -1;
    }

    failed = CSF_AT(uint8_t, context, 0) != 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (!failed)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            reinterpret_cast<int (__fastcall*)(void*, void*, uint32_t*, void*)>(
                CSF_AT(void**, serializer, 0)[0x54 / 4])(
                    serializer, nullptr, &self->field_0008, context) == -1;
    }

    failed = CSF_AT(uint8_t, context, 0) != 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (!failed)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            reinterpret_cast<int (__fastcall*)(void*, void*, uint32_t*, void*)>(
                CSF_AT(void**, serializer, 0)[0x54 / 4])(
                    serializer, nullptr, &self->field_000c, context) == -1;
    }

    failed = CSF_AT(uint8_t, context, 0) != 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (!failed)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            reinterpret_cast<int (__fastcall*)(void*, void*, uint32_t*, void*)>(
                CSF_AT(void**, serializer, 0)[0x54 / 4])(
                    serializer, nullptr, &self->field_0010, context) == -1;
    }

    failed = CSF_AT(uint8_t, context, 0) != 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (!failed)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            reinterpret_cast<int (__fastcall*)(void*, void*, uint32_t*, void*)>(
                CSF_AT(void**, serializer, 0)[0x48 / 4])(
                    serializer, nullptr, &self->field_0014, context) == -1;
    }

    failed = CSF_AT(uint8_t, context, 0) != 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (!failed)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            reinterpret_cast<int (__fastcall*)(void*, void*, uint32_t*, void*)>(
                CSF_AT(void**, serializer, 0)[0x48 / 4])(
                    serializer, nullptr, &self->field_0018, context) == -1;
    }

    failed = CSF_AT(uint8_t, context, 0) != 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (!failed)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            reinterpret_cast<int (__fastcall*)(void*, void*, uint32_t*, void*)>(
                CSF_AT(void**, serializer, 0)[0x48 / 4])(
                    serializer, nullptr, &self->field_001c, context) == -1;
    }

    CSF_AT(uint32_t, context, 0x1C) = savedState;
    if (CSF_AT(uint8_t, context, 0) != 0)
    {
        CSF_AT(uint8_t, context, 0) = 1;
        return 1;
    }

    void* serializer = CSF_AT(void*, context, 0x04);
    const int result =
        reinterpret_cast<int (__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, serializer, 0)[0x7C / 4])(
                serializer, nullptr, context);

    if (static_cast<uint8_t>(result) == 0)
    {
        CSF_AT(uint8_t, context, 0) = 1;
        return 1;
    }

    CSF_AT(uint8_t, context, 0) = 0;
    return result;
}

HOOK(0x0048C850, C_0083380c_SerializeFields, "auto");
