#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);
CSF_GLOBAL(float, g_ColorChannelScale, 0x0082FEA8);

[[clang::optnone, clang::disable_tail_calls]]
static void __fastcall HudStringShow_UpdateThirdPanel(
    void* self, void* /*edx*/, float deltaTime)
{
    if (CSF_AT(int32_t, self, 0x48) == -1082130432)
        return;

    if (CSF_AT(uint32_t, self, 0x20) == 0)
    {
        void* controls = MenuControls_ResolveObject(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr);
        void* state = reinterpret_cast<void*>(GetGlobalState_50D190());

        void* resolvedPanel =
            ((void*(__fastcall*)(void*, void*, uint32_t))
                CSF_AT(void**, controls, 0)[0x54 / 4])(
                controls, nullptr, CSF_AT(uint32_t, state, 0x8C));

        CSF_AT(uint32_t, self, 0x20) =
            reinterpret_cast<uint32_t>(resolvedPanel);
    }

    void* panel = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x20));
    void* currentColor =
        ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, panel, 0)[0x64 / 4])(panel, nullptr);

    alignas(8) uint8_t colorBytes[0x10];
    float elapsed = deltaTime + CSF_AT(float, self, 0x48);

    CSF_AT(uint32_t, colorBytes, 0x00) = CSF_AT(uint32_t, currentColor, 0x00);
    CSF_AT(uint32_t, colorBytes, 0x04) = CSF_AT(uint32_t, currentColor, 0x04);
    CSF_AT(uint32_t, colorBytes, 0x08) = CSF_AT(uint32_t, currentColor, 0x08);
    uint32_t alpha = CSF_AT(uint32_t, currentColor, 0x0C);
    CSF_AT(float, self, 0x48) = elapsed;
    CSF_AT(uint32_t, colorBytes, 0x0C) = alpha;

    void* settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    float timer = CSF_AT(float, self, 0x48);
    float duration = CSF_AT(float, settings, 0x18E0);

    // The original also takes this branch for an unordered comparison.
    if (timer >= duration || std::isnan(timer) || std::isnan(duration))
    {
        panel = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x20));
        CSF_AT(int32_t, self, 0x48) = -1082130432;

        ((void(__fastcall*)(void*, void*, uint8_t*))
            CSF_AT(void**, panel, 0)[0x3C / 4])(
            panel, nullptr, &g_byte_82E65C);

        panel = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x20));
        ((void(__fastcall*)(void*, void*, void*))
            CSF_AT(void**, panel, 0)[0x60 / 4])(
            panel, nullptr, colorBytes);
        return;
    }

    void* fadeSettings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    float fadeStart =
        CSF_AT(float, fadeSettings, 0x18F0) *
        CSF_AT(float, settings, 0x18E0);

    timer = CSF_AT(float, self, 0x48);
    if (timer < fadeStart || std::isnan(timer) || std::isnan(fadeStart))
    {
        panel = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x20));
        CSF_AT(float, colorBytes, 0x0C) = 255.0f;
    }
    else
    {
        settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
        // The x87 chain at 004F40FC..004F4120 has no intermediate float stores.
        long double fadeElapsed =
            static_cast<long double>(CSF_AT(float, self, 0x48)) - fadeStart;
        panel = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x20));

        CSF_AT(float, colorBytes, 0x0C) =
            g_ColorChannelScale -
            fadeElapsed * g_ColorChannelScale /
            (static_cast<long double>(CSF_AT(float, settings, 0x18E0)) - fadeStart);
    }

    ((void(__fastcall*)(void*, void*, void*))
        CSF_AT(void**, panel, 0)[0x60 / 4])(
        panel, nullptr, colorBytes);
}

HOOK(0x004F4050, HudStringShow_UpdateThirdPanel, "auto");
