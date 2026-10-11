#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char*, __fastcall, AiCore_GetTaskTargetNameBuffer, 0x0047AE10, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, AiAttackTasks_BuildTargetData, 0x0047CCA0, (void* self, void* edx, uint32_t* arg0));
CSF_FUNC(int, __fastcall, AiAttackTasks_ApplyTargetData, 0x0048C9F0, (void* self, void* edx, int a2));
CSF_FUNC(bool, __fastcall, AiAttackTasks_IsTargetReady, 0x0048D030, (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, HasNonzeroLinkedValueDifferentFrom, 0x0048E680, (void* self, void* , int comparisonValue));
CSF_FUNC(int, __fastcall, CharActionFsm_IsWeaponActionBlocked, 0x00492BD0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiCore_UpdateTargetData, 0x004D1730, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, TCharTask_BaseUpdate, 0x004DB0F0, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, ResolveSubObject, 0x004DB190, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, TFncGirar_OnRotationComplete, 0x004DB1D0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CharTask_LookupByOffset, 0x004DB440, (void* self, void* edx, int a2));
CSF_FUNC(bool, __fastcall, AiAttackTasks_IsWeaponReady, 0x004E5980, (void* self, void* edx));

static void __fastcall TCharTask_UpdateAttackTarget(
    TCharTask* self, void* /*edx*/, int updateArgument)
{
    // These two addresses are also listed as vtable bindings in the packet.
    (void)CSF_VTBL(0x0047AE10);
    (void)CSF_VTBL(0x0048C9F0);

    TCharTask_BaseUpdate(self, nullptr, updateArgument);

    void* character = reinterpret_cast<void*>(self->field_0008);
    if (static_cast<uint8_t>(CharActionFsm_IsWeaponActionBlocked(
            CSF_AT(void*, character, 0x14), nullptr)) != 0)
    {
        CharTask_LookupByOffset(self, nullptr,
                               static_cast<int>(self->field_000c + 104));
        return;
    }

    if (self->field_0010 != 0)
        return;

    character = reinterpret_cast<void*>(self->field_0008);
    // The original's x87 test takes the continuing path for unordered values.
    if (CSF_AT(float, character, 0x2AC) >= CSF_AT(float, character, 0x2B0))
    {
        TFncGirar_OnRotationComplete(self, nullptr, 0);
        return;
    }

    character = reinterpret_cast<void*>(self->field_0008);
    if (static_cast<uint8_t>(HasNonzeroLinkedValueDifferentFrom(
            CSF_AT(void*, character, 0x14), nullptr,
            static_cast<int>(self->field_0008))) != 0)
        return;

    character = reinterpret_cast<void*>(self->field_0008);
    if (!AiAttackTasks_IsWeaponReady(
            CSF_AT(void*, CSF_AT(void*, character, 0x14), 0x3C), nullptr))
        return;

    alignas(8) uint8_t targetDataBytes[0xC] = {};
    AiAttackTasks_BuildTargetData(
        reinterpret_cast<void*>(self->field_0008), nullptr,
        reinterpret_cast<uint32_t*>(targetDataBytes));

    if (CSF_AT(int32_t, targetDataBytes, 0x8) < 0)
        return;

    char* targetNameBuffer = AiCore_GetTaskTargetNameBuffer(
        reinterpret_cast<void*>(self->field_0008), nullptr);
    AiCore_UpdateTargetData(
        targetNameBuffer, nullptr, reinterpret_cast<int>(targetDataBytes));

    character = reinterpret_cast<void*>(self->field_0008);
    AiAttackTasks_ApplyTargetData(
        CSF_AT(void*, CSF_AT(void*, character, 0x14), 0x204), nullptr,
        reinterpret_cast<int>(targetDataBytes));

    character = reinterpret_cast<void*>(self->field_0008);
    if (AiAttackTasks_IsTargetReady(
            CSF_AT(void*, CSF_AT(void*, character, 0x14), 0x204), nullptr, 1))
    {
        CharTask_LookupByOffset(self, nullptr,
                               static_cast<int>(self->field_000c + 40));
    }
    else
    {
        ResolveSubObject(self, nullptr, reinterpret_cast<int>(
            reinterpret_cast<uint8_t*>(self->field_0008) + 96));
    }
}

HOOK(0x0047D660, TCharTask_UpdateAttackTarget, "auto");
