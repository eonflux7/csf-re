#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// WaterParams_Construct: __thiscall (ecx = this), bound as __fastcall with unused edx.
CSF_FUNC(uint32_t*, __fastcall, WaterParams_Construct, 0x006FDEE0, (void* self, void* edx));

// RwMatrixCreate: __cdecl, no arguments, returns a new RW matrix.
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());

static uint8_t* __fastcall S_006fdf70_Initialize(S_006fdf70* self, void* /*edx*/)
{
    self->field_0000 = reinterpret_cast<uint32_t>(CSF_VTBL(0x862FEC));
    self->field_0004 = 0;

    WaterParams_Construct(reinterpret_cast<uint8_t*>(self) + 0x0C, nullptr);

    self->field_00b0 = 15;
    self->field_00ac = 0;
    self->field_009c = 0;
    self->field_00b8 = reinterpret_cast<uint32_t>(RwMatrixCreate());
    self->field_00bc = 1;

    return reinterpret_cast<uint8_t*>(self);
}

HOOK(0x6FDF70, S_006fdf70_Initialize, "auto");
