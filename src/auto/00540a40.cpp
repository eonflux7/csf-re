#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Base-part constructor invoked on the embedded sub-object at +0x04 (thiscall, no stack args).
CSF_FUNC(void*, __fastcall, C_00830438_Construct, 0x712CC0, (void* self, void* edx));

// CMenuMpScoreBar constructor (thiscall, one stack arg: owner/parent value stored at +0x34).
static CMenuMpScoreBar* __fastcall CMenuMpScoreBar_Construct(CMenuMpScoreBar* self, void* /*edx*/, int ownerValue)
{
    // Primary vtable (CMenuScreen-family part of CMenuMpScoreBar).
    CSF_AT(uint32_t, self, 0x00) = CSF_ADDR_VT(0x0083042C);

    // Construct the embedded sub-object at +0x04.
    C_00830438_Construct(reinterpret_cast<uint8_t*>(self) + 0x04, nullptr);

    // Stored argument.
    self->field_0034 = static_cast<uint32_t>(ownerValue);

    // Sub-object vtable at +0x04.
    self->field_0004 = CSF_ADDR_VT(0x008430A8);

    // Zero-initialise members (same order as original).
    self->field_0030 = 0;
    self->field_002c = 0;
    CSF_AT(uint8_t, self, 0x38) = 0;
    self->field_0010 = 0;
    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;

    // Final vtable for CMenuMpScoreBar.
    CSF_AT(uint32_t, self, 0x00) = CSF_ADDR_VT(0x008430F8);

    return self;
}

HOOK(0x00540A40, CMenuMpScoreBar_Construct, "auto");
