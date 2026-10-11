#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original: CHandleTable::Resolve(this=g_pCurrentScene, handle=&this->field_0118), __fastcall-style callee with one stack arg (ret 4).
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Returns CHandleTable::Resolve(g_pCurrentScene, address of the handle slot at +0x118)
// when the enabled flag at +0x115 is set; otherwise returns 0.
static int __fastcall C_0082dfd0_ResolveHandleIfFlagged(C_0082dfd0* self, void* /*edx*/) {
    int result = 0;
    if (self->field_0115) {
        int handleSlotAddress = static_cast<int>(reinterpret_cast<uintptr_t>(self) + 0x118);
        return CHandleTable_Resolve(g_pCurrentScene, nullptr, handleSlotAddress);
    }
    return result;
}

HOOK(0x411E50, C_0082dfd0_ResolveHandleIfFlagged, "auto");
