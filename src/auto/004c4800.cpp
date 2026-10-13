#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_4C4700, 0x004C4700, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiCombat_SelectBestOrder, 0x004B8980, (void* self, void* , uint32_t* output));
CSF_FUNC(int, __fastcall, sub_4AD990, 0x004AD990, (void* self, void* edx, uint32_t* a2));

// Vtable of S_004ad990, referenced by the original near its call to sub_4AD990.
[[maybe_unused]] static const void* const kS004adVtbl = CSF_VTBL(0x004AD990);

// Vtable of C_0082d448, stored into the order slots (a2[3] and a2[9]).
static uint32_t GetOrderSlotVtableValue() {
    return static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x0082D448)));
}

static char __fastcall S_004ad990_TryGoToCover(S_004ad990* self, void* /*edx*/) {
    // Order-slot array laid out in the original frame: 13 dwords (ebp-0x34 .. ebp-0x4).
    uint32_t orderSlots[13];

    orderSlots[3] = GetOrderSlotVtableValue();
    orderSlots[9] = GetOrderSlotVtableValue();
    orderSlots[6] = 0;
    orderSlots[7] = 0;
    orderSlots[8] = 0;
    orderSlots[0] = 0xFFFFFFFFu;
    orderSlots[1] = 0xFFFFFFFFu;
    orderSlots[2] = 0xFFFFFFFFu;
    orderSlots[4] = 0xFFFFFFFFu;
    orderSlots[5] = 0xFFFFFFFFu;
    orderSlots[10] = 0xFFFFFFFFu;
    orderSlots[11] = 0xFFFFFFFFu;   // v4 (best order index), -1 until selected
    orderSlots[12] = 0;             // v5

    void* subObject = reinterpret_cast<uint8_t*>(self) + 4;

    if (!sub_4C4700(subObject, nullptr)) {
        return 0;
    }

    AiCombat_SelectBestOrder(subObject, nullptr, orderSlots);

    if (static_cast<int>(orderSlots[11]) < 0) {
        return 0;
    }

    sub_4AD990(self, nullptr, orderSlots);
    return 1;
}

HOOK(0x004C4800, S_004ad990_TryGoToCover, "auto");
