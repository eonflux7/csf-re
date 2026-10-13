#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, ResolveFlaggedControlAction, 0x004481B0, (int action, int handle));
CSF_FUNC(char*, __fastcall, AiCore_GetTaskTargetNameBuffer, 0x0047AE10, (S_0047ae10* self, void*));
CSF_FUNC(int, __fastcall, CharActionFsm_IsWeaponActionBlocked, 0x00492BD0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, AiAlert_UpdateCoverAttackFeedback, 0x004B30D0, (void* self, void* , float elapsedTime));
CSF_FUNC(char, __stdcall, AiDistracted_CheckExpiredDistraction, 0x004BA690, (int objectValue));
CSF_FUNC(void, __fastcall, AiGoToCover_UpdateCoverTask, 0x004C4B80, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, AiCore_UpdateTargetData, 0x004D1730, (S_0048c9f0* self, void* , int a2));
CSF_FUNC(int, __fastcall, IsField10Null, 0x004DB5B0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __stdcall, AiCoverAttackTasks_UpdateCharacterTasks, 0x004DBD50, (int a1, int a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static const auto g_TaskTargetNameBufferVtable = CSF_VTBL(0x0047AE10);

static void* AiGoToCover_GetLinkedCharacter(void* object)
{
    int linkedValue = GetLinkedObjectValue_4DB5C0(object, nullptr);
    return linkedValue ? reinterpret_cast<void*>(linkedValue - 4) : nullptr;
}

static int __stdcall TCombate_UpdateCoverAttackTasks(int objectValue, int elapsedTimeBits)
{
    void* object = reinterpret_cast<void*>(objectValue);

    AiCoverAttackTasks_UpdateCharacterTasks(objectValue, elapsedTimeBits);
    int result = IsField10Null(object, nullptr);
    if (static_cast<std::uint8_t>(result))
        return result;

    void* character = AiGoToCover_GetLinkedCharacter(object);
    result = CharActionFsm_IsWeaponActionBlocked(
        CSF_AT(void*, character, 0x2C), nullptr);
    if (static_cast<std::uint8_t>(result))
        return result;

    character = AiGoToCover_GetLinkedCharacter(object);
    AiGoToCover_UpdateCoverTask(
        reinterpret_cast<std::uint8_t*>(character) + 0x5DC,
        nullptr, elapsedTimeBits);

    character = AiGoToCover_GetLinkedCharacter(object);
    if (CSF_AT(int, character, 0x608) == 14)
    {
        character = AiGoToCover_GetLinkedCharacter(object);
        float elapsedTime;
        std::memcpy(&elapsedTime, &elapsedTimeBits, sizeof(elapsedTime));
        AiAlert_UpdateCoverAttackFeedback(character, nullptr, elapsedTime);

        character = AiGoToCover_GetLinkedCharacter(object);
        void* targetHandle = reinterpret_cast<std::uint8_t*>(character) +
            (CSF_AT(int, character, 0x630) < 0 ? 0x610 : 0x628);

        if (CSF_AT(int, targetHandle, 0x8) >= 0)
        {
            void* handleCharacter = AiGoToCover_GetLinkedCharacter(object);
            void* aiCharacter = AiGoToCover_GetLinkedCharacter(object);
            targetHandle = reinterpret_cast<std::uint8_t*>(handleCharacter) +
                (CSF_AT(int, handleCharacter, 0x630) < 0 ? 0x610 : 0x628);

            char* targetData = AiCore_GetTaskTargetNameBuffer(
                CSF_AT(S_0047ae10*, aiCharacter, 0x718), nullptr);
            if (CSF_AT(std::uint32_t, targetData, 0x14) !=
                CSF_AT(std::uint32_t, targetHandle, 0x4))
            {
                handleCharacter = AiGoToCover_GetLinkedCharacter(object);
                void* actionCharacter = AiGoToCover_GetLinkedCharacter(object);
                targetHandle = reinterpret_cast<std::uint8_t*>(handleCharacter) +
                    (CSF_AT(int, handleCharacter, 0x630) < 0 ? 0x610 : 0x628);

                void* action = reinterpret_cast<std::uint8_t*>(
                    CSF_AT(void*, actionCharacter, 0x2C)) + 0x1C;

                // Preserve the scene reference; its +0xC4 value only
                // supplied unused ECX to the stdcall callee.
                (void)g_pCurrentScene;
                ResolveFlaggedControlAction(
                    reinterpret_cast<int>(action),
                    reinterpret_cast<int>(targetHandle));
            }
        }

        void* handleCharacter = AiGoToCover_GetLinkedCharacter(object);
        void* aiCharacter = AiGoToCover_GetLinkedCharacter(object);
        targetHandle = reinterpret_cast<std::uint8_t*>(handleCharacter) +
            (CSF_AT(int, handleCharacter, 0x630) < 0 ? 0x610 : 0x628);

        char* targetData = AiCore_GetTaskTargetNameBuffer(
            CSF_AT(S_0047ae10*, aiCharacter, 0x718), nullptr);
        AiCore_UpdateTargetData(
            reinterpret_cast<S_0048c9f0*>(targetData), nullptr,
            reinterpret_cast<int>(targetHandle));
    }

    return AiDistracted_CheckExpiredDistraction(objectValue);
}

HOOK(0x004C4F80, TCombate_UpdateCoverAttackTasks, "auto");
