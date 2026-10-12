#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee: resets the inherited base-class state (C_00860a30 chain).
CSF_FUNC(int, __fastcall, sub_6CA660, 0x006CA660, (void* self, void* edx));

// Callee: initializes the embedded transform subobject at +0x44.
CSF_FUNC(uint32_t*, __fastcall, S00402350_InitTransform, 0x00402350, (S_00402350* self, void* edx));

// Constructor of C_00862748 (RpPrtStd atomic update). __thiscall, no stack args.
static uint32_t* __fastcall C_00862748_Construct(C_00862748* self, void* /*edx*/)
{
    sub_6CA660(self, nullptr);

    self->field_0020 = reinterpret_cast<uint32_t>(CSF_VTBL(0x008626C4));
    self->vtbl = CSF_VTBL(0x00862748);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x00862740));
    self->field_0008 = reinterpret_cast<uint32_t>(CSF_VTBL(0x00862724));
    self->field_0020 = reinterpret_cast<uint32_t>(CSF_VTBL(0x008626F4));

    S00402350_InitTransform(
        reinterpret_cast<S_00402350*>(reinterpret_cast<uint8_t*>(self) + 0x44), nullptr);

    self->field_0084 = 0;
    self->field_0080 = 0xFFFFFFFFu;
    self->field_0024 = 0;
    self->field_002c = 0;
    self->field_0030 = 0;
    self->field_0038 = 0;
    self->field_0034 = 0;
    self->field_0035 = 0;
    self->field_0028 = 0;
    self->field_0040 = 0.0f;
    self->field_003c = 1;

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x006F4CD0, C_00862748_Construct, "auto");
