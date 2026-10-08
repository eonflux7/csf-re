// CMenuMpPause constructor (0x00535310). Thiscall, one stack argument, callee pops 4.
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

// Vtable of the CMenuScreen base sub-object at +0x00 (written first, overwritten at the end).
static const uint32_t kCMenuScreenBaseVtable = CSF_ADDR_VT(0x0083042C);
// Vtable of the embedded sub-object at +0x04 (the one passed to the init call).
static constexpr uint32_t kEmbeddedSubobjectVtable = 0x00841D20;
// Final vtable of CMenuMpPause.
static const uint32_t kCMenuMpPauseVtable = CSF_ADDR_VT(0x00841D70);

// Initializes the embedded sub-object at this+0x04 (original sub_712CC0, thiscall, no extra args).
CSF_FUNC(void, __fastcall, C_00830438_Construct, 0x00712CC0, (void* self, void* edx));

static CMenuMpPause* __fastcall CMenuMpPause_Construct(CMenuMpPause* self, void* /*edx*/, int ownerArg)
{
    // Base vtable is installed before the embedded sub-object is initialized.
    CSF_AT(uint32_t, self, 0x000) = kCMenuScreenBaseVtable;

    void* embeddedSubobject = reinterpret_cast<uint8_t*>(self) + 0x004;
    C_00830438_Construct(embeddedSubobject, nullptr);

    self->field_003c = static_cast<uint32_t>(ownerArg);

    CSF_AT(uint32_t, self, 0x004) = kEmbeddedSubobjectVtable;

    self->field_0038 = 0;
    self->field_0010 = 0;
    self->field_0040 = 0;
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_002c = 0;
    self->field_0030 = 0;
    self->field_0044 = 0;
    self->field_0045 = 0;
    self->field_0048 = 0.0f;
    self->field_004c = 0.0f;
    self->field_0034 = 0;

    CSF_AT(uint32_t, self, 0x000) = kCMenuMpPauseVtable;

    self->field_0014 = 0xFFFFFFFFu;

    return self;
}

HOOK(0x00535310, CMenuMpPause_Construct, "auto");
