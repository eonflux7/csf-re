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

// Returns the entry at index m_idUp (offset 0x70) of the table at 0x60, whose
// length is at 0x68; returns 0 when the index is -1 or out of range.
static int __fastcall CGuiControl_GetSceneMenuContext(CGuiControl* self, void* /*edx*/) {
    uint32_t index = CSF_AT(uint32_t, self, 0x70);
    int result = 0;
    if (index != 0xFFFFFFFFu && index < CSF_AT(uint32_t, self, 0x68)) {
        uint32_t* table = CSF_AT(uint32_t*, self, 0x60);
        result = static_cast<int>(table[index]);
    }
    return result;
}

HOOK_KEEP_ECX(0x0044FC60, CGuiControl_GetSceneMenuContext, 0, "game_scene_access");
