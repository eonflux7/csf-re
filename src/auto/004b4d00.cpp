#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, AttackTasks_SetFieldTransitionScale, 0x0047B150, (uint32_t* self, void* , char enableScale));
CSF_FUNC(int, __fastcall, AiAttackTasks_CopyTargetHandle, 0x0048C9F0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, TaskCharAction_48CFE0, 0x0048CFE0, (void* self, void* edx, int a2, char a3, char a4));
CSF_FUNC(char*, __fastcall, TAiState_GetName, 0x004AB2A0, (void* self, void* edx, void* a2));
CSF_FUNC(C_008319c0*, __fastcall, S_004aef70_SelectUsableTarget, 0x004AEF70, (S_004aef70* self, void*));
CSF_FUNC(void, __fastcall, AiAlert_SetCombatDelay, 0x004B30D0, (void* self, void* edx, float a3));
CSF_FUNC(int, __fastcall, C_004D10F0_ResetFields, 0x004D10F0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(char*, __cdecl, AiAlert_LookupDescription, 0x00761400, (int a1));

static uint8_t* AiAlert_GetLinkedOwner(void* context)
{
    const uint32_t linked =
        static_cast<uint32_t>(GetLinkedObjectValue_4DB5C0(context, nullptr));
    return reinterpret_cast<uint8_t*>(linked ? linked - 4 : 0);
}

static const char* __fastcall TCombate_GetName(
    TCombate* self, void* /*edx*/, int a2)
{
    // Code addresses also classified as vtable references by the packet.
    (void)CSF_VTBL(0x004AEF70);
    (void)CSF_VTBL(0x0048C9F0);

    void* context = reinterpret_cast<void*>(a2);
    uint8_t* owner = AiAlert_GetLinkedOwner(context);
    void* character = CSF_AT(void*, owner, 0x2C);
    void* descriptionSource = CSF_AT(void*, character, 0x34);
    AiAlert_LookupDescription(CSF_AT(int, descriptionSource, 0x304));

    owner = AiAlert_GetLinkedOwner(context);
    S_004aef70_SelectUsableTarget(
        reinterpret_cast<S_004aef70*>(owner), nullptr);

    owner = AiAlert_GetLinkedOwner(context);
    AiAlert_SetCombatDelay(owner, nullptr, 5.0f);

    owner = AiAlert_GetLinkedOwner(context);
    C_004D10F0_ResetFields(
        reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(owner) + 0x690),
        nullptr);

    owner = AiAlert_GetLinkedOwner(context);
    AttackTasks_SetFieldTransitionScale(
        CSF_AT(uint32_t*, owner, 0x718), nullptr, 1);

    uint8_t* actionOwner = AiAlert_GetLinkedOwner(context);
    owner = AiAlert_GetLinkedOwner(context);
    character = CSF_AT(void*, owner, 0x2C);
    TaskCharAction_48CFE0(
        CSF_AT(void*, character, 0x204), nullptr,
        static_cast<int>(reinterpret_cast<uintptr_t>(actionOwner)), 7, 1);

    owner = AiAlert_GetLinkedOwner(context);
    alignas(8) uint8_t targetHandleBytes[0xC];
    CSF_AT(int, targetHandleBytes, 0x4) = -1;
    CSF_AT(int, targetHandleBytes, 0x8) = -1;
    CSF_AT(void**, targetHandleBytes, 0x0) = CSF_VTBL(0x0082D448);

    character = CSF_AT(void*, owner, 0x2C);
    AiAttackTasks_CopyTargetHandle(
        CSF_AT(void*, character, 0x204), nullptr,
        static_cast<int>(reinterpret_cast<uintptr_t>(targetHandleBytes)));

    return TAiState_GetName(self, nullptr, context);
}

HOOK(0x004B4D00, TCombate_GetName, "auto");
