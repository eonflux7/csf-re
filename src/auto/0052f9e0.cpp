#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"
#include "globals.h"

// Vtable/data addresses referenced by the original constructor.
static const uint32_t kVtblCMenuCreditsPartial = CSF_ADDR_VT(0x0083042C); // intermediate vtable, overwritten below
static const uint32_t kVtblCMenuCreditsSubBase = CSF_ADDR_VT(0x00841868); // vtable stored at this+4
static const uint32_t kVtblCMenuCredits = CSF_ADDR_VT(0x008418B8);        // final vtable stored at this+0

// __thiscall callees, bound by address (arg count: this only).
CSF_FUNC(void*, __fastcall, C_00830438_Construct, 0x00712CC0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, UiSubObject_Construct, 0x00508310, (void* self, void* edx));

// CMenuCredits constructor: __thiscall, one stack argument (ret 4).
static CMenuCredits* __fastcall CMenuCredits_Initialize(CMenuCredits* self, void* /*edx*/, int /*a2*/)
{
    // Provisional vtable, then construct the embedded base member at this+4.
    CSF_AT(uint32_t, self, 0x000) = kVtblCMenuCreditsPartial;
    C_00830438_Construct(reinterpret_cast<uint8_t*>(self) + 0x004, nullptr);

    // Final vtables.
    CSF_AT(uint32_t, self, 0x000) = kVtblCMenuCredits;
    self->field_0004 = kVtblCMenuCreditsSubBase;

    // Construct the embedded sub-object at this+0x20.
    UiSubObject_Construct(reinterpret_cast<uint8_t*>(self) + 0x020, nullptr);

    // Zero CMenuCredits-specific fields (same order as original).
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0010 = 0;
    self->field_0014 = 0;

    return self;
}

HOOK(0x0052F9E0, CMenuCredits_Initialize, "auto");
