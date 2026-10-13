#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, ReadSerializedByte, 0x00655E20, (void* self, void* edx, char* a2));
CSF_FUNC(int, __fastcall, ReadCounterWord, 0x00655E40, (void* self, void* edx, char* a2));
CSF_FUNC(uint32_t*, __fastcall, NetCogibleDdl_ReadIntoSlot, 0x00655F00, (void* self, void* edx, int a2));

static int __fastcall C_008622e8_ReadSerializedFields(C_008622e8* self, void* /*edx*/, void* a2)
{
    ReadSerializedByte(a2, nullptr, reinterpret_cast<char*>(self));
    NetCogibleDdl_ReadIntoSlot(a2, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(&self->field_0004)));
    ReadCounterWord(a2, nullptr, reinterpret_cast<char*>(&self->field_0008));
    return ReadCounterWord(a2, nullptr, reinterpret_cast<char*>(&self->field_0008) + 2);
}

HOOK(0x0074FCE0, C_008622e8_ReadSerializedFields, "auto");
