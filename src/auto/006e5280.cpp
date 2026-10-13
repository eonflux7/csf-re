#include <cstdint>
#include "globals.h"
#include "hook.h"

static int __fastcall GfxObjectInstances_ProcessScalarFields(
    void* self, void* /*edx*/, void* context)
{
    bool failed = CSF_AT(uint8_t, context, 0x00) == 0;
    if (failed)
    {
        void* processor = CSF_AT(void*, context, 0x04);
        failed = !((bool(__fastcall*)(void*, void*, int, void*))
            CSF_AT(void**, processor, 0)[0x80 / 4])(
                processor, nullptr, 0, context);
    }

    uint32_t savedState = CSF_AT(uint32_t, context, 0x1C);
    CSF_AT(uint8_t, context, 0x00) = failed;
    CSF_AT(uint32_t, context, 0x1C) = 0;

    if (!failed)
    {
        void* processor = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, processor, 0)[0x54 / 4])(
                    processor, nullptr,
                    static_cast<uint8_t*>(self) + 0x08, context) == -1;
    }

    failed = CSF_AT(uint8_t, context, 0x00) != 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (!failed)
    {
        void* processor = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, processor, 0)[0x54 / 4])(
                    processor, nullptr,
                    static_cast<uint8_t*>(self) + 0x0C, context) == -1;
    }

    void* scalarObject = static_cast<uint8_t*>(self) - 4;
    float scalar = static_cast<float>(
        ((double(__fastcall*)(void*, void*))
            CSF_AT(void**, scalarObject, 0)[0x08 / 4])(
                scalarObject, nullptr));

    failed = CSF_AT(uint8_t, context, 0x00) != 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (!failed)
    {
        void* processor = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, processor, 0)[0x54 / 4])(
                    processor, nullptr, &scalar, context) == -1;
    }

    if (CSF_AT(uint8_t, context, 0x14) == 0 &&
        CSF_AT(uint8_t, context, 0x00) == 0)
    {
        ((void(__fastcall*)(void*, void*, float))
            CSF_AT(void**, scalarObject, 0)[0x04 / 4])(
                scalarObject, nullptr, scalar);
    }

    failed = CSF_AT(uint8_t, context, 0x00) != 0;
    CSF_AT(uint32_t, context, 0x1C) = savedState;
    if (!failed)
    {
        void* processor = CSF_AT(void*, context, 0x04);
        int result = ((int(__fastcall*)(void*, void*, void*))
            CSF_AT(void**, processor, 0)[0x7C / 4])(
                processor, nullptr, context);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, context, 0x00) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, context, 0x00) = 1;
    return 1;
}

HOOK(0x006E5280, GfxObjectInstances_ProcessScalarFields, "auto");
