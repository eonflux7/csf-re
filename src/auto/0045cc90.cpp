#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ActionTarget_ResolveFromSlot, 0x00485960, (void* self, void* edx));

// Stores -1 into the holder object's +4 slot, resolves the handle stored at
// owner->obj+12 (when non-negative) through the current scene's handle table,
// and, if the resolved object has no flag at +0x50 and a sub-object at +0x44,
// resolves that sub-object and stores the dereferenced value of the next lookup
// into the holder's +4 slot. Returns the last computed pointer (eax).
static CCharActionFsm* __stdcall CharActionFsm_ResolveHandleAndStore(int a1, int a2, int a3)
{
    // holder = *(a1 + 4); holder->+4 = -1
    *CSF_AT(int32_t*, *CSF_AT(void**, (void*)a1, 4), 4) = -1;

    // obj = (*(a3))->+4
    uint8_t* owner = *CSF_AT(uint8_t**, (void*)a3, 0);
    uint8_t* obj = *CSF_AT(uint8_t**, owner, 4);
    int32_t handleValue = *CSF_AT(int32_t*, obj, 12);
    CCharActionFsm* result = reinterpret_cast<CCharActionFsm*>(obj + 4);

    if (handleValue >= 0)
    {
        result = reinterpret_cast<CCharActionFsm*>(
            CHandleTable_Resolve(g_pCurrentScene, nullptr, (int)(obj + 4)));
        if (result)
        {
            if (!result->field_0050)
            {
                result = reinterpret_cast<CCharActionFsm*>(result->field_0044);
                if (result)
                {
                    result = reinterpret_cast<CCharActionFsm*>(
                        ActionTarget_ResolveFromSlot(result, nullptr));
                    if (result)
                    {
                        // Holder pointer is re-read here in the original.
                        void* holder = *CSF_AT(void**, (void*)a1, 4);
                        result = *reinterpret_cast<CCharActionFsm**>(
                            Handle_GetValuePointer(result, nullptr));
                        *CSF_AT(CCharActionFsm**, holder, 4) = result;
                    }
                }
            }
        }
    }
    return result;
}

HOOK(0x0045CC90, CharActionFsm_ResolveHandleAndStore, "auto");
