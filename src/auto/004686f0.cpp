#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Scene id getter (no args, __fastcall; ecx/edx unused).
CSF_FUNC(int, __fastcall, GetCurrentSceneIndex, 0x0045AA50, (void* self, void* edx));

// Searches the record table of C_00833b88 for an entry whose scene index
// equals sceneIndex and whose inner record's +4 field equals recordKey.
// Returns the matching record pointer, or 0 when nothing matches.
static int __fastcall C_00833b88_FindStateRecord(C_00833b88* self, void* /*edx*/, int sceneIndex, int recordKey)
{
    uint32_t recordCount = self->field_0030;
    if (recordCount == 0)
        return 0;

    for (uint32_t index = 0; index < recordCount; ++index) {
        uint32_t recordPtr = reinterpret_cast<uint32_t*>(self->field_0028)[index];
        if (recordPtr == 0)
            continue;

        // mov ecx, [eax + esi*4]; call 0x45AA50: the record is the call's `this`
        if (GetCurrentSceneIndex(reinterpret_cast<void*>(recordPtr), nullptr) != sceneIndex)
            continue;

        // The scene call may modify the table; reload the entry before reading it.
        recordPtr = reinterpret_cast<uint32_t*>(self->field_0028)[index];
        uint8_t* innerRecord = CSF_AT(uint8_t*, reinterpret_cast<uint8_t*>(recordPtr), 0x10);
        if (CSF_AT(int, innerRecord, 4) != recordKey)
            continue;

        return static_cast<int>(reinterpret_cast<uint32_t*>(self->field_0028)[index]);
    }
    return 0;
}

HOOK(0x004686F0, C_00833b88_FindStateRecord, "auto");

