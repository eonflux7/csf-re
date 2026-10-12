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

// Walks the child list stored at field_0004 (count field_000c) and asks each child,
// through its vtable slot 4 (+0x10), whether it accepts the four forwarded arguments.
// Returns true as soon as one child accepts; false if none does (or the list is empty).
static bool __fastcall C_008627d8_AnyChildAccepts(C_008627d8* self, void* /*edx*/,
                                                  int a2, int a3, int a4, int a5) {
    bool result = false;
    uint32_t count = self->field_000c;
    if (count == 0) {
        return result;
    }
    for (uint32_t i = 0; i < count; ++i) {
        // The child list pointer is re-read on every iteration, as the original does.
        void* child = reinterpret_cast<void*>(reinterpret_cast<uint32_t*>(self->field_0004)[i]);
        // Slot 0x10 / 4 = 4 in the child's vtable; the child is __thiscall with 4 stack args.
        bool accepted = ((bool(__fastcall*)(void*, void*, int, int, int, int))
                            CSF_AT(void**, child, 0)[4])(child, nullptr, a2, a3, a4, a5);
        if (accepted) {
            return true;
        }
    }
    return false;
}

HOOK(0x6F5830, C_008627d8_AnyChildAccepts, "auto");
