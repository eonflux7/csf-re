#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void, __cdecl, Game_RaiseStatusNotice, 0x00646D30, (int arg0, int arg1, uint32_t code, int arg3));
CSF_FUNC(int, __fastcall, MpSpawnPoints_FindPlayerIndex, 0x0074A190, (C_00867900* self, void* /*edx*/, int a2));

// Ends a network entry: looks up the spawn-point index for the entry's id and clears its flag byte.
static int __fastcall NetHost_EndEntry(C_00867900* self, void* /*edx*/, int a2) {
    int result = a2;
    if (a2) {
        uint8_t* entry = reinterpret_cast<uint8_t*>(a2);
        uint32_t entryId = CSF_AT(uint32_t, entry, 0x2C);
        if ((entryId & 0x3FFFFF) == 0) {
            Game_RaiseStatusNotice(0, 0, 0xE000000E, 0);
            entryId = 0;
        }
        result = MpSpawnPoints_FindPlayerIndex(self, nullptr, static_cast<int>(entryId));
        if (result != -1) {
            result *= 48;
            CSF_AT(uint8_t, self, 0x2D + result) = 0;
        }
    }
    return result;
}

HOOK(0x0074A470, NetHost_EndEntry, "auto");
