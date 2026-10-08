#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"
#include "globals.h"

// 0x0053C100 — constructs the CMenuProfiles base sub-object. __thiscall(this, a2), ret 4.
CSF_FUNC(void, __fastcall, CMenuProfiles_Construct, 0x53C100, (CMenuProfilesPC* self, void* edx, int a2));

// 0x005445F0 — constructs the member sub-object embedded at this+0x44. __thiscall(this), ret 0.
CSF_FUNC(void, __fastcall, MenuSaveGame_ResetState, 0x5445F0, (void* self, void* edx));

// Constructor of CMenuProfilesPC: run base ctor, install the two vtables, construct the +0x44 member, return this.
static CMenuProfilesPC* __fastcall CMenuProfilesPC_Constructor(CMenuProfilesPC* self, void* /*edx*/, int a2)
{
    CMenuProfiles_Construct(self, nullptr, a2);

    CSF_AT(uint32_t, self, 0x00) = CSF_ADDR_VT(0x843A90);   // primary vtable
    self->field_0004            = CSF_ADDR_DATA(0x843A40);   // secondary vtable (offset +4)

    MenuSaveGame_ResetState(reinterpret_cast<uint8_t*>(self) + 0x44, nullptr);

    return self;
}

HOOK(0x544670, CMenuProfilesPC_Constructor, "auto");
