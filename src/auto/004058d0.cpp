#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());

// Virtual signatures used by the slot calls below (all __thiscall, written as __fastcall).
using NoArgPtrSlotFn = void* (__fastcall*)(void* self, void* edx);
using IntArgPtrSlotFn = void* (__fastcall*)(void* self, void* edx, int arg);
using IntArgVoidSlotFn = void(__fastcall*)(void* self, void* edx, int arg);

// Resolves the render matrix entry for `index` and forwards `outAddr` to it. A negative index
// selects the secondary lookup (index with the sign bit cleared). When neither lookup yields an
// entry, a fresh RW matrix is created and stored at outAddr, with its flag byte set to 1.
static int __fastcall C_0082d510_FetchMatrixForIndex(C_0082d510* self, void* /*edx*/, int outAddr, int index)
{
    void* managerObj = reinterpret_cast<void*>(static_cast<std::uintptr_t>(self->field_0008));
    // slot 0x28 / 4 = 10: no-argument getter returning the lookup table object
    void* table = reinterpret_cast<NoArgPtrSlotFn>(CSF_AT(void**, managerObj, 0)[10])(managerObj, nullptr);

    if (index >= 0)
    {
        // slot 0x80 / 4 = 32
        void* entry = reinterpret_cast<IntArgPtrSlotFn>(CSF_AT(void**, table, 0)[32])(table, nullptr, index);
        if (entry)
        {
            // slot 0x3c / 4 = 15
            reinterpret_cast<IntArgVoidSlotFn>(CSF_AT(void**, entry, 0)[15])(entry, nullptr, outAddr);
            return outAddr;
        }
    }
    else
    {
        // slot 0x68 / 4 = 26
        void* subEntry = reinterpret_cast<NoArgPtrSlotFn>(CSF_AT(void**, table, 0)[26])(table, nullptr);
        if (subEntry)
        {
            // slot 0x6c / 4 = 27
            void* leaf = reinterpret_cast<IntArgPtrSlotFn>(CSF_AT(void**, subEntry, 0)[27])(subEntry, nullptr, index & 0x7FFFFFFF);
            if (leaf)
            {
                // slot 0x1c / 4 = 7
                reinterpret_cast<IntArgVoidSlotFn>(CSF_AT(void**, leaf, 0)[7])(leaf, nullptr, outAddr);
                return outAddr;
            }
        }
    }

    void* outObj = reinterpret_cast<void*>(static_cast<std::uintptr_t>(outAddr));
    CSF_AT(std::uint32_t*, outObj, 0) = RwMatrixCreate();
    CSF_AT(std::uint8_t, outObj, 4) = 1;
    return outAddr;
}

HOOK(0x004058D0, C_0082d510_FetchMatrixForIndex, "auto");
