#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __cdecl, Hud_UnregisterObject, 0x004754D0, (int a1));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __fastcall, ReleaseEmbeddedMember, 0x0044AFF0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// __thiscall with one stack argument (char flags), callee pops 4 bytes.
// Returns this; flags bit 0 requests freeing the object's own storage.
static void** __fastcall C_008311a0_Destroy(C_008311a0* self, void* /*edx*/, char flags) {
    *(void***)self = CSF_VTBL(0x008311A0);

    Hud_UnregisterObject(reinterpret_cast<int>(self));

    if (self->field_0018) {
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_0018));
    }
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0020 = 0;

    ReleaseEmbeddedMember(reinterpret_cast<uint8_t*>(self) + 4, nullptr);

    if ((flags & 1) != 0) {
        CoreMemory_Free(self);
    }

    return reinterpret_cast<void**>(self);
}

HOOK(0x0044B270, C_008311a0_Destroy, "auto");
