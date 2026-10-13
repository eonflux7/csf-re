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
CSF_FUNC(int, __cdecl, sub_7DE320, 0x007DE320, (int a1, int a2, int a3));

// Queries the item count via vtable slot 0xB4, allocates a uint32 array, fills it
// by calling vtable slot 0xB0 once per element with the object's field_0004 as the
// argument, then hands the array to sub_7DE320. Returns whether the count is > 0.
static int __fastcall Audio_QueryAndStoreEffectList(C_00863ba8* self, void* /*edx*/) {
    using CountFn = int(__fastcall*)(C_00863ba8*, void*);
    using ItemFn = int(__fastcall*)(C_00863ba8*, void*, uint32_t);

    auto getCount = reinterpret_cast<CountFn>(CSF_AT(void**, self, 0)[0xb4 / 4]);
    int count = getCount(self, nullptr);
    self->field_0010 = static_cast<uint32_t>(count);

    if (count > 0) {
        self->field_000c = static_cast<uint32_t>(CoreMemory_AllocUntagged(4 * static_cast<std::size_t>(count)));

        int i = 0;
        while (i < static_cast<int>(self->field_0010)) {
            auto getItem = reinterpret_cast<ItemFn>(CSF_AT(void**, self, 0)[0xb0 / 4]);
            int item = getItem(self, nullptr, self->field_0004);
            reinterpret_cast<uint32_t*>(self->field_000c)[i] = static_cast<uint32_t>(item);
            ++i;
        }

        sub_7DE320(static_cast<int>(self->field_0004),
                   static_cast<int>(self->field_000c),
                   static_cast<int>(self->field_0010));
    }

    return static_cast<int>(self->field_0010) > 0 ? 1 : 0;
}

HOOK(0x0070F160, Audio_QueryAndStoreEffectList, "auto");
