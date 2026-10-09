#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// CHandleTable::Resolve(this = g_pCurrentScene, handle) -> object pointer or 0
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
// sub_48E760(this = resolved object, -1)
CSF_FUNC(int, __fastcall, sub_48E760, 0x0048E760, (void* self, void* edx, int a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __stdcall ResolveHandleAndReleaseIfFound(int a1, int a2, int a3)
{
    (void)a1;
    (void)a2;

    // a3 points to a slot holding a pointer; follow [a3] -> +4 -> object record
    uint8_t* slotPtr = reinterpret_cast<uint8_t*>(a3);
    uint8_t* ownerPtr = reinterpret_cast<uint8_t*>(*reinterpret_cast<uint32_t*>(slotPtr));
    uint8_t* recordPtr = reinterpret_cast<uint8_t*>(*reinterpret_cast<uint32_t*>(ownerPtr + 4));

    int32_t recordFlags = *reinterpret_cast<int32_t*>(recordPtr + 12);
    uint32_t handleValue = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(recordPtr + 4));
    uint32_t result = handleValue;

    if (recordFlags >= 0)
    {
        result = static_cast<uint32_t>(CHandleTable_Resolve(
            g_pCurrentScene, nullptr, static_cast<int>(handleValue)));
        if (result != 0)
        {
            return sub_48E760(reinterpret_cast<void*>(static_cast<uintptr_t>(result)), nullptr, -1);
        }
    }
    return static_cast<int>(result);
}

HOOK(0x461370, ResolveHandleAndReleaseIfFound, "auto");
