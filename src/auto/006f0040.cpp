#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_00860a30_InitBase, 0x006CA660, (C_00860a30* self, void*));
CSF_FUNC(uint32_t*, __fastcall, C_00862748_Construct, 0x006F4CD0, (C_00862748* self, void*));

static uint32_t* __fastcall C_00862240_Construct(C_00862240* self, void* /*edx*/)
{
    C_00860a30_InitBase(self, nullptr);

    self->field_0020 = (uint32_t)(uintptr_t)CSF_ADDR_VT(0x008621E4);
    self->vtbl = CSF_VTBL(0x00862240);
    self->field_0004 = (uint32_t)(uintptr_t)CSF_ADDR_VT(0x00862238);
    self->field_0008 = (uint32_t)(uintptr_t)CSF_ADDR_VT(0x0086221C);
    self->field_0020 = (uint32_t)(uintptr_t)CSF_ADDR_VT(0x00862200);

    C_00862748_Construct((C_00862748*)((uint8_t*)self + 0x24), nullptr);

    self->field_00ac = 0;
    self->field_00b0 = 0;
    self->field_00b4 = 0;
    self->field_00b8 = 10;
    self->field_00bc = 0;
    self->field_00c0 = 0;
    self->field_00c4 = 0;
    self->field_00c8 = 10;
    self->field_00cc = 1;

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x006F0040, C_00862240_Construct, "auto");
