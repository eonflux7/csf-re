#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Computes the packed bit slot for a (a2, a3, a4) triple; caller passes this in ecx.
CSF_FUNC(unsigned int, __fastcall, MissionScript_ComputeBitSlot, 0x004705C0,
         (int a1, int a2, int a3, uint32_t* a4, unsigned int a5));

// Applies a bit range update to the two bitmap words and records the flag byte.
CSF_FUNC(int*, __cdecl, MissionScript_ApplyBitRange, 0x0046F3E0,
         (unsigned int a1, int* a2, int a3, int* a4, uint8_t* a5));

// __thiscall(this=ecx) with 4 stack args (ret 0x10).
static int* __fastcall MissionScript_SetBitRange(void* self, void* /*edx*/,
                                                 int a2, int a3, int a4, char a5)
{
    // Callee's bit slot; the original pushes a2, a3, a4 and passes this in ecx.
    unsigned int slot = MissionScript_ComputeBitSlot(
        static_cast<int>(reinterpret_cast<uintptr_t>(self)), 0,
        a2,
        reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(a3)),
        static_cast<unsigned int>(a4));

    // The original reloads the bitmap base pointer (this+8) after the call.
    uint32_t bitmapBase = CSF_AT(uint32_t, self, 0x08);

    unsigned int startBit = slot & 0x1F;
    int* startWord = reinterpret_cast<int*>(bitmapBase + 4 * (slot >> 5));

    unsigned int endBitRaw = slot + static_cast<unsigned int>(a4);
    unsigned int endBit = endBitRaw & 0x1F;
    int* endWord = reinterpret_cast<int*>(bitmapBase + 4 * (endBitRaw >> 5));

    return MissionScript_ApplyBitRange(startBit, startWord, static_cast<int>(endBit),
                                       endWord, reinterpret_cast<uint8_t*>(&a5));
}

HOOK(0x00470A50, MissionScript_SetBitRange, "auto");
