#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, TCharTask_CheckBaseCondition_47AE20, 0x0047AE20, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, TCharTask_RunAttackStep_44BBC0, 0x0044BBC0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(void*, __fastcall, UmsVirtualProcessor_GetDefaultDestination, 0x004FE4A0, (void* self, void* edx));

// Returns true when the base condition holds and the object referenced at +0x14
// has no pending value at +0x78; in that case the attack step runs and the result
// is inverted (1 when the step reported failure).
static char __fastcall C_00831ac0_CanFireWithDefaultDestination(C_00831ac0* self, void* /*edx*/, int a2)
{
    char result = TCharTask_CheckBaseCondition_47AE20(self, nullptr, a2);
    if (result) {
        void* inner = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x14));
        if (!CSF_AT(int, inner, 0x78)) {
            void* globalValueObject = reinterpret_cast<void*>(GetGlobalValue_8B0568());
            void* defaultDestination = UmsVirtualProcessor_GetDefaultDestination(globalValueObject, nullptr);
            return static_cast<char>(TCharTask_RunAttackStep_44BBC0(defaultDestination, nullptr, a2) == 0);
        }
    }
    return result;
}

HOOK(0x0047B280, C_00831ac0_CanFireWithDefaultDestination, "auto");
