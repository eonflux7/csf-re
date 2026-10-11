#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, TFncGirar_OnRotationComplete, 0x004DB1D0, (void* self, void* edx, int a2));

// Clears the byte flag at +0x314 of the object pointed to by this->+0x08, then
// runs the rotation-complete handler with a2 = 0.
static void __fastcall TFncGirar_ClearFlagAndCompleteRotation(TFncGirar* self, void* /*edx*/)
{
    uint8_t* owner = CSF_AT(uint8_t*, self, 0x08);
    CSF_AT(uint8_t, owner, 0x314) = 0;
    TFncGirar_OnRotationComplete(self, nullptr, 0);
}

HOOK(0x004D74A0, TFncGirar_ClearFlagAndCompleteRotation, "auto");
