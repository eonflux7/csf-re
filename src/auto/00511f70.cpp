#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

// Local-time snapshot filled by GetLocalTime. Matches the Win32 SYSTEMTIME
// layout (8 x uint16_t, 16 bytes) without including <windows.h>.
struct SaveGamesLocalTimeFields {
    uint16_t wYear;
    uint16_t wMonth;
    uint16_t wDayOfWeek;
    uint16_t wDay;
    uint16_t wHour;
    uint16_t wMinute;
    uint16_t wSecond;
    uint16_t wMilliseconds;
};

// Import slot of GetLocalTime (the original does `call dword ptr [0x82d214]`).
CSF_GLOBAL(void*, g_ImportGetLocalTime, 0x0082D214);

// Scale factor milliseconds -> seconds (read as a dword float by fmul).
CSF_GLOBAL(float, g_SceneLoadMillisecondsToSeconds, 0x0082D4F0);

using GetLocalTimeFn = void(__stdcall*)(SaveGamesLocalTimeFields* systemTime);

// Fills a 0x18-byte time snapshot { year, month, day, hour, minute, seconds(float) }
// from the current local time and returns the same pointer.
static int __stdcall SaveGames_FillLocalTimeSnapshot(int a1)
{
    SaveGamesLocalTimeFields systemTime{};
    reinterpret_cast<GetLocalTimeFn>(g_ImportGetLocalTime)(&systemTime);

    uint8_t* snapshot = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a1));

    CSF_AT(int, snapshot, 0x04) = systemTime.wMonth;
    CSF_AT(int, snapshot, 0x00) = systemTime.wYear;
    CSF_AT(int, snapshot, 0x0C) = systemTime.wHour;
    CSF_AT(int, snapshot, 0x08) = systemTime.wDay;
    CSF_AT(int, snapshot, 0x10) = systemTime.wMinute;
    CSF_AT(float, snapshot, 0x14) =
        static_cast<float>(systemTime.wMilliseconds) * g_SceneLoadMillisecondsToSeconds
        + static_cast<float>(systemTime.wSecond);

    return a1;
}

HOOK(0x00511F70, SaveGames_FillLocalTimeSnapshot, "auto");
