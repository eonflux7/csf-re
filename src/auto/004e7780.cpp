#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Walks the 12-byte entry table at field_0090 (field_0098 entries). Any entry whose
// second dword is not -1 and no longer resolves in the handle table is reset to -1.
static int __fastcall C_0083eb70_InvalidateStaleHandles(C_0083eb70* self, void* /*edx*/)
{
    int result = static_cast<int>(self->field_0098);
    if (result != 0) {
        uint32_t remaining = self->field_0098;
        uint32_t offset = 0;
        do {
            uint8_t* table = reinterpret_cast<uint8_t*>(self->field_0090);
            int32_t slotValue = CSF_AT(int32_t, table, offset + 4);
            uint32_t handle = self->field_0090 + offset;
            if (slotValue != -1 &&
                CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(handle)) == 0) {
                uint8_t* entry = reinterpret_cast<uint8_t*>(self->field_0090 + offset);
                CSF_AT(int32_t, entry, 4) = -1;
                CSF_AT(int32_t, entry, 8) = -1;
            }
            offset += 12;
            result = static_cast<int>(--remaining);
        } while (remaining != 0);
    }
    return result;
}

HOOK(0x004E7780, C_0083eb70_InvalidateStaleHandles, "auto");
