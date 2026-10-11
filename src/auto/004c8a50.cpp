#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __fastcall, AiCoverAttackTasks_TestMovementThreshold, 0x004AA4D0, (void* self, void* edx, float a2));
CSF_FUNC(bool, __fastcall, AiCoverAttackTasks_IsMovementBlocked, 0x004AA5A0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, AiCoverAttackTasks_TestCoverPosition, 0x004ADAE0, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, AiCoverAttackTasks_TestAttackPosition, 0x004B2E20, (void* self, void* edx, float* a2, int a3));
CSF_FUNC(char, __fastcall, AiCoverAttackTasks_TestCoverApproach, 0x004B2EE0, (void* self, void* edx, float* a2, int a3));
CSF_FUNC(char, __fastcall, AiCoverAttackTasks_SetCoverMode, 0x004B33E0, (void* self, void* edx, char arg0));
CSF_FUNC(int, __stdcall, AiCoverAttackTasks_RefreshMovement, 0x004C95B0, (int a1));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_SetCoverTransition, 0x004D1100, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_SelectAction, 0x004DC380, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GetStateValue, 0x0050DD30, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

// The packet also classifies the movement predicate's address as a vtable.
// No vtable access is present at that call site.
static const auto AiCoverAttackTasks_ReportedVtable = CSF_VTBL(0x004AA4D0);

static uint8_t* AiCoverAttackTasks_GetCharacter(void* task)
{
    const int linkedObject = GetLinkedObjectValue_4DB5C0(task, nullptr);
    return reinterpret_cast<uint8_t*>(linkedObject ? linkedObject - 4 : 0);
}

