#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_GLOBAL(float, g_fZero, 0x82D4CC);

// Applies a flag to three attached child objects (vtable slots 15, 13, 11).
// When the low byte of a2 is nonzero, it is replaced by (field_0024 > 0.0f).
static int __fastcall ApplyFlagToChildren(void* self, void* /*edx*/, int a2) {
    int flag = a2;
    if (static_cast<uint8_t>(flag)) {
        float threshold = CSF_AT(float, self, 0x24);
        int positive = (threshold > g_fZero) ? 1 : 0;
        flag = (flag & ~0xFF) | positive;
    }

    uint32_t childAt8 = CSF_AT(uint32_t, self, 0x8);
    if (childAt8) {
        void* child = reinterpret_cast<void*>(childAt8);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, child, 0)[0x3C / 4])(child, nullptr, flag);
    }

    uint32_t childAt4 = CSF_AT(uint32_t, self, 0x4);
    if (childAt4) {
        void* child = reinterpret_cast<void*>(childAt4);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, child, 0)[0x34 / 4])(child, nullptr, flag);
    }

    int result = 0;
    uint32_t childAt12 = CSF_AT(uint32_t, self, 0xC);
    if (childAt12) {
        void* child = reinterpret_cast<void*>(childAt12);
        result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, child, 0)[0x2C / 4])(child, nullptr, flag);
    }
    return result;
}

HOOK(0x6C86E0, ApplyFlagToChildren, "auto");
