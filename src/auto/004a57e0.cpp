#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Helper: the original keeps 32-bit integer results that are really pointers.
static inline void* ReturnedIntToPtr(int value) {
    return reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(value)));
}

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __fastcall, C_008344f0_ResetBaseState, 0x004A4030, (void* self, void* edx));

// Resets the C_00834598 object: restores its vtables, stops the scene it
// was attached to, frees owned buffers, then tail-jumps into the base reset.
static int __fastcall C_00834598_Reset(C_00834598* self, void* /*edx*/) {
    self->vtbl = CSF_VTBL(0x00834598);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x00834578));

    if (self->field_008c != 0) {
        void* globalInt = ReturnedIntToPtr(GetGlobalInt());
        void* sceneController = ReturnedIntToPtr(SceneLoader_GetSceneController(globalInt, nullptr));
        // Virtual slot 0x18 / 4 = 6 on the scene controller: takes the stored id.
        using SceneStopFn = void(__fastcall*)(void* self, void* edx, uint32_t id);
        SceneStopFn stopScene = reinterpret_cast<SceneStopFn>(CSF_AT(void**, sceneController, 0)[6]);
        stopScene(sceneController, nullptr, self->field_008c);
    }
    self->field_008c = 0;

    if (self->field_00a8 >= 0x10 && self->field_0094 != 0) {
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_0094));
    }
    self->field_00a8 = 15;
    self->field_00a4 = 0;
    CSF_AT(uint8_t, self, 0x94) = 0;

    if (self->field_007c != 0) {
        Mem_Free(reinterpret_cast<void*>(self->field_007c));
        self->field_007c = 0;
    }

    // Tail jump: ecx = this, args unchanged.
    return C_008344f0_ResetBaseState(self, nullptr);
}

HOOK(0x4A57E0, C_00834598_Reset, "auto");
