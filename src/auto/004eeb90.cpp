#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SceneMissionSetup_SetObjectState, 0x00490E00, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __stdcall, SceneMissionSetup_CreateObject, 0x004E8EA0, (int a2, int a3));
CSF_FUNC(unsigned int, __fastcall, SceneMissionSetup_InitializeType31, 0x004EA140, (void* self, void* edx, void* a2, int a3));
CSF_FUNC(int, __fastcall, SceneMissionSetup_InitializeType32, 0x004EA2E0, (void* self, void* edx, void* a1, void* a2));
CSF_FUNC(unsigned int, __fastcall, SceneMissionSetup_InitializeAllowedStateObject, 0x004EA480, (void* self, void* edx, void* a2, int a3, char a4));
CSF_FUNC(unsigned int, __fastcall, SceneMissionSetup_InitializeTypeZero, 0x004EA980, (void* self, void* edx, void* arg0, int a3));
CSF_FUNC(char, __fastcall, SceneMissionSetup_InitializeCharacter, 0x004EAEE0, (void* self, void* edx, void* a2, int a3, int a4, int a5));
CSF_FUNC(unsigned int, __fastcall, SceneMissionSetup_InitializeType4, 0x004EB250, (void* self, void* edx, void* a2, int a3));
CSF_FUNC(unsigned int, __fastcall, SceneMissionSetup_InitializeType5, 0x004EB500, (void* self, void* edx, void* a2, int a3));
CSF_FUNC(unsigned int, __fastcall, SceneMissionSetup_InitializeType33, 0x004EB7B0, (void* self, void* edx, void* a2, int a3));
CSF_FUNC(int, __fastcall, SceneMissionSetup_InitializeType7, 0x004EBA60, (void* self, void* edx, void* a1, void* a2));
CSF_FUNC(unsigned int, __fastcall, SceneMissionSetup_InitializeType6, 0x004EBD10, (void* self, void* edx, void* a2, int a3));
CSF_FUNC(unsigned int, __fastcall, SceneMissionSetup_InitializeType25, 0x004EBFC0, (void* self, void* edx, void* a2, int a3));
CSF_FUNC(unsigned int, __fastcall, SceneMissionSetup_InitializeType30, 0x004EC240, (void* self, void* edx, void* a2, int a3));
CSF_FUNC(unsigned int, __fastcall, SceneMissionSetup_InitializeType19, 0x004EC4C0, (void* self, void* edx, void* a2, int a3));
CSF_FUNC(int, __fastcall, SceneMissionSetup_InitializeTypes12And34, 0x004EC6C0, (void* self, void* edx, void* a1, void* a2));
CSF_FUNC(unsigned int, __fastcall, SceneMissionSetup_InitializeRemovableObject, 0x004EC900, (void* self, void* edx, void* a2, int a3, int a4));
CSF_FUNC(unsigned int, __fastcall, SceneMissionSetup_InitializeType13, 0x004ECC30, (void* self, void* edx, void* a2, int a3));
CSF_FUNC(unsigned int, __fastcall, SceneMissionSetup_InitializeSpecialStateObject, 0x004ECDB0, (void* self, void* edx, void* a2, int a3));
CSF_FUNC(int, __fastcall, SceneMissionSetup_InitializeType29, 0x004ED030, (void* self, void* edx, void* a1, void* a2));
CSF_FUNC(uint32_t*, __fastcall, SceneMissionSetup_InitializeType28, 0x004EE420, (void* self, void* edx, void* a2, int a3));
CSF_FUNC(bool, __cdecl, SceneMissionSetup_CanCreateState, 0x004F0B90, (int a1));
CSF_FUNC(void, __cdecl, SceneMissionSetup_ApplyObjectDefinition, 0x004F0BD0, (int a1, int a2));
CSF_FUNC(int, __cdecl, IsStateCodeAllowed, 0x00755810, (unsigned int a1));
CSF_FUNC(int, __cdecl, Scene_IsObjectRemovable, 0x007558C0, (int a1));
CSF_FUNC(int, __cdecl, SceneMissionSetup_IsSpecialStateCode, 0x00755940, (int a1));

