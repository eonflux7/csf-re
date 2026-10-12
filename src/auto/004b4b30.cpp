#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(char*, __fastcall, AiCore_GetTaskTargetNameBuffer, 0x0047AE10, (S_0047ae10* self, void*));
CSF_FUNC(int, __fastcall, AiAttackTasks_CopyTargetHandle, 0x0048C9F0, (void* self, void* edx, int a2));
CSF_FUNC(char*, __fastcall, S_00492b20_GetPtr0x1B4, 0x00492B20, (void* self, void*));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_SetCoverTransition, 0x004D1100, (void* self, void* , int a2, int a3));
CSF_FUNC(void, __fastcall, AiCoverAttackTasks_UpdateCoverState, 0x004D1120, (float* self, void* , float a2));
CSF_FUNC(int, __fastcall, ResetState, 0x004D16F0, (void* self, void*));
CSF_FUNC(int, __fastcall, IsField10Null, 0x004DB5B0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, Dispatch_NoLinkedTarget, 0x004DBCB0, (void* self, void* edx, int a2));
CSF_FUNC(int, __stdcall, AiCoverAttackTasks_UpdateCharacterTasks, 0x004DBD50, (int a1, int a2));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));
CSF_FUNC(bool, __fastcall, C_0083e9d0_IsLinkedObjectActive, 0x004E5980, (C_0083e9d0* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __stdcall AiAlert_UpdateLinkedCoverTask(int taskAddress, float elapsed)
{
    int elapsedBits;
    std::memcpy(&elapsedBits, &elapsed, sizeof(elapsedBits));
    AiCoverAttackTasks_UpdateCharacterTasks(taskAddress, elapsedBits);

    void* task = reinterpret_cast<void*>(taskAddress);
    int result = IsField10Null(task, nullptr);
    if (static_cast<uint8_t>(result) != 0)
        return result;

    int linked = GetLinkedObjectValue_4DB5C0(task, nullptr);
    void* character = reinterpret_cast<void*>(linked ? linked - 4 : 0);
    void* characterObject = CSF_AT(void*, character, 0x2C);
    void* scene = g_pCurrentScene;
    char* handle = S_00492b20_GetPtr0x1B4(characterObject, nullptr);
    int resolved = CHandleTable_Resolve(scene, nullptr, reinterpret_cast<int>(handle));
    if (resolved != 0)
    {
        C_0083e9d0* linkedTarget =
            CSF_AT(C_0083e9d0*, reinterpret_cast<void*>(resolved), 0x3C);
        if (linkedTarget != nullptr &&
            C_0083e9d0_IsLinkedObjectActive(linkedTarget, nullptr))
            return Dispatch_NoLinkedTarget(task, nullptr, 5);
    }

    linked = GetLinkedObjectValue_4DB5C0(task, nullptr);
    character = reinterpret_cast<void*>(linked ? linked - 4 : 0);
    bool needsReset = CSF_AT(int, character, 0x608) == -1 &&
                      CSF_AT(int, character, 0x60C) == -1;
    if (!needsReset)
    {
        linked = GetLinkedObjectValue_4DB5C0(task, nullptr);
        character = reinterpret_cast<void*>(linked ? linked - 4 : 0);
        needsReset = CSF_AT(float, character, 0x678) > g_TransitionOne;
    }

    if (!needsReset)
    {
        GetLinkedObjectValue_4DB5C0(task, nullptr);
        linked = GetLinkedObjectValue_4DB5C0(task, nullptr);
        character = reinterpret_cast<void*>(linked ? linked - 4 : 0);

        int firstGlobal = GetGlobalValue_8BC1E0();
        int secondGlobal = GetGlobalValue_8BC1E0();
        float duration = CSF_AT(float, reinterpret_cast<void*>(secondGlobal), 0xA94);
        int durationBits = CSF_AT(int, reinterpret_cast<void*>(firstGlobal), 0xA94);
        float halfDuration = duration * g_HalfDurationScale;
        int halfDurationBits;
        std::memcpy(&halfDurationBits, &halfDuration, sizeof(halfDurationBits));

        void* coverState = reinterpret_cast<void*>(
            reinterpret_cast<uintptr_t>(character) + 0x690);
        return AiCoverAttackTasks_SetCoverTransition(
            coverState, nullptr, halfDurationBits, durationBits);
    }

    linked = GetLinkedObjectValue_4DB5C0(task, nullptr);
    character = reinterpret_cast<void*>(linked ? linked - 4 : 0);
    float remaining = CSF_AT(float, character, 0x694) - elapsed;
    if (!(remaining > g_fZero))
    {
        int linkedTask = GetLinkedObjectField10(task, nullptr);
        return AiCoverAttackTasks_ResetLinkedTaskState(task, nullptr, linkedTask + 20);
    }

    linked = GetLinkedObjectValue_4DB5C0(task, nullptr);
    character = reinterpret_cast<void*>(linked ? linked - 4 : 0);
    char* targetName = AiCore_GetTaskTargetNameBuffer(
        CSF_AT(S_0047ae10*, character, 0x718), nullptr);
    ResetState(targetName, nullptr);

    characterObject = CSF_AT(void*, character, 0x2C);
    alignas(8) uint8_t targetHandleBytes[0xC];
    CSF_AT(void**, targetHandleBytes, 0) = CSF_VTBL(0x0082D448);
    CSF_AT(int, targetHandleBytes, 0x4) = -1;
    CSF_AT(int, targetHandleBytes, 0x8) = -1;
    AiAttackTasks_CopyTargetHandle(
        CSF_AT(void*, characterObject, 0x204), nullptr,
        reinterpret_cast<int>(targetHandleBytes));

    linked = GetLinkedObjectValue_4DB5C0(task, nullptr);
    character = reinterpret_cast<void*>(linked ? linked - 4 : 0);
    float* coverState = reinterpret_cast<float*>(
        reinterpret_cast<uintptr_t>(character) + 0x690);
    AiCoverAttackTasks_UpdateCoverState(coverState, nullptr, -elapsed);
    return reinterpret_cast<int>(character);
}

HOOK(0x004B4B30, AiAlert_UpdateLinkedCoverTask, "auto");
