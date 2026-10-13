#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char*, __fastcall, AiCore_GetTaskTargetNameBuffer, 0x0047AE10, (S_0047ae10* self, void*));
CSF_FUNC(void, __fastcall, S_0048c9f0_CopyTargetHandle, 0x0048C9F0, (S_0048c9f0* self, void* , const void* handle));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_SetCoverTransition, 0x004D1100, (void* self, void* , int a2, int a3));
CSF_FUNC(void, __fastcall, AiCoverAttackTasks_UpdateCoverState, 0x004D1120, (float* self, void* , float a2));
CSF_FUNC(int, __fastcall, ResetState, 0x004D16F0, (void* self, void*));
CSF_FUNC(int, __fastcall, IsField10Null, 0x004DB5B0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __stdcall, AiCoverAttackTasks_UpdateCharacterTasks, 0x004DBD50, (int a1, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

static int __stdcall AiFncSearch_UpdateCharacterSearch(int character, float elapsedTime)
{
    int elapsedBits;
    std::memcpy(&elapsedBits, &elapsedTime, sizeof(elapsedBits));
    AiCoverAttackTasks_UpdateCharacterTasks(character, elapsedBits);

    void* task = reinterpret_cast<void*>(character);
    int result = IsField10Null(task, nullptr);
    if (static_cast<uint8_t>(result) != 0)
        return result;

    int linked = GetLinkedObjectValue_4DB5C0(task, nullptr);
    auto* owner = reinterpret_cast<uint8_t*>(linked ? linked - 4 : 0);

    bool resetTarget =
        CSF_AT(int, owner, 0x608) == -1 &&
        CSF_AT(int, owner, 0x60C) == -1;

    if (!resetTarget) {
        linked = GetLinkedObjectValue_4DB5C0(task, nullptr);
        owner = reinterpret_cast<uint8_t*>(linked ? linked - 4 : 0);
        resetTarget = CSF_AT(float, owner, 0x678) > g_TransitionOne;
    }

    if (!resetTarget) {
        GetLinkedObjectValue_4DB5C0(task, nullptr);
        linked = GetLinkedObjectValue_4DB5C0(task, nullptr);
        owner = reinterpret_cast<uint8_t*>(linked ? linked - 4 : 0);

        void* globalState = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
        return AiCoverAttackTasks_SetCoverTransition(
            owner + 0x690, nullptr, 0, CSF_AT(int, globalState, 0xA98));
    }

    linked = GetLinkedObjectValue_4DB5C0(task, nullptr);
    owner = reinterpret_cast<uint8_t*>(linked ? linked - 4 : 0);

    char* targetName = AiCore_GetTaskTargetNameBuffer(
        CSF_AT(S_0047ae10*, owner, 0x718), nullptr);
    ResetState(targetName, nullptr);

    void* characterState = CSF_AT(void*, owner, 0x2C);
    alignas(8) uint8_t emptyHandle[0xC];
    CSF_AT(void**, emptyHandle, 0) = CSF_VTBL(0x0082D448);
    CSF_AT(int, emptyHandle, 4) = -1;
    CSF_AT(int, emptyHandle, 8) = -1;
    S_0048c9f0_CopyTargetHandle(
        CSF_AT(S_0048c9f0*, characterState, 0x204), nullptr, emptyHandle);

    linked = GetLinkedObjectValue_4DB5C0(task, nullptr);
    result = linked ? linked - 4 : 0;
    owner = reinterpret_cast<uint8_t*>(result);
    AiCoverAttackTasks_UpdateCoverState(
        reinterpret_cast<float*>(owner + 0x690), nullptr, elapsedTime);
    return result;
}

HOOK(0x004B55F0, AiFncSearch_UpdateCharacterSearch, "auto");