static C_00833908* __fastcall S_004e9090_CreateMissionObject(
    S_004e9090* self, void* /*edx*/, C_00833908* source, int objectId)
{
    void* definitions = reinterpret_cast<void*>(self->field_1fc8);
    C_00833908* definition =
        reinterpret_cast<C_00833908* (__fastcall*)(void*, void*, C_00833908*)>(
            CSF_AT(void**, definitions, 0)[2])(definitions, nullptr, source);

    if (!SceneMissionSetup_CanCreateState(definition->field_00d8))
        return nullptr;

    const int sourceAddress = reinterpret_cast<int>(source);
    C_00833908* object = reinterpret_cast<C_00833908*>(
        SceneMissionSetup_CreateObject(sourceAddress, definition->field_00b4));

    if (objectId != -1)
    {
        object->field_0020 = objectId;
        object->field_0024 = -1;
    }

    char enableFlag = 1;
    if (definition->field_00b4 != 26)
        enableFlag = definition->field_0158;

    if (enableFlag)
        object->field_0170 = object->field_0170 | 0x20;
    else
        object->field_0170 = object->field_0170 & 0xFFFFFFDF;

    if (!definition->field_00b4)
    {
        SceneMissionSetup_InitializeTypeZero(self, nullptr, object, sourceAddress);
    }
    else if (static_cast<uint8_t>(IsStateCodeAllowed(definition->field_00b4)) &&
             definition->field_00b4 != 31)
    {
        SceneMissionSetup_InitializeAllowedStateObject(
            self, nullptr, object, sourceAddress, 0);
    }
    else if (static_cast<uint8_t>(Scene_IsObjectRemovable(definition->field_00b4)))
    {
        SceneMissionSetup_InitializeRemovableObject(
            self, nullptr, object, sourceAddress, 0);
    }
    else if (static_cast<uint8_t>(
                 SceneMissionSetup_IsSpecialStateCode(definition->field_00b4)))
    {
        SceneMissionSetup_InitializeSpecialStateObject(
            self, nullptr, object, sourceAddress);
    }
    else
    {
        switch (definition->field_00b4)
        {
        case 4u:
            SceneMissionSetup_InitializeType4(self, nullptr, object, sourceAddress);
            break;
        case 5u:
            SceneMissionSetup_InitializeType5(self, nullptr, object, sourceAddress);
            break;
        case 6u:
            SceneMissionSetup_InitializeType6(self, nullptr, object, sourceAddress);
            break;
        case 7u:
            SceneMissionSetup_InitializeType7(self, nullptr, object, source);
            break;
        case 0xCu:
        case 0x22u:
            SceneMissionSetup_InitializeTypes12And34(self, nullptr, object, source);
            break;
        case 0xDu:
            SceneMissionSetup_InitializeType13(self, nullptr, object, sourceAddress);
            break;
        case 0x13u:
            SceneMissionSetup_InitializeType19(self, nullptr, object, sourceAddress);
            break;
        case 0x19u:
            SceneMissionSetup_InitializeType25(self, nullptr, object, sourceAddress);
            break;
        case 0x1Au:
        case 0x1Bu:
            if (!SceneMissionSetup_InitializeCharacter(
                    self, nullptr, object, sourceAddress, objectId,
                    definition->field_00b4))
            {
                if (!object)
                    return object;
                reinterpret_cast<void* (__fastcall*)(void*, void*, unsigned int)>(
                    CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
                return nullptr;
            }
            break;
        case 0x1Cu:
            SceneMissionSetup_InitializeType28(self, nullptr, object, sourceAddress);
            break;
        case 0x1Du:
            SceneMissionSetup_InitializeType29(self, nullptr, object, source);
            break;
        case 0x1Eu:
            SceneMissionSetup_InitializeType30(self, nullptr, object, sourceAddress);
            break;
        case 0x1Fu:
            SceneMissionSetup_InitializeType31(self, nullptr, object, sourceAddress);
            break;
        case 0x20u:
            SceneMissionSetup_InitializeType32(self, nullptr, object, source);
            break;
        case 0x21u:
            SceneMissionSetup_InitializeType33(self, nullptr, object, sourceAddress);
            break;
        default:
            break;
        }
    }

    if (object)
    {
        int state = definition->field_00d8;
        switch (definition->field_00b4)
        {
        case 0xCu:
        case 0x22u:
            state = 9;
            break;
        case 0xDu:
            state = 8;
            break;
        case 0x1Au:
            state = 16;
            break;
        case 0x1Bu:
            state = 17;
            break;
        case 0x20u:
            state = 20;
            break;
        default:
            break;
        }

        SceneMissionSetup_SetObjectState(object, nullptr, state);
        SceneMissionSetup_ApplyObjectDefinition(
            reinterpret_cast<int>(object), reinterpret_cast<int>(definition));
    }
    return object;
}

HOOK(0x004EEB90, S_004e9090_CreateMissionObject, "auto");
