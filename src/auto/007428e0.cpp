#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callees bound by address (signatures per the packet).
CSF_FUNC(int, __fastcall, sub_655E40, 0x00655E40, (void* self, void* edx, char* a2));
CSF_FUNC(uint32_t*, __fastcall, sub_655F00, 0x00655F00, (void* self, void* edx, int a2));
// a2 is pushed as a full dword (the original pushes the whole [ebp-4] slot), so it is bound as 32 bits.
CSF_FUNC(void, __fastcall, sub_740020, 0x00740020, (void* self, void* edx, uint32_t a2, int a3));

// __thiscall(CMenuMpStats* this, C_008622e8* arg0) with ret 4.
static void __fastcall CMenuMpStats_ForwardListboxSelection(CMenuMpStats* self, void* /*edx*/, C_008622e8* arg0)
{
    // 4-byte stack slot (ebp-4). The prologue's `push ecx` stores ecx (this) into it; the first callee writes
    // only its low word (a 16-bit store), so the high word keeps this's high half.
    uint32_t selectedIndex = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(self));

    // First call fills the low word of the selection index slot from arg0.
    sub_655E40(arg0, nullptr, reinterpret_cast<char*>(&selectedIndex));

    // Second call receives the address of the arg0 parameter slot.
    sub_655F00(arg0, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(&arg0)));

    // Forward the whole slot and arg0 to the target-specific handler.
    sub_740020(self, nullptr, selectedIndex, static_cast<int>(reinterpret_cast<uintptr_t>(arg0)));
}

HOOK(0x007428E0, CMenuMpStats_ForwardListboxSelection, "auto");
