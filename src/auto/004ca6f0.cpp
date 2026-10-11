#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, AiCoverAttackTasks_GetCoverDestination, 0x004A9CA0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_IsCoverActionComplete, 0x004AA110, (void* self, void* edx));
CSF_FUNC(int, __stdcall, AiCoverAttackTasks_CheckAttackTask, 0x004AA440, (void* a1));
CSF_FUNC(bool, __fastcall, AiCoverAttackTasks_ShouldRefreshCover, 0x004AA5A0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, AiCoverAttackTasks_UpdateCharacterAttack, 0x004AEE90, (void* self, void* edx, void* a2, int a3));
CSF_FUNC(char, __fastcall, AiCoverAttackTasks_SetCharacterAttackMode, 0x004B33E0, (void* self, void* edx, char arg0));
CSF_FUNC(char, __fastcall, AiCoverAttackTasks_ShouldEndAttack, 0x004B9950, (void* self, void* edx));
CSF_FUNC(int, __stdcall, AiCoverAttackTasks_AssignCoverDestination, 0x004C9170, (int a1, uint32_t* arg4));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_RefreshCoverState, 0x004D1100, (void* self, void* edx, int a2, int a3));
CSF_FUNC(void, __fastcall, AiCoverAttackTasks_UpdateCoverState, 0x004D1120, (void* self, void* edx, float a2));
CSF_FUNC(int, __fastcall, IsField10Null, 0x004DB5B0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __stdcall, AiCoverAttackTasks_UpdateTaskBase, 0x004DBD50, (int a1, int a2));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_SetTaskState, 0x004DC380, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static uint8_t* AiCoverAttackTasks_GetLinkedCharacter(void* task)
{
    const int linked = GetLinkedObjectValue_4DB5C0(task, nullptr);
    return linked ? reinterpret_cast<uint8_t*>(linked - 4) : nullptr;
}

