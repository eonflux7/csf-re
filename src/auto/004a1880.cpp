#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, HudAlertIcons_ProcessAdditionalFields, 0x004A16C0,
         (void* self, void* edx, int a2));

static int __fastcall C_00834400_ProcessFields(
    C_00834400* self, void* /*edx*/, int contextAddress)
{
    void* context = reinterpret_cast<void*>(contextAddress);

    bool stopped = false;
    if (CSF_AT(uint8_t, context, 0x00) == 0)
    {
        void* visitor = CSF_AT(void*, context, 0x04);
        stopped = !((bool(__fastcall*)(void*, void*, int, int))
            CSF_AT(void**, visitor, 0)[0x80 / 4])(
                visitor, nullptr, 0, contextAddress);
    }

    const uint32_t savedContext = CSF_AT(uint32_t, context, 0x1C);
    CSF_AT(uint8_t, context, 0x00) = stopped;
    CSF_AT(uint32_t, context, 0x1C) = 0;

    if (!stopped)
    {
        void* visitor = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, uint32_t*, int))
                CSF_AT(void**, visitor, 0)[0x1C / 4])(
                    visitor, nullptr, &self->field_0004, contextAddress) == -1;
    }

    bool continueProcessing = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (continueProcessing)
    {
        void* visitor = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, uint32_t*, int))
                CSF_AT(void**, visitor, 0)[0x54 / 4])(
                    visitor, nullptr, &self->field_0014, contextAddress) == -1;
    }

    continueProcessing = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (continueProcessing)
    {
        void* visitor = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, uint32_t*, int))
                CSF_AT(void**, visitor, 0)[0x54 / 4])(
                    visitor, nullptr, &self->field_0018, contextAddress) == -1;
    }

    continueProcessing = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (continueProcessing)
    {
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        HudAlertIcons_ProcessAdditionalFields(
            &self->field_001c, nullptr, contextAddress);
    }

    continueProcessing = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = savedContext;
    if (continueProcessing)
    {
        void* visitor = CSF_AT(void*, context, 0x04);
        const int completed =
            ((int(__fastcall*)(void*, void*, int))
                CSF_AT(void**, visitor, 0)[0x7C / 4])(
                    visitor, nullptr, contextAddress);
        if (static_cast<uint8_t>(completed) != 0)
        {
            CSF_AT(uint8_t, context, 0x00) = 0;
            return completed;
        }
    }

    CSF_AT(uint8_t, context, 0x00) = 1;
    return 1;
}

HOOK(0x004A1880, C_00834400_ProcessFields, "auto");
