#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Stores the C_0082d410 vtable pointer at +0x1C of each 40-byte entry in [first, end).
static int __stdcall HudStatusIcons_SetVtableRange(int first, int end) {
    int cursor = first;
    for (; cursor != end; cursor += 40) {
        CSF_AT(uint32_t, reinterpret_cast<void*>(cursor), 0x1C) =
            static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x0082D410)));
    }
    return cursor;
}

HOOK(0x458E20, HudStatusIcons_SetVtableRange, "auto");
