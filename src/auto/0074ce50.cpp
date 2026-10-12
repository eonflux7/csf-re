#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, NetPlayerDDL_ResolveObject, 0x00655DF0, (void* self, void* edx, bool* a2));
CSF_FUNC(void*, __fastcall, ReadSerializedByte, 0x00655E20, (void* self, void* edx, char* a2));
CSF_FUNC(int, __fastcall, ReadCounterWord, 0x00655E40, (void* self, void* edx, char* a2));
CSF_FUNC(uint32_t*, __fastcall, NetCogibleDdl_ReadIntoSlot, 0x00655F00, (void* self, void* edx, int a2));

static int __fastcall C_008622e8_ReadSerializedFields(
    C_008622e8* self, void* /*edx*/, C_008622e8* reader)
{
    NetPlayerDDL_ResolveObject(reader, nullptr, &CSF_AT(bool, self, 0x00));
    NetPlayerDDL_ResolveObject(reader, nullptr, &CSF_AT(bool, self, 0x01));
    NetPlayerDDL_ResolveObject(reader, nullptr, &CSF_AT(bool, self, 0x02));
    NetPlayerDDL_ResolveObject(reader, nullptr, &CSF_AT(bool, self, 0x03));
    NetPlayerDDL_ResolveObject(
        reader, nullptr, reinterpret_cast<bool*>(&self->field_0004));

    ReadSerializedByte(
        reader, nullptr, reinterpret_cast<char*>(&self->field_0004) + 1);
    ReadSerializedByte(
        reader, nullptr, reinterpret_cast<char*>(&self->field_0004) + 2);

    for (int i = 0; i < 128; ++i)
        ReadSerializedByte(
            reader, nullptr, reinterpret_cast<char*>(&self->field_0004) + i + 3);

    for (int j = 0; j < 128; ++j)
        ReadSerializedByte(reader, nullptr, &CSF_AT(char, self, j + 135));

    for (int k = 0; k < 128; ++k)
        ReadSerializedByte(reader, nullptr, &CSF_AT(char, self, k + 263));

    NetCogibleDdl_ReadIntoSlot(
        reader, nullptr, reinterpret_cast<int>(&CSF_AT(uint32_t, self, 392)));
    NetCogibleDdl_ReadIntoSlot(
        reader, nullptr, reinterpret_cast<int>(&CSF_AT(uint32_t, self, 396)));
    NetCogibleDdl_ReadIntoSlot(
        reader, nullptr, reinterpret_cast<int>(&CSF_AT(uint32_t, self, 400)));

    ReadCounterWord(reader, nullptr, &CSF_AT(char, self, 404));
    ReadCounterWord(reader, nullptr, &CSF_AT(char, self, 406));
    ReadCounterWord(reader, nullptr, &CSF_AT(char, self, 408));
    ReadCounterWord(reader, nullptr, &CSF_AT(char, self, 410));
    return ReadCounterWord(reader, nullptr, &CSF_AT(char, self, 412));
}

HOOK(0x0074CE50, C_008622e8_ReadSerializedFields, "auto");
