#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callees bound by address (signatures per the packet).
CSF_FUNC(int, __fastcall, sub_655E40, 0x00655E40, (void* self, void* edx, char* a2));
CSF_FUNC(uint32_t*, __fastcall, sub_655F00, 0x00655F00, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, sub_740020, 0x00740020, (void* self, void* edx, uint16_t a2, int a3));

// __thiscall(CMenuMpStats* this, C_008622e8* arg0) with ret 4.
static void __fastcall CMenuMpStats_ForwardListboxSelection(CMenuMpStats* self, void* /*edx*/, C_008622e8* arg0)
{
    // 4-byte stack buffer (ebp-4), filled by the first callee.
    uint16_t selectedIndex[2];

    // First call fills the selection index buffer from arg0.
    sub_655E40(arg0, nullptr, reinterpret_cast<char*>(selectedIndex));

    // Second call receives the address of the arg0 parameter slot.
    sub_655F00(arg0, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(&arg0)));

    // Forward the selection index and arg0 to the target-specific handler.
    sub_740020(self, nullptr, selectedIndex[0], static_cast<int>(reinterpret_cast<uintptr_t>(arg0)));
}

HOOK(0x007428E0, CMenuMpStats_ForwardListboxSelection, "auto");
