#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __stdcall, Game_ProcessFallbackStage, 0x0040B980,
         (uint32_t* a1, int a2));

static int __fastcall Game_ProcessContextStages(uint32_t* self, void* /*edx*/, int a2)
{
    void* context = reinterpret_cast<void*>(a2);
    bool handled = false;

    if (CSF_AT(uint8_t, context, 0) == 0)
    {
        void* processor = CSF_AT(void*, context, 0x04);
        handled = !((bool(__fastcall*)(void*, void*, int, int))
            CSF_AT(void**, processor, 0)[0x80 / 4])(
                processor, nullptr, 0, a2);
    }

    const uint32_t savedState = CSF_AT(uint32_t, context, 0x1C);
    CSF_AT(uint8_t, context, 0) = handled;
    CSF_AT(uint32_t, context, 0x1C) = 0;

    if (!handled)
    {
        void* processor = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            ((int(__fastcall*)(void*, void*, uint32_t*, int))
                CSF_AT(void**, processor, 0)[0x54 / 4])(
                    processor, nullptr, self, a2) == -1;
    }

    bool needsProcessing = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (needsProcessing)
    {
        void* processor = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            ((int(__fastcall*)(void*, void*, uint32_t*, int))
                CSF_AT(void**, processor, 0)[0x54 / 4])(
                    processor, nullptr, self + 1, a2) == -1;
    }

    needsProcessing = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (needsProcessing)
    {
        void* processor = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            ((int(__fastcall*)(void*, void*, uint32_t*, int))
                CSF_AT(void**, processor, 0)[0x54 / 4])(
                    processor, nullptr, self + 2, a2) == -1;
    }

    needsProcessing = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (needsProcessing)
    {
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        Game_ProcessFallbackStage(self + 3, a2);
    }

    CSF_AT(uint32_t, context, 0x1C) = savedState;

    if (CSF_AT(uint8_t, context, 0) == 0)
    {
        void* processor = CSF_AT(void*, context, 0x04);
        const int result =
            ((int(__fastcall*)(void*, void*, int))
                CSF_AT(void**, processor, 0)[0x7C / 4])(
                    processor, nullptr, a2);

        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, context, 0) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, context, 0) = 1;
    return 1;
}

HOOK(0x0040C110, Game_ProcessContextStages, "auto");
