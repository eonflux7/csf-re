#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetSceneRegionAt, 0x004FC130, (void* self, void* edx, int a2));
CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(int, __fastcall, ResolveClumpManager, 0x00652020, (int managerId, void* edx));
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, RenderHandle_FromObject, 0x006B2D50, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetAttachmentRegionOverrideFlag, 0x006B8150, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetBit2OfField4, 0x006B8170, (void* self, void*));
CSF_FUNC(int, __fastcall, IsFlagBit6Set, 0x006B81A0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetInnerByteFlag, 0x006B81E0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetAttachmentPrimaryPassFlag, 0x006B81F0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetAttachmentSecondaryPassFlag, 0x006B8210, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetAttachmentRenderKey, 0x006B8250, (void* self, void* edx));
CSF_FUNC(double, __fastcall, GetAttachmentFirstWeight, 0x006B8290, (void* self, void* edx, char a2));
CSF_FUNC(double, __fastcall, GetAttachmentSecondWeight, 0x006B82C0, (void* self, void* edx, char a2));
CSF_FUNC(int, __cdecl, SceneLoader_GetObjectAttachment, 0x006B87A0, (int a1));
CSF_FUNC(int, __fastcall, C_008670f0_SetField0C, 0x006C4620, (C_008670f0* self, void* , int value));
CSF_FUNC(int*, __fastcall, FindSubobjectKeyedValue, 0x006C4960, (void* self, void* edx, int* a2, int a3));
CSF_FUNC(int, __fastcall, SetSubobjectState, 0x006C4B00, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, Subobject_SetKeyedValue, 0x006C4BF0, (void* self, void* edx, int a2, unsigned int a3));
CSF_FUNC(unsigned int, __fastcall, SetSubobjectSecondaryRegion, 0x006C4CF0, (void* self, void* edx, unsigned int a2));
CSF_FUNC(void, __cdecl, InitializeObjectRenderState, 0x006C4DF0, (int a1, int a2));
CSF_FUNC(int, __cdecl, ApplyObjectRenderOptions, 0x006C4F50, (int a1, int a2));
CSF_FUNC(int, __fastcall, C_0083e500_SetField0008, 0x00713B10, (C_0083e500* self, void* , int value));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static int __fastcall C_00860288_ConfigureSkinnedObject(
    C_00860288* self, void* /*edx*/, int object, int renderHandle,
    C_0085e7b8* source)
{
    alignas(8) uint8_t optionsBytes[0xC] = {};
    alignas(8) uint8_t lookupBytes[0x8] = {};
    CSF_AT(C_00860288*, lookupBytes, 0) = self;

    void* attachment = reinterpret_cast<void*>(SceneLoader_GetObjectAttachment(object));
    CSF_AT(uint8_t, lookupBytes, 7) =
        static_cast<uint8_t>(GetAttachmentRegionOverrideFlag(attachment, nullptr));

    SceneLoader_NoOp();
    InitializeObjectRenderState(object, object);

    void* globalObject = LookupObjectById(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    void* renderInfo =
        reinterpret_cast<void*>(
            ((int(__fastcall*)(void*, void*))
                CSF_AT(void**, globalObject, 0)[0x18 / 4])(globalObject, nullptr));

    CSF_AT(uint8_t, optionsBytes, 0) =
        static_cast<uint8_t>(IsFlagBit6Set(attachment, nullptr)) == 0;
    CSF_AT(uint32_t, optionsBytes, 4) = CSF_AT(uint32_t, renderInfo, 0x20);
    if (((bool(__fastcall*)(void*, void*))
            CSF_AT(void**, globalObject, 0)[0x40 / 4])(globalObject, nullptr))
    {
        CSF_AT(uint32_t, optionsBytes, 8) = CSF_AT(uint32_t, renderInfo, 0x1C);
    }
    else
    {
        CSF_AT(uint32_t, optionsBytes, 8) = 0;
    }

    ApplyObjectRenderOptions(object, reinterpret_cast<int>(optionsBytes));

    void* handle = reinterpret_cast<void*>(renderHandle);
    C_008670f0* subobject =
        reinterpret_cast<C_008670f0*>(
            ((int(__fastcall*)(void*, void*))
                CSF_AT(void**, handle, 0)[0x1C / 4])(handle, nullptr));

    if (source)
    {
        void* sourceHandle =
            reinterpret_cast<void*>(RenderHandle_FromObject(source, nullptr));
        int region =
            ((int(__fastcall*)(void*, void*))
                CSF_AT(void**, sourceHandle, 0)[0x1C / 4])(sourceHandle, nullptr);
        C_008670f0_SetField0C(subobject, nullptr, region);

        if (static_cast<uint8_t>(GetAttachmentPrimaryPassFlag(attachment, nullptr)))
        {
            Subobject_SetKeyedValue(subobject, nullptr, 20, 1u);
            Subobject_SetKeyedValue(subobject, nullptr, 29, 5u);
        }
        else
        {
            if (static_cast<uint8_t>(GetAttachmentSecondaryPassFlag(attachment, nullptr)))
                Subobject_SetKeyedValue(subobject, nullptr, 12, 1u);
            Subobject_SetKeyedValue(subobject, nullptr, 20, 2u);
        }
    }
    else
    {
        C_00860288* owner = CSF_AT(C_00860288*, lookupBytes, 0);
        if (CSF_AT(uint8_t, lookupBytes, 7))
        {
            int region = GetSceneRegionAt(
                reinterpret_cast<void*>(owner->field_0004), nullptr, 4);
            C_008670f0_SetField0C(subobject, nullptr, region);
        }

        switch (GetInnerByteFlag(attachment, nullptr))
        {
        case 0:
            if (!CSF_AT(uint8_t, lookupBytes, 7))
            {
                int region = GetSceneRegionAt(
                    reinterpret_cast<void*>(owner->field_0004), nullptr, 0);
                C_008670f0_SetField0C(subobject, nullptr, region);
            }
            if (GetAttachmentFirstWeight(attachment, nullptr, 1) > g_fZero ||
                GetAttachmentSecondWeight(attachment, nullptr, 1) > g_fZero)
            {
                Subobject_SetKeyedValue(subobject, nullptr, 20, 2u);
            }
            break;

        case 1:
        case 3:
        {
            int region = GetSceneRegionAt(
                reinterpret_cast<void*>(owner->field_0004), nullptr, 6);
            if (CSF_AT(uint8_t, lookupBytes, 7))
                SetSubobjectSecondaryRegion(subobject, nullptr, region);
            else
                C_008670f0_SetField0C(subobject, nullptr, region);

            int* storedKey = FindSubobjectKeyedValue(
                subobject, nullptr, reinterpret_cast<int*>(lookupBytes), 30);
            if (GetAttachmentRenderKey(attachment, nullptr) != *storedKey)
            {
                unsigned int key = GetAttachmentRenderKey(attachment, nullptr);
                Subobject_SetKeyedValue(subobject, nullptr, 30, key);
            }
            if (GetInnerByteFlag(attachment, nullptr) == 3)
                Subobject_SetKeyedValue(subobject, nullptr, 20, 2u);
            break;
        }

        case 2:
        case 4:
            if (CSF_AT(uint8_t, lookupBytes, 7))
            {
                Subobject_SetKeyedValue(subobject, nullptr, 12, 1u);
            }
            else
            {
                int region = GetSceneRegionAt(
                    reinterpret_cast<void*>(owner->field_0004), nullptr, 7);
                C_008670f0_SetField0C(subobject, nullptr, region);
            }
            if (GetInnerByteFlag(attachment, nullptr) == 4)
                Subobject_SetKeyedValue(subobject, nullptr, 20, 2u);
            break;

        default:
            break;
        }
    }

    if (static_cast<uint8_t>(GetBit2OfField4(attachment, nullptr)))
    {
        SetSubobjectState(subobject, nullptr, 4, 1);
        // The original replaces only the low byte of the object argument.
        unsigned int keyedValue = (static_cast<unsigned int>(object) & 0xFFFFFF00u) | 1u;
        Subobject_SetKeyedValue(subobject, nullptr, 12, keyedValue);
        Subobject_SetKeyedValue(subobject, nullptr, 10, 5u);
        Subobject_SetKeyedValue(subobject, nullptr, 11, 2u);
        Subobject_SetKeyedValue(subobject, nullptr, 20, 1u);
    }

    void* objectPointer = reinterpret_cast<void*>(object);
    if (source &&
        (CSF_AT(uint8_t,
            ((void*(__fastcall*)(void*, void*))
                CSF_AT(void**, source, 0)[0x20 / 4])(source, nullptr), 0) & 0x10) != 0)
    {
        auto callback = CSF_ADDR_FN(0x006C5B20);
        CSF_AT(uint32_t, objectPointer, 0x48) = reinterpret_cast<uint32_t>(callback);
        if (!callback)
            CSF_AT(uint32_t, objectPointer, 0x48) =
                reinterpret_cast<uint32_t>(CSF_ADDR_FN(0x0058E680));
    }
    else
    {
        auto callback = CSF_ADDR_FN(0x006C66B0);
        CSF_AT(uint32_t, objectPointer, 0x48) = reinterpret_cast<uint32_t>(callback);
        if (!callback)
            CSF_AT(uint32_t, objectPointer, 0x48) =
                reinterpret_cast<uint32_t>(CSF_ADDR_FN(0x0058E680));
    }

    void* clumpManager =
        reinterpret_cast<void*>(ResolveClumpManager(GetGlobalInt(), nullptr));
    ((void(__fastcall*)(void*, void*, int))
        CSF_AT(void**, clumpManager, 0)[0x8C / 4])(clumpManager, nullptr, object);

    C_0083e500* finalAttachment =
        reinterpret_cast<C_0083e500*>(SceneLoader_GetObjectAttachment(object));
    return C_0083e500_SetField0008(finalAttachment, nullptr, renderHandle);
}

HOOK(0x006C68F0, C_00860288_ConfigureSkinnedObject, "auto");
