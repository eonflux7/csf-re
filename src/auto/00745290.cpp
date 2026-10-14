#include <cstdint>
#include "globals.h"
#include <cstring>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static CSF_FUNC(int, __fastcall, ReadCounterWord, 0x00655E40,
               (void* self, void* edx, char* a2));
static CSF_FUNC(int16_t, __fastcall, NetCogibleDDL_AddToCounter, 0x00740E20,
               (S_00740e20* self, void* , int index, int16_t delta));

static int16_t __fastcall S_00740e20_ReadAndAddToCounter(
    S_00740e20* self, void* /*edx*/, int a2)
{
    void* reader = reinterpret_cast<void*>(static_cast<uintptr_t>(a2));
    alignas(8) uint8_t indexBytes[0x4];
    // The original pushes `this` into this slot (00745293 push ecx) before the first read fills its low word.
    CSF_AT(void*, indexBytes, 0) = self;

    ReadCounterWord(reader, nullptr, reinterpret_cast<char*>(indexBytes));
    ReadCounterWord(reader, nullptr, reinterpret_cast<char*>(&a2) + 2);

    int16_t index;
    int16_t delta;
    std::memcpy(&index, indexBytes, sizeof(index));
    std::memcpy(&delta, reinterpret_cast<char*>(&a2) + 2, sizeof(delta));
    return NetCogibleDDL_AddToCounter(self, nullptr, index, delta);
}

HOOK(0x00745290, S_00740e20_ReadAndAddToCounter, "auto");
