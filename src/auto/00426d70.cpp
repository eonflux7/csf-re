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

CSF_FUNC(void, __fastcall, S_00426550_RunInitStep, 0x00426550, (void* self, void* edx));

// The original references S_00426550's vtable at 0x426550 (see packet); keep the binding in the file.
static const void* const kS_00426550_VtblRef = CSF_VTBL(0x00426550);

static uint32_t* __fastcall S_00426550_FindPendingEntry(S_00426550* self, void* /*edx*/)
{
    if (self->field_000c < 2)
    {
        ++self->field_0024;
        S_00426550_RunInitStep(self, nullptr);
    }

    // The count is re-read after the possible call above.
    const uint32_t count = self->field_000c;

    if (count <= 1)
    {
        // Fall through to the block below.
    }
    else
    {
        uint8_t* child = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_0004));
        uint32_t* childEntry = reinterpret_cast<uint32_t*>(child + 0x24);
        if (CSF_AT(uint32_t, child, 0x24) != 1)
        {
            return childEntry;
        }
    }

    // Block: count <= 1, or child's +0x24 entry equals 1.
    if (count <= 2)
    {
        return nullptr;
    }

    uint8_t* child = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_0004));
    uint32_t* result = reinterpret_cast<uint32_t*>(child + 0x48);
    if (CSF_AT(uint32_t, child, 0x48) == 1)
    {
        return nullptr;
    }
    return result;
}

HOOK(0x00426D70, S_00426550_FindPendingEntry, "auto");
