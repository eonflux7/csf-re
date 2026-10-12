#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Copies three dwords from the caller's array into the object at +0x14 and sets the dirty flag at +0x3C.
// Returns the address of the copied triple (this + 0x14).
static int __fastcall CameraProjection_SetVec3AndMarkDirty(void* self, void* /*edx*/, const uint32_t* a2) {
    CSF_AT(uint32_t, self, 0x14) = a2[0];
    CSF_AT(uint32_t, self, 0x18) = a2[1];
    CSF_AT(uint32_t, self, 0x1C) = a2[2];
    CSF_AT(uint8_t, self, 0x3C) = 1;
    return static_cast<int>(reinterpret_cast<uintptr_t>(self) + 0x14);
}

HOOK(0x6F5BB0, CameraProjection_SetVec3AndMarkDirty, "auto");
