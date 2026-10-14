#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, AudioSoundManager_ProcessArchiveContext, 0x00405450, (void* context));

static int __fastcall AiStateFactory_ProcessArchiveFields(
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
    AudioSoundManager_ProcessArchiveContext(context);

    bool processField = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (processField)
    {
        void* archive = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        const int result = ((int(__fastcall*)(void*, void*, void*, void*))
            CSF_AT(void**, archive, 0x00)[0x44 / 4])(
                archive, nullptr, static_cast<uint8_t*>(self) + 0x15, context);
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
            CSF_AT(void**, archive, 0x00)[0x38 / 4])(
                archive, nullptr, static_cast<uint8_t*>(self) + 0x04, context);
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
                archive, nullptr, static_cast<uint8_t*>(self) + 0x10, context);
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
            CSF_AT(void**, archive, 0x00)[0x44 / 4])(
                archive, nullptr, static_cast<uint8_t*>(self) + 0x14, context);
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
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, context, 0x00) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, context, 0x00) = 1;
    return 1;
}

HOOK(0x004C1CB0, AiStateFactory_ProcessArchiveFields, "auto");
