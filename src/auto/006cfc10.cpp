#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, MenuExtras_InitializeFile, 0x00692B10, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(int, __fastcall, ParseIntegerComponent, 0x00692C40, (std::uint32_t component));
CSF_FUNC(char, __fastcall, CopyAttributeText, 0x00692D40, (std::uint8_t* attribute, void* edx, char* buffer, int capacity));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(int, __fastcall, ReadIntegerProperty, 0x00693910, (std::uint8_t* self, void* edx, char* name));
CSF_FUNC(double, __fastcall, ReadFloatProperty, 0x00693940, (std::uint8_t* self, void* edx, char* name));
CSF_FUNC(int, __fastcall, DataNode_ReadVector3Property, 0x00693A60, (void* self, void* edx, char* String1, int a3));
CSF_FUNC(int, __fastcall, MenuExtras_CloseFile, 0x00694380, (void* self, void* edx));
CSF_FUNC(char, __fastcall, MenuExtras_LoadFile, 0x00694770, (void* self, void* edx, const char* path));

CSF_FUNC(int, __stdcall, GfxCollisionShapes_ParseShapeType, 0x006CF990, (char* String1));
CSF_FUNC(int, __stdcall, GfxCollisionShapes_ParseLabel, 0x006CFA00, (char* String1));
CSF_FUNC(char, __fastcall, GfxCollisionShapes_GrowShapes, 0x006CFA50, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, GfxCollisionShapes_GrowHotpoints, 0x006CFB20, (void* self, void* edx, int a2));

CSF_GLOBAL(const char*, g_pszKey_VERSION_gfx_collision_shapes, 0x008A7F18);
CSF_GLOBAL(const char*, g_pszKey_SHAPE_TYPE, 0x008A7F1C);
CSF_GLOBAL(const char*, g_pszKey_USE_OBJETO3D, 0x008A7F20);
CSF_GLOBAL(const char*, g_pszKey_INTERNAL_SHAPES, 0x008A7F24);
CSF_GLOBAL(const char*, g_pszKey_RADIUS, 0x008A7F28);
CSF_GLOBAL(const char*, g_pszKey_CENTER, 0x008A7F2C);
CSF_GLOBAL(const char*, g_pszKey_DIM, 0x008A7F30);
CSF_GLOBAL(const char*, g_pszKey_BONE_ID, 0x008A7F34);
CSF_GLOBAL(const char*, g_pszKey_BONE_INDEX, 0x008A7F38);
CSF_GLOBAL(const char*, g_pszKey_ATOMIC_ID, 0x008A7F3C);
CSF_GLOBAL(const char*, g_pszKey_ATOMIC_INDEX, 0x008A7F40);
CSF_GLOBAL(const char*, g_pszKey_LABEL, 0x008A7F44);
CSF_GLOBAL(const char*, g_pszKey_HOTPOINTS, 0x008A7F48);
CSF_GLOBAL(const char*, g_pszKey_OFFSET, 0x008A7F4C);
CSF_GLOBAL(const char*, g_pszKey_MATERIAL, 0x008A7F50);

// These packet-listed table addresses coincide with the direct callees.
[[maybe_unused]] static const auto kFileTable = CSF_VTBL(0x00692B10);
[[maybe_unused]] static const auto kNodeTable = CSF_VTBL(0x00692B80);

