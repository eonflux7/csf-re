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

CSF_FUNC(void*, __fastcall, NetPlayerDDL_ResolveObject, 0x00655DF0, (void* self, void* edx, bool* a2));
CSF_FUNC(uint32_t*, __fastcall, NetCogibleDdl_ReadIntoSlot, 0x00655F00, (void* self, void* edx, int a2));

// Reads the control record's fields from the reader: first resolves the player object
// against this record, then reads nine consecutive dword slots (+4 through +0x24) and
// a final slot at +0x28. `self` is the record (ecx); `reader` is the stack argument.
// The return value is the result of the last slot read.
static uint32_t* __fastcall NetControlDdl_ReadRecordFields(C_008622e8* self, void* /*edx*/, C_008622e8* reader)
{
    NetPlayerDDL_ResolveObject(reader, nullptr, reinterpret_cast<bool*>(self));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&self->field_0004));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&self->field_0008));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&self->field_000c));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&self->field_0010));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&self->field_0014));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&self->field_0018));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&self->field_001c));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&self->field_0020));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&self->field_0024));
    return NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x28));
}

HOOK(0x0074EB10, NetControlDdl_ReadRecordFields, "auto");