static char __stdcall AiCoverAttackTasks_UpdateAttackFromCover(
    int taskAddress, int /*unused2*/, int /*unused3*/)
{
    void* task = reinterpret_cast<void*>(taskAddress);

    uint8_t* character = AiCoverAttackTasks_GetCharacter(task);
    void* owner = CSF_AT(void*, character, 0x2C);
    void* actor = CSF_AT(void*, owner, 0x34);

    bool hasCoverPosition = false;
    character = AiCoverAttackTasks_GetCharacter(task);
    if (CSF_AT(int, character, 0x608) != -1)
    {
        character = AiCoverAttackTasks_GetCharacter(task);
        uint8_t* coverPosition = character + 0x628;
        if (CSF_AT(int, character, 0x630) < 0)
            coverPosition = character + 0x610;

        hasCoverPosition = AiCoverAttackTasks_TestCoverPosition(
            character + 0x5DC, nullptr,
            reinterpret_cast<int>(coverPosition)) != 0;
    }

    const bool isSpecialActor = CSF_AT(int, actor, 0x304) == 3;

    if (hasCoverPosition)
    {
        uint8_t* positionCharacter = AiCoverAttackTasks_GetCharacter(task);
        uint8_t* approachCharacter = AiCoverAttackTasks_GetCharacter(task);
        uint8_t* coverPosition = positionCharacter + 0x628;
        if (CSF_AT(int, positionCharacter, 0x630) < 0)
            coverPosition = positionCharacter + 0x610;

        if (!CSF_AT(int, approachCharacter, 0x6C8)
            || CSF_AT(int, approachCharacter, 0x6C0) == -1
            || CSF_AT(int, approachCharacter, 0x6C4) == -1
            || !AiCoverAttackTasks_TestCoverApproach(
                approachCharacter, nullptr,
                reinterpret_cast<float*>(approachCharacter + 0x6B4),
                reinterpret_cast<int>(coverPosition)))
        {
            goto RefreshMovement;
        }

        positionCharacter = AiCoverAttackTasks_GetCharacter(task);
        uint8_t* attackCharacter = AiCoverAttackTasks_GetCharacter(task);
        coverPosition = positionCharacter + 0x628;
        if (CSF_AT(int, positionCharacter, 0x630) < 0)
            coverPosition = positionCharacter + 0x610;

        if (CSF_AT(int, attackCharacter, 0x6C8)
            && CSF_AT(int, attackCharacter, 0x6C0) != -1
            && CSF_AT(int, attackCharacter, 0x6C4) != -1
            && AiCoverAttackTasks_TestAttackPosition(
                attackCharacter, nullptr,
                reinterpret_cast<float*>(attackCharacter + 0x6B4),
                reinterpret_cast<int>(coverPosition)))
        {
            if (!isSpecialActor
                || GetStateValue(
                    reinterpret_cast<void*>(GetGlobalState_50D190()),
                    nullptr, 100) >= 30)
            {
                const int actions = GetLinkedObjectField10(task, nullptr);
                return static_cast<char>(
                    AiCoverAttackTasks_SelectAction(task, nullptr, actions + 28));
            }

            const int actions = GetLinkedObjectField10(task, nullptr);
            return static_cast<char>(
                AiCoverAttackTasks_SelectAction(task, nullptr, actions + 16));
        }
    }

    character = AiCoverAttackTasks_GetCharacter(task);
    if (!CSF_AT(uint8_t, character, 0x690))
    {
        const uint32_t stateValue = static_cast<uint32_t>(GetStateValue(
            reinterpret_cast<void*>(GetGlobalState_50D190()), nullptr, 100));

        if (hasCoverPosition && stateValue < 0x1E)
        {
            const int actions = GetLinkedObjectField10(task, nullptr);
            return static_cast<char>(
                AiCoverAttackTasks_SelectAction(task, nullptr, actions + 20));
        }

        if (isSpecialActor
            && GetStateValue(
                reinterpret_cast<void*>(GetGlobalState_50D190()),
                nullptr, 100) < 30)
        {
            const int actions = GetLinkedObjectField10(task, nullptr);
            return static_cast<char>(
                AiCoverAttackTasks_SelectAction(task, nullptr, actions + 16));
        }

        character = AiCoverAttackTasks_GetCharacter(task);
        if (!AiCoverAttackTasks_IsMovementBlocked(character, nullptr))
        {
            character = AiCoverAttackTasks_GetCharacter(task);
            bool resetCover = !CSF_AT(uint8_t, character, 0x684);
            if (!resetCover)
            {
                character = AiCoverAttackTasks_GetCharacter(task);
                resetCover = CSF_AT(int, character, 0x608) == -1;
            }

            if (resetCover)
            {
                character = AiCoverAttackTasks_GetCharacter(task);
                AiCoverAttackTasks_SetCoverMode(character, nullptr, 0);

                character = AiCoverAttackTasks_GetCharacter(task);
                void* globalObject =
                    reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
                const int transition = CSF_AT(int, globalObject, 0xA98);
                AiCoverAttackTasks_SetCoverTransition(
                    character + 0x690, nullptr, 0, transition);
            }
        }
    }

RefreshMovement:
    AiCoverAttackTasks_RefreshMovement(taskAddress);
    character = AiCoverAttackTasks_GetCharacter(task);
    if (CSF_AT(uint8_t, character, 0x690))
        return static_cast<char>(reinterpret_cast<uintptr_t>(character));

    character = AiCoverAttackTasks_GetCharacter(task);
    uint8_t* movementCharacter =
        AiCoverAttackTasks_GetCharacter(character + 0x8);
    void* movementOwner = CSF_AT(void*, movementCharacter, 0x2C);
    void* movementParameters = CSF_AT(void*, movementOwner, 0x3C);
    const float threshold = CSF_AT(float, movementParameters, 0x34);

    const bool thresholdReached = AiCoverAttackTasks_TestMovementThreshold(
        character, nullptr, threshold);
    if (thresholdReached)
        return static_cast<char>(thresholdReached);

    character = AiCoverAttackTasks_GetCharacter(task);
    if (CSF_AT(uint8_t, character, 0x684))
        return static_cast<char>(reinterpret_cast<uintptr_t>(character));

    const int stateValue = GetStateValue(
        reinterpret_cast<void*>(GetGlobalState_50D190()), nullptr, 100);
    if (stateValue >= 10)
        return static_cast<char>(stateValue);

    character = AiCoverAttackTasks_GetCharacter(task);
    AiCoverAttackTasks_SetCoverMode(character, nullptr, 0);

    character = AiCoverAttackTasks_GetCharacter(task);
    void* globalObject = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    const int transition = CSF_AT(int, globalObject, 0xA98);
    return static_cast<char>(AiCoverAttackTasks_SetCoverTransition(
        character + 0x690, nullptr, 0, transition));
}

HOOK(0x004C8A50, AiCoverAttackTasks_UpdateAttackFromCover, "auto");
