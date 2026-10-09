#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Callees (bound by address)
CSF_FUNC(int, __fastcall, SceneObject_SetCategory, 0x00490E00, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __stdcall, MissionSetup_CreateObjectOfType, 0x004E8EA0, (int a2, int a3));
CSF_FUNC(unsigned int, __fastcall, MissionSetup_InitType31Object, 0x004EA140, (void* self, void* edx, void* a2, int a3));
CSF_FUNC(int, __fastcall, MissionSetup_InitType32Object, 0x004EA2E0, (void* self, void* edx, void* a1, void* a2));
CSF_FUNC(unsigned int, __fastcall, MissionSetup_InitType18Object, 0x004EA770, (void* self, void* edx, void* a2, int a3, char a4));
CSF_FUNC(unsigned int, __fastcall, MissionSetup_InitUntypedObject, 0x004EA980, (void* self, void* edx, void* arg0, int a3));
CSF_FUNC(unsigned int, __fastcall, MissionSetup_InitRemovableObject, 0x004EC900, (void* self, void* edx, void* a2, int a3, int a4));
CSF_FUNC(unsigned int, __fastcall, MissionSetup_InitDefinedCategoryObject, 0x004ECDB0, (void* self, void* edx, void* a2, int a3));
CSF_FUNC(int, __cdecl, Scene_IsObjectRemovable, 0x007558C0, (int a1));
CSF_FUNC(int, __cdecl, Scene_IsDefinedCategoryType, 0x00755940, (int a1));

// Creates a scene object from its definition, initialises it according to the
// definition's object type and assigns its category.
static C_00833908* __fastcall S_004e9090_CreateSceneObjectFromDefinition(
    S_004e9090* self, void* /*edx*/, C_00833908* definitionKey, uint32_t ownerId)
{
    void* definitionTable = reinterpret_cast<void*>(self->field_1fc8);
    void* definition = ((void*(__fastcall*)(void*, void*, C_00833908*))CSF_AT(void**, definitionTable, 0)[2])(
        definitionTable, nullptr, definitionKey);

    int objectType = CSF_AT(int, definition, 0xB4);
    if (objectType == 0)
        objectType = 18;

    C_00833908* object = reinterpret_cast<C_00833908*>(
        MissionSetup_CreateObjectOfType(reinterpret_cast<int>(definitionKey), objectType));

    if (ownerId != 0xFFFFFFFF)
    {
        object->field_0020 = ownerId;
        object->field_0024 = 0xFFFFFFFF;
    }

    if (objectType == 0)
    {
        MissionSetup_InitUntypedObject(self, nullptr, object, reinterpret_cast<int>(definitionKey));
    }
    else if (static_cast<uint8_t>(Scene_IsObjectRemovable(objectType)))
    {
        MissionSetup_InitRemovableObject(self, nullptr, object, reinterpret_cast<int>(definitionKey), 1);
    }
    else if (static_cast<uint8_t>(Scene_IsDefinedCategoryType(objectType)))
    {
        MissionSetup_InitDefinedCategoryObject(self, nullptr, object, reinterpret_cast<int>(definitionKey));
    }
    else
    {
        switch (objectType)
        {
        case 18:
            MissionSetup_InitType18Object(self, nullptr, object, reinterpret_cast<int>(definitionKey), 1);
            break;
        case 31:
            MissionSetup_InitType31Object(self, nullptr, object, reinterpret_cast<int>(definitionKey));
            break;
        case 32:
            MissionSetup_InitType32Object(self, nullptr, object, definitionKey);
            break;
        default:
            break;
        }
    }

    if (!object)
        return object;

    definitionTable = reinterpret_cast<void*>(self->field_1fc8);
    definition = ((void*(__fastcall*)(void*, void*, C_00833908*))CSF_AT(void**, definitionTable, 0)[2])(
        definitionTable, nullptr, definitionKey);

    switch (objectType)
    {
    case 11:
        SceneObject_SetCategory(object, nullptr, 6);
        break;
    case 14:
        SceneObject_SetCategory(object, nullptr, 10);
        break;
    case 18:
        SceneObject_SetCategory(object, nullptr, 12);
        break;
    case 31:
        SceneObject_SetCategory(object, nullptr, 19);
        break;
    case 32:
        SceneObject_SetCategory(object, nullptr, 20);
        break;
    default:
        if (static_cast<uint8_t>(Scene_IsDefinedCategoryType(objectType)))
        {
            int definedCategory = CSF_AT(int, definition, 0xD8);
            if (definedCategory != 0)
                SceneObject_SetCategory(object, nullptr, definedCategory);
        }
        break;
    }
    return object;
}

HOOK(0x004EE260, S_004e9090_CreateSceneObjectFromDefinition, "auto");
