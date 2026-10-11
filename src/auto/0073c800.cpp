#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Layout of the SYSTEMTIME struct filled by GetLocalTime (16 bytes).
struct NetSessionLocalTime {
    uint16_t wYear;
    uint16_t wMonth;
    uint16_t wDayOfWeek;
    uint16_t wDay;
    uint16_t wHour;
    uint16_t wMinute;
    uint16_t wSecond;
    uint16_t wMilliseconds;
};

// __stdcall GetLocalTime function pointer type (named so the CSF_GLOBAL macro can cast to it).
typedef void(__stdcall* NetSessionGetLocalTimeFn)(NetSessionLocalTime*);

// Import pointer slot for GetLocalTime (called via `call dword ptr [0x82d214]`).
CSF_GLOBAL(NetSessionGetLocalTimeFn, g_ImportGetLocalTime, 0x82D214);

// _sprintf (cdecl, varargs).
CSF_FUNC(int, __cdecl, _sprintf, 0x007AF9BB, (char* buffer, const char* format, ...));

// Writes "HH:MM:SS,mmm" of the current local time into self+0x11C and returns that pointer.
static char* __fastcall NetSession_FormatTimestampField(char* self, void* /*edx*/)
{
    alignas(8) uint8_t timeBytes[0x10];
    NetSessionLocalTime& systemTime = *reinterpret_cast<NetSessionLocalTime*>(timeBytes);

    g_ImportGetLocalTime(&systemTime);

    char* buffer = self + 0x11C;
    _sprintf(buffer,
             CSF_STR(0x867640, "%02d:%02d:%02d,%03d"),
             static_cast<int>(systemTime.wHour),
             static_cast<int>(systemTime.wMinute),
             static_cast<int>(systemTime.wSecond),
             static_cast<int>(systemTime.wMilliseconds));
    return buffer;
}

HOOK(0x73C800, NetSession_FormatTimestampField, "auto");
