#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "types/auto_classes.h"

// Callee at 0x0040B130: __stdcall (int entryAddress, float* ratio) -> float*, pops 8.
CSF_FUNC(float*, __stdcall, Wheel_ApplyRatioToEntry, 0x0040B130, (int a1, float* a2));

// Wheel_ApplyRatio: __thiscall(this, int index, float* ratio), ret 8.
// Returns the callee's result. Entries are 28 bytes each, starting at field_0010.
static float* __fastcall C_0082daf4_ApplyRatio(C_0082daf4* self, void* /*edx*/, int a2, float* a3)
{
    uint32_t entryAddress = self->field_0010 + 28u * static_cast<uint32_t>(a2);
    return Wheel_ApplyRatioToEntry(static_cast<int>(entryAddress), a3);
}

HOOK(0x0040B550, C_0082daf4_ApplyRatio, "auto");
