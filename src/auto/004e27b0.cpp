#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(char, __fastcall, MpSpawnPoints_UpdateRelatedObject, 0x004E1E40, (void* self, void* edx));

// 0x004E27B0: updates the related spawn-points object, then tail-jumps to
// the object's vtable slot 0x9C with the caller's stack arguments untouched.
static hook::Tail __fastcall C_0083e8c8_UpdateRelatedObjectAndTailSlot9C(void* self, const uint32_t* args) {
    (void)args;
    MpSpawnPoints_UpdateRelatedObject(self, nullptr);
    // The original reloads [esi] after the call, so the vtable is read here, after the call.
    return hook::tail_jump(self, (*reinterpret_cast<void***>(self))[0x9C / 4]);
}

HOOK_TAIL(0x004E27B0, C_0083e8c8_UpdateRelatedObjectAndTailSlot9C, "auto");
