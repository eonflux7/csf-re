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

CSF_FUNC(int, __fastcall, IsField1BCNonNegative, 0x00492B10, (void* self, void* edx));
CSF_FUNC(char*, __fastcall, S_00492b20_GetPtr0x1B4, 0x00492B20, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, C_008319c0_GetSubObjectField74, 0x004760A0, (C_008319c0* self, void* edx));
CSF_FUNC(int, __cdecl, Sub761210_IsFlagSet, 0x00761210, (unsigned int a1));

// Checks whether the order target held by this task is still valid and available:
// either a live handle resolves to an object of type 19, or the indirect target
// object's sub-object reports a set flag.
static bool __fastcall C_0083ab58_IsTargetAvailable(C_0083ab58* self, void* /*edx*/)
{
    C_00833908* targetRef = reinterpret_cast<C_00833908*>(self->field_0014);

    if (static_cast<uint8_t>(IsField1BCNonNegative(targetRef, nullptr)) != 0)
    {
        (void)CSF_VTBL(0x00492B20);
        char* handleValue = S_00492b20_GetPtr0x1B4(targetRef, nullptr);
        int resolved = CHandleTable_Resolve(g_pCurrentScene, nullptr,
                                            static_cast<int>(reinterpret_cast<uintptr_t>(handleValue)));
        if (resolved == 0)
            return false;

        uint8_t* resolvedObj = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(resolved));
        if (CSF_AT(int, resolvedObj, 0x50) != 19)
            return false;
    }
    else
    {
        void* owner = reinterpret_cast<void*>(static_cast<uintptr_t>(targetRef->field_0044));
        auto getSubObject = reinterpret_cast<void* (__fastcall*)(void*, void*)>(CSF_AT(void**, owner, 0)[0xD8 / 4]);
        C_008319c0* subObject = reinterpret_cast<C_008319c0*>(getSubObject(owner, nullptr));
        if (subObject == nullptr)
            return false;

        int subObjectField74 = C_008319c0_GetSubObjectField74(subObject, nullptr);
        if (static_cast<uint8_t>(Sub761210_IsFlagSet(static_cast<unsigned int>(subObjectField74))) == 0)
            return false;
    }

    return true;
}

HOOK(0x004CD510, C_0083ab58_IsTargetAvailable, "auto");
