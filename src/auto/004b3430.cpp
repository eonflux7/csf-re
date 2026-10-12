#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, AiAttackTasks_CopyTargetHandle, 0x0048C9F0, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, AttackTasks_SetCoverAttackPosition, 0x0048CA40, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, TargetRecord_Activate, 0x004ADB50, (void* self, void* edx));
CSF_FUNC(void, __fastcall, AiStatic_UpdateCoverAttack, 0x004AF1C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(bool, __fastcall, C_0083e9d0_ResolveAndDispatchHandleEntry, 0x004E55C0, (C_0083e9d0* self, void* , int a2, int a3));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GetStateValue, 0x0050DD30, (void* self, void* edx, int a2));

[[maybe_unused]] static const auto g_AttackTasksPacketVtable =
    CSF_VTBL(0x0048C9F0);

static std::uint8_t* AiAlert_GetLinkedOwner(void* link)
{
    int value = GetLinkedObjectValue_4DB5C0(link, nullptr);
    return reinterpret_cast<std::uint8_t*>(value ? value - 4 : 0);
}

static std::uint8_t* AiAlert_SelectTargetHandle(std::uint8_t* owner)
{
    return owner + (CSF_AT(int, owner, 0x630) < 0 ? 0x610 : 0x628);
}

static int __fastcall AiAlert_UpdateDistractedLinkedState(
    void* self, void* /*edx*/)
{
    auto* link = static_cast<std::uint8_t*>(self) + 8;
    auto* owner = AiAlert_GetLinkedOwner(link);
    void* actor = CSF_AT(void*, owner, 0x2C);
    void* state = CSF_AT(void*, actor, 0x30);

    bool enabled = reinterpret_cast<bool (__fastcall*)(void*, void*)>(
        CSF_AT(void**, state, 0)[0xB8 / 4])(state, nullptr);
    if (!enabled)
        return 0;

    owner = AiAlert_GetLinkedOwner(link);
    bool clearTarget =
        CSF_AT(int, owner, 0x608) == -1 &&
        CSF_AT(int, owner, 0x60C) == -1;

    if (!clearTarget)
    {
        owner = AiAlert_GetLinkedOwner(link);
        if (CSF_AT(int, owner, 0x60C) == -1)
        {
            owner = AiAlert_GetLinkedOwner(link);
            auto* handle = AiAlert_SelectTargetHandle(owner);
            if (CSF_AT(int, handle, 0x8) >= 0)
            {
                auto* handleOwner = AiAlert_GetLinkedOwner(link);
                auto* dispatchOwner = AiAlert_GetLinkedOwner(link);
                handle = AiAlert_SelectTargetHandle(handleOwner);
                actor = CSF_AT(void*, dispatchOwner, 0x2C);
                auto* dispatcher = CSF_AT(C_0083e9d0*, actor, 0x3C);
                clearTarget = !C_0083e9d0_ResolveAndDispatchHandleEntry(
                    dispatcher, nullptr, reinterpret_cast<int>(handle), 14);
            }
        }
    }

    if (clearTarget)
    {
        owner = AiAlert_GetLinkedOwner(link);

        alignas(8) std::uint8_t emptyHandleBytes[0x10];
        CSF_AT(void**, emptyHandleBytes, 0x0) = CSF_VTBL(0x0082D448);
        CSF_AT(int, emptyHandleBytes, 0x4) = -1;
        CSF_AT(int, emptyHandleBytes, 0x8) = -1;
        CSF_AT(void*, emptyHandleBytes, 0xC) = self;

        actor = CSF_AT(void*, owner, 0x2C);
        return AiAttackTasks_CopyTargetHandle(
            CSF_AT(void*, actor, 0x204), nullptr,
            reinterpret_cast<int>(emptyHandleBytes));
    }

    owner = AiAlert_GetLinkedOwner(link);
    auto* handle = AiAlert_SelectTargetHandle(owner);
    if (CSF_AT(int, handle, 0x8) >= 0)
    {
        auto* handleOwner = AiAlert_GetLinkedOwner(link);
        auto* dispatchOwner = AiAlert_GetLinkedOwner(link);
        handle = AiAlert_SelectTargetHandle(handleOwner);
        actor = CSF_AT(void*, dispatchOwner, 0x2C);
        auto* dispatcher = CSF_AT(C_0083e9d0*, actor, 0x3C);

        if (C_0083e9d0_ResolveAndDispatchHandleEntry(
                dispatcher, nullptr, reinterpret_cast<int>(handle), 2))
        {
            handleOwner = AiAlert_GetLinkedOwner(link);
            auto* attackOwner = AiAlert_GetLinkedOwner(link);
            handle = AiAlert_SelectTargetHandle(handleOwner);
            actor = CSF_AT(void*, attackOwner, 0x2C);
            return AiAttackTasks_CopyTargetHandle(
                CSF_AT(void*, actor, 0x204), nullptr,
                reinterpret_cast<int>(handle));
        }
    }

    owner = AiAlert_GetLinkedOwner(link);
    actor = CSF_AT(void*, owner, 0x2C);
    if (CSF_AT(int, actor, 0x1D8) == 2)
    {
        owner = AiAlert_GetLinkedOwner(link);
        if (CSF_AT(int, owner, 0x60C) != -1)
        {
            owner = AiAlert_GetLinkedOwner(link);
            if (static_cast<std::uint8_t>(
                    TargetRecord_Activate(owner + 0x5DC, nullptr)) != 0)
            {
                void* globalState =
                    reinterpret_cast<void*>(GetGlobalState_50D190());
                int stateValue = GetStateValue(globalState, nullptr, 100);
                if (stateValue < 10)
                {
                    AiStatic_UpdateCoverAttack(self, nullptr);
                    return 0;
                }
                return stateValue;
            }
        }
    }

    auto* positionOwner = AiAlert_GetLinkedOwner(link);
    auto* attackOwner = AiAlert_GetLinkedOwner(link);
    actor = CSF_AT(void*, attackOwner, 0x2C);
    AttackTasks_SetCoverAttackPosition(
        CSF_AT(void*, actor, 0x204), nullptr,
        reinterpret_cast<uint32_t*>(positionOwner + 0x64C));
    return 0;
}

HOOK(0x004B3430, AiAlert_UpdateDistractedLinkedState, "auto");
