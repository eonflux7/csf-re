#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, AiAutonomous_SerializeBaseState, 0x004D19C0,
         (void* self, void* edx, void* a2));

static int __fastcall S_0041f0a0_ProcessFields(
    S_0041f0a0* self, void* /*edx*/, S_0041f0a0* a2)
{
    void* context = a2;
    bool failed = false;

    if (CSF_AT(uint8_t, context, 0) == 0)
    {
        void* processor = CSF_AT(void*, context, 0x4);
        failed = !reinterpret_cast<bool (__fastcall*)(void*, void*, int, void*)>(
            CSF_AT(void**, processor, 0)[0x80 / 4])(
                processor, nullptr, 0, context);
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
            reinterpret_cast<int (__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, processor, 0)[0x48 / 4])(
                    processor, nullptr,
                    reinterpret_cast<uint8_t*>(self) + 0x3C, context);
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
            reinterpret_cast<int (__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, processor, 0)[0x48 / 4])(
                    processor, nullptr,
                    reinterpret_cast<void*>(&self->field_0040), context);
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
            reinterpret_cast<int (__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, processor, 0)[0x48 / 4])(
                    processor, nullptr,
                    reinterpret_cast<uint8_t*>(self) + 0x44, context);
        CSF_AT(uint8_t, context, 0) = result == -1;
    }

    CSF_AT(uint32_t, context, 0x1C) = savedState;
    if (CSF_AT(uint8_t, context, 0) == 0)
    {
        void* processor = CSF_AT(void*, context, 0x4);
        const int result =
            reinterpret_cast<int (__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, processor, 0)[0x7C / 4])(
                    processor, nullptr, context);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, context, 0) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, context, 0) = 1;
    return 1;
}

HOOK(0x004CF410, S_0041f0a0_ProcessFields, "auto");
