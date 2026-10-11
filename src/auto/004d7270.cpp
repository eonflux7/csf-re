#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, TCharTask_BaseUpdate, 0x004DB0F0, (void* self, void* edx, int a2));
CSF_FUNC(bool, __fastcall, AiAttackTasks_IsWeaponActionAvailable, 0x0048D030, (void* self, void* edx, char a2));
CSF_FUNC(void*, __fastcall, ResolveSubObject, 0x004DB190, (void* self, void* edx, int a2));

static char __fastcall C_004223f0_UpdateAndResolveAction(S_004223f0* self, void* /*edx*/, int a2)
{
    TCharTask_BaseUpdate(self, nullptr, a2);

    uint32_t result = self->field_0010;
    if (result == 0)
    {
        // owner -> +0x14 -> +0x204 is the weapon object passed to the availability check
        auto* owner = reinterpret_cast<uint8_t*>(self->field_0008);
        uint32_t ownerChild = CSF_AT(uint32_t, owner, 0x14);
        auto* childPtr = reinterpret_cast<uint8_t*>(ownerChild);
        auto* weapon = CSF_AT(S_0048cad0*, childPtr, 0x204);

        uint8_t lowByte = AiAttackTasks_IsWeaponActionAvailable(weapon, nullptr, 1) ? 1 : 0;
        result = lowByte;
        if (lowByte == 0)
        {
            uint32_t subObjectArg = self->field_0008 + 0x40;
            uint8_t resolved = static_cast<uint8_t>(reinterpret_cast<uintptr_t>(
                ResolveSubObject(self, nullptr, static_cast<int>(subObjectArg))));
            result = resolved;
        }
    }
    return static_cast<char>(result);
}

HOOK(0x004D7270, C_004223f0_UpdateAndResolveAction, "auto");
