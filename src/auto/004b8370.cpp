#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(char*, __fastcall, AiCore_GetTaskTargetNameBuffer, 0x0047AE10, (S_0047ae10* self, void*));
CSF_FUNC(int, __fastcall, SceneObj_IsInactive, 0x0048E100, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiCore_CopyCoverAttackState, 0x004D16C0, (void* self, void* , uint32_t* source));
CSF_FUNC(int, __fastcall, IsField10Null, 0x004DB5B0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __stdcall, AiCoverAttackTasks_UpdateCharacterTasks, 0x004DBD50, (int a1, int a2));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_WeaponPlacementTraceDepth, 0x0082E11C);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static void __stdcall AiCombat_UpdateCoverAttackTask(int taskAddress, int updateArgument)
{
    // The packet also classifies this callable address as a vtable reference.
    (void)CSF_VTBL(0x0047AE10);

    AiCoverAttackTasks_UpdateCharacterTasks(taskAddress, updateArgument);
    void* task = reinterpret_cast<void*>(taskAddress);
    if (static_cast<uint8_t>(IsField10Null(task, nullptr)) != 0)
        return;

    int linked = GetLinkedObjectValue_4DB5C0(task, nullptr);
    void* owner = reinterpret_cast<void*>(linked ? linked - 4 : 0);
    if (CSF_AT(int, owner, 0x608) != -1 ||
        CSF_AT(int, owner, 0x60C) != -1)
    {
        linked = GetLinkedObjectValue_4DB5C0(task, nullptr);
        owner = reinterpret_cast<void*>(linked ? linked - 4 : 0);
        char* state = AiCore_GetTaskTargetNameBuffer(
            CSF_AT(S_0047ae10*, owner, 0x718), nullptr);
        if (CSF_AT(int, state, 0x18) < 0)
        {
            linked = GetLinkedObjectValue_4DB5C0(task, nullptr);
            void* sourceOwner = reinterpret_cast<void*>(linked ? linked - 4 : 0);

            linked = GetLinkedObjectValue_4DB5C0(task, nullptr);
            owner = reinterpret_cast<void*>(linked ? linked - 4 : 0);
            uint32_t* source = reinterpret_cast<uint32_t*>(
                reinterpret_cast<uintptr_t>(sourceOwner) + 0x64C);
            char* destination = AiCore_GetTaskTargetNameBuffer(
                CSF_AT(S_0047ae10*, owner, 0x718), nullptr);
            AiCore_CopyCoverAttackState(destination, nullptr, source);
        }
    }

    linked = GetLinkedObjectValue_4DB5C0(task, nullptr);
    owner = reinterpret_cast<void*>(linked ? linked - 4 : 0);
    if (CSF_AT(int, CSF_AT(void*, owner, 0x2C), 0x1D8) == 3)
    {
        linked = GetLinkedObjectValue_4DB5C0(task, nullptr);
        owner = reinterpret_cast<void*>(linked ? linked - 4 : 0);
        void* controller = CSF_AT(void*, owner, 0x718);
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, controller, 0)[0x90 / 4])(
                controller, nullptr, updateArgument);
    }

    void* scene = g_pCurrentScene;
    int linkedState = GetLinkedObjectField10(task, nullptr);
    int sceneObject = CHandleTable_Resolve(scene, nullptr, linkedState + 0x14);
    if (!sceneObject ||
        static_cast<uint8_t>(SceneObj_IsInactive(
            reinterpret_cast<void*>(sceneObject), nullptr)) != 0)
    {
        linkedState = GetLinkedObjectField10(task, nullptr);
        AiCoverAttackTasks_ResetLinkedTaskState(task, nullptr, linkedState + 4);
    }
    else
    {
        linked = GetLinkedObjectValue_4DB5C0(task, nullptr);
        owner = reinterpret_cast<void*>(linked ? linked - 4 : 0);
        void* position = GetObjectPositionVector(
            CSF_AT(void*, owner, 0x2C), nullptr);
        linkedState = GetLinkedObjectField10(task, nullptr);
        void* state = reinterpret_cast<void*>(linkedState);

        float dx = CSF_AT(float, state, 0x20) - CSF_AT(float, position, 0);
        float dy = CSF_AT(float, state, 0x24) - CSF_AT(float, position, 4);
        float dz = CSF_AT(float, state, 0x28) - CSF_AT(float, position, 8);
        if (dz * dz + dx * dx + dy * dy < g_WeaponPlacementTraceDepth)
        {
            linkedState = GetLinkedObjectField10(task, nullptr);
            AiCoverAttackTasks_ResetLinkedTaskState(task, nullptr, linkedState + 0xC);
        }
    }
}

HOOK(0x004B8370, AiCombat_UpdateCoverAttackTask, "auto");
