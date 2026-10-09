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

// CHandleTable::Resolve(this = g_pCurrentScene, handle) -> entry pointer or null
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
// ActionTarget_ResolveFromSlot(slot) -> target object or null
CSF_FUNC(int, __fastcall, ActionTarget_ResolveFromSlot, 0x00485960, (void* self, void* edx));
// Handle_GetValuePointer(handle entry) -> pointer to the stored value
CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Resets holder->+4 to -1, resolves the owner's handle through the current scene's
// handle table, follows the +0x44 sub-object when +0x50 is clear, and stores the
// dereferenced lookup result into holder->+4.
static CCharActionFsm* __stdcall CharActionFsm_ResolveHandleAndStore(int a1, int a2, int a3)
{
    (void)a2;

    // holder = *(a1 + 4); holder->+4 = -1
    void* owner = reinterpret_cast<void*>(a1);
    void* holder = CSF_AT(void*, owner, 4);
    CSF_AT(int, holder, 4) = -1;

    // v3 = *(*(a3) + 4); v4 = v3->+12
    void* handleSource = reinterpret_cast<void*>(CSF_AT(int, reinterpret_cast<void*>(a3), 0));
    int v3 = CSF_AT(int, handleSource, 4);
    int v4 = CSF_AT(int, reinterpret_cast<void*>(v3), 12);

    CCharActionFsm* result = reinterpret_cast<CCharActionFsm*>(v3 + 4);
    if (v4 >= 0)
    {
        result = reinterpret_cast<CCharActionFsm*>(
            CHandleTable_Resolve(g_pCurrentScene, nullptr, reinterpret_cast<int>(result)));
        if (result)
        {
            if (!result->field_0050)
            {
                result = (CCharActionFsm*)result->field_0044;
                if (result)
                {
                    result = (CCharActionFsm*)ActionTarget_ResolveFromSlot(result, nullptr);
                    if (result)
                    {
                        void* v6 = CSF_AT(void*, owner, 4);
                        int value = *reinterpret_cast<int*>(
                            Handle_GetValuePointer(reinterpret_cast<void*>(result), nullptr));
                        result = reinterpret_cast<CCharActionFsm*>(value);
                        CSF_AT(int, v6, 4) = value;
                    }
                }
            }
        }
    }
    return result;
}

HOOK(0x0045CC90, CharActionFsm_ResolveHandleAndStore, "auto");
