#include <cstdint>
#include <cstddef>
#include <cstring>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee at 0x0040B240: __stdcall (float a1, float a2) -> int, pops 8.
// a1 receives the raw bit pattern of the computed entry address (COERCE_FLOAT).
CSF_FUNC(int, __stdcall, VehicleChildEntry_SetParameter, 0x0040B240, (float a1, float a2));

// Vehicle_SetChildParameter: __thiscall(this, int index, float value), ret 8.
// Entries are 28 bytes each, starting at field_0010.
static int __fastcall C_0082daf4_SetChildParameter(C_0082daf4* self, void* /*edx*/, int a2, float a3)
{
    uint32_t entryAddress = self->field_0010 + 28u * static_cast<uint32_t>(a2);

    float entryAsFloat;
    std::memcpy(&entryAsFloat, &entryAddress, sizeof(entryAsFloat));

    return VehicleChildEntry_SetParameter(entryAsFloat, a3);
}

HOOK(0x0040B590, C_0082daf4_SetChildParameter, "auto");
