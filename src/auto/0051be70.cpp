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

CSF_FUNC(void, __fastcall, BddTables_RegisterIndexSlot, 0x00519EC0, (void* self, void* edx, int a2, uint32_t* a3));
CSF_FUNC(bool, __fastcall, BddTables_RefreshEntry, 0x0051BC90, (void* self, void* edx, int a2));

// __thiscall, one stack arg, ret 4.
static bool __fastcall C_008406b0_SetCurrentIndexAndRefresh(C_008406b0* self, void* /*edx*/, int a2)
{
    int index = a2;
    if (a2 != -1) {
        // Stack temp for the callee to write through (room: 8 bytes).
        alignas(8) uint8_t slotBytes[8];
        void* subobject = reinterpret_cast<uint8_t*>(self) + 0x18;
        BddTables_RegisterIndexSlot(
            subobject,
            nullptr,
            static_cast<int>(reinterpret_cast<uintptr_t>(slotBytes)),
            reinterpret_cast<uint32_t*>(&a2));
    }
    return BddTables_RefreshEntry(self, nullptr, index);
}

HOOK(0x0051BE70, C_008406b0_SetCurrentIndexAndRefresh, "auto");
