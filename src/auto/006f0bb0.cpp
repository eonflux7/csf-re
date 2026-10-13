#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ProcessContextStageHandlers, 0x006CA6F0, (void* self, void* , int contextAddress));

[[maybe_unused]] static const auto kContextStageHandlersVtable =
    CSF_VTBL(0x006CA6F0);

static int __fastcall S_006ca6f0_UpdateContextState(
    S_006ca6f0* self, void* /*edx*/, int contextAddress)
{
    auto* context = reinterpret_cast<uint8_t*>(contextAddress);

    bool inactive = false;
    if (CSF_AT(uint8_t, context, 0) == 0)
    {
        void* handler = CSF_AT(void*, context, 0x04);
        inactive = !reinterpret_cast<bool(__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, handler, 0)[0x80 / 4])(
                handler, nullptr, 0, contextAddress);
    }

    const uint32_t savedState = CSF_AT(uint32_t, context, 0x1C);
    CSF_AT(uint8_t, context, 0) = inactive;
    ProcessContextStageHandlers(self, nullptr, contextAddress);

    const bool needsProcessing = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (needsProcessing)
    {
        void* handler = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;

        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint8_t*, int)>(
                CSF_AT(void**, handler, 0)[0x38 / 4])(
                    handler, nullptr,
                    reinterpret_cast<uint8_t*>(self) + 0x20,
                    contextAddress);
        CSF_AT(uint8_t, context, 0) = result == -1;
    }

    if (CSF_AT(uint8_t, context, 0x14) == 0)
    {
        void* owner = reinterpret_cast<uint8_t*>(self) - 8;
        reinterpret_cast<void(__fastcall*)(void*, void*, uint8_t*)>(
            CSF_AT(void**, owner, 0)[0x38 / 4])(
                owner, nullptr, reinterpret_cast<uint8_t*>(self) + 0x20);
    }

    CSF_AT(uint32_t, context, 0x1C) = savedState;

    if (CSF_AT(uint8_t, context, 0) == 0)
    {
        void* handler = CSF_AT(void*, context, 0x04);
        if (reinterpret_cast<bool(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, handler, 0)[0x7C / 4])(
                    handler, nullptr, contextAddress))
        {
            CSF_AT(uint8_t, context, 0) = 0;
            return 0;
        }
    }

    CSF_AT(uint8_t, context, 0) = 1;
    return 1;
}

HOOK(0x006F0BB0, S_006ca6f0_UpdateContextState, "auto");
