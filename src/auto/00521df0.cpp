#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall S_00521df0_ApplyPairIfNotUnity(S_00521df0* self, void* /*edx*/, int a2) {
    void* target = reinterpret_cast<void*>(a2);

    // Vtable slot 0x28 / 4 = 10: reset call with two zero arguments.
    ((void(__fastcall*)(void*, void*, uint32_t, uint32_t))CSF_AT(void**, target, 0)[10])(target, nullptr, 0u, 0u);

    int result = 1065353216;
    if (self->field_0060 != 1065353216u || self->field_0064 != 1065353216u) {
        // Vtable slot 0x68 / 4 = 26: apply the stored pair. The vtable is re-read here, as in the original.
        return ((int(__fastcall*)(void*, void*, uint32_t, uint32_t))CSF_AT(void**, target, 0)[26])(
            target, nullptr, self->field_0060, self->field_0064);
    }
    return result;
}

HOOK(0x00521DF0, S_00521df0_ApplyPairIfNotUnity, "auto");
