#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, InitS00402350Object, 0x00402350, (void* self, void* edx));
CSF_FUNC(int, __cdecl, RpGeometryRemoveUserDataArray, 0x0058BCC0, (int a1, int a2));
CSF_FUNC(int, __cdecl, RpGeometryGetUserDataArrayCount, 0x0058BCE0, (int a1));
CSF_FUNC(int, __cdecl, RpGeometryGetUserDataArray, 0x0058BD10, (int a1, int a2));
CSF_FUNC(int, __cdecl, RpGeometryForAllMaterials, 0x00590670, (int geometry, int (__cdecl* callback)(int, int), int userData));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

CSF_FUNC(uint32_t*, __fastcall, GfxObjectProps_EnsureExtendedProperties, 0x006B8020, (void* self, void* edx));
CSF_FUNC(bool, __cdecl, GfxObjectProps_HasTag, 0x006B8900, (int a1, char* a2));
CSF_FUNC(int, __fastcall, GfxObjectProps_IsExtendedPropertiesEmpty, 0x006B8A30, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GfxObjectProps_IsBoundsEmpty, 0x006B8F40, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GfxObjectProps_UnregisterProperty, 0x006B96C0, (void* self, void* edx, int* a2));
CSF_FUNC(char, __cdecl, GfxObjectProps_ParseUserDataArray, 0x006BA6D0, (void* a1, float* a2, int* a3, bool* a4, int a5, int a6, int a7));
CSF_FUNC(int, __cdecl, GfxObjectProps_ApplyMaterialProperties, 0x006BB960, (void** a1));

CSF_FUNC(int, __cdecl, GfxObjectProps_ProcessMaterial, 0x006BBBF0, (int material, int userData));
CSF_FUNC(int, __cdecl, GfxObjectProps_CheckMaterialAlpha, 0x006B8FC0, (int material, int userData));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(void*, g_pRwEngineInstance, 0x008B68C0);
CSF_GLOBAL(int, g_EnginePropertiesOffset, 0x008BB474);
CSF_GLOBAL(int, g_ObjectPropertiesOffset, 0x008BB478);

// The packet also classifies these direct callee addresses as vtable references.
[[maybe_unused]] static const auto kExtendedPropertiesVtable = CSF_VTBL(0x006B8020);
[[maybe_unused]] static const auto kBoundsVtable = CSF_VTBL(0x00402350);

