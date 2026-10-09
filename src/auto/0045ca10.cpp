#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));

// Clears the dword at *(*(holder + 4) + 4) to 0. Reads the handle record through
// *(*handleRef + 4); if its handle at +12 is non-negative, resolves record+4 through the
// current scene's handle table. On success, copies the dword at (resolved->+0x2C)->+0xBC
// into the holder's target. Returns the record+4 address when unresolved, the resolved
// sub-object pointer on success, or 0 when the resolve returns null.
static int __stdcall StoreResolvedHandleValue(int holderArg, int /*unusedArg*/, int handleRefArg)
{
    uint32_t holderAddr = static_cast<uint32_t>(holderArg);
    uint32_t handleRefAddr = static_cast<uint32_t>(handleRefArg);

    // *(*(a1 + 4) + 4) = 0
    uint32_t target = CSF_AT(uint32_t, holderAddr, 4);
    CSF_AT(uint32_t, target, 4) = 0;

    // v3 = *(*a3 + 4); v4 = *(v3 + 12); result = v3 + 4
    uint32_t firstLevel = CSF_AT(uint32_t, handleRefAddr, 0);
    uint32_t record = CSF_AT(uint32_t, firstLevel, 4);
    int32_t handle = CSF_AT(int32_t, record, 12);
    uint32_t resultAddr = record + 4;

    if (handle >= 0)
    {
        // result = CHandleTable::Resolve(g_pCurrentScene, result)
        int resolved = CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(resultAddr));
        resultAddr = static_cast<uint32_t>(resolved);
        if (resultAddr != 0)
        {
            // result = result->+0x2C
            resultAddr = CSF_AT(uint32_t, resultAddr, 0x2C);
            // *(*(a1 + 4) + 4) = result->+0xBC  (target reloaded after the call)
            uint32_t reloadedTarget = CSF_AT(uint32_t, holderAddr, 4);
            CSF_AT(uint32_t, reloadedTarget, 4) = CSF_AT(uint32_t, resultAddr, 0xBC);
        }
    }
    return static_cast<int>(resultAddr);
}

HOOK(0x0045CA10, StoreResolvedHandleValue, "auto");
