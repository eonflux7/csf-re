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

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(void, __fastcall, ProcessAltCategoryBlock, 0x004EF340,
         (void* self, void* edx, int a2, int a3, int a4, int a5, int a6));

static int __stdcall DispatchEntryBlockIfResultNonNegative(int a1, int a2, uint32_t* a3)
{
    (void)a1;
    (void)a2;

    // v3 = *(*a3 + 4)
    uint8_t* outerObj = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a3[0]));
    uint8_t* v3 = reinterpret_cast<uint8_t*>(CSF_AT(uintptr_t, outerObj, 4));

    // result = *(v3 + 12)
    int result = CSF_AT(int, v3, 12);

    // v6 = 11 - (*(byte*)(*(a3[2] + 4) + 1) != 0)
    uint8_t* flagHolder = reinterpret_cast<uint8_t*>(
        CSF_AT(uintptr_t, reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a3[2])), 4));
    uint8_t flagByte = CSF_AT(uint8_t, flagHolder, 1);
    int v6 = 11 - (flagByte != 0 ? 1 : 0);

    if (result >= 0)
    {
        int valueHolder = GetValue8B0554();
        void* self = CSF_AT(void*, reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(valueHolder)), 0x1FDC);

        // Stack block built by the original: three vtable stores (last wins), then two fields from v3.
        uintptr_t blockDwords[3];
        *reinterpret_cast<void***>(&blockDwords[0]) = CSF_VTBL(0x0082D410);
        *reinterpret_cast<void***>(&blockDwords[0]) = CSF_VTBL(0x0082D428);
        *reinterpret_cast<void***>(&blockDwords[0]) = CSF_VTBL(0x0082D448);
        blockDwords[1] = CSF_AT(uintptr_t, v3, 8);
        blockDwords[2] = CSF_AT(uintptr_t, v3, 12);

        // entry = *(*(a3[1] + 4) + 4)
        uint8_t* argOneObj = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a3[1]));
        uint8_t* argOneInner = reinterpret_cast<uint8_t*>(CSF_AT(uintptr_t, argOneObj, 4));
        uintptr_t entryValue = CSF_AT(uintptr_t, argOneInner, 4);

        ProcessAltCategoryBlock(self, nullptr, v6, static_cast<int>(entryValue),
                                static_cast<int>(blockDwords[0]),
                                static_cast<int>(blockDwords[1]),
                                static_cast<int>(blockDwords[2]));
        return result;
    }
    return result;
}

HOOK(0x461D40, DispatchEntryBlockIfResultNonNegative, "auto");
