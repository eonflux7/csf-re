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

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(uint32_t*, __fastcall, sub_6E6C20, 0x006E6C20, (void* self, void* edx));

// Base-object initializer for the 76-byte attachment controller (orig 0x7060C0).
static void* GfxAttachmentController_InitSmall(void* self) {
    sub_6E6C20(self, nullptr);
    *(void***)self = CSF_VTBL(0x00863260);
    return self;
}

// Base-object initializer for the 88-byte attachment controller (orig 0x7066C0).
static void* GfxAttachmentController_InitLarge(void* self) {
    sub_6E6C20(self, nullptr);
    *(void***)self = CSF_VTBL(0x008632C0);
    CSF_AT(int, self, 0x50) = 0;
    CSF_AT(int, self, 0x54) = -1;
    return self;
}

// Returns a freshly allocated attachment controller for the given kind (0 = small, 1 = large),
// or nullptr for an unsupported kind or a failed allocation.
static uint32_t* __cdecl GfxObject_GetAttachmentController(int a1) {
    uint32_t* result = nullptr;
    if (a1 == 0) {
        void* obj = reinterpret_cast<void*>(CoreMemory_AllocUntagged(76));
        if (obj) {
            return reinterpret_cast<uint32_t*>(GfxAttachmentController_InitSmall(obj));
        }
    } else if (a1 == 1) {
        void* obj = reinterpret_cast<void*>(CoreMemory_AllocUntagged(88));
        if (obj) {
            return reinterpret_cast<uint32_t*>(GfxAttachmentController_InitLarge(obj));
        }
    }
    return result;
}

HOOK(0x006E6AA0, GfxObject_GetAttachmentController, "auto");
