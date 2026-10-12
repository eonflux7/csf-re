#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, AiAttackTasks_CopyTargetHandle, 0x0048C9F0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, TaskCharAction_48CFE0, 0x0048CFE0, (void* self, void* edx, int a2, char a3, char a4));
CSF_FUNC(char*, __fastcall, AiAlert_GetStateName, 0x004AABC0, (void* self, void* edx, void* a2));
CSF_FUNC(C_008319c0*, __fastcall, S_004aef70_SelectUsableTarget, 0x004AEF70, (S_004aef70* self, void*));
CSF_FUNC(int, __fastcall, AiAlert_PrepareLinkedCharacter, 0x004B2F60, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_004D10F0_ResetFields, 0x004D10F0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(char*, __cdecl, AiAlert_GetErrorDescription, 0x007613F0, (int a1));

static void* AiAlert_GetLinkedCharacter(void* state)
{
    const std::uintptr_t linked =
        static_cast<std::uint32_t>(GetLinkedObjectValue_4DB5C0(state, nullptr));
    return reinterpret_cast<void*>(linked ? linked - 4 : 0);
}

static const char* __fastcall TAlerta_GetName(
    TAlerta* self, void* /*edx*/, int stateValue)
{
    void* state = reinterpret_cast<void*>(stateValue);

    void* character = AiAlert_GetLinkedCharacter(state);
    void* characterContext = CSF_AT(void*, character, 0x2C);
    void* descriptionContext = CSF_AT(void*, characterContext, 0x34);
    AiAlert_GetErrorDescription(CSF_AT(int, descriptionContext, 0x300));

    character = AiAlert_GetLinkedCharacter(state);
    // Additional class-identity bindings supplied by the packet.
    (void)CSF_VTBL(0x004AEF70);
    S_004aef70_SelectUsableTarget(
        reinterpret_cast<S_004aef70*>(character), nullptr);

    character = AiAlert_GetLinkedCharacter(state);
    C_004D10F0_ResetFields(
        reinterpret_cast<void*>(
            reinterpret_cast<std::uintptr_t>(character) + 0x690),
        nullptr);

    character = AiAlert_GetLinkedCharacter(state);
    AiAlert_PrepareLinkedCharacter(character, nullptr);

    character = AiAlert_GetLinkedCharacter(state);
    CSF_AT(std::uint8_t, character, 0x6A0) = 1;

    void* actionCharacter = AiAlert_GetLinkedCharacter(state);
    character = AiAlert_GetLinkedCharacter(state);
    characterContext = CSF_AT(void*, character, 0x2C);
    TaskCharAction_48CFE0(
        CSF_AT(void*, characterContext, 0x204), nullptr,
        reinterpret_cast<int>(actionCharacter), 1, 1);

    character = AiAlert_GetLinkedCharacter(state);

    alignas(8) std::uint8_t targetHandleBytes[0xC];
    CSF_AT(int, targetHandleBytes, 0x4) = -1;
    CSF_AT(int, targetHandleBytes, 0x8) = -1;
    CSF_AT(void**, targetHandleBytes, 0x0) = CSF_VTBL(0x0082D448);

    characterContext = CSF_AT(void*, character, 0x2C);
    (void)CSF_VTBL(0x0048C9F0);
    AiAttackTasks_CopyTargetHandle(
        CSF_AT(void*, characterContext, 0x204), nullptr,
        reinterpret_cast<int>(targetHandleBytes));

    return AiAlert_GetStateName(self, nullptr, state);
}

HOOK(0x004B3C10, TAlerta_GetName, "auto");
