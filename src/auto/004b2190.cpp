#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGroupIndexForSlot, 0x004E51D0, (int slot));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Fills the combat-target record at self (int[]) for slot a2 from the source entry a3,
// then copies the target's position vector into the record. Returns the last value written.
static int __fastcall AiCombat_BindCombatTarget(int* self, void* /*edx*/, int a2, int a3)
{
    int* sourceEntry = reinterpret_cast<int*>(a3);

    self[0] = GetGroupIndexForSlot(a2);
    self[1] = a2;
    self[2] = -1;
    self[4] = sourceEntry[1];
    self[5] = sourceEntry[2];

    int result = self[5];
    if (result >= 0)
    {
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, reinterpret_cast<int>(self + 3));
        if (result)
        {
            void* targetObject = nullptr;
            if (self[5] >= 0)
                targetObject = reinterpret_cast<void*>(
                    CHandleTable_Resolve(g_pCurrentScene, nullptr, reinterpret_cast<int>(self + 3)));

            int* positionVector = GetObjectPositionVector(targetObject, nullptr);
            self[6] = positionVector[0];
            self[7] = positionVector[1];
            result = positionVector[2];
            self[8] = result;
        }
    }
    return result;
}

HOOK(0x4B2190, AiCombat_BindCombatTarget, "auto");
