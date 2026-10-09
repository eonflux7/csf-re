#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));

static void* __fastcall S_0041e5f0_EnsureButtonCapacity(S_0041e5f0* self, void* /*edx*/, void* requestedCount) {
    uint32_t currentCapacity = self->field_0004;
    uintptr_t requested = reinterpret_cast<uintptr_t>(requestedCount);
    void* result = requestedCount;
    if (requested > currentCapacity)
        return GrowButtonArray(self, nullptr, static_cast<int>(requested - currentCapacity));
    return result;
}

HOOK(0x42FB10, S_0041e5f0_EnsureButtonCapacity, "auto");
