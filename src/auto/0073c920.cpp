#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"

// Session/transport lookup: returns the active session object (char* in the original).
CSF_FUNC(char*, __cdecl, sub_668B70, 0x00668B70, ());

// Thiscall (ecx = this, 2 stack args popped by callee). Stores a named value into the session dump.
CSF_FUNC(int, __fastcall, NetSession_StoreNamedValue, 0x00680D30, (void* self, void* edx, const char* name, char* value));

// Cdecl sprintf.
CSF_FUNC(int, __cdecl, _sprintf, 0x007AF9BB, (char* buffer, const char* format, ...));

static int __stdcall NetSession_ReportCrcValue(int crcValue)
{
    int result = static_cast<int>(reinterpret_cast<intptr_t>(sub_668B70()));
    if (result)
    {
        char crcText[0x80];
        std::memset(crcText, 0, 0x80);
        _sprintf(crcText, CSF_STR(0x0083090C, "%d"), crcValue);

        char* session = sub_668B70();
        return NetSession_StoreNamedValue(session, nullptr,
                                          CSF_STR(0x00867654, "CRC"), crcText);
    }
    return result;
}

HOOK(0x0073C920, NetSession_ReportCrcValue, "auto");
