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

// Game heap free (cdecl, 1 stack arg).
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Releases the object stored in one slot of the table (__fastcall, edx unused).
CSF_FUNC(void, __fastcall, ReleaseSlotObject, 0x006A9F80, (void* self, void* edx));

// Frees the slot table at this->field_0328 (up to 10 dword slots of 40 bytes),
// releasing each non-null entry's object, then the table itself.
// Returns the result of the table's Mem_Free, or 0 when there is no table.
static int __fastcall C_0085fe28_FreeSlotTable(C_0085fe28* self, void* /*edx*/)
{
    int result = 0;
    if (self->field_0328 == 0) {
        return result;
    }

    for (int i = 0; i < 40; i += 4) {
        uint32_t entry = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(self->field_0328), i);
        if (entry) {
            ReleaseSlotObject(reinterpret_cast<void*>(entry), nullptr);
            Mem_Free(reinterpret_cast<void*>(entry));
            CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(self->field_0328), i) = 0;
        }
    }

    result = static_cast<int>(self->field_0328);
    if (result) {
        result = Mem_Free(reinterpret_cast<void*>(self->field_0328));
        self->field_0328 = 0;
    }
    return result;
}

HOOK(0x006AA6F0, C_0085fe28_FreeSlotTable, "auto");
