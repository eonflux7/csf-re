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

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* /*edx*/, int additionalCapacity));

// Vtable of the embedded S_0041e5f0 list object (referenced via its grow routine).
[[maybe_unused]] static const auto s_ArrayListVtable = CSF_VTBL(0x0041E5F0);

static int __fastcall GfxObjectInstances_ShouldSkipRendering(void* self, void* /*edx*/, int a2)
{
    if (CSF_AT(uint8_t, self, 0x48) == 0)
    {
        // Embedded array object at +0x38: [0]=data, [4]=capacity, [8]=count, [0xC]=growth step.
        S_0041e5f0* list = reinterpret_cast<S_0041e5f0*>(static_cast<uint8_t*>(self) + 0x38);

        if (CSF_AT(uint32_t, self, 0x40) >= CSF_AT(uint32_t, self, 0x3C))
        {
            S_0041e5f0_GrowArrayCapacity(list, nullptr, CSF_AT(int, self, 0x44));
        }

        uint32_t index = CSF_AT(uint32_t, list, 0x8);
        uint8_t* dataBase = CSF_AT(uint8_t*, list, 0x0);
        CSF_AT(int, dataBase, static_cast<uint32_t>(index * 4)) = a2;
        CSF_AT(uint32_t, list, 0x8) = index + 1;
        return 1;
    }

    return CSF_AT(uint8_t, self, 0x49) != 0 ? 1 : 0;
}

HOOK(0x006D5F60, GfxObjectInstances_ShouldSkipRendering, "auto");
