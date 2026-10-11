#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CoverAttack_ProcessRecord, 0x004C2380, (void* self, void* edx, int a2));

static int __fastcall AiStateFactory_ProcessStateTransitionContext(
    void* self, void* /*edx*/, void* context)
{
    const uint32_t savedContextValue = CSF_AT(uint32_t, context, 0x1C);

    bool failed = false;
    if (CSF_AT(uint8_t, context, 0x00) == 0)
    {
        void* archive = CSF_AT(void*, context, 0x04);
        failed = !((bool(__fastcall*)(void*, void*, int, void*))
            CSF_AT(void**, archive, 0x00)[0x80 / 4])(
                archive, nullptr, 0, context);
    }
    CSF_AT(uint8_t, context, 0x00) = failed;
    CoverAttack_ProcessRecord(self, nullptr, reinterpret_cast<int>(context));

    bool processField = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (processField)
    {
        void* archive = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        const int result = ((int(__fastcall*)(void*, void*, void*, void*))
            CSF_AT(void**, archive, 0x00)[0x54 / 4])(
                archive, nullptr, static_cast<uint8_t*>(self) + 0x14, context);
        CSF_AT(uint8_t, context, 0x00) = result == -1;
    }

    processField = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (processField)
    {
        void* archive = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        const int result = ((int(__fastcall*)(void*, void*, void*, void*))
            CSF_AT(void**, archive, 0x00)[0x54 / 4])(
                archive, nullptr, static_cast<uint8_t*>(self) + 0x18, context);
        CSF_AT(uint8_t, context, 0x00) = result == -1;
    }

    processField = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (processField)
    {
        void* archive = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        const int result = ((int(__fastcall*)(void*, void*, void*, void*))
            CSF_AT(void**, archive, 0x00)[0x54 / 4])(
                archive, nullptr, static_cast<uint8_t*>(self) + 0x1C, context);
        CSF_AT(uint8_t, context, 0x00) = result == -1;
    }

    processField = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (processField)
    {
        void* archive = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        const int result = ((int(__fastcall*)(void*, void*, void*, void*))
            CSF_AT(void**, archive, 0x00)[0x54 / 4])(
                archive, nullptr, static_cast<uint8_t*>(self) + 0x20, context);
        CSF_AT(uint8_t, context, 0x00) = result == -1;
    }

    processField = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = savedContextValue;
    if (processField)
    {
        void* archive = CSF_AT(void*, context, 0x04);
        const int result = ((int(__fastcall*)(void*, void*, void*))
            CSF_AT(void**, archive, 0x00)[0x7C / 4])(
                archive, nullptr, context);
        if ((result & 0xFF) != 0)
        {
            CSF_AT(uint8_t, context, 0x00) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, context, 0x00) = 1;
    return 1;
}

HOOK(0x004C26D0, AiStateFactory_ProcessStateTransitionContext, "auto");
