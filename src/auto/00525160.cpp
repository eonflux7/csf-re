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

// Bound original callee: resets and releases the slot array embedded at +0x28.
CSF_FUNC(int, __fastcall, ResetAndReleaseObjectSlots, 0x00525060, (void* self, void*));

// Releases three owned child objects (at +0x0C, +0x08, +0x10) through their
// virtual release slot (vtable offset 0x4C), clearing each pointer afterwards.
static int __fastcall SoundObject_ReleaseChildren(void* self, void* /*edx*/)
{
    int result = ResetAndReleaseObjectSlots(static_cast<uint8_t*>(self) + 0x28, nullptr);

    void* child_0C = CSF_AT(void*, self, 0x0C);
    if (child_0C) {
        result = ((int(__fastcall*)(void*, void*))CSF_AT(void**, child_0C, 0)[0x4C / 4])(child_0C, nullptr);
        CSF_AT(void*, self, 0x0C) = nullptr;
    }

    void* child_08 = CSF_AT(void*, self, 0x08);
    if (child_08) {
        result = ((int(__fastcall*)(void*, void*))CSF_AT(void**, child_08, 0)[0x4C / 4])(child_08, nullptr);
        CSF_AT(void*, self, 0x08) = nullptr;
    }

    void* child_10 = CSF_AT(void*, self, 0x10);
    if (child_10) {
        result = ((int(__fastcall*)(void*, void*))CSF_AT(void**, child_10, 0)[0x4C / 4])(child_10, nullptr);
        CSF_AT(void*, self, 0x10) = nullptr;
    }

    return result;
}

HOOK(0x00525160, SoundObject_ReleaseChildren, "auto");
