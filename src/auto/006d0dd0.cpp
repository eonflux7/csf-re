#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// The binary replaces only AL, forwarding the other 24 return bits unchanged.
static int __fastcall C_00860e68_InvokeNegatedQuery(C_00860e68* self, void*) {
    auto* table = self->vtbl;
    using Query = uint32_t (__thiscall*)(C_00860e68*);
    using Set = int (__thiscall*)(C_00860e68*, uint32_t);
    auto** slots = reinterpret_cast<void**>(table);
    uint32_t value = reinterpret_cast<Query>(slots[0x68/4])(self);
    value = (value & 0xffffff00u) | ((value & 0xffu) == 0);
    return reinterpret_cast<Set>(slots[0x64/4])(self, value);
}
HOOK(0x006D0DD0, C_00860e68_InvokeNegatedQuery, "auto");
