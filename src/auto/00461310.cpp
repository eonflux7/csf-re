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

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, MenuExtras_ApplyDescriptionToControl, 0x0048E760, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, MenuExtras_ParseDescriptionId, 0x00698D90, (const char* text));

static int __stdcall MenuExtras_ApplyDescriptionFromRecord(int a1, int a2, uint32_t* a3)
{
    (void)a1;
    (void)a2;

    // v3 = *(*a3 + 4): first record pointer is stored in a3[0].
    uint32_t recordBase = CSF_AT(uint32_t, reinterpret_cast<void*>(a3[0]), 4);
    int32_t handleFlags = CSF_AT(int32_t, reinterpret_cast<void*>(recordBase), 12);
    int result = static_cast<int>(recordBase + 4);

    if (handleFlags >= 0)
    {
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, result);
        if (result)
        {
            void* resolvedControl = reinterpret_cast<void*>(result);

            // v6 = *(a3[1] + 4)
            uint32_t descriptorBase = CSF_AT(uint32_t, reinterpret_cast<void*>(a3[1]), 4);
            uint32_t descriptorCount = CSF_AT(uint32_t, reinterpret_cast<void*>(descriptorBase), 28);
            uint32_t descriptorField = descriptorBase + 4;

            uint32_t descriptionText;
            if (descriptorCount < 0x10)
                descriptionText = descriptorField + 4;
            else
                descriptionText = CSF_AT(uint32_t, reinterpret_cast<void*>(descriptorField), 4);

            int descriptionId = MenuExtras_ParseDescriptionId(reinterpret_cast<const char*>(descriptionText));
            return MenuExtras_ApplyDescriptionToControl(resolvedControl, nullptr, descriptionId);
        }
    }

    return result;
}

HOOK(0x461310, MenuExtras_ApplyDescriptionFromRecord, "auto");
