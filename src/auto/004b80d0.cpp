#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, AiAttackTasks_CopyTargetHandle, 0x0048C9F0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, Dispatch_NoLinkedTarget, 0x004DBCB0, (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static const auto g_TargetHandleVtable = CSF_VTBL(0x0048C9F0);

static void* AiCombat_GetLinkedOwner(void* context)
{
    const int linked = GetLinkedObjectValue_4DB5C0(context, nullptr);
    return linked
        ? reinterpret_cast<void*>(static_cast<std::uintptr_t>(linked) - 4)
        : nullptr;
}

static void* AiCombat_GetPreferredTargetHandle(void* owner)
{
    const std::uintptr_t offset =
        CSF_AT(std::int32_t, owner, 0x630) < 0 ? 0x610 : 0x628;
    return reinterpret_cast<void*>(
        reinterpret_cast<std::uintptr_t>(owner) + offset);
}

static int __stdcall AiCombat_UpdateAttackTarget(int contextAddress, float elapsedTime)
{
    (void)g_TargetHandleVtable;
    void* context = reinterpret_cast<void*>(contextAddress);

    void* owner = AiCombat_GetLinkedOwner(context);
    CSF_AT(float, owner, 0x69C) =
        CSF_AT(float, owner, 0x69C) - elapsedTime;

    owner = AiCombat_GetLinkedOwner(context);
    if (CSF_AT(float, owner, 0x69C) < g_fZero)
        return Dispatch_NoLinkedTarget(context, nullptr, 0);

    owner = AiCombat_GetLinkedOwner(context);
    if (CSF_AT(std::int32_t, owner, 0x608) == -1 &&
        CSF_AT(std::int32_t, owner, 0x60C) == -1)
        return Dispatch_NoLinkedTarget(context, nullptr, 0);

    owner = AiCombat_GetLinkedOwner(context);
    if (!CSF_AT(std::uint8_t, owner, 0x684))
        return Dispatch_NoLinkedTarget(context, nullptr, 0);

    owner = AiCombat_GetLinkedOwner(context);
    void* handle = AiCombat_GetPreferredTargetHandle(owner);
    if (CSF_AT(std::int32_t, handle, 0x8) < 0)
        return Dispatch_NoLinkedTarget(context, nullptr, 0);

    owner = AiCombat_GetLinkedOwner(context);
    handle = AiCombat_GetPreferredTargetHandle(owner);
    void* target = reinterpret_cast<void*>(
        CHandleTable_Resolve(g_pCurrentScene, nullptr,
                            reinterpret_cast<int>(handle)));
    if (!target || CSF_AT(std::uint32_t, target, 0x50))
        return Dispatch_NoLinkedTarget(context, nullptr, 0);

    void* targetOwner = AiCombat_GetLinkedOwner(context);
    void* taskOwner = AiCombat_GetLinkedOwner(context);
    handle = AiCombat_GetPreferredTargetHandle(targetOwner);

    void* actor = CSF_AT(void*, taskOwner, 0x2C);
    void* attackTask = CSF_AT(void*, actor, 0x204);
    return AiAttackTasks_CopyTargetHandle(
        attackTask, nullptr, reinterpret_cast<int>(handle));
}

HOOK(0x004B80D0, AiCombat_UpdateAttackTarget, "auto");
