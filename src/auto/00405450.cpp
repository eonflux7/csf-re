#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __stdcall AudioSoundManager_ProcessArchiveContext(void* context)
{
    bool pending = false;
    if (CSF_AT(uint8_t, context, 0x0) == 0)
    {
        void* archive = CSF_AT(void*, context, 0x4);
        pending = !reinterpret_cast<bool(__fastcall*)(void*, void*, int, void*)>(
            CSF_AT(void**, archive, 0x0)[0x80 / 4])(
                archive, nullptr, 0, context);
    }

    CSF_AT(uint8_t, context, 0x0) = pending;
    if (pending)
    {
        CSF_AT(uint8_t, context, 0x0) = 1;
        return 1;
    }

    void* archive = CSF_AT(void*, context, 0x4);
    if (!reinterpret_cast<bool(__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, archive, 0x0)[0x7C / 4])(
                archive, nullptr, context))
    {
        CSF_AT(uint8_t, context, 0x0) = 1;
        return 1;
    }

    CSF_AT(uint8_t, context, 0x0) = 0;
    return 0;
}

HOOK(0x00405450, AudioSoundManager_ProcessArchiveContext, "auto");
