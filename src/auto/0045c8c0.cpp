#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));

// Writes -1 into the holder's target. If the handle record reached through *(*a3)+4 has a
// non-negative handle at +12, it is resolved through the current scene's handle table, and
// the first dword of the resolved object's +0x2C pointer is copied into the target.
// Returns the record address + 4 (or the resolved pointer when a resolve succeeded).
static uint32_t* __stdcall StoreResolvedHandleValue(uint32_t holderAddr, uint32_t /*unusedArg*/, uint32_t handleRefAddr)
{
    // *(*(a1 + 4) + 4) = -1
    uint32_t target = CSF_AT(uint32_t, holderAddr, 4);
    CSF_AT(int32_t, target, 4) = -1;

    // v3 = *(*a3 + 4); v4 = *(v3 + 12); result = v3 + 4
    uint32_t firstLevel = CSF_AT(uint32_t, handleRefAddr, 0);
    uint32_t record = CSF_AT(uint32_t, firstLevel, 4);
    int32_t handle = CSF_AT(int32_t, record, 12);
    uint32_t resultAddr = record + 4;

    if (handle >= 0)
    {
        // result = CHandleTable::Resolve(g_pCurrentScene, result)
        int resolved = CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(resultAddr));
        uint32_t resolvedAddr = static_cast<uint32_t>(resolved);
        if (resolvedAddr != 0)
        {
            // result = result[11]  (the +0x2C pointer of the resolved object)
            resultAddr = CSF_AT(uint32_t, resolvedAddr, 0x2C);
            // *(*(a1 + 4) + 4) = *result
            uint32_t reloadedTarget = CSF_AT(uint32_t, holderAddr, 4);
            CSF_AT(uint32_t, reloadedTarget, 4) = CSF_AT(uint32_t, resultAddr, 0);
        }
    }
    return reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(resultAddr));
}

HOOK(0x0045C8C0, StoreResolvedHandleValue, "auto");
