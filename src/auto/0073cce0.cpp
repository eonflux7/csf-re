#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, NetSession_LookupEntry, 0x0074A690, (int a1));
CSF_FUNC(void, __cdecl, Game_RaiseStatusNotice, 0x00646D30, (int arg0, int arg1, uint32_t code, int arg3));

// Returns the flags word of the net-session entry for a1, or -1 if none.
// Flags with the low 22 bits clear are reported through a status notice and yield 0.
static int __stdcall NetSession_GetEntryFlags(int a1)
{
    if (a1 == 0)
        return -1;

    int entry = NetSession_LookupEntry(a1);
    if (entry == 0)
        return -1;

    int flags = CSF_AT(int, reinterpret_cast<uint8_t*>(entry), 0x2C);
    if ((flags & 0x3FFFFF) == 0) {
        Game_RaiseStatusNotice(0, 0, 0xE000000E, 0);
        return 0;
    }
    return flags;
}

HOOK(0x73CCE0, NetSession_GetEntryFlags, "auto");
