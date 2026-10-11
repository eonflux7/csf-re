#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __cdecl, RpClumpGetNumAtomics, 0x0058F560, (int a1));
CSF_FUNC(int, __cdecl, RpClumpGetNumLights, 0x0058F590, (int a1, int a2));
CSF_FUNC(int, __cdecl, RpClumpDestroy, 0x0058F740, (int a1));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __fastcall, GfxEnvironment_GetAttachmentId, 0x006B8080, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetUint16OrSentinelMinusOne, 0x006B80A0, (void* self, void*));
CSF_FUNC(int, __fastcall, ObjectAttachment_HasSoundTrigger, 0x006B8140, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetField04Bit1, 0x006B8160, (void* self, void*));
CSF_FUNC(int, __fastcall, C_6B81C0_GetFirstItem, 0x006B81C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetReferencedField14, 0x006B8270, (void* self, void*));
CSF_FUNC(double, __fastcall, GetAttachmentFirstWeight, 0x006B8290, (void* self, void* edx, char a2));
CSF_FUNC(double, __fastcall, GetAttachmentSecondWeight, 0x006B82C0, (void* self, void* edx, char a2));
CSF_FUNC(double, __fastcall, GfxSkin_GetFadeWidth, 0x006B82F0, (void* self, void* edx, char a2));
CSF_FUNC(int, __cdecl, SceneLoader_GetObjectAttachment, 0x006B87A0, (int a1));
CSF_FUNC(unsigned int, __fastcall, GfxObjectProps_SetFlagBit01, 0x006B8B80, (void* self, void* edx, char a2));
CSF_FUNC(unsigned int, __fastcall, GfxObjectProps_SetFlagBit400, 0x006B8BB0, (void* self, void* edx, char a2));
CSF_FUNC(int, __cdecl, GfxObjectProps_ProcessGeometryUserData, 0x006BBC10, (int a1));
CSF_FUNC(int, __fastcall, C_008605d8_FindNamedClump, 0x006BCF80, (void* self, void* edx, int a2, char* String2));
CSF_FUNC(char*, __fastcall, GfxEnvironment_BuildClumpDescriptor, 0x006BDC50, (void* self, void* edx, int a2, int a3, void* a4, int a5, int a6, int a7, int a8, int a9));
CSF_FUNC(int, __fastcall, GfxEnvironment_AppendClumpDescriptor, 0x006BE5C0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(char, __cdecl, GfxEnvironment_ProcessSectorObject, 0x006C0160, (int a1, char a2));
CSF_FUNC(int, __cdecl, SceneLoader_CollectSectorObjects, 0x006C0B90, (int a1, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(int, __fastcall, C_00860288_AddEntryAndRegister, 0x006C6D40, (C_00860288* self, void* , int a1));

CSF_GLOBAL(int*, g_SectorObjects, 0x008A7E50);
CSF_GLOBAL(std::uint32_t, g_SectorObjectCount, 0x008A7E58);

// The descriptor constructor's integer-typed bindings carry float payloads.
static int GfxEnvironment_EncodeFloatArgument(float value)
{
    int result;
    std::memcpy(&result, &value, sizeof(result));
    return result;
}

static char __fastcall C_008605d8_AddClump(
    C_008605d8* self, void* /*edx*/, int clump)
{
    const bool hasAtomics = RpClumpGetNumAtomics(clump) > 0;
    // The atomic-count call's argument is still on the stack at this call.
    const bool hasLights = RpClumpGetNumLights(clump, clump) > 0;

    if (!hasAtomics)
    {
        if (!hasLights)
        {
            RpClumpDestroy(clump);
            return 1;
        }

        auto* lights = reinterpret_cast<S_0041e5f0*>(
            reinterpret_cast<std::uint8_t*>(self) + 0x5C);
        if (self->field_0064 >= self->field_0060)
            S_0041e5f0_GrowArrayCapacity(
                lights, nullptr, lights->field_000c);

        const std::uint32_t index = lights->field_0008;
        auto* destination =
            reinterpret_cast<int*>(lights->field_0000) + index;
        lights->field_0008 = index + 1;
        *destination = clump;
        return 1;
    }

    SceneLoader_CollectSectorObjects(
        clump, reinterpret_cast<int>(&g_SectorObjects));
    int representative = g_SectorObjects[0];

    for (std::uint32_t index = 0; index < g_SectorObjectCount; ++index)
    {
        const int object = g_SectorObjects[index];
        GfxObjectProps_ProcessGeometryUserData(object);
        void* attachment =
            reinterpret_cast<void*>(SceneLoader_GetObjectAttachment(object));
        GfxObjectProps_SetFlagBit01(attachment, nullptr, 1);
        GfxObjectProps_SetFlagBit400(attachment, nullptr, 1);

        attachment = reinterpret_cast<void*>(
            SceneLoader_GetObjectAttachment(g_SectorObjects[index]));
        if (GfxEnvironment_GetAttachmentId(attachment, nullptr) != -1)
            representative = g_SectorObjects[index];
    }

    void* attachment =
        reinterpret_cast<void*>(SceneLoader_GetObjectAttachment(representative));

    if (GetReferencedField14(attachment, nullptr))
    {
        if (self->field_0008)
        {
            RpClumpDestroy(clump);
            return 1;
        }

        if (!C_6B81C0_GetFirstItem(attachment, nullptr) ||
            !CSF_AT(std::uint32_t,
                reinterpret_cast<void*>(C_6B81C0_GetFirstItem(attachment, nullptr)),
                0x14))
        {
            GetReferencedField14(attachment, nullptr);
            RpClumpDestroy(clump);
            return 1;
        }

        const int group = GetReferencedField14(attachment, nullptr);
        void* firstItem =
            reinterpret_cast<void*>(C_6B81C0_GetFirstItem(attachment, nullptr));
        char* name;
        if (CSF_AT(std::uint32_t, firstItem, 0x18) < 0x10u)
            name = reinterpret_cast<char*>(firstItem) + 4;
        else
            name = CSF_AT(char*, firstItem, 4);

        if (C_008605d8_FindNamedClump(self, nullptr, group, name) != -1)
        {
            C_6B81C0_GetFirstItem(attachment, nullptr);
            RpClumpDestroy(clump);
            return 1;
        }

        const std::uint32_t groupIndex =
            static_cast<std::uint32_t>(GetReferencedField14(attachment, nullptr));
        auto* groupClumps = reinterpret_cast<S_0041e5f0*>(
            reinterpret_cast<std::uint8_t*>(self) + 0x2C + 16 * groupIndex);
        if (groupClumps->field_0008 >= groupClumps->field_0004)
            S_0041e5f0_GrowArrayCapacity(
                groupClumps, nullptr, groupClumps->field_000c);

        const std::uint32_t index = groupClumps->field_0008;
        auto* destination =
            reinterpret_cast<int*>(groupClumps->field_0000) + index;
        groupClumps->field_0008 = index + 1;
        *destination = clump;
        return 1;
    }

    if (static_cast<std::uint8_t>(GetField04Bit1(attachment, nullptr)))
    {
        auto* flaggedClumps = reinterpret_cast<S_0041e5f0*>(
            reinterpret_cast<std::uint8_t*>(self) + 0x6C);
        if (self->field_0074 >= self->field_0070)
            S_0041e5f0_GrowArrayCapacity(
                flaggedClumps, nullptr, CSF_AT(int, self, 0x78));

        const std::uint32_t index = self->field_0074;
        auto* destination =
            reinterpret_cast<int*>(CSF_AT(std::uint32_t, self, 0x6C)) + index;
        self->field_0074 = index + 1;
        *destination = clump;
        return 1;
    }

    if (static_cast<std::uint8_t>(
            ObjectAttachment_HasSoundTrigger(attachment, nullptr)) &&
        self->field_0008)
    {
        RpClumpDestroy(clump);
        return 1;
    }

    if (GfxEnvironment_GetAttachmentId(attachment, nullptr) >= 1000)
    {
        std::uint32_t* matrix = RwMatrixCreate();
        void* frame = reinterpret_cast<void*>(
            CSF_AT(std::uint32_t, reinterpret_cast<void*>(clump), 4));
        std::memcpy(matrix, reinterpret_cast<std::uint8_t*>(frame) + 0x10, 0x40);

        void* referencedObject = GetReferencedObject(attachment, nullptr);
        const float fadeWidth =
            static_cast<float>(GfxSkin_GetFadeWidth(attachment, nullptr, 1));
        const float secondWeight =
            static_cast<float>(GetAttachmentSecondWeight(attachment, nullptr, 1));
        const float firstWeight =
            static_cast<float>(GetAttachmentFirstWeight(attachment, nullptr, 1));
        void* firstItem =
            reinterpret_cast<void*>(C_6B81C0_GetFirstItem(attachment, nullptr));
        const int secondaryId = GetUint16OrSentinelMinusOne(attachment, nullptr);
        const int id = GfxEnvironment_GetAttachmentId(attachment, nullptr);

        alignas(8) std::uint8_t descriptorBytes[0x40];
        char* descriptor = GfxEnvironment_BuildClumpDescriptor(
            descriptorBytes, nullptr, id, secondaryId, firstItem,
            reinterpret_cast<int>(matrix),
            GfxEnvironment_EncodeFloatArgument(firstWeight),
            GfxEnvironment_EncodeFloatArgument(secondWeight),
            GfxEnvironment_EncodeFloatArgument(fadeWidth),
            reinterpret_cast<int>(referencedObject));
        GfxEnvironment_AppendClumpDescriptor(
            reinterpret_cast<std::uint8_t*>(self) + 0x7C,
            nullptr, reinterpret_cast<int>(descriptor), 0);

        if (CSF_AT(std::uint32_t, descriptorBytes, 0x20) >= 0x10 &&
            CSF_AT(void*, descriptorBytes, 0x0C))
        {
            Mem_FreeDebug(CSF_AT(void*, descriptorBytes, 0x0C));
        }

        RpClumpDestroy(clump);
        return 1;
    }

    if (self->field_0014 >= self->field_0010)
        S_0041e5f0_GrowArrayCapacity(
            reinterpret_cast<S_0041e5f0*>(
                reinterpret_cast<std::uint8_t*>(self) + 0x0C),
            nullptr, self->field_0018);

    const std::uint32_t index = self->field_0014;
    auto* destination = reinterpret_cast<int*>(self->field_000c) + index;
    self->field_0014 = index + 1;
    *destination = clump;

    if (!clump)
        return 1;

    for (std::uint32_t objectIndex = 0;
         objectIndex < g_SectorObjectCount; ++objectIndex)
    {
        const int object = g_SectorObjects[objectIndex];
        void* objectAttachment =
            reinterpret_cast<void*>(SceneLoader_GetObjectAttachment(object));
        if (!GetReferencedField14(objectAttachment, nullptr) &&
            !static_cast<std::uint8_t>(
                GetField04Bit1(objectAttachment, nullptr)))
        {
            auto* registry =
                reinterpret_cast<C_00860288*>(GetGlobalValue_6C5760());
            C_00860288_AddEntryAndRegister(registry, nullptr, object);
        }
        GfxEnvironment_ProcessSectorObject(object, 0);
    }
    return 1;
}

HOOK(0x006BE640, C_008605d8_AddClump, "auto");
