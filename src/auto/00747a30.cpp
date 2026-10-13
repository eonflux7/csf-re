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

CSF_FUNC(int, __fastcall, ReadCounterWord, 0x00655E40, (void* self, void* edx, char* a2));
CSF_FUNC(uint32_t*, __fastcall, NetCogibleDdl_ReadIntoSlot, 0x00655F00, (void* self, void* edx, int a2));

// Reads the 0x24-byte payload block of a projectile network record from the reader.
// `self` is the record (ecx); `reader` is the stack argument (C_008622e8*).
// Layout: dword @+0, word @+4, word @+6, then dwords @+8,+12,...,+32, and the last dword @+36.
static uint32_t* __fastcall NetProyectilDdl_ReadPayloadFields(char* self, void* /*edx*/, C_008622e8* reader)
{
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, (int)self);
    ReadCounterWord(reader, nullptr, self + 4);
    ReadCounterWord(reader, nullptr, self + 6);
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, (int)(self + 8));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, (int)(self + 12));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, (int)(self + 16));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, (int)(self + 20));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, (int)(self + 24));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, (int)(self + 28));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, (int)(self + 32));
    return NetCogibleDdl_ReadIntoSlot(reader, nullptr, (int)(self + 36));
}

HOOK(0x00747A30, NetProyectilDdl_ReadPayloadFields, "auto");
