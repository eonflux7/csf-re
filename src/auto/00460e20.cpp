#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// CHandleTable::Resolve: __thiscall, one stack arg, ret 4.
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x44FC30, (void* self, void* edx, int handle));

CSF_GLOBAL(void*, g_pCurrentScene, 0x8AFC90);

// Resolves the handle stored at a3's first object and, if the handle is valid and
// its index is non-negative, writes a value taken from the third argument chain
// into the resolved entry at +0x78. Returns the resolved entry (or the raw handle
// value when the index is negative), as the original does.
static int __stdcall ResolveHandleAndStoreTableValue(int /*a1*/, int /*a2*/, uint32_t* a3)
{
    uint32_t objectPtr = *a3;
    uint32_t headerPtr = *reinterpret_cast<uint32_t*>(objectPtr + 4);
    int32_t indexValue = *reinterpret_cast<int32_t*>(headerPtr + 12);

    int result = static_cast<int>(headerPtr + 4);
    if (indexValue >= 0)
    {
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, result);
        if (result)
        {
            uint32_t sourceObj = a3[1];
            uint32_t sourceHeader = *reinterpret_cast<uint32_t*>(sourceObj + 4);
            uint32_t sourceValue = *reinterpret_cast<uint32_t*>(sourceHeader + 4);
            uint8_t* entry = reinterpret_cast<uint8_t*>(result);
            *reinterpret_cast<uint32_t*>(entry + 0x78) = sourceValue;
        }
    }
    return result;
}

HOOK(0x460E20, ResolveHandleAndStoreTableValue, "auto");
