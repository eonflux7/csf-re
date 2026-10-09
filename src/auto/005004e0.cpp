#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, SceneLoader_SetComponentFlag, 0x0040F4B0, (void* self, void* edx, int a2, char a3));
CSF_FUNC(int, __fastcall, CompareStringSubstring, 0x0042E200, (void* string, void* edx, unsigned int position, unsigned int count, const char* other, unsigned int otherCount));
CSF_FUNC(int, __fastcall, SceneLoader_AssociateSceneEntry, 0x00452A80, (void* self, void* edx, int a2, int a3, int a4));
CSF_FUNC(int, __fastcall, SceneLoader_FindSceneEntry, 0x00456650, (void* self, void* edx, char* String1));
CSF_FUNC(int, __fastcall, SceneLoader_AssociateSlotHandle, 0x0045AA30, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, SceneSlotMgr_FindSlot, 0x00469040, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, SceneSlotMgr_ReleaseSlot, 0x00469310, (void* self, void* edx, uint32_t* slot));
CSF_FUNC(char*, __fastcall, SceneLoader_SetObjectName, 0x0048E0B0, (void* self, void* edx, char* Source));
CSF_FUNC(int, __fastcall, SceneLoader_SetObjectPosition, 0x0048E460, (void* self, void* edx, int* a2));
CSF_FUNC(int, __fastcall, ApplyStringToResolvedObject, 0x0048E730, (void* self, void* edx, char* a2));
CSF_FUNC(int, __fastcall, HandleTargetUnavailable, 0x0048FE90, (void* self, void* edx));
CSF_FUNC(char, __fastcall, SceneLoader_SetObjectTransform, 0x00490790, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(char, __fastcall, SetMenuContextState, 0x0049A0E0, (void* self, void* edx, int a4, char a5));
CSF_FUNC(unsigned int, __fastcall, SceneGrid_RegisterHandlePosition, 0x004DD220, (void* self, void* edx, int a2, uint32_t* a3));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __stdcall, SceneLoader_RegisterObject, 0x004E8DF0, (void* a1));
CSF_FUNC(C_00833908*, __fastcall, SceneLoader_ResolveObject, 0x004EEEF0, (S_004e9090* self, void* edx, int a2, uint32_t a3, char a4));
CSF_FUNC(int, __fastcall, GetSceneRegionAt, 0x004FC130, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __fastcall, SceneLoader_SetMatrixTranslation, 0x006AD9E0, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, SceneLoader_SetMatrixRotation, 0x006AE2F0, (void* self, void* edx, float a2, float a3, float a4));
CSF_FUNC(int, __cdecl, IsStateCodeAllowed, 0x00755810, (unsigned int a1));
CSF_FUNC(int, __cdecl, IsResolvedEntryActive, 0x00755880, (int a1));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);
CSF_GLOBAL(float, g_SceneDegreesToRadians, 0x0083F4B4);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static C_008342e8* __stdcall SceneLoader_LoadRegionObject(int regionIndex)
{
    auto* scene = reinterpret_cast<void*>(GetValue8B0554());
    auto* region = reinterpret_cast<uint8_t*>(
        GetSceneRegionAt(scene, nullptr, regionIndex));

    const uint32_t definition = CSF_AT(uint32_t, region, 0x4);
    const int objectId = CSF_AT(int, region, 0x0);
    auto* resolver = reinterpret_cast<S_004e9090*>(GetValue8B0554());
    auto* object = reinterpret_cast<C_008342e8*>(
        SceneLoader_ResolveObject(resolver, nullptr, objectId, definition, 0));
    C_008342e8* result = object;
    if (!object)
        return result;

    if (!object->field_0050)
        SetMenuContextState(object, nullptr, 1, 1);

    if (object->field_0050 == 22)
    {
        uint32_t matrixState[2];
        auto* matrix = RwMatrixCreate();
        const float secondAngle =
            CSF_AT(float, region, 0x78) * g_SceneDegreesToRadians;
        matrixState[0] = reinterpret_cast<uint32_t>(matrix);
        const float firstAngle =
            CSF_AT(float, region, 0x74) * g_SceneDegreesToRadians;
        matrixState[1] = 1;
        SceneLoader_SetMatrixRotation(
            matrixState, nullptr, firstAngle, secondAngle, 0.0f);
        SceneLoader_SetMatrixTranslation(
            matrixState, nullptr, reinterpret_cast<uint32_t*>(region + 0x2C));
        SceneLoader_SetObjectTransform(object, nullptr, matrixState);
        if (CSF_AT(uint8_t, matrixState, 0x4))
            RwMatrixDestroy(reinterpret_cast<uint32_t*>(matrixState[0]));
    }
    else
    {
        SceneLoader_SetObjectPosition(
            object, nullptr, reinterpret_cast<int*>(region + 0x2C));

        auto* component = reinterpret_cast<void*>(object->vGetField016c());
        auto* orientation =
            reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, component, 0)[0x18 / 4])(component, nullptr);
        reinterpret_cast<void (__fastcall*)(void*, void*, float)>(
            CSF_AT(void**, orientation, 0)[0x30 / 4])(
                orientation, nullptr, CSF_AT(float, region, 0x74));

        component = reinterpret_cast<void*>(object->vGetField016c());
        orientation =
            reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, component, 0)[0x18 / 4])(component, nullptr);
        reinterpret_cast<void (__fastcall*)(void*, void*, float)>(
            CSF_AT(void**, orientation, 0)[0x34 / 4])(
                orientation, nullptr, CSF_AT(float, region, 0x78));
    }

    char* name = CSF_AT(uint32_t, region, 0x20) < 0x10u
        ? reinterpret_cast<char*>(region + 0xC)
        : CSF_AT(char*, region, 0xC);
    SceneLoader_SetObjectName(object, nullptr, name);

    auto* component = reinterpret_cast<void*>(object->vGetField016c());
    reinterpret_cast<void (__fastcall*)(void*, void*, unsigned int)>(
        CSF_AT(void**, component, 0)[0x48 / 4])(
            component, nullptr, CSF_AT(uint8_t, region, 0x7C));

    const bool needsComponentUpdate = object->field_0050 == 26;
    object->field_0098 = CSF_AT(uint32_t, region, 0xDC);
    if (needsComponentUpdate)
    {
        auto* stateComponent = reinterpret_cast<void*>(object->field_004c);
        reinterpret_cast<void (__fastcall*)(void*, void*)>(
            CSF_AT(void**, stateComponent, 0)[0x48 / 4])(
                stateComponent, nullptr);
    }

    if (object->field_0050 == 28)
    {
        const uint32_t flags = CSF_AT(uint32_t, region, 0xDC);
        auto* stateComponent = reinterpret_cast<void*>(object->field_004c);
        const char flagTwo = (flags & 2) == 2;
        const char flagFour = (flags & 4) == 4;
        const char flagOne = (flags & 1) == 1;
        SceneLoader_SetComponentFlag(stateComponent, nullptr, 1, flagOne);
        SceneLoader_SetComponentFlag(stateComponent, nullptr, 2, flagTwo);
        SceneLoader_SetComponentFlag(stateComponent, nullptr, 4, flagFour);
    }

    char* resolvedName = CSF_AT(uint32_t, region, 0xB8) < 0x10u
        ? reinterpret_cast<char*>(region + 0xA4)
        : CSF_AT(char*, region, 0xA4);
    if (resolvedName)
        ApplyStringToResolvedObject(object, nullptr, resolvedName);

    GetValue8B0554();
    SceneLoader_RegisterObject(object);
    scene = reinterpret_cast<void*>(GetValue8B0554());
    SceneGrid_RegisterHandlePosition(
        reinterpret_cast<uint8_t*>(CSF_AT(void*, scene, 0x1FD8)) + 0x34,
        nullptr,
        reinterpret_cast<int>(&object->field_001c),
        reinterpret_cast<uint32_t*>(region + 0x2C));

    const uint32_t regionValue = CSF_AT(uint32_t, region, 0x24);
    if (regionValue != uint32_t(-1))
        object->field_0078 = regionValue;

    if (CompareStringSubstring(
            region + 0xC0, nullptr, 0, CSF_AT(uint32_t, region, 0xD4),
            reinterpret_cast<const char*>(&g_byte_82E65C), 0)
        && static_cast<uint8_t>(IsStateCodeAllowed(object->field_0050)))
    {
        char* entryName = CSF_AT(uint32_t, region, 0xD8) < 0x10u
            ? reinterpret_cast<char*>(region + 0xC4)
            : CSF_AT(char*, region, 0xC4);
        const int entry = SceneLoader_FindSceneEntry(
            g_pCurrentScene, nullptr, entryName);
        if (entry != -1)
            SceneLoader_AssociateSceneEntry(
                g_pCurrentScene, nullptr, 1,
                reinterpret_cast<int>(&object->field_001c), entry);
    }

    if (static_cast<uint8_t>(IsResolvedEntryActive(object->field_0050)))
    {
        if (CSF_AT(uint8_t, region, 0xBC))
        {
            auto* activeComponent = reinterpret_cast<void*>(object->field_0038);
            if (activeComponent)
                CSF_AT(uint8_t, activeComponent, 0x48) = 1;
        }
    }

    result = reinterpret_cast<C_008342e8*>(object->field_0034);
    if (result)
    {
        uint32_t slotCount = 0;
        const uint32_t slotsBegin = CSF_AT(uint32_t, region, 0x84);
        if (slotsBegin)
        {
            slotCount = (CSF_AT(uint32_t, region, 0x88) - slotsBegin) >> 2;
            result = reinterpret_cast<C_008342e8*>(slotCount);
        }

        for (uint32_t index = 0; index < slotCount; )
        {
            uint32_t* slot = SceneSlotMgr_FindSlot(
                CSF_AT(void*, g_pCurrentScene, 0xBC), nullptr,
                CSF_AT(int, CSF_AT(void*, region, 0x84), 4 * index));
            if (slot)
            {
                if (CSF_AT(uint8_t, reinterpret_cast<void*>(slot[4]), 0))
                {
                    HandleTargetUnavailable(object, nullptr);
                    SceneSlotMgr_ReleaseSlot(
                        CSF_AT(void*, g_pCurrentScene, 0xBC), nullptr, slot);
                }
                else
                {
                    SceneLoader_AssociateSlotHandle(
                        slot, nullptr, reinterpret_cast<int>(&object->field_001c));
                }
            }
            result = reinterpret_cast<C_008342e8*>(++index);
        }
    }

    auto* finalComponent = reinterpret_cast<void*>(object->field_0048);
    if (finalComponent)
        return reinterpret_cast<C_008342e8* (__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, finalComponent, 0)[0xC / 4])(
                finalComponent, nullptr, region + 0x90);
    return result;
}

HOOK(0x005004E0, SceneLoader_LoadRegionObject, "auto");
