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

CSF_FUNC(int, __fastcall, C_00834050_SetActionMode, 0x00492760, (void* self, void* , int newActionMode));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));

// Applies newActionMode to the object linked to 'owner'. When the link is missing,
// the action-mode owner is read from the null-based address 0x2C, as in the original.
static void SetLinkedObjectActionMode(int owner, int newActionMode)
{
    void* ownerObj = reinterpret_cast<void*>(static_cast<uintptr_t>(owner));
    int linked = GetLinkedObjectValue_4DB5C0(ownerObj, nullptr);
    if (linked)
    {
        void* linkedObj = reinterpret_cast<void*>(static_cast<uintptr_t>(linked));
        void* actionOwner = CSF_AT(void*, linkedObj, 0x28);
        C_00834050_SetActionMode(actionOwner, nullptr, newActionMode);
    }
    else
    {
        void* nullBase = reinterpret_cast<void*>(static_cast<uintptr_t>(0));
        void* actionOwner = CSF_AT(void*, nullBase, 0x2C);
        C_00834050_SetActionMode(actionOwner, nullptr, newActionMode);
    }
}

static char __stdcall AiStateFactory_SetActionModeFromLinkedObject(int a1)
{
    void* ownerObj = reinterpret_cast<void*>(static_cast<uintptr_t>(a1));
    int linked = GetLinkedObjectValue_4DB5C0(ownerObj, nullptr);
    int taskBase = linked ? linked - 4 : 0;
    void* taskData = CSF_AT(void*, reinterpret_cast<void*>(static_cast<uintptr_t>(taskBase)), 0x2C);
    int holdFlag = CSF_AT(int, taskData, 0x1C4);

    if (holdFlag != 0)
        SetLinkedObjectActionMode(a1, 0);
    else
        SetLinkedObjectActionMode(a1, 1);

    return 0;
}

HOOK(0x004AC590, AiStateFactory_SetActionModeFromLinkedObject, "auto");
