#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetValue_8afc80, 0x004419C0, ());
CSF_FUNC(int, __fastcall, CScene_AttachObjectLinks, 0x00452800, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, CScene_DetachObjectLinks, 0x004528D0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(void, __fastcall, SceneLoader_RemoveObjectEntry, 0x00475480, (void* self, void* edx, int a2));
CSF_FUNC(char*, __fastcall, SceneLoader_SetObjectName, 0x0048E0B0, (void* self, void* edx, char* Source));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_ConfigureFocusEntry, 0x0048E460, (void* self, void* edx, int* a2));
CSF_FUNC(int, __fastcall, SceneLoader_SetObjectColor, 0x0048E790, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, SceneLoader_SetObjectProperty1F4, 0x00492BA0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, SceneLoader_SetObjectProperty1F8, 0x00492BB0, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, SceneLoader_ProcessSceneObject, 0x004DE7C0, (void* self, void* edx, void* a2));
CSF_FUNC(char, __fastcall, SceneLoader_UnregisterSceneObject, 0x004DE7D0, (void* self, void* edx, void* a2));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(C_00833908*, __fastcall, SceneLoader_ResolveObject, 0x004EEEF0, (S_004e9090* self, void* edx, int a2, uint32_t a3, char a4));
CSF_FUNC(int32_t, __cdecl, GetGlobalValue_51F740, 0x0051F740, ());
CSF_FUNC(int, __fastcall, SceneLoader_RemoveObjectReference, 0x005201D0, (void* self, void* edx, int a2));
CSF_FUNC(int, __stdcall, SceneLoader_ReleaseManagedObject, 0x00750C00, (void* object));

