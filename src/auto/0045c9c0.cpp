#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, sub_4950B0, 0x004950B0, (void* self, void* edx));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static CPlayerCharacter* __stdcall ResolveSceneObjectHandleAndCache(void* outputHolder, int /*unused*/, void* handleHolder)
{
    // *(*(a1 + 4) + 4) = 0;
    CSF_AT(uint32_t, CSF_AT(void*, outputHolder, 4), 4) = 0;

    // v3 = *(*(a3) + 4); the handle entry lives at v3.
    void* handleEntry = CSF_AT(void*, handleHolder, 0);
    uint32_t entryAddress = CSF_AT(uint32_t, handleEntry, 4);
    int32_t entryFlags = CSF_AT(int32_t, reinterpret_cast<void*>(entryAddress), 12);

    CPlayerCharacter* result = reinterpret_cast<CPlayerCharacter*>(entryAddress + 4);

    if (entryFlags >= 0)
    {
        result = reinterpret_cast<CPlayerCharacter*>(
            CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(entryAddress + 4)));

        if (result)
        {
            if (!result->field_0050)
            {
                // Re-read the output holder before the call, as the original does.
                void* cacheHolder = CSF_AT(void*, outputHolder, 4);
                result = reinterpret_cast<CPlayerCharacter*>(sub_4950B0(result, nullptr));
                CSF_AT(CPlayerCharacter*, cacheHolder, 4) = result;
            }
        }
    }
    return result;
}

HOOK(0x0045C9C0, ResolveSceneObjectHandleAndCache, "auto");
