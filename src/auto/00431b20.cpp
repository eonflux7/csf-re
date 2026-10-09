#include <cstddef>
#include <cstdint>
#include <cstring>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, Hud_DrawFormattedText, 0x0042F970, (int a1, int* a2, char* Format, ...));
CSF_FUNC(void*, __cdecl, GetData_8BAD98, 0x00690DA0, ());
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GetFpsStatsFromGlobal, 0x006B2B50, (void* self, void* edx));

// Draws one formatted HUD line at (x, y). Returns the draw call's result.
static int Hud_DrawOverlayLine(int hudHandle, int x, int y, const char* format, int value)
{
    int position[2];
    position[0] = x;
    position[1] = y;
    return Hud_DrawFormattedText(hudHandle, position, const_cast<char*>(format), value);
}

// Truncating float -> 64-bit integer conversion (as the original's __ftol2 does),
// then only the low 32 bits are kept. NaN / out-of-range inputs produce the x87
// "integer indefinite" value 0x8000000000000000, whose low 32 bits are 0.
static int FloatToLowInt32(float value)
{
    if (!(value >= -9223372036854775808.0f && value < 9223372036854775808.0f))
        return 0;
    const int64_t wide = static_cast<int64_t>(value);
    return static_cast<int>(static_cast<uint32_t>(static_cast<uint64_t>(wide)));
}

static char __fastcall Hud_DrawPerformanceOverlay(void* self, void* /*edx*/)
{
    if (CSF_AT(int, self, 0x240) == 0 || CSF_AT(uint8_t, self, 0x128) == 0)
        return 0;

    const int hudHandle = static_cast<int>(reinterpret_cast<std::uintptr_t>(self));

    // Global -> FPS stats object; its vtable slot 1 yields a float array:
    // [0] compensated FPS, [1] minimum FPS (0x7F7FFFFF sentinel = unset), [2] average FPS.
    void* globalObject = reinterpret_cast<void*>(static_cast<std::uintptr_t>(GetGlobalInt()));
    void* statsObject = reinterpret_cast<void*>(
        static_cast<std::uintptr_t>(GetFpsStatsFromGlobal(globalObject, nullptr)));
    using FloatArrayGetter = float* (__fastcall*)(void* self, void* edx);
    float* fpsStats = ((FloatArrayGetter)CSF_AT(void**, statsObject, 0)[1])(statsObject, nullptr);

    const float compensatedFps = fpsStats[0];
    Hud_DrawOverlayLine(hudHandle, 30, 30, const_cast<char*>(CSF_STR(0x008309F0, "FPSCompen. %d")),
                        FloatToLowInt32(compensatedFps));

    // The minimum line is skipped while the raw bits still hold the 0x7F7FFFFF sentinel.
    uint32_t minimumBits;
    std::memcpy(&minimumBits, &fpsStats[1], sizeof minimumBits);
    if (minimumBits != 2139095039) {
        const float minimumFps = fpsStats[1];
        Hud_DrawOverlayLine(hudHandle, 30, 45, const_cast<char*>(CSF_STR(0x008309E0, "FPSMinimos %d")),
                            FloatToLowInt32(minimumFps));
    }

    const float averageFps = fpsStats[2];
    Hud_DrawOverlayLine(hudHandle, 30, 60, const_cast<char*>(CSF_STR(0x008309D0, "FPSMedios  %d")),
                        FloatToLowInt32(averageFps));

    // Memory used: vtable slot 19 (0x4C) on the Data object returns a stats pointer; +0 = used bytes.
    void* dataObjectUsed = GetData_8BAD98();
    using PointerGetter = void* (__fastcall*)(void* self, void* edx);
    void* memUsedStats = ((PointerGetter)CSF_AT(void**, dataObjectUsed, 0)[19])(dataObjectUsed, nullptr);
    Hud_DrawOverlayLine(hudHandle, 30, 75, const_cast<char*>(CSF_STR(0x008309C0, "MemUsed  %d KB")),
                        CSF_AT(int, memUsedStats, 0) / 1024);

    // Memory max used: same slot; +4 = max used bytes.
    void* dataObjectMax = GetData_8BAD98();
    void* memMaxStats = ((PointerGetter)CSF_AT(void**, dataObjectMax, 0)[19])(dataObjectMax, nullptr);
    const int lastDrawResult = Hud_DrawOverlayLine(
        hudHandle, 30, 90, const_cast<char*>(CSF_STR(0x008309AC, "MemMaxUsed  %d KB")),
        CSF_AT(int, memMaxStats, 4) / 1024);

    return static_cast<char>(lastDrawResult);
}

HOOK(0x00431B20, Hud_DrawPerformanceOverlay, "auto");
