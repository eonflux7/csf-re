#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callees, bound by address with the signatures given in the packet.
CSF_FUNC(char, __fastcall, S_0048f590_RefreshState, 0x0048F590, (void* self, void* edx));
CSF_FUNC(int, __fastcall, S_00833908_UpdateOwnerLink, 0x00499080, (void* self, void* edx));
CSF_FUNC(int, __fastcall, S_00833ee8_ReleaseChildSlot, 0x004993E0, (void* self, void* edx));

// __thiscall S_0048f590::<method at 0x499940>, ecx = this, no stack args (ret 0).
// Optionally releases a child slot when flag 0x2000 is set in field_02dc, refreshes
// this object's state, then forwards to the owner link with this-4 and returns its result.
static int __fastcall S_0048f590_UpdateAndForwardToOwner(S_0048f590* self, void* /*edx*/) {
    if ((self->field_02dc & 0x2000) != 0) {
        S_00833ee8_ReleaseChildSlot(reinterpret_cast<uint8_t*>(self) - 4, nullptr);
    }
    S_0048f590_RefreshState(self, nullptr);
    return S_00833908_UpdateOwnerLink(reinterpret_cast<uint8_t*>(self) - 4, nullptr);
}

HOOK(0x499940, S_0048f590_UpdateAndForwardToOwner, "auto");
