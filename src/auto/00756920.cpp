#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, ResizeIntVectorToCount, 0x0046D800, (S_0046c410* self, void* /*edx*/, unsigned int requestedCount, int fillValue));

// Sets the C_0086a3c0 vtable, zeroes the three counters of the embedded
// pad sub-object at +0x4 and resizes its int vector to 101 entries filled with -1.
static uint32_t* __fastcall C_0086a3c0_Initialize(C_0086a3c0* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x0086A3C0);

    S_0046c410* vectorHolder = reinterpret_cast<S_0046c410*>(reinterpret_cast<uint8_t*>(self) + 4);
    vectorHolder->field_0004 = 0;
    vectorHolder->field_0008 = 0;
    vectorHolder->field_000c = 0;

    ResizeIntVectorToCount(vectorHolder, nullptr, 101, -1);

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x00756920, C_0086a3c0_Initialize, "auto");