static uint32_t* __stdcall C_0085e760_UpdateCoverAttack(
    C_0085e760* self, float deltaTime)
{
    int deltaTimeBits;
    std::memcpy(&deltaTimeBits, &deltaTime, sizeof(deltaTimeBits));
    AiCoverAttackTasks_UpdateTaskBase(reinterpret_cast<int>(self), deltaTimeBits);

    int status = IsField10Null(self, nullptr);
    if (static_cast<uint8_t>(status))
        return reinterpret_cast<uint32_t*>(status);

    uint8_t* character = AiCoverAttackTasks_GetLinkedCharacter(self);
    void* controller = CSF_AT(void*, character, 0x2C);
    void* actionInterface = CSF_AT(void*, controller, 0x48);
    status = reinterpret_cast<int(__fastcall*)(void*, void*)>(
        CSF_AT(void**, actionInterface, 0)[0x40 / 4])(actionInterface, nullptr);
    if (status)
        return reinterpret_cast<uint32_t*>(status);

    GetLinkedObjectValue_4DB5C0(self, nullptr);
    status = AiCoverAttackTasks_CheckAttackTask(self);
    if (static_cast<uint8_t>(status))
        return reinterpret_cast<uint32_t*>(status);

    character = AiCoverAttackTasks_GetLinkedCharacter(self);
    if (CSF_AT(uint8_t, character, 0x690))
    {
        character = AiCoverAttackTasks_GetLinkedCharacter(self);
        bool refreshCover = AiCoverAttackTasks_ShouldRefreshCover(character, nullptr);
        if (!refreshCover)
        {
            character = AiCoverAttackTasks_GetLinkedCharacter(self);
            if (CSF_AT(uint8_t, character, 0x684))
            {
                character = AiCoverAttackTasks_GetLinkedCharacter(self);
                refreshCover = CSF_AT(int, character, 0x608) != -1;
            }
        }

        if (refreshCover)
        {
            GetLinkedObjectValue_4DB5C0(self, nullptr);
            character = AiCoverAttackTasks_GetLinkedCharacter(self);
            const int globalState = GetGlobalValue_8BC1E0();
            AiCoverAttackTasks_RefreshCoverState(
                character + 0x690, nullptr, 0,
                CSF_AT(int, reinterpret_cast<void*>(globalState), 0xA98));
        }
        else
        {
            character = AiCoverAttackTasks_GetLinkedCharacter(self);
            AiCoverAttackTasks_SetCharacterAttackMode(character, nullptr, 0);
            character = AiCoverAttackTasks_GetLinkedCharacter(self);
            AiCoverAttackTasks_UpdateCoverState(character + 0x690, nullptr, deltaTime);
        }
    }

    character = AiCoverAttackTasks_GetLinkedCharacter(self);
    AiCoverAttackTasks_UpdateCharacterAttack(character, nullptr, self, deltaTimeBits);
    character = AiCoverAttackTasks_GetLinkedCharacter(self);
    if (AiCoverAttackTasks_ShouldEndAttack(character, nullptr))
    {
        const int linkedState = GetLinkedObjectField10(self, nullptr);
        return reinterpret_cast<uint32_t*>(
            AiCoverAttackTasks_SetTaskState(self, nullptr, linkedState + 36));
    }

    character = AiCoverAttackTasks_GetLinkedCharacter(self);
    if (!CSF_AT(int, character, 0x25C))
        return reinterpret_cast<uint32_t*>(character);

    character = AiCoverAttackTasks_GetLinkedCharacter(self);
    void* coverAction = character + 0x258;
    reinterpret_cast<void(__fastcall*)(void*, void*, C_0085e760*, int)>(
        CSF_AT(void**, coverAction, 0)[0x2C / 4])(
            coverAction, nullptr, self, deltaTimeBits);

    character = AiCoverAttackTasks_GetLinkedCharacter(self);
    status = AiCoverAttackTasks_IsCoverActionComplete(character + 0x258, nullptr);
    if (!static_cast<uint8_t>(status))
        return reinterpret_cast<uint32_t*>(status);

    character = AiCoverAttackTasks_GetLinkedCharacter(self);
    controller = CSF_AT(void*, character, 0x2C);
    actionInterface = CSF_AT(void*, controller, 0x44);
    const bool ready = reinterpret_cast<bool(__fastcall*)(void*, void*)>(
        CSF_AT(void**, actionInterface, 0)[0x90 / 4])(actionInterface, nullptr);
    if (!ready)
        return nullptr;

    character = AiCoverAttackTasks_GetLinkedCharacter(self);
    if (CSF_AT(int, character, 0x25C) == 3)
    {
        const int linkedState = GetLinkedObjectField10(self, nullptr);
        AiCoverAttackTasks_SetTaskState(self, nullptr, linkedState + 24);

        character = AiCoverAttackTasks_GetLinkedCharacter(self);
        uint32_t* destination = AiCoverAttackTasks_GetCoverDestination(
            character + 0x290, nullptr);
        const int entry = ResolveEntryLookupValue(self, nullptr);
        void* provider = GetCachedObjectProvider(reinterpret_cast<void*>(entry), nullptr);
        GetLinkedObjectField10(self, nullptr);
        return reinterpret_cast<uint32_t*>(
            AiCoverAttackTasks_AssignCoverDestination(
                reinterpret_cast<int>(provider), destination));
    }

    character = AiCoverAttackTasks_GetLinkedCharacter(self);
    coverAction = character + 0x258;
    reinterpret_cast<void(__fastcall*)(void*, void*, C_0085e760*)>(
        CSF_AT(void**, coverAction, 0)[0x28 / 4])(coverAction, nullptr, self);

    character = AiCoverAttackTasks_GetLinkedCharacter(self);
    CSF_AT(float, character, 0x264) = 0.3f;
    CSF_AT(float, character, 0x268) = 0.3f;
    return reinterpret_cast<uint32_t*>(character);
}

HOOK(0x004CA6F0, C_0085e760_UpdateCoverAttack, "auto");
