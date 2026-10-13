#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall C_00860908_Serialize(C_00860908* self, void* /*edx*/, void* context)
{
    bool failed = false;
    if (CSF_AT(uint8_t, context, 0x00) == 0)
    {
        void* handler = CSF_AT(void*, context, 0x04);
        failed = !reinterpret_cast<bool(__fastcall*)(void*, void*, int, void*)>(
            CSF_AT(void**, handler, 0)[0x80 / 4])(handler, nullptr, 0, context);
    }

    const uint32_t savedState = CSF_AT(uint32_t, context, 0x1C);
    CSF_AT(uint8_t, context, 0x00) = failed;
    CSF_AT(uint32_t, context, 0x1C) = 0;

    if (!failed)
    {
        void* handler = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, void*)>(
                CSF_AT(void**, handler, 0)[0x54 / 4])(
                handler, nullptr, &self->field_0024, context) == -1;
    }

    failed = CSF_AT(uint8_t, context, 0x00) != 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (!failed)
    {
        void* handler = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, void*)>(
                CSF_AT(void**, handler, 0)[0x54 / 4])(
                handler, nullptr, &self->field_0028, context) == -1;
    }

    failed = CSF_AT(uint8_t, context, 0x00) != 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (!failed)
    {
        void* handler = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint8_t*, void*)>(
                CSF_AT(void**, handler, 0)[0x44 / 4])(
                handler, nullptr, &self->field_002c, context) == -1;
    }

    failed = CSF_AT(uint8_t, context, 0x00) != 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (!failed)
    {
        void* handler = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint8_t*, void*)>(
                CSF_AT(void**, handler, 0)[0x44 / 4])(
                handler, nullptr, &CSF_AT(uint8_t, self, 0x2D), context) == -1;
    }

    CSF_AT(uint32_t, context, 0x1C) = savedState;
    if (CSF_AT(uint8_t, context, 0x00) == 0)
    {
        void* handler = CSF_AT(void*, context, 0x04);
        // The low byte controls the branch, but the full result is forwarded.
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, handler, 0)[0x7C / 4])(handler, nullptr, context);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, context, 0x00) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, context, 0x00) = 1;
    return 1;
}

HOOK(0x006C88F0, C_00860908_Serialize, "auto");
