#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(std::uint32_t, g_ScreenWidth, 0x00899B9C);
CSF_GLOBAL(std::uint32_t, g_ScreenHeight, 0x00899BA0);
CSF_FUNC(std::int32_t, __cdecl, GetGlobalValue_753A30, 0x00753A30, ());

static void __cdecl MenuMpPause_AdvanceLayoutPosition(float* position)
{
    const std::int32_t width = static_cast<std::int32_t>(g_ScreenWidth);
    const std::int32_t height = static_cast<std::int32_t>(g_ScreenHeight);

    void* settings = reinterpret_cast<void*>(
        static_cast<std::uintptr_t>(GetGlobalValue_753A30()));
    const float roundedWidth = static_cast<float>(width);

    position[0] = static_cast<float>(
        static_cast<double>(width) * CSF_AT(float, settings, 0x1D4)
        + static_cast<double>(position[0]));

    settings = reinterpret_cast<void*>(
        static_cast<std::uintptr_t>(GetGlobalValue_753A30()));
    if (static_cast<double>(roundedWidth) * CSF_AT(float, settings, 0x1CC)
            + static_cast<double>(position[0])
        > static_cast<double>(roundedWidth))
    {
        settings = reinterpret_cast<void*>(
            static_cast<std::uintptr_t>(GetGlobalValue_753A30()));
        position[0] = static_cast<float>(
            static_cast<double>(roundedWidth) * CSF_AT(float, settings, 0x1C4));

        settings = reinterpret_cast<void*>(
            static_cast<std::uintptr_t>(GetGlobalValue_753A30()));
        position[1] = static_cast<float>(
            static_cast<double>(height) * CSF_AT(float, settings, 0x1D8)
            + static_cast<double>(position[1]));
    }
}

HOOK(0x005339B0, MenuMpPause_AdvanceLayoutPosition, "auto");
