#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetValue_8B0574, 0x00518980, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_SceneLoadMillisecondsToSeconds, 0x0082D4F0);
CSF_GLOBAL(float, g_flt_0082DC84, 0x0082DC84);

static int __fastcall GameBddLoader_UpdateFrameTime(void* self, void* /*edx*/)
{
    GetValue_8B0574();

    void* timer = CSF_AT(void*, self, 0x90);
    double elapsedSeconds =
        reinterpret_cast<double(__fastcall*)(void*, void*)>(
            CSF_AT(void**, timer, 0)[5])(timer, nullptr) *
        g_SceneLoadMillisecondsToSeconds;

    const uint8_t allowLongFrame = CSF_AT(uint8_t, self, 0xD0);
    CSF_AT(float, self, 0xA0) = static_cast<float>(elapsedSeconds);
    if (!allowLongFrame && elapsedSeconds > g_flt_0082DC84)
        CSF_AT(float, self, 0xA0) = 0.1f;

    if (CSF_AT(uint8_t, self, 0x89))
    {
        CSF_AT(uint32_t, self, 0x9C) = 0;
    }
    else if (CSF_AT(uint8_t, self, 0x94))
    {
        const uint8_t singleStepMode = CSF_AT(uint8_t, self, 0xA8);
        CSF_AT(uint32_t, self, 0x9C) = CSF_AT(uint32_t, self, 0x98);
        if (singleStepMode)
        {
            if (!CSF_AT(uint8_t, self, 0xA9))
                CSF_AT(uint32_t, self, 0x9C) = 0;
            else
                CSF_AT(uint8_t, self, 0xA9) = 0;
        }
    }
    else
    {
        CSF_AT(uint32_t, self, 0x9C) = CSF_AT(uint32_t, self, 0xA0);
    }

    CSF_AT(uint32_t, self, 0xA4) = CSF_AT(uint32_t, self, 0x9C);

    int result = GetGlobalValue_521BB0();
    if (CSF_AT(uint8_t, reinterpret_cast<void*>(result), 0x78))
    {
        result = GetGlobalValue_8BC1E0();
        CSF_AT(float, self, 0xA4) =
            CSF_AT(float, reinterpret_cast<void*>(result), 0x18A0) *
            CSF_AT(float, self, 0xA4);
    }
    return result;
}

HOOK(0x0050ED40, GameBddLoader_UpdateFrameTime, "auto");
