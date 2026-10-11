#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void, __cdecl, Game_RaiseStatusNotice, 0x00646D30, (int arg0, int arg1, uint32_t code, int arg3));

// Returns the flags word of the session record at this+0xD4, or raises a status notice
// when the low 22 bits are clear. Returns -1 when there is no session record.
static int __fastcall GetSpawnFlagFromGlobal(void* self, void* /*edx*/)
{
    uint32_t sessionRecordAddr = CSF_AT(uint32_t, self, 0xD4);
    if (sessionRecordAddr == 0)
        return -1;

    int sessionFlags = CSF_AT(int, reinterpret_cast<void*>(sessionRecordAddr), 0x2C);
    if ((sessionFlags & 0x3FFFFF) == 0)
    {
        Game_RaiseStatusNotice(0, 0, 0xE000000Eu, 0);
        return 0;
    }
    return sessionFlags;
}

HOOK(0x0073CD30, GetSpawnFlagFromGlobal, "auto");
