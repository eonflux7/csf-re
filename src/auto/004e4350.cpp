#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetValue_8afc80, 0x004419C0, ());
CSF_FUNC(int, __fastcall, SceneLoader_CreateObject, 0x00441F10,
         (void* self, void* edx, int a2, int a3, int a4));
CSF_FUNC(int, __fastcall, CScene_AddObject, 0x00453420,
         (CScene* self, void*, C_00867260* object));
CSF_FUNC(void, __fastcall, Game_SetMappedValue, 0x00455DF0,
         (void* self, void*, const void* key, const void* value));
CSF_FUNC(int*, __fastcall, GetObjectPositionVector, 0x0048E430,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_ConfigureFocusEntry, 0x0048E460,
         (void* self, void* edx, int* a2));
CSF_FUNC(int, __fastcall, GetRelatedObjectAndClearFlag, 0x0048E6F0,
         (void* self, void*));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static char __fastcall C_0083e7d8_InitializeReplacementObject(
    C_0083e7d8* self, void* /*edx*/, int /*unused*/)
{
    if (self->field_0028)
    {
        const char flag = static_cast<char>(self->field_0048);
        if (flag)
        {
            self->field_0050 = 10.0f;
            self->field_004c = 2;
        }
        return flag;
    }

    void* registry = CSF_AT(
        void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FC8);
    auto* source = reinterpret_cast<C_00833908*>(self->field_000c);
    void* definition = reinterpret_cast<void*>(source->field_002c);
    const int definitionId = CSF_AT(int, definition, 0xC0);
    const int index =
        reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, registry, 0)[0x10 / 4])(
                registry, nullptr, definitionId);

    char result = static_cast<char>(index);
    if (index != -1)
    {
        registry = CSF_AT(
            void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FC8);
        void* record =
            reinterpret_cast<void*(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, registry, 0)[0x8 / 4])(
                    registry, nullptr, index);

        const int objectId = CSF_AT(int, record, 0);
        const int objectType = CSF_AT(int, record, 0xB4);
        void* factory = reinterpret_cast<void*>(GetValue_8afc80());
        const int createdObject =
            SceneLoader_CreateObject(factory, nullptr, objectType, objectId, 0);

        void* sourceForPosition = reinterpret_cast<void*>(self->field_000c);
        self->field_0054 = static_cast<uint32_t>(createdObject);
        int* position = GetObjectPositionVector(sourceForPosition, nullptr);
        SceneLoader_ConfigureFocusEntry(
            reinterpret_cast<void*>(self->field_0054), nullptr, position);

        source = reinterpret_cast<C_00833908*>(self->field_000c);
        void* sourceRelated = reinterpret_cast<void*>(source->vGetField016c());
        void* sourceState =
            reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, sourceRelated, 0)[0x18 / 4])(
                    sourceRelated, nullptr);

        auto* replacement = reinterpret_cast<C_00833908*>(self->field_0054);
        void* replacementRelated =
            reinterpret_cast<void*>(replacement->vGetField016c());
        void* replacementState =
            reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, replacementRelated, 0)[0x18 / 4])(
                    replacementRelated, nullptr);

        void** replacementStateVtable = CSF_AT(void**, replacementState, 0);
        const int stateValue =
            reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, sourceState, 0)[0x48 / 4])(
                    sourceState, nullptr);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            replacementStateVtable[0x4C / 4])(
                replacementState, nullptr, stateValue);

        auto* sceneObject = reinterpret_cast<C_00867260*>(self->field_0054);
        CScene_AddObject(
            reinterpret_cast<CScene*>(g_pCurrentScene), nullptr, sceneObject);

        replacement = reinterpret_cast<C_00833908*>(self->field_0054);
        source = reinterpret_cast<C_00833908*>(self->field_000c);
        const void* mappedValue = &replacement->field_001c;
        const void* mappedKey = &source->field_001c;
        Game_SetMappedValue(g_pCurrentScene, nullptr, mappedKey, mappedValue);

        replacement = reinterpret_cast<C_00833908*>(self->field_0054);
        replacementRelated =
            reinterpret_cast<void*>(replacement->vGetField016c());
        void* replacementController =
            reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, replacementRelated, 0)[0x78 / 4])(
                    replacementRelated, nullptr);
        if (replacementController)
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, int, uint32_t*)>(
                CSF_AT(void**, replacementController, 0)[0x48 / 4])(
                    replacementController, nullptr, 1189765120,
                    &self->field_002c);
        }

        GetRelatedObjectAndClearFlag(
            reinterpret_cast<void*>(self->field_000c), nullptr);

        source = reinterpret_cast<C_00833908*>(self->field_000c);
        sourceRelated = reinterpret_cast<void*>(source->vGetField016c());
        void* sourceController =
            reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, sourceRelated, 0)[0x78 / 4])(
                    sourceRelated, nullptr);

        result = 0;
        if (sourceController)
        {
            const bool active =
                reinterpret_cast<bool(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, sourceController, 0)[0x28 / 4])(
                        sourceController, nullptr);
            if (active)
            {
                result = reinterpret_cast<char(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, sourceController, 0)[0x24 / 4])(
                        sourceController, nullptr);
            }
        }
    }

    self->field_0028 = 1;
    return result;
}

HOOK(0x004E4350, C_0083e7d8_InitializeReplacementObject, "auto");
