#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Callee: C_00840f48::IsActive, __thiscall (ecx = element, edx unused). Returns bool/char.
CSF_FUNC(char, __fastcall, C_00840f48_IsActive, 0x0051C0B0, (void* self, void* edx));

// Returns the first element of the container whose IsActive() is true, or 0 if none.
// Container layout (as read by the original): +0x04 = element array pointer, +0x0C = element count.
static int __fastcall GfxCameraProjection_FindFirstActiveElement(void* self, void* /*edx*/) {
    uint32_t index = 0;
    if (CSF_AT(uint32_t, self, 0x0C) == 0) {
        return 0;
    }
    while (true) {
        void** elements = reinterpret_cast<void**>(CSF_AT(uint32_t, self, 0x04));
        void* element = elements[index];
        if (C_00840f48_IsActive(element, nullptr) == 1) {
            return static_cast<int>(reinterpret_cast<uintptr_t>(element));
        }
        ++index;
        if (index >= CSF_AT(uint32_t, self, 0x0C)) {
            return 0;
        }
    }
}

HOOK(0x006F5750, GfxCameraProjection_FindFirstActiveElement, "auto");