static int __cdecl GfxObjectProps_ProcessGeometryUserData(int atomicAddress)
{
    void* atomic = reinterpret_cast<void*>(atomicAddress);
    void* properties = CSF_AT(void*, atomic, g_ObjectPropertiesOffset + 4);

    alignas(8) uint8_t boundsBytes[0x34];
    float* bounds = reinterpret_cast<float*>(boundsBytes);
    int& geometry = CSF_AT(int, boundsBytes, 0x30);
    geometry = CSF_AT(int, atomic, 0x18);

    int result = RpGeometryGetUserDataArrayCount(geometry);
    if (result == 0)
        return result;

    alignas(8) uint8_t tagBytes[0x6];
    int& parsedTag = CSF_AT(int, tagBytes, 0);
    bool retainDefaultType = true;
    char hasAlpha = 0;

    for (int axis = 0; axis < 6; ++axis)
        bounds[axis] = 0.0f;
    parsedTag = -1;

    int tag = -1;
    int index = static_cast<int>(static_cast<uint32_t>(result) - 1);
    if (index >= 0)
    {
        do
        {
            int currentGeometry = geometry;
            int userData = RpGeometryGetUserDataArray(currentGeometry, index);
            // The getter's two cdecl arguments remain on the original stack
            // and become the parser's final two arguments.
            if (GfxObjectProps_ParseUserDataArray(
                    properties, bounds, &parsedTag, &retainDefaultType,
                    userData, currentGeometry, index))
            {
                RpGeometryRemoveUserDataArray(currentGeometry, index);
            }
            --index;
        } while (index >= 0);
        tag = parsedTag;
    }

    if (GfxObjectProps_HasTag(
            atomicAddress, const_cast<char*>(CSF_STR(0x00860558, "hayTagID"))))
    {
        CSF_AT(uint32_t, properties, 0) |= 0xFFFFu;
    }

    if (tag != -1)
    {
        CSF_AT(uint32_t, properties, 0) =
            (static_cast<uint32_t>(tag) << 16) +
            static_cast<uint16_t>(CSF_AT(uint32_t, properties, 0));
    }

    if (GfxObjectProps_HasTag(
            atomicAddress, const_cast<char*>(CSF_STR(0x0086054C, "bNivelTest"))))
    {
        void* extended = CSF_AT(void*, properties, 0x10);
        if (extended)
            CSF_AT(int, extended, 0x10) = 32;
    }

    if (!retainDefaultType)
    {
        void* extended = CSF_AT(void*, properties, 0x10);
        int type = extended ? CSF_AT(uint8_t, extended, 0xC) : 0;
        if (type == 2)
        {
            GfxObjectProps_EnsureExtendedProperties(properties, nullptr);
            CSF_AT(uint8_t, CSF_AT(void*, properties, 0x10), 0xC) = 4;
        }
        else if (type == 1)
        {
            GfxObjectProps_EnsureExtendedProperties(properties, nullptr);
            CSF_AT(uint8_t, CSF_AT(void*, properties, 0x10), 0xC) = 3;
        }
    }

    int* extended = CSF_AT(int*, properties, 0x10);
    if (extended &&
        static_cast<uint8_t>(
            GfxObjectProps_IsExtendedPropertiesEmpty(extended, nullptr)) != 0)
    {
        GfxObjectProps_UnregisterProperty(
            CSF_AT(void*, g_pRwEngineInstance, g_EnginePropertiesOffset),
            nullptr, extended);
        GfxObjectProps_UnregisterProperty(
            CSF_AT(void*, g_pRwEngineInstance, g_EnginePropertiesOffset),
            nullptr, extended + 1);
        Mem_Free(extended);
        CSF_AT(void*, properties, 0x10) = nullptr;
    }

    RpGeometryForAllMaterials(
        CSF_AT(int, atomic, 0x18), GfxObjectProps_ProcessMaterial, 0);
    GfxObjectProps_ApplyMaterialProperties(CSF_AT(void**, atomic, 0x4));

    int materialGeometry = CSF_AT(int, atomic, 0x18);
    hasAlpha = 0;
    RpGeometryForAllMaterials(
        materialGeometry, GfxObjectProps_CheckMaterialAlpha,
        static_cast<int>(reinterpret_cast<uintptr_t>(&hasAlpha)));
    if (hasAlpha)
        CSF_AT(uint32_t, properties, 0x4) |= 4u;

    result = GfxObjectProps_IsBoundsEmpty(bounds, nullptr);
    if (static_cast<uint8_t>(result) != 0)
        return result;

    uint32_t flags = CSF_AT(uint32_t, properties, 0x4);
    bool needsBounds = (flags & 0x80) != 0;
    if (!needsBounds)
    {
        result = CSF_AT(int, properties, 0x10);
        needsBounds =
            (result != 0 && CSF_AT(int, reinterpret_cast<void*>(result), 0x14) == 1) ||
            (flags & 4) != 0;
    }
    if (!needsBounds)
        return result;

    if (!CSF_AT(void*, properties, 0x14))
    {
        void* storage = Mem_Alloc(60);
        uint32_t* initialized = storage
            ? InitS00402350Object(storage, nullptr)
            : nullptr;
        CSF_AT(void*, properties, 0x14) = initialized;
    }

    for (int axis = 0; axis < 3; ++axis)
    {
        if (bounds[axis] > bounds[axis + 3])
        {
            float oldMaximum = bounds[axis + 3];
            bounds[axis + 3] = bounds[axis];
            bounds[axis] = oldMaximum;
        }
    }

    void* output = CSF_AT(void*, properties, 0x14);
    float halfX = (bounds[3] - bounds[0]) * g_HalfDurationScale;
    float halfY = (bounds[4] - bounds[1]) * g_HalfDurationScale;
    float halfZ = (bounds[5] - bounds[2]) * g_HalfDurationScale;

    CSF_AT(float, output, 0) = halfX + bounds[0];
    CSF_AT(float, output, 0x4) = halfY + bounds[1];
    CSF_AT(float, output, 0xC) = halfX;
    CSF_AT(float, output, 0x8) = halfZ + bounds[2];
    CSF_AT(float, output, 0x10) = halfY;
    CSF_AT(float, output, 0x14) = halfZ;

    std::memcpy(&result, &halfY, sizeof(result));
    return result;
}

HOOK(0x006BBC10, GfxObjectProps_ProcessGeometryUserData, "auto");
