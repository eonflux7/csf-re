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

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __fastcall, C_00831618_AttachEntry, 0x00474440, (void* self, void* edx, int a2));

// Allocates a 68-byte entry, initializes its fields, attaches it to this owner
// and returns it. If allocation fails, attaches a null entry and returns 0.
static int __fastcall C_00831618_CreateAndAttach(C_00831618* self, void* /*edx*/)
{
    int allocated = CoreMemory_AllocUntagged(68);
    if (allocated != 0)
    {
        C_00831618* entry = reinterpret_cast<C_00831618*>(allocated);

        entry->field_000c = 0xFFFFFFFFu;
        entry->field_0010 = 0xFFFFFFFFu;
        entry->field_0008 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
        entry->field_0014 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
        entry->field_0018 = 0xFFFFFFFFu;
        entry->field_001c = 0xFFFFFFFFu;
        entry->field_0020 = 0;
        entry->field_0024 = 0;
        entry->field_002c = 0;
        entry->field_0028 = 0;
        entry->field_0030 = 0;
        entry->field_0004 = 0xFFFFFFFFu;
        *reinterpret_cast<void***>(entry) = CSF_VTBL(0x00831618);
        entry->field_0038 = 0;
        entry->field_003c = 0;
        entry->field_0040 = 0;

        C_00831618_AttachEntry(self, nullptr, allocated);
        return allocated;
    }

    C_00831618_AttachEntry(self, nullptr, 0);
    return 0;
}

HOOK(0x00474940, C_00831618_CreateAndAttach, "auto");
