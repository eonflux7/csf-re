#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __fastcall, Interact_IsLinkedObjectActive, 0x004E5980, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, Core_ComputeAttackTargetData, 0x0047CCA0, (void* self, void* edx, uint32_t* arg0));
CSF_FUNC(char*, __fastcall, AiCore_GetTaskTargetNameBuffer, 0x0047AE10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiCore_UpdateTargetData, 0x004D1730, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CharTask_StartMovement, 0x004DB140, (void* self, void* edx, int a2));

static int __fastcall SetPendingTargetAndStartMovement(C_0082f9f8* self, void* /*edx*/, int a2)
{
    // Store the requested value and clear the companion slot in the owner object.
    CSF_AT(int, reinterpret_cast<uint8_t*>(self->field_0008), 0x2B0) = a2;
    CSF_AT(int, reinterpret_cast<uint8_t*>(self->field_0008), 0x2AC) = 0;

    // Owner -> +0x14 (parent object) -> +0x3C (linked object).
    uint8_t* parent = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(self->field_0008), 0x14));
    C_0083e9d0* linked = reinterpret_cast<C_0083e9d0*>(CSF_AT(uint32_t, parent, 0x3C));

    if (Interact_IsLinkedObjectActive(linked, nullptr))
    {
        uint32_t targetData[3];
        Core_ComputeAttackTargetData(reinterpret_cast<void*>(self->field_0008), nullptr, targetData);

        if (static_cast<int>(targetData[2]) >= 0)
        {
            char* nameBuffer = AiCore_GetTaskTargetNameBuffer(reinterpret_cast<void*>(self->field_0008), nullptr);
            AiCore_UpdateTargetData(nameBuffer, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(targetData)));
        }
    }

    return CharTask_StartMovement(self, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(&self->field_0014)));
}

HOOK(0x0047D3F0, SetPendingTargetAndStartMovement, "auto");
