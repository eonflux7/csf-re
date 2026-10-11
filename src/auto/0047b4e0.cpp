#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, TFncGirar_OnRotationComplete, 0x004DB1D0, (void* self, void* edx, int a2));

static void __fastcall TFncGirar_CompleteRotationWithoutFlag(TFncGirar* self, void* /*edx*/) {
    TFncGirar_OnRotationComplete(self, nullptr, 0);
}

HOOK(0x0047B4E0, TFncGirar_CompleteRotationWithoutFlag, "auto");
