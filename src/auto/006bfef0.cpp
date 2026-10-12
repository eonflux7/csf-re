#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, C_00860700_RefreshAfterSetPosition, 0x006BFC70, (void* self, void* edx));

static char __fastcall C_00860700_SetPositionVector(C_00860700* self, void* /*edx*/, const uint32_t* a2) {
    // The original copies the three dwords of the argument verbatim (raw bits, no int->float conversion).
    CSF_AT(uint32_t, self, 0x14) = a2[0];
    CSF_AT(uint32_t, self, 0x18) = a2[1];
    CSF_AT(uint32_t, self, 0x1C) = a2[2];
    return C_00860700_RefreshAfterSetPosition(self, nullptr);
}

HOOK(0x006BFEF0, C_00860700_SetPositionVector, "auto");
