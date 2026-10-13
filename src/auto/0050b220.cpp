#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// __cdecl, no stack arguments (ret 0 in the original).
CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
// __thiscall callee bound as __fastcall; arg is the C_008300c0 control object.
CSF_FUNC(C_00860130*, __fastcall, C_008300c0_ApplyControlBindings, 0x00428EC0, (C_008300c0* self, void* edx));

// Resets the 32-entry binding table embedded at +0x04 and hands it to the global control object.
static uint32_t* __fastcall S_0050b220_InitDefaultBindings(S_0050b220* self, void* /*edx*/) {
    self->field_0000 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x83FEAC)));

    uint8_t* entry = self->pad_0004;
    for (int entryIndex = 32; entryIndex != 0; --entryIndex) {
        CSF_AT(int32_t, entry, 0x00) = -1;
        CSF_AT(int32_t, entry, 0x04) = -1;
        CSF_AT(int32_t, entry, 0x08) = 0;
        CSF_AT(float, entry, 0x0C) = 1.0f;
        CSF_AT(int32_t, entry, 0x10) = 0;
        entry += 20;
    }

    uint8_t* globalObject = reinterpret_cast<uint8_t*>(GetGlobalValue_8B056C());
    int controlObject = CSF_AT(int, globalObject, 0x1C);
    if (controlObject != 0) {
        void* controlObj = reinterpret_cast<void*>(static_cast<uintptr_t>(controlObject));
        // Slot 3 (+0x0C) of the control object's vtable, __thiscall(this, table, 0).
        ((void(__fastcall*)(void*, void*, uint8_t*, int))CSF_AT(void**, controlObj, 0)[3])(
            controlObj, nullptr, self->pad_0004, 0);
        C_008300c0_ApplyControlBindings(reinterpret_cast<C_008300c0*>(controlObj), nullptr);
    }

    return &self->field_0000;
}

HOOK(0x0050B220, S_0050b220_InitDefaultBindings, "auto");
