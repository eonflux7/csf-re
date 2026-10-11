#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __stdcall, CoverAttack_SetupParapetCommands, 0x004C65F0, (int a1));
CSF_FUNC(int, __stdcall, CoverAttackTasks_SetupCloseRangeCommands, 0x004C6760, (int a1));
CSF_FUNC(int, __stdcall, CoverAttackTasks_SetupMidRangeCommands, 0x004C6890, (int a1));
CSF_FUNC(void, __fastcall, CoverAttackTasks_DispatchStep, 0x004CA0F0, (void* self, void* edx, int a1));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, CoverAttackTasks_SelectCommand, 0x004DC380, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GetStateValue, 0x0050DD30, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

CSF_GLOBAL(float, g_CoverAttackFarDistanceSquared, 0x0082D4C0);
CSF_GLOBAL(float, g_CoverAttackNearDistanceSquared, 0x0083A9DC);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static uint8_t* CoverAttackTasks_GetOwner(C_0085e760* task)
{
    int linked = GetLinkedObjectValue_4DB5C0(task, nullptr);
    return linked ? reinterpret_cast<uint8_t*>(linked - 4) : nullptr;
}

static int CoverAttackTasks_GetTargetHandleAddress(uint8_t* owner)
{
    uintptr_t base = reinterpret_cast<uintptr_t>(owner);
    return static_cast<int>(base +
        (CSF_AT(int, owner, 0x630) < 0 ? 0x610 : 0x628));
}

// Preserve incoming ECX for DispatchStep, while popping the original three
// stack arguments. The last two stack arguments are unused.
static void __fastcall C_0085e760_UpdateCoverAttackState(
    void* context, void* /*edx*/, C_0085e760* task,
    int /*unused2*/, int /*unused3*/)
{
    int globalState = GetGlobalState_50D190();
    uint32_t state = static_cast<uint32_t>(
        GetStateValue(reinterpret_cast<void*>(globalState), nullptr, 100));

    if (state < 0x19)
    {
        uint8_t* owner = CoverAttackTasks_GetOwner(task);
        if (CSF_AT(uint8_t, owner, 0x684))
        {
            CoverAttackTasks_DispatchStep(
                context, nullptr, reinterpret_cast<int>(task));
        }
        else
        {
            int commands = GetLinkedObjectField10(task, nullptr);
            CoverAttackTasks_SelectCommand(task, nullptr, commands + 20);
        }
        return;
    }

    if (state >= 0x32)
    {
        int commands = GetLinkedObjectField10(task, nullptr);
        CoverAttackTasks_SelectCommand(task, nullptr, commands + 12);
        return;
    }

    uint8_t* owner = CoverAttackTasks_GetOwner(task);
    int handleAddress = CoverAttackTasks_GetTargetHandleAddress(owner);
    if (CSF_AT(int, reinterpret_cast<void*>(handleAddress), 8) < 0)
    {
        int commands = GetLinkedObjectField10(task, nullptr);
        CoverAttackTasks_SelectCommand(task, nullptr, commands + 12);
        return;
    }

    owner = CoverAttackTasks_GetOwner(task);
    handleAddress = CoverAttackTasks_GetTargetHandleAddress(owner);
    void* target = reinterpret_cast<void*>(
        CHandleTable_Resolve(g_pCurrentScene, nullptr, handleAddress));
    if (!target)
        return;

    owner = CoverAttackTasks_GetOwner(task);
    float* ownerPosition = reinterpret_cast<float*>(
        GetObjectPositionVector(CSF_AT(void*, owner, 0x2C), nullptr));
    float* targetPosition = reinterpret_cast<float*>(
        GetObjectPositionVector(target, nullptr));

    float dx = targetPosition[0] - ownerPosition[0];
    float dy = targetPosition[1] - ownerPosition[1];
    float dz = targetPosition[2] - ownerPosition[2];
    float distanceSquared = dz * dz + dx * dx + dy * dy;

    if (distanceSquared > g_CoverAttackFarDistanceSquared)
    {
        int commands = GetLinkedObjectField10(task, nullptr);
        CoverAttackTasks_SelectCommand(task, nullptr, commands + 4);
        int entry = ResolveEntryLookupValue(task, nullptr);
        void* provider = GetCachedObjectProvider(
            reinterpret_cast<void*>(entry), nullptr);
        GetLinkedObjectField10(task, nullptr);
        CoverAttack_SetupParapetCommands(reinterpret_cast<int>(provider));
    }
    else if (distanceSquared < g_CoverAttackNearDistanceSquared)
    {
        int commands = GetLinkedObjectField10(task, nullptr);
        CoverAttackTasks_SelectCommand(task, nullptr, commands + 4);
        int entry = ResolveEntryLookupValue(task, nullptr);
        void* provider = GetCachedObjectProvider(
            reinterpret_cast<void*>(entry), nullptr);
        GetLinkedObjectField10(task, nullptr);
        CoverAttackTasks_SetupCloseRangeCommands(reinterpret_cast<int>(provider));
    }
    else
    {
        owner = CoverAttackTasks_GetOwner(task);
        if (CSF_AT(int, owner, 0x314) == 3)
        {
            int commands = GetLinkedObjectField10(task, nullptr);
            CoverAttackTasks_SelectCommand(task, nullptr, commands + 4);
            int entry = ResolveEntryLookupValue(task, nullptr);
            void* provider = GetCachedObjectProvider(
                reinterpret_cast<void*>(entry), nullptr);
            GetLinkedObjectField10(task, nullptr);
            CoverAttackTasks_SetupMidRangeCommands(reinterpret_cast<int>(provider));
        }
        else
        {
            CoverAttackTasks_DispatchStep(
                context, nullptr, reinterpret_cast<int>(task));
        }
    }
}

HOOK(0x004CA160, C_0085e760_UpdateCoverAttackState, "auto");
