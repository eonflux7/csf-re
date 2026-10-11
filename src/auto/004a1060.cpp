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

CSF_FUNC(unsigned int, __fastcall, S_00413570_GrowEventRecords, 0x00413570,
         (S_00413570* self, void* edx, int additionalCapacity));

// Referenced vtable of the embedded event-record container (S_00413570 at self+4).
[[maybe_unused]] static auto kEventRecordsVtbl = CSF_VTBL(0x00413570);

// Appends a 12-byte event record (copies record+4 and record+8 into the new slot)
// to the container embedded at self+4, growing it first when full.
static int __fastcall SceneLoader_AssociateEntryRecord(void* self, void* edx, const void* record)
{
    (void)edx;

    // Embedded container: S_00413570 lives at self+4.
    auto* eventRecords = reinterpret_cast<S_00413570*>(static_cast<uint8_t*>(self) + 4);

    if (CSF_AT(int, self, 0x0C) >= CSF_AT(int, self, 0x08))
    {
        S_00413570_GrowEventRecords(eventRecords, nullptr, CSF_AT(int, self, 0x10));
    }

    // Reload the count and base after a possible grow.
    int count = CSF_AT(int, self, 0x0C);
    uintptr_t base = static_cast<uintptr_t>(CSF_AT(int, self, 0x04));
    CSF_AT(int, self, 0x0C) = count + 1;

    uint8_t* entry = reinterpret_cast<uint8_t*>(base + 12u * static_cast<uintptr_t>(count));
    CSF_AT(int, entry, 0x04) = CSF_AT(int, record, 0x04);
    int result = CSF_AT(int, record, 0x08);
    CSF_AT(int, entry, 0x08) = result;
    return result;
}

HOOK(0x004A1060, SceneLoader_AssociateEntryRecord, "auto");
