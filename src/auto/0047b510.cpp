#include <cstdint>
#include <cstring>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Tail target: TCharTask base update. Forwarded with ecx=this and the caller's
// single stack argument; the target pops it.
CSF_FUNC(int, __fastcall, TCharTask_BaseUpdate, 0x4DB0F0, (void* self, void* edx, uint32_t argBits));

// Adds a2 to the float accumulator at this->field_0008 + 0x2AC, then tail-forwards
// to TCharTask_BaseUpdate with the argument unchanged.
static int __fastcall TDisparar_AddToAccumulatorAndUpdate(TDisparar* self, void* /*edx*/, float a2)
{
    uint8_t* accumulatorOwner = reinterpret_cast<uint8_t*>(self->field_0008);
    CSF_AT(float, accumulatorOwner, 0x2AC) = a2 + CSF_AT(float, accumulatorOwner, 0x2AC);

    // The original jumps with the caller's stack word untouched, so pass the raw bits.
    uint32_t argBits = 0;
    std::memcpy(&argBits, &a2, sizeof(argBits));
    return TCharTask_BaseUpdate(self, nullptr, argBits);
}

HOOK(0x47B510, TDisparar_AddToAccumulatorAndUpdate, "auto");
