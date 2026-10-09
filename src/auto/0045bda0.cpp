#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x8AFC90);
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));

// Returns non-zero when the handle stored at +0x04 still resolves to a live object
// whose flags (+0x170) have bit 0x10 set and whose linked child (+0x34), if any,
// does not carry flag 0x02 in its +0x18 byte. Falls back to the cached flag at +0x80.
static char __fastcall IsTrackedHandleActive(S_0045bda0* self, void* /*edx*/)
{
    if ((self->field_000c & 0x80000000u) != 0)
        return static_cast<char>(self->field_0080);

    int handle = static_cast<int>(reinterpret_cast<uintptr_t>(reinterpret_cast<uint8_t*>(self) + 4));
    int resolvedObject = CHandleTable_Resolve(g_pCurrentScene, nullptr, handle);
    if (resolvedObject == 0)
        return static_cast<char>(self->field_0080);

    if (self->field_0080 != 0) {
        void* objectPtr = reinterpret_cast<void*>(static_cast<uintptr_t>(resolvedObject));
        if ((CSF_AT(uint8_t, objectPtr, 0x170) & 0x10) != 0) {
            uint32_t linkedObject = CSF_AT(uint32_t, objectPtr, 0x34);
            if (linkedObject == 0) {
                return 1;
            }
            void* linkedPtr = reinterpret_cast<void*>(static_cast<uintptr_t>(linkedObject));
            if ((CSF_AT(uint8_t, linkedPtr, 0x18) & 0x02) != 0) {
                return 1;
            }
        }
    }
    return 0;
}

HOOK(0x0045BDA0, IsTrackedHandleActive, "auto");
