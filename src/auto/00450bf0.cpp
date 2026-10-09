#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_457A80, 0x00457A80, ());
CSF_FUNC(int, __fastcall, CCharActionFsm_CountQualifyingEntries, 0x00485A70, (CCharActionFsm* self, void*));
CSF_FUNC(int, __fastcall, CCharActionFsm_CountMatchingSlots, 0x00485AC0, (CCharActionFsm* self, void*));
CSF_FUNC(void, __fastcall, Character_ApplyPlanarInput, 0x00493B40, (void* self, void* edx, float a2, float a3));
CSF_FUNC(void, __fastcall, Character_RequestPrimaryAction, 0x00495270, (void* self, void* edx));
CSF_FUNC(char, __fastcall, Character_RequestSecondaryAction, 0x00495580, (void* self, void* edx));
CSF_FUNC(char, __fastcall, Character_RequestAuxiliaryAction, 0x00495AE0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, Character_RequestSpecialAction, 0x00495B10, (void* self, void* edx));
CSF_FUNC(void, __fastcall, SetObjectFlag80000, 0x00495B50, (void* self, void*));
CSF_FUNC(void, __fastcall, Character_RequestMovementAction, 0x00498FF0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, Character_RequestAlternateMovementAction, 0x00499050, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Character_ApplyVectorInput, 0x004998E0, (void* self, void* edx, float* a2));
CSF_FUNC(char, __fastcall, Character_EnableSpecialMode, 0x0049A8A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Character_DisableSpecialMode, 0x0049A970, (void* self, void* edx));

static void __fastcall DispatchSelectedCharacterCommand(
    void* self, void* /*edx*/, void* command)
{
    const uint32_t selectedIndex = CSF_AT(uint32_t, self, 0x70);
    if (selectedIndex == static_cast<uint32_t>(-1) ||
        selectedIndex >= CSF_AT(uint32_t, self, 0x68))
        return;

    void* character = CSF_AT(void**, self, 0x60)[selectedIndex];
    if (!character)
        return;
    if (CSF_AT(uint8_t, self, 0xF8))
        return;

    // The character's command interface is embedded at +4.
    void* commandInterface = static_cast<uint8_t*>(character) + 4;
    int32_t selection[2];
    uint32_t argumentWord;

    switch (CSF_AT(uint32_t, command, 0))
    {
    case 0:
        Character_ApplyPlanarInput(character, nullptr,
                                  CSF_AT(float, command, 4),
                                  CSF_AT(float, command, 12));
        break;
    case 1:
        Character_ApplyVectorInput(
            character, nullptr,
            reinterpret_cast<float*>(static_cast<uint8_t*>(command) + 4));
        break;
    case 2:
        Character_RequestMovementAction(character, nullptr);
        break;
    case 3:
        Character_RequestPrimaryAction(character, nullptr);
        break;
    case 4:
        Character_RequestAlternateMovementAction(character, nullptr);
        break;
    case 5:
        Character_RequestSecondaryAction(character, nullptr);
        break;
    case 6:
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, commandInterface, 0)[0x60 / 4])(
                commandInterface, nullptr);
        break;
    case 7:
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, commandInterface, 0)[0x64 / 4])(
                commandInterface, nullptr);
        break;
    case 9:
        CSF_AT(uint32_t, character, 0x2E4) |= 8u;
        break;
    case 0xA:
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, commandInterface, 0)[0x70 / 4])(
                commandInterface, nullptr);
        break;
    case 0xB:
        if (static_cast<uint32_t>(CCharActionFsm_CountQualifyingEntries(
                CSF_AT(CCharActionFsm*, character, 0x44), nullptr)) > 1)
        {
            selection[0] = 2;
            selection[1] = -1;
            reinterpret_cast<void(__fastcall*)(void*, void*, int32_t*)>(
                CSF_AT(void**, commandInterface, 0)[0x68 / 4])(
                    commandInterface, nullptr, selection);
        }
        break;
    case 0xC:
        if (static_cast<uint32_t>(CCharActionFsm_CountQualifyingEntries(
                CSF_AT(CCharActionFsm*, character, 0x44), nullptr)) > 1)
        {
            selection[0] = 1;
            selection[1] = -1;
            reinterpret_cast<void(__fastcall*)(void*, void*, int32_t*)>(
                CSF_AT(void**, commandInterface, 0)[0x68 / 4])(
                    commandInterface, nullptr, selection);
        }
        break;
    case 0xD:
        if (static_cast<uint32_t>(CCharActionFsm_CountMatchingSlots(
                CSF_AT(CCharActionFsm*, character, 0x44), nullptr)) > 1)
        {
            selection[0] = 2;
            selection[1] = -1;
            reinterpret_cast<void(__fastcall*)(void*, void*, int32_t*)>(
                CSF_AT(void**, commandInterface, 0)[0x6C / 4])(
                    commandInterface, nullptr, selection);
        }
        break;
    case 0xE:
        if (static_cast<uint32_t>(CCharActionFsm_CountMatchingSlots(
                CSF_AT(CCharActionFsm*, character, 0x44), nullptr)) > 1)
        {
            selection[0] = 1;
            selection[1] = -1;
            reinterpret_cast<void(__fastcall*)(void*, void*, int32_t*)>(
                CSF_AT(void**, commandInterface, 0)[0x6C / 4])(
                    commandInterface, nullptr, selection);
        }
        break;
    case 0xF:
        argumentWord = static_cast<uint32_t>(
            reinterpret_cast<uintptr_t>(command));
        reinterpret_cast<uint8_t*>(&argumentWord)[0] = 0;
        reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t*)>(
            CSF_AT(void**, commandInterface, 0)[0x5C / 4])(
                commandInterface, nullptr, &argumentWord);
        break;
    case 0x10:
        argumentWord = static_cast<uint32_t>(
            reinterpret_cast<uintptr_t>(command));
        reinterpret_cast<uint8_t*>(&argumentWord)[0] = 1;
        reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t*)>(
            CSF_AT(void**, commandInterface, 0)[0x5C / 4])(
                commandInterface, nullptr, &argumentWord);
        break;
    case 0x11:
        Character_RequestAuxiliaryAction(character, nullptr);
        break;
    case 0x12:
        CSF_AT(uint32_t, character, 0x2E4) |= 1u;
        break;
    case 0x13:
        CSF_AT(uint32_t, character, 0x2E4) &= ~1u;
        break;
    case 0x14:
        CSF_AT(uint32_t, character, 0x2E4) |= 2u;
        break;
    case 0x15:
        CSF_AT(uint32_t, character, 0x2E4) &= ~2u;
        break;
    case 0x16:
        Character_RequestSpecialAction(character, nullptr);
        break;
    case 0x17:
        if (!CSF_AT(uint32_t,
                    reinterpret_cast<void*>(GetGlobalValue_457A80()), 0x1C))
            SetObjectFlag80000(character, nullptr);
        break;
    case 0x18:
        Character_EnableSpecialMode(character, nullptr);
        break;
    case 0x19:
        Character_DisableSpecialMode(character, nullptr);
        break;
    default:
        return;
    }
}

HOOK(0x00450BF0, DispatchSelectedCharacterCommand, "auto");
