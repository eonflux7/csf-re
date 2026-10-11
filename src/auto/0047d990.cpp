#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));
CSF_FUNC(char*, __fastcall, AiCore_GetTaskTargetNameBuffer, 0x0047AE10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AttackTasks_IsAttackDelayElapsed, 0x0047AEB0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, AttackTasks_UpdateAttackDelay, 0x0047AED0, (void* self, void* edx, float a2));
CSF_FUNC(int, __fastcall, AttackTasks_UpdateAttackPosition, 0x0047CB50, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, AttackTasks_StopAttack, 0x0047CBE0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AttackTasks_StartAttackAtPosition, 0x0047CC40, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(void, __fastcall, AttackTasks_SetCoverAttackPosition, 0x0048CA40, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(bool, __fastcall, AttackTasks_TestCoverAttackMode, 0x0048D030, (void* self, void* edx, char a2));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, HasNonzeroLinkedValueDifferentFrom, 0x0048E680, (void* self, void* , int comparisonValue));
CSF_FUNC(int, __fastcall, CharActionFsm_IsWeaponActionBlocked, 0x00492BD0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiCore_CopyCoverAttackState, 0x004D16C0, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(void, __fastcall, TCharTask_BaseUpdate, 0x004DB0F0, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, TFncGirar_OnRotationComplete, 0x004DB1D0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CharTask_LookupByOffset, 0x004DB440, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GetStateValue, 0x0050DD30, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetCurrentItemCount, 0x00681F40, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (C_00833b88* self, void*));

CSF_GLOBAL(float, g_SceneLoadMillisecondsToSeconds, 0x0082D4F0);

// The packet also classifies this directly called function address as a vtable.
[[maybe_unused]] static const auto kTaskTargetBufferAddress = CSF_VTBL(0x0047AE10);

static void __fastcall TCharTask_UpdateCoverAttack(
    TCharTask* self, void* /*edx*/, int elapsedBits)
{
    TCharTask_BaseUpdate(self, nullptr, elapsedBits);

    if (static_cast<uint8_t>(CharActionFsm_IsWeaponActionBlocked(
            CSF_AT(void*, CSF_AT(void*, self, 0x8), 0x14), nullptr)))
    {
        CharTask_LookupByOffset(
            self, nullptr,
            static_cast<int>(reinterpret_cast<uintptr_t>(
                static_cast<uint8_t*>(CSF_AT(void*, self, 0xC)) + 80)));
        return;
    }

    float elapsed;
    std::memcpy(&elapsed, &elapsedBits, sizeof(elapsed));
    AttackTasks_UpdateAttackDelay(
        static_cast<uint8_t*>(CSF_AT(void*, self, 0x8)) + 728,
        nullptr, elapsed);

    void* core = CSF_AT(void*, self, 0x8);
    if (static_cast<uint8_t>(HasNonzeroLinkedValueDifferentFrom(
            CSF_AT(void*, core, 0x14), nullptr,
            static_cast<int>(reinterpret_cast<uintptr_t>(core)))))
    {
        AttackTasks_StopAttack(CSF_AT(void*, self, 0x8), nullptr);
        return;
    }

    void* weapon = CSF_AT(
        void*, CSF_AT(void*, CSF_AT(void*, self, 0x8), 0x14), 0x44);
    void* handle =
        (reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, weapon, 0)[216 / 4]))(weapon, nullptr);
    const int valueAddress = Handle_GetValuePointer(handle, nullptr);
    const bool inactive = CSF_AT(
        uint32_t, reinterpret_cast<void*>(valueAddress), 0x7C) == 0;

    const bool checksItemCount =
        (reinterpret_cast<bool (__fastcall*)(void*, void*)>(
            CSF_AT(void**, weapon, 0)[144 / 4]))(weapon, nullptr);
    if (checksItemCount)
    {
        void* currentItem =
            (reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, weapon, 0)[216 / 4]))(weapon, nullptr);
        if (!GetCurrentItemCount(currentItem, nullptr))
        {
            AttackTasks_StopAttack(CSF_AT(void*, self, 0x8), nullptr);
            CharTask_LookupByOffset(
                self, nullptr,
                static_cast<int>(reinterpret_cast<uintptr_t>(
                    static_cast<uint8_t*>(CSF_AT(void*, self, 0xC)) + 60)));
            return;
        }
    }
    else if (inactive)
    {
        return;
    }

    core = CSF_AT(void*, self, 0x8);
    if (!(CSF_AT(float, core, 684) < CSF_AT(float, core, 688)))
    {
        TFncGirar_OnRotationComplete(self, nullptr, 0);
        return;
    }

    alignas(8) uint8_t targetBytes[0xC];
    float* target = reinterpret_cast<float*>(targetBytes);
    alignas(8) uint8_t directionBytes[0xC];
    float* direction = reinterpret_cast<float*>(directionBytes);

    void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
    const int timeJitter = GetStateValue(globalState, nullptr, 400) - 200;
    void* path = static_cast<uint8_t*>(CSF_AT(void*, self, 0xC)) + 136;
    void** pathVtable = CSF_AT(void**, path, 0);
    const float sampleTime =
        static_cast<float>(timeJitter) * g_SceneLoadMillisecondsToSeconds
        + CSF_AT(float, CSF_AT(void*, self, 0x8), 684);
    // `lea ecx, [eax + 0x88]`: the path object is this of the sampler
    (reinterpret_cast<void (__fastcall*)(void*, void*, float*, float)>(
        pathVtable[20 / 4]))(path, nullptr, target, sampleTime);

    globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
    const float displacement =
        static_cast<float>(GetStateValue(globalState, nullptr, 100) - 50);
    float* position = GetObjectPositionVector(
        CSF_AT(void*, CSF_AT(void*, self, 0x8), 0x14), nullptr);
    const float deltaZ = position[2] - target[2];
    const float deltaY = position[1] - target[1];
    direction[0] = position[0] - target[0];
    direction[1] = deltaY;
    direction[2] = deltaZ;
    NormalizeVector3(reinterpret_cast<C_00833b88*>(directionBytes), nullptr);

    core = CSF_AT(void*, self, 0x8);
    target[0] += direction[0] * displacement;
    target[1] += direction[1] * displacement;
    target[2] += direction[2] * displacement;

    char* targetState = AiCore_GetTaskTargetNameBuffer(core, nullptr);
    AiCore_CopyCoverAttackState(
        targetState, nullptr, reinterpret_cast<uint32_t*>(targetBytes));

    AttackTasks_SetCoverAttackPosition(
        CSF_AT(void*, CSF_AT(void*, CSF_AT(void*, self, 0x8), 0x14), 0x204),
        nullptr, reinterpret_cast<uint32_t*>(targetBytes));
    const bool canAttack = AttackTasks_TestCoverAttackMode(
        CSF_AT(void*, CSF_AT(void*, CSF_AT(void*, self, 0x8), 0x14), 0x204),
        nullptr, 7);

    if (inactive)
    {
        if (canAttack)
        {
            AttackTasks_StartAttackAtPosition(
                CSF_AT(void*, self, 0x8), nullptr,
                reinterpret_cast<uint32_t*>(targetBytes));
        }
    }
    else if (static_cast<uint8_t>(AttackTasks_IsAttackDelayElapsed(
                 static_cast<uint8_t*>(CSF_AT(void*, self, 0x8)) + 728,
                 nullptr)) || !canAttack)
    {
        AttackTasks_StopAttack(CSF_AT(void*, self, 0x8), nullptr);
    }
    else
    {
        AttackTasks_UpdateAttackPosition(
            CSF_AT(void*, self, 0x8), nullptr,
            reinterpret_cast<uint32_t*>(targetBytes));
    }
}

HOOK(0x0047D990, TCharTask_UpdateCoverAttack, "auto");
