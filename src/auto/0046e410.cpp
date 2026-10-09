#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __fastcall, MissionScript_FindNameIndex, 0x0046C310, (void* self, void* edx, char* String1));
CSF_FUNC(unsigned int, __fastcall, MissionScript_AppendValue, 0x0046D9F0, (void* self, void* edx, uint32_t* a3));
CSF_FUNC(unsigned int, __fastcall, MissionScript_InsertNameEntry, 0x0046E310, (void* self, void* edx, void* a3));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __fastcall S_0046c260_SetNamedValue(
    S_0046c260* self, void* /*edx*/, char* name, int value)
{
    int index = MissionScript_FindNameIndex(self, nullptr, name);
    if (index == -1)
    {
        auto* values = reinterpret_cast<uint8_t*>(self->field_0044);
        const uint32_t begin = CSF_AT(uint32_t, values, 0x40);
        index = begin
            ? (static_cast<int32_t>(CSF_AT(uint32_t, values, 0x44) - begin) >> 2)
            : 0;

        MissionScript_AppendValue(
            values + 0x3C, nullptr, reinterpret_cast<uint32_t*>(&value));

        alignas(4) uint8_t entry[0x20] = {};
        CSF_AT(uint32_t, entry, 0x18) = 15;
        CSF_AT(uint32_t, entry, 0x14) = 0;
        CSF_AT(uint8_t, entry, 0x04) = 0;
        CSF_AT(int, entry, 0x1C) = -1;

        AssignStringRange_429B60(
            entry, nullptr, name, static_cast<unsigned int>(std::strlen(name)));

        CSF_AT(int, entry, 0x1C) = index;
        MissionScript_InsertNameEntry(
            reinterpret_cast<uint8_t*>(self) + 4, nullptr, entry);

        if (CSF_AT(uint32_t, entry, 0x18) >= 0x10)
        {
            if (CSF_AT(uint32_t, entry, 0x04))
                Mem_FreeDebug(reinterpret_cast<void*>(CSF_AT(uint32_t, entry, 0x04)));
        }
        return index;
    }

    if (value != -1)
    {
        auto* values = reinterpret_cast<uint8_t*>(self->field_0044);
        auto* elements = reinterpret_cast<int*>(CSF_AT(uint32_t, values, 0x40));
        elements[index] = value;
    }
    return index;
}

HOOK(0x0046E410, S_0046c260_SetNamedValue, "auto");
