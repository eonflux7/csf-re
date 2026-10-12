#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, S_00439f80_GrowByDelta, 0x00439F80, (void* self, void* edx, int a2));

static const void* const s_S_00439f80_VtableRef = CSF_VTBL(0x00439F80);

static int __fastcall S_00439f80_EnsureCapacity(S_00439f80* self, void* /*edx*/, unsigned int requestedCount) {
    uint32_t currentCapacity = self->field_0004;
    int result = static_cast<int>(requestedCount);
    if (requestedCount > currentCapacity)
        return S_00439f80_GrowByDelta(self, nullptr, static_cast<int>(requestedCount - currentCapacity));
    return result;
}

HOOK(0x43A160, S_00439f80_EnsureCapacity, "auto");
