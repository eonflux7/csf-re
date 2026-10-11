#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, AiAutonomous_SerializeBaseState, 0x004D19C0,
         (void* self, void* edx, void* a2));

static int __fastcall S_0041f0a0_ProcessContextFields(
    S_0041f0a0* self, void* /*edx*/, int contextAddress)
{
    void* context = reinterpret_cast<void*>(contextAddress);
    bool failed = false;

    if (CSF_AT(uint8_t, context, 0) == 0)
    {
        void* processor = CSF_AT(void*, context, 0x4);
        failed = !reinterpret_cast<bool (__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, processor, 0)[0x80 / 4])(
                processor, nullptr, 0, contextAddress);
    }

    const uint32_t savedState = CSF_AT(uint32_t, context, 0x1C);
    CSF_AT(uint8_t, context, 0) = failed;
    AiAutonomous_SerializeBaseState(self, nullptr, context);

    bool processField = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (processField)
    {
        void* processor = CSF_AT(void*, context, 0x4);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        const int result =
            reinterpret_cast<int (__fastcall*)(void*, void*, void*, int)>(
                CSF_AT(void**, processor, 0)[0x58 / 4])(
                    processor, nullptr,
                    reinterpret_cast<uint8_t*>(self) + 0x3C, contextAddress);
        CSF_AT(uint8_t, context, 0) = result == -1;
    }

    processField = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (processField)
    {
        void* processor = CSF_AT(void*, context, 0x4);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        const int result =
            reinterpret_cast<int (__fastcall*)(void*, void*, void*, int)>(
                CSF_AT(void**, processor, 0)[0x54 / 4])(
                    processor, nullptr,
                    &self->field_0040, contextAddress);
        CSF_AT(uint8_t, context, 0) = result == -1;
    }

    processField = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (processField)
    {
        void* processor = CSF_AT(void*, context, 0x4);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        const int result =
            reinterpret_cast<int (__fastcall*)(void*, void*, void*, int)>(
                CSF_AT(void**, processor, 0)[0x38 / 4])(
                    processor, nullptr,
                    reinterpret_cast<uint8_t*>(self) + 0x44, contextAddress);
        CSF_AT(uint8_t, context, 0) = result == -1;
    }

    CSF_AT(uint32_t, context, 0x1C) = savedState;
    if (CSF_AT(uint8_t, context, 0) == 0)
    {
        void* processor = CSF_AT(void*, context, 0x4);
        const int result =
            reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, processor, 0)[0x7C / 4])(
                    processor, nullptr, contextAddress);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, context, 0) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, context, 0) = 1;
    return 1;
}

HOOK(0x004CF260, S_0041f0a0_ProcessContextFields, "auto");
