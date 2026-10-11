#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callees bound by address (signatures per the packet).
CSF_FUNC(int, __fastcall, sub_655E40, 0x00655E40, (void* self, void* edx, char* a2));
CSF_FUNC(uint32_t*, __fastcall, sub_655F00, 0x00655F00, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, sub_73FFC0, 0x0073FFC0, (void* self, void* edx, uint16_t a2, int a3));

// __thiscall(CMenuMpStats* this, C_008622e8* arg0) with ret 4.
static void __fastcall CMenuMpStats_HandleListboxSelection(CMenuMpStats* self, void* /*edx*/, C_008622e8* arg0)
{
    uint16_t selectedIndex[2];

    // Fill selectedIndex from arg0 (callee writes up to 4 bytes).
    sub_655E40(arg0, nullptr, reinterpret_cast<char*>(selectedIndex));

    // Second call receives the address of the arg0 parameter slot.
    sub_655F00(arg0, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(&arg0)));

    sub_73FFC0(self, nullptr, selectedIndex[0], static_cast<int>(reinterpret_cast<uintptr_t>(arg0)));
}

HOOK(0x007427E0, CMenuMpStats_HandleListboxSelection, "auto");
