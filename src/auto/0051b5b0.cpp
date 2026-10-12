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

// Callee: __fastcall(ecx = self, stack: value, out pointer) -> ret 8
CSF_FUNC(int, __fastcall, sub_519EC0, 0x00519EC0, (void* self, void* edx, int a2, uint32_t* a3));
// Callee: __thiscall(ecx = self, stack: index) -> ret 4
CSF_FUNC(int, __fastcall, sub_78BB40, 0x0078BB40, (void* self, void* edx, int a2));

static int __fastcall C_00840698_LoadElementAndRefresh(C_00840698* self, void* /*edx*/, int a2)
{
    // Stack slot [ebp-8]: 8 bytes of room for the callee's out value.
    alignas(8) uint8_t scratchBytes[8];
    uint32_t* outValue = reinterpret_cast<uint32_t*>(scratchBytes);

    // Element table: element a2 is at table_base + a2 * 164 (0xA4).
    uint32_t tableBase = CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0004), 4);
    uint32_t elementAddress = tableBase + static_cast<uint32_t>(a2) * 164u;

    sub_519EC0(reinterpret_cast<uint8_t*>(self) + 0x08, nullptr, static_cast<int>(elementAddress), outValue);

    return sub_78BB40(self, nullptr, a2);
}

HOOK(0x0051B5B0, C_00840698_LoadElementAndRefresh, "auto");
