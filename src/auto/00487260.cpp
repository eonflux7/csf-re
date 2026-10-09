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

// Callee: __thiscall with one stack argument (ret 4), bound as given in the packet.
CSF_FUNC(int, __fastcall, sub_485A20, 0x00485A20, (void* self, void* edx, int a2));

// Returns the item stored in slot 28 of the action list, or 0 when that slot is
// empty or the item's flag at +0x80 is clear.
static int __fastcall CCharActionFsm_GetSlot28ItemIfUsable(CCharActionFsm* self, void* /*edx*/)
{
    int slotIndex = sub_485A20(self, nullptr, 28);
    if (slotIndex == -1) {
        return 0;
    }

    // Element is read after the call, as the original reloads field_0078 here.
    uint32_t itemValue = reinterpret_cast<uint32_t*>(self->field_0078)[slotIndex];
    uint8_t* item = reinterpret_cast<uint8_t*>(itemValue);

    if (CSF_AT(uint32_t, item, 0x80) == 0) {
        return 0;
    }
    return static_cast<int>(itemValue);
}

HOOK(0x00487260, CCharActionFsm_GetSlot28ItemIfUsable, "auto");
