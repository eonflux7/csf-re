#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Callee: moves/builds the element at a3+40 from the slot list (original 0x4583D0, cdecl, 3 args).
CSF_FUNC(uint32_t*, __cdecl, sub_4583D0, 0x004583D0, (char* a1, char* a2, uint32_t* a3));

// __thiscall: self = list object (self+8 = end pointer), a2 = out pointer, a3 = source record.
static uint32_t* __fastcall HudStatusIcons_PopLastSlotInto(void* self, void* /*edx*/, uint32_t* outResult, uint32_t a3)
{
    // First read of the end pointer (passed as the 2nd argument to the callee).
    uint8_t* listEnd = CSF_AT(uint8_t*, self, 0x08);
    sub_4583D0(reinterpret_cast<char*>(a3 + 40), reinterpret_cast<char*>(listEnd), reinterpret_cast<uint32_t*>(a3));

    // Re-read the end pointer after the call; stamp the vtable into the last slot.
    listEnd = CSF_AT(uint8_t*, self, 0x08);
    uint8_t* slot = listEnd - 40;
    do
    {
        CSF_AT(void*, slot, 0x1C) = CSF_VTBL(0x0082D410);
        slot += 40;
    } while (slot != listEnd);

    // Shrink the list by one 40-byte element.
    CSF_AT(uint8_t*, self, 0x08) = CSF_AT(uint8_t*, self, 0x08) - 40;

    *outResult = a3;
    return outResult;
}

HOOK(0x00458FE0, HudStatusIcons_PopLastSlotInto, "auto");
