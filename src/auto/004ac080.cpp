#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CoverAttack_SetState_4DC150, 0x004DC150, (void* self, void* edx, int a2));

// Original: GetLinkedObjectValue(a2) (result discarded), then
// return CoverAttack_SetState(a2, (int)(this + 1)); where this + 1 is the
// sub-object at +4 of the TFncVigilarDesdeParapeto instance.
static int __fastcall TFncVigilarDesdeParapeto_ActivateWatchState(TFncVigilarDesdeParapeto* self, void* /*edx*/, TAiState* a2)
{
    GetLinkedObjectValue_4DB5C0(a2, nullptr);
    uintptr_t embeddedAddress = reinterpret_cast<uintptr_t>(reinterpret_cast<uint8_t*>(self) + 4);
    return CoverAttack_SetState_4DC150(a2, nullptr, static_cast<int>(embeddedAddress));
}

HOOK(0x004AC080, TFncVigilarDesdeParapeto_ActivateWatchState, "auto");
