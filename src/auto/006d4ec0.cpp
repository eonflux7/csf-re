#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __cdecl, MissionScript_CopyBitRange, 0x0046F010, (uint32_t* cursorOut, uint32_t srcBit, uint32_t* srcWord, uint32_t srcEndBit, uint32_t* srcEndWord, uint32_t dstBit, uint32_t* dstWord));
CSF_FUNC(void, __fastcall, S_00470440_Advance, 0x0046FF80, (void* self, void* edx, int a2));

static int* __fastcall S_00470440_EraseBit(
    S_00470440* self, void* /*edx*/, int* result,
    unsigned int bit, int* word)
{
    const uint32_t baseAddress = self->field_0008;
    const uint32_t bitCount = self->field_0000;
    const uint32_t wordAddress =
        static_cast<uint32_t>(reinterpret_cast<uintptr_t>(word));
    const int32_t wordOffset =
        static_cast<int32_t>(wordAddress - baseAddress) >> 2;
    const uint32_t position = bit + 32u * static_cast<uint32_t>(wordOffset);

    uint32_t endBit = 0;
    uint32_t endAddress = baseAddress;
    if (bitCount != 0) {
        endAddress = baseAddress + 4 * (bitCount >> 5);
        endBit = bitCount & 0x1F;
    }

    // The original uses its adjacent bit and word argument slots as this output.
    uint32_t copiedCursor[2];
    const uint32_t nextBit = bit + 1;
    MissionScript_CopyBitRange(
        copiedCursor,
        nextBit & 0x1F,
        reinterpret_cast<uint32_t*>(wordAddress + 4 * (nextBit >> 5)),
        endBit,
        reinterpret_cast<uint32_t*>(endAddress),
        bit,
        reinterpret_cast<uint32_t*>(word));

    S_00470440_Advance(
        self, nullptr, static_cast<int32_t>(self->field_0000 - 1));

    const uint32_t resultAddress =
        self->field_0008 + 4 * (position >> 5);
    result[0] = static_cast<int>(position & 0x1F);
    result[1] = static_cast<int32_t>(resultAddress);
    return result;
}

HOOK(0x006D4EC0, S_00470440_EraseBit, "auto");
