#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Forwards `value` to the object stored in slot `index` of the table at self+0x0C,
// through that object's vtable slot 14 (+0x38). Returns `value`.
static int __fastcall IndexedChildForwardValue(void* self, void* /*edx*/, int value, int index) {
    uint8_t* table = CSF_AT(uint8_t*, self, 0x0C);
    void* childObject = CSF_AT(void*, table, 28 * index);
    ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, childObject, 0)[0x38 / 4])(childObject, nullptr, value);
    return value;
}

HOOK(0x004084E0, IndexedChildForwardValue, "auto");
