#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"

// Callee bindings (by original address).
CSF_FUNC(int, __cdecl, SessionTable_FindEntry, 0x0074A730, (int a1));
CSF_FUNC(void, __cdecl, Game_RaiseStatusNotice, 0x00646D30, (int arg0, int arg1, uint32_t code, int arg3));
CSF_FUNC(int, __cdecl, PlayerEntry_ReportCode, 0x006525A0, (int code));
CSF_FUNC(char, __cdecl, PlayerEntry_FailureResult, 0x00652660, ());

// Registers a player entry: looks it up, reports its flags, or raises a status notice when
// the flags carry no bits in the low 22 bits.
static int __stdcall MpSpawnPoints_RegisterPlayerEntry(int a1)
{
    if (!a1)
        return PlayerEntry_FailureResult();

    int entryHandle = SessionTable_FindEntry(a1);
    if (!entryHandle)
        return PlayerEntry_FailureResult();

    auto* entryBase = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(entryHandle));
    uint32_t entryFlags = CSF_AT(uint32_t, entryBase, 0x2C);
    if ((entryFlags & 0x3FFFFF) != 0)
        return PlayerEntry_ReportCode(static_cast<int>(entryFlags));

    Game_RaiseStatusNotice(0, 0, 0xE000000Eu, 0);
    return PlayerEntry_ReportCode(0);
}

HOOK(0x0073CC70, MpSpawnPoints_RegisterPlayerEntry, "auto");
