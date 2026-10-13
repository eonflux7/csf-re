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

CSF_FUNC(void*, __fastcall, ReadSerializedByte, 0x00655E20, (void* self, void* edx, char* a2));
CSF_FUNC(uint32_t*, __fastcall, NetCogibleDdl_ReadIntoSlot, 0x00655F00, (void* self, void* edx, int a2));

// Reads one serialized byte into the buffer at a2, then reads the slot at a2+4
// from the same DDL reader object. Returns the slot reader's result.
static uint32_t* __stdcall NetGhostDdl_ReadByteThenSlot(int /*a1*/, char* a2, int /*a3*/, C_008622e8* a4, int /*a5*/)
{
    ReadSerializedByte(a4, nullptr, a2);
    return NetCogibleDdl_ReadIntoSlot(a4, nullptr, reinterpret_cast<int>(a2 + 4));
}

HOOK(0x74B7C0, NetGhostDdl_ReadByteThenSlot, "auto");