static int __fastcall GfxCollisionShapes_Load(
    void* /*self*/, void* /*edx*/, const char* path, void* collisionData)
{
    char buffer[0x100];
    std::memset(buffer, 0, sizeof(buffer));

    alignas(8) std::uint8_t fileBytes[0x58];
    S_00692b10& file = *reinterpret_cast<S_00692b10*>(fileBytes);
    MenuExtras_InitializeFile(&file, nullptr);
    MenuExtras_LoadFile(&file, nullptr, path);

    auto* root = reinterpret_cast<std::uint8_t*>(file.field_0000);
    if (!root ||
        ReadIntegerProperty(root, nullptr,
            const_cast<char*>(g_pszKey_VERSION_gfx_collision_shapes)) != 1)
    {
        return MenuExtras_CloseFile(&file, nullptr);
    }

    auto* useObject = FindProperty(root, nullptr,
        const_cast<char*>(g_pszKey_USE_OBJETO3D));
    if (useObject)
    {
        const int enabled = ParseIntegerComponent(
            reinterpret_cast<std::uint32_t>(useObject));
        CSF_AT(std::uint8_t, collisionData, 0x38) = enabled != 0;
    }
    else
    {
        CSF_AT(std::uint8_t, collisionData, 0x38) = 0;
    }

    auto* shapes = FindProperty(root, nullptr,
        const_cast<char*>(g_pszKey_INTERNAL_SHAPES));
    const std::uint32_t shapeCount = MenuExtras_GetChildCount(shapes, nullptr);
    const std::uint32_t shapeCapacity =
        CSF_AT(std::uint32_t, collisionData, 0x40);
    if (shapeCount > shapeCapacity)
    {
        GfxCollisionShapes_GrowShapes(
            static_cast<std::uint8_t*>(collisionData) + 0x3C, nullptr,
            static_cast<int>(shapeCount - shapeCapacity));
    }
    CSF_AT(std::uint32_t, collisionData, 0x44) = shapeCount;

    std::uint32_t shapeOffset = 0;
    for (std::uint32_t index = 0; index < shapeCount; ++index, shapeOffset += 56)
    {
        auto* component = reinterpret_cast<std::uint8_t*>(
            GetComponent(shapes, nullptr, index));

        auto* typeProperty = FindProperty(component, nullptr,
            const_cast<char*>(g_pszKey_SHAPE_TYPE));
        CopyAttributeText(typeProperty, nullptr, buffer, 256);
        const int shapeType = GfxCollisionShapes_ParseShapeType(buffer);
        CSF_AT(int, CSF_AT(std::uint8_t*, collisionData, 0x3C),
            shapeOffset) = shapeType;

        if (shapeType == 0)
        {
            auto* shape = CSF_AT(std::uint8_t*, collisionData, 0x3C) + shapeOffset;
            const double radius = ReadFloatProperty(component, nullptr,
                const_cast<char*>(g_pszKey_RADIUS));
            CSF_AT(float, shape, 0x1C) = static_cast<float>(radius);
        }
        else if (shapeType == 2)
        {
            DataNode_ReadVector3Property(component, nullptr,
                const_cast<char*>(g_pszKey_CENTER),
                reinterpret_cast<int>(
                    CSF_AT(std::uint8_t*, collisionData, 0x3C) + shapeOffset + 4));
            DataNode_ReadVector3Property(component, nullptr,
                const_cast<char*>(g_pszKey_DIM),
                reinterpret_cast<int>(
                    CSF_AT(std::uint8_t*, collisionData, 0x3C) + shapeOffset + 16));
        }

        auto* boneId = FindProperty(component, nullptr,
            const_cast<char*>(g_pszKey_BONE_ID));
        auto* boneIndex = FindProperty(component, nullptr,
            const_cast<char*>(g_pszKey_BONE_INDEX));
        auto* atomicId = FindProperty(component, nullptr,
            const_cast<char*>(g_pszKey_ATOMIC_ID));
        auto* atomicIndex = FindProperty(component, nullptr,
            const_cast<char*>(g_pszKey_ATOMIC_INDEX));

        if (boneIndex)
        {
            auto* shape = CSF_AT(std::uint8_t*, collisionData, 0x3C) + shapeOffset;
            const int value = ParseIntegerComponent(
                reinterpret_cast<std::uint32_t>(boneIndex));
            CSF_AT(int, shape, 0x24) = value;
        }
        else if (boneId)
        {
            auto* shape = CSF_AT(std::uint8_t*, collisionData, 0x3C) + shapeOffset;
            const int value = ParseIntegerComponent(
                reinterpret_cast<std::uint32_t>(boneId));
            CSF_AT(int, shape, 0x20) = value;
        }
        else if (atomicIndex)
        {
            auto* shape = CSF_AT(std::uint8_t*, collisionData, 0x3C) + shapeOffset;
            const int value = ParseIntegerComponent(
                reinterpret_cast<std::uint32_t>(atomicIndex));
            CSF_AT(int, shape, 0x2C) = value;
        }
        else
        {
            auto* shape = CSF_AT(std::uint8_t*, collisionData, 0x3C) + shapeOffset;
            const int value = ParseIntegerComponent(
                reinterpret_cast<std::uint32_t>(atomicId));
            CSF_AT(int, shape, 0x28) = value;
        }

        auto* labelProperty = FindProperty(component, nullptr,
            const_cast<char*>(g_pszKey_LABEL));
        if (labelProperty)
        {
            CopyAttributeText(labelProperty, nullptr, buffer, 256);
            const int label = GfxCollisionShapes_ParseLabel(buffer);
            CSF_AT(int, CSF_AT(std::uint8_t*, collisionData, 0x3C),
                shapeOffset + 48) = label == -1 ? 0 : label;
        }

        auto* material = FindProperty(component, nullptr,
            const_cast<char*>(g_pszKey_MATERIAL));
        if (material)
        {
            auto* shape = CSF_AT(std::uint8_t*, collisionData, 0x3C) + shapeOffset;
            const int value = ParseIntegerComponent(
                reinterpret_cast<std::uint32_t>(material));
            CSF_AT(int, shape, 52) = value;
        }
    }

    auto* hotpoints = FindProperty(root, nullptr,
        const_cast<char*>(g_pszKey_HOTPOINTS));
    if (hotpoints)
    {
        const std::uint32_t hotpointCount =
            MenuExtras_GetChildCount(hotpoints, nullptr);
        const std::uint32_t hotpointCapacity =
            CSF_AT(std::uint32_t, collisionData, 0x50);
        if (hotpointCount > hotpointCapacity)
        {
            GfxCollisionShapes_GrowHotpoints(
                static_cast<std::uint8_t*>(collisionData) + 0x4C, nullptr,
                static_cast<int>(hotpointCount - hotpointCapacity));
        }
        CSF_AT(std::uint32_t, collisionData, 0x54) = hotpointCount;

        std::uint32_t hotpointOffset = 0;
        for (std::uint32_t index = 0; index < hotpointCount;
             ++index, hotpointOffset += 32)
        {
            auto* component = reinterpret_cast<std::uint8_t*>(
                GetComponent(hotpoints, nullptr, index));

            auto* labelProperty = FindProperty(component, nullptr,
                const_cast<char*>(g_pszKey_LABEL));
            CopyAttributeText(labelProperty, nullptr, buffer, 256);
            const int label = GfxCollisionShapes_ParseLabel(buffer);
            CSF_AT(int, CSF_AT(std::uint8_t*, collisionData, 0x4C),
                hotpointOffset) = label == -1 ? 0 : label;

            auto* boneId = FindProperty(component, nullptr,
                const_cast<char*>(g_pszKey_BONE_ID));
            auto* boneIndex = FindProperty(component, nullptr,
                const_cast<char*>(g_pszKey_BONE_INDEX));
            auto* atomicId = FindProperty(component, nullptr,
                const_cast<char*>(g_pszKey_ATOMIC_ID));
            auto* atomicIndex = FindProperty(component, nullptr,
                const_cast<char*>(g_pszKey_ATOMIC_INDEX));

            if (boneIndex)
            {
                auto* hotpoint =
                    CSF_AT(std::uint8_t*, collisionData, 0x4C) + hotpointOffset;
                const int value = ParseIntegerComponent(
                    reinterpret_cast<std::uint32_t>(boneIndex));
                CSF_AT(int, hotpoint, 8) = value;
            }
            else if (boneId)
            {
                auto* hotpoint =
                    CSF_AT(std::uint8_t*, collisionData, 0x4C) + hotpointOffset;
                const int value = ParseIntegerComponent(
                    reinterpret_cast<std::uint32_t>(boneId));
                CSF_AT(int, hotpoint, 4) = value;
            }
            else if (atomicIndex)
            {
                auto* hotpoint =
                    CSF_AT(std::uint8_t*, collisionData, 0x4C) + hotpointOffset;
                const int value = ParseIntegerComponent(
                    reinterpret_cast<std::uint32_t>(atomicIndex));
                CSF_AT(int, hotpoint, 16) = value;
            }
            else
            {
                auto* hotpoint =
                    CSF_AT(std::uint8_t*, collisionData, 0x4C) + hotpointOffset;
                const int value = ParseIntegerComponent(
                    reinterpret_cast<std::uint32_t>(atomicId));
                CSF_AT(int, hotpoint, 12) = value;
            }

            DataNode_ReadVector3Property(component, nullptr,
                const_cast<char*>(g_pszKey_OFFSET),
                reinterpret_cast<int>(
                    CSF_AT(std::uint8_t*, collisionData, 0x4C) +
                    hotpointOffset + 20));
        }
    }

    return MenuExtras_CloseFile(&file, nullptr);
}

HOOK(0x006CFC10, GfxCollisionShapes_Load, "auto");