static int __fastcall CScene_ReplaceObject(
    CScene* self, void* /*edx*/, int objectReference, int objectDefinition)
{
    void* reference = reinterpret_cast<void*>(objectReference);
    int result = CSF_AT(int, reference, 0x08);
    if (result < 0 || result >= CSF_AT(int, self, 0x18))
        return result;

    void* oldObject = CSF_AT(void**, self, 0x10)[result];
    if (!oldObject)
        return result;

    result = CSF_AT(int, oldObject, 0x20);
    if (result != CSF_AT(int, reference, 0x04))
        return result;

    const int linkIndex = CSF_AT(int, oldObject, 0x94);
    CScene_DetachObjectLinks(self, nullptr, objectReference, linkIndex);

    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, oldObject, 0)[0x44 / 4])(oldObject, nullptr, 0);

    void* oldComponent = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
        CSF_AT(void**, oldObject, 0)[0x28 / 4])(oldObject, nullptr);
    if (oldComponent)
    {
        void* manager = reinterpret_cast<void*>(GetGlobalValue_51F740());
        SceneLoader_RemoveObjectReference(manager, nullptr, objectReference);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, oldComponent, 0)[0x34 / 4])(oldComponent, nullptr, 0);
        if (reinterpret_cast<bool(__fastcall*)(void*, void*)>(
                CSF_AT(void**, oldComponent, 0)[0xA8 / 4])(oldComponent, nullptr))
        {
            reinterpret_cast<void(__fastcall*)(void*, void*)>(
                CSF_AT(void**, oldComponent, 0)[0xA4 / 4])(oldComponent, nullptr);
        }
    }

    SceneLoader_RemoveObjectEntry(
        CSF_AT(void*, self, 0x184), nullptr,
        reinterpret_cast<int>(static_cast<uint8_t*>(oldObject) + 0x10));

    void* sceneObjectManager = CSF_AT(void*, self, 0x12C);
    if (sceneObjectManager)
        SceneLoader_UnregisterSceneObject(sceneObjectManager, nullptr, oldObject);

    const uint32_t objectId = CSF_AT(uint32_t, reference, 0x04);
    auto* loader = reinterpret_cast<S_004e9090*>(GetValue8B0554());
    C_00833908* replacement =
        SceneLoader_ResolveObject(loader, nullptr, objectDefinition, objectId, 0);

    replacement->field_0020 = CSF_AT(uint32_t, reference, 0x04);
    replacement->field_0024 = CSF_AT(uint32_t, reference, 0x08);
    CSF_AT(C_00833908**, self, 0x10)[CSF_AT(int, reference, 0x08)] = replacement;
    CScene_AttachObjectLinks(self, nullptr, objectReference, linkIndex);

    if (replacement->field_0050)
        replacement->vSlot44(1);

    if (CSF_AT(uint8_t, self, 0x180) &&
        reinterpret_cast<bool(__fastcall*)(void*, void*)>(
            replacement->vtbl[0x48 / 4])(replacement, nullptr))
    {
        float color[5] = {0.4f, 0.4f, 0.4f, 1.0f, 0.0f};
        SceneLoader_SetObjectColor(replacement, nullptr, reinterpret_cast<int>(color));
    }

    sceneObjectManager = CSF_AT(void*, self, 0x12C);
    if (sceneObjectManager)
        SceneLoader_ProcessSceneObject(sceneObjectManager, nullptr, replacement);

    if (replacement->vGetField016c())
    {
        void* component = reinterpret_cast<void*>(replacement->vGetField016c());
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, component, 0)[0x34 / 4])(component, nullptr, 1);
    }

    SceneLoader_SetObjectName(
        replacement, nullptr, reinterpret_cast<char*>(oldObject) + 0x58);
    int* position = reinterpret_cast<int*>(GetObjectPositionVector(oldObject, nullptr));
    SceneLoader_ConfigureFocusEntry(replacement, nullptr, position);

    oldComponent = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
        CSF_AT(void**, oldObject, 0)[0x28 / 4])(oldObject, nullptr);
    void* newComponent = reinterpret_cast<void*>(replacement->vGetField016c());
    auto setComponentValue = reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, newComponent, 0)[0x48 / 4]);
    int componentValue = reinterpret_cast<int(__fastcall*)(void*, void*)>(
        CSF_AT(void**, oldComponent, 0)[0x44 / 4])(oldComponent, nullptr);
    setComponentValue(newComponent, nullptr, componentValue);

    oldComponent = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
        CSF_AT(void**, oldObject, 0)[0x28 / 4])(oldObject, nullptr);
    void* oldParameters = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
        CSF_AT(void**, oldComponent, 0)[0x18 / 4])(oldComponent, nullptr);
    newComponent = reinterpret_cast<void*>(replacement->vGetField016c());
    void* newParameters = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
        CSF_AT(void**, newComponent, 0)[0x18 / 4])(newComponent, nullptr);
    auto setFirstParameter = reinterpret_cast<void(__fastcall*)(void*, void*, float)>(
        CSF_AT(void**, newParameters, 0)[0x30 / 4]);
    float firstParameter = static_cast<float>(
        reinterpret_cast<double(__fastcall*)(void*, void*)>(
            CSF_AT(void**, oldParameters, 0)[0x24 / 4])(oldParameters, nullptr));
    setFirstParameter(newParameters, nullptr, firstParameter);

    oldComponent = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
        CSF_AT(void**, oldObject, 0)[0x28 / 4])(oldObject, nullptr);
    oldParameters = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
        CSF_AT(void**, oldComponent, 0)[0x18 / 4])(oldComponent, nullptr);
    newComponent = reinterpret_cast<void*>(replacement->vGetField016c());
    newParameters = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
        CSF_AT(void**, newComponent, 0)[0x18 / 4])(newComponent, nullptr);
    auto setSecondParameter = reinterpret_cast<void(__fastcall*)(void*, void*, float)>(
        CSF_AT(void**, newParameters, 0)[0x34 / 4]);
    float secondParameter = static_cast<float>(
        reinterpret_cast<double(__fastcall*)(void*, void*)>(
            CSF_AT(void**, oldParameters, 0)[0x28 / 4])(oldParameters, nullptr));
    setSecondParameter(newParameters, nullptr, secondParameter);

    replacement->field_0098 = CSF_AT(uint32_t, oldObject, 0x98);
    replacement->field_0090 = CSF_AT(uint32_t, oldObject, 0x90);
    replacement->field_0078 = CSF_AT(uint32_t, oldObject, 0x78);

    if ((CSF_AT(uint32_t, oldObject, 0x170) & 0x2000) != 0)
    {
        SceneLoader_SetObjectProperty1F4(
            replacement, nullptr, CSF_AT(int, oldObject, 0x1F4));
        SceneLoader_SetObjectProperty1F8(
            replacement, nullptr, CSF_AT(int, oldObject, 0x1F8));
    }

    if (CSF_AT(int8_t, oldObject, 0x170) >= 0)
    {
        return reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, oldObject, 0)[0])(oldObject, nullptr, 1);
    }

    GetValue_8afc80();
    return SceneLoader_ReleaseManagedObject(oldObject);
}

HOOK(0x00452C70, CScene_ReplaceObject, "auto");
