#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(const void*, __fastcall, ResizePairBuffer, 0x0045BBA0, (void* self, void* , int extraCount));
CSF_FUNC(int, __fastcall, S_00692b80_FindIndexOfValue, 0x00462A80, (S_00692b80* self, void* , int value));
CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(double, __fastcall, ReadComponentFloat, 0x00692CC0, (std::uint32_t component));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(int, __fastcall, ReadIntegerAttribute, 0x00693970, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(double, __fastcall, ReadFloatAttribute, 0x00693990, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0, (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));
CSF_FUNC(int, __fastcall, DataNode_ReadVector3Property, 0x00693A60, (void* self, void* edx, char* String1, int a3));
CSF_FUNC(char, __fastcall, GrowControlBindingArray, 0x0075C1C0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Table_FindIndex, 0x0075CE20, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, WaypointGraph_DestroyTemporaryGroup, 0x0075D430, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, WaypointGraph_ReservePoints, 0x0075D7D0, (void* self, void* edx, unsigned int a2));
CSF_FUNC(uint32_t*, __fastcall, WaypointGraph_AppendPoint, 0x0075DC40, (void* self, void* edx, uint32_t* a3));
CSF_FUNC(unsigned int, __fastcall, WaypointGraph_ReserveGroups, 0x0075E240, (void* self, void* edx, unsigned int a2));
CSF_FUNC(uint32_t*, __fastcall, WaypointGraph_AppendGroup, 0x0075E780, (void* self, void* edx, uint32_t* a3));

CSF_GLOBAL(const char*, g_pszKey_NOMBRE_8A8CF0, 0x008A8CF0);
CSF_GLOBAL(const char*, g_pszKey_TIPO_8A8CF4, 0x008A8CF4);
CSF_GLOBAL(const char*, g_pszKey_GRUPOS, 0x008A8CF8);
CSF_GLOBAL(const char*, g_pszKey_PUNTOS, 0x008A8CFC);
CSF_GLOBAL(const char*, g_pszKey_ID_8A8D00, 0x008A8D00);
CSF_GLOBAL(const char*, g_pszKey_POS_8A8D04, 0x008A8D04);
CSF_GLOBAL(const char*, g_pszKey_ROT, 0x008A8D08);
CSF_GLOBAL(const char*, g_pszKey_ROT_X, 0x008A8D0C);
CSF_GLOBAL(const char*, g_pszKey_CONEXIONES, 0x008A8D10);
CSF_GLOBAL(const char*, g_pszKey_PUNTO_ORI, 0x008A8D14);
CSF_GLOBAL(const char*, g_pszKey_PUNTO_DST, 0x008A8D18);
CSF_GLOBAL(const char*, g_pszKey_GRUPO_ORI, 0x008A8D1C);
CSF_GLOBAL(const char*, g_pszKey_GRUPO_DST, 0x008A8D20);

static char __fastcall C_0086c288_LoadWaypointGraph(
    C_0086c288* self, void* /*edx*/, S_00692b80* node)
{
    // The packet also identifies the direct child-count entry as a vtable binding.
    (void)CSF_VTBL(0x00692B80);

    auto* groups = FindProperty(reinterpret_cast<std::uint8_t*>(node), nullptr,
                                const_cast<char*>(g_pszKey_GRUPOS));
    if (!groups)
        return 0;

    alignas(8) std::uint8_t textBytes[0x100] = {};
    char* text = reinterpret_cast<char*>(textBytes);
    alignas(8) std::uint8_t groupBytes[0x48];
    alignas(8) std::uint8_t pointBytes[0x54] = {};

    const unsigned int groupCount = MenuExtras_GetChildCount(groups, nullptr);
    WaypointGraph_ReserveGroups(&self->field_0004, nullptr, groupCount);

    for (unsigned int groupIndex = 0; groupIndex < groupCount; ++groupIndex)
    {
        auto* groupNode = reinterpret_cast<std::uint8_t*>(
            GetComponent(groups, nullptr, groupIndex));

        // Unspecified parts of the temporary are passed as zero rather than garbage.
        std::memset(groupBytes, 0, sizeof(groupBytes));
        CSF_AT(std::uint32_t, groupBytes, 0x1C) = 15;
        CSF_AT(std::uint32_t, groupBytes, 0x18) = 0;
        CSF_AT(char, groupBytes, 0x08) = 0;
        CSF_AT(std::uint32_t, groupBytes, 0x24) = 0;
        CSF_AT(std::uint32_t, groupBytes, 0x28) = 0;
        CSF_AT(std::uint32_t, groupBytes, 0x2C) = 0;
        CSF_AT(std::uint32_t, groupBytes, 0x30) = 10;
        CSF_AT(std::uint32_t, groupBytes, 0x38) = 0;
        CSF_AT(std::uint32_t, groupBytes, 0x3C) = 0;
        CSF_AT(std::uint32_t, groupBytes, 0x40) = 0;
        CSF_AT(int, groupBytes, 0x00) = -1;
        CSF_AT(std::uint32_t, groupBytes, 0x20) = 0;
        CSF_AT(std::uint32_t, groupBytes, 0x44) = 1;

        WaypointGraph_AppendGroup(&self->field_0004, nullptr,
                                 reinterpret_cast<uint32_t*>(groupBytes));
        WaypointGraph_DestroyTemporaryGroup(groupBytes, nullptr);

        auto* group = reinterpret_cast<std::uint8_t*>(self->field_000c) - 72;
        CSF_AT(int, group, 0x00) =
            ReadIntegerAttribute(groupNode, nullptr, const_cast<char*>(g_pszKey_ID_8A8D00));
        MenuExtras_ReadText(groupNode, nullptr, const_cast<char*>(g_pszKey_NOMBRE_8A8CF0),
                            text, 256);
        AssignStringRange_429B60(group + 4, nullptr, text,
                                static_cast<unsigned int>(std::strlen(text)));
        CSF_AT(int, group, 0x20) =
            ReadIntegerAttribute(groupNode, nullptr, const_cast<char*>(g_pszKey_TIPO_8A8CF4));

        auto* points = FindProperty(groupNode, nullptr, const_cast<char*>(g_pszKey_PUNTOS));
        if (!points)
            continue;

        const unsigned int pointCount = MenuExtras_GetChildCount(points, nullptr);
        WaypointGraph_ReservePoints(group + 52, nullptr, pointCount);
        if (pointCount)
        {
            std::memset(pointBytes + 4, 0, 36);
            CSF_AT(std::uint32_t, pointBytes, 0x28) = 10;
            CSF_AT(int, pointBytes, 0x00) = -1;

            for (unsigned int pointIndex = 0; pointIndex < pointCount; ++pointIndex)
            {
                auto* pointNode = reinterpret_cast<std::uint8_t*>(
                    GetComponent(points, nullptr, pointIndex));
                WaypointGraph_AppendPoint(group + 52, nullptr,
                                         reinterpret_cast<uint32_t*>(pointBytes));

                auto* point = reinterpret_cast<std::uint8_t*>(
                    CSF_AT(std::uint32_t, group, 0x3C)) - 44;
                CSF_AT(int, point, 0x00) =
                    ReadIntegerAttribute(pointNode, nullptr, const_cast<char*>(g_pszKey_ID_8A8D00));
                MenuExtras_ReadText(pointNode, nullptr, const_cast<char*>(g_pszKey_NOMBRE_8A8CF0),
                                    text, 256);
                DataNode_ReadVector3Property(
                    pointNode, nullptr, const_cast<char*>(g_pszKey_POS_8A8D04),
                    reinterpret_cast<int>(point + 4));
                CSF_AT(float, point, 0x14) = static_cast<float>(
                    ReadFloatAttribute(pointNode, nullptr, const_cast<char*>(g_pszKey_ROT)));
                auto* rotationX = FindProperty(pointNode, nullptr, const_cast<char*>(g_pszKey_ROT_X));
                if (rotationX)
                    CSF_AT(float, point, 0x10) = static_cast<float>(
                        ReadComponentFloat(reinterpret_cast<std::uint32_t>(rotationX)));
            }
        }

        auto* connections = FindProperty(groupNode, nullptr, const_cast<char*>(g_pszKey_CONEXIONES));
        if (!connections)
            continue;

        const unsigned int connectionCount = MenuExtras_GetChildCount(connections, nullptr);
        for (unsigned int connectionIndex = 0; connectionIndex < connectionCount; ++connectionIndex)
        {
            auto* connection = reinterpret_cast<std::uint8_t*>(
                GetComponent(connections, nullptr, connectionIndex));
            int sourceId = ReadIntegerAttribute(connection, nullptr, const_cast<char*>(g_pszKey_PUNTO_ORI));
            const unsigned int sourceIndex = S_00692b80_FindIndexOfValue(
                reinterpret_cast<S_00692b80*>(group), nullptr, sourceId);
            if (sourceIndex == static_cast<unsigned int>(-1))
                continue;

            int destinationId = ReadIntegerAttribute(connection, nullptr, const_cast<char*>(g_pszKey_PUNTO_DST));
            const int destinationIndex = S_00692b80_FindIndexOfValue(
                reinterpret_cast<S_00692b80*>(group), nullptr, destinationId);
            if (destinationIndex == -1)
                continue;

            auto* sourceLinks = reinterpret_cast<std::uint8_t*>(
                CSF_AT(std::uint32_t, group, 0x38)) + 44 * sourceIndex + 28;
            if (CSF_AT(std::uint32_t, sourceLinks, 0x08) >=
                CSF_AT(std::uint32_t, sourceLinks, 0x04))
                ResizePairBuffer(sourceLinks, nullptr, CSF_AT(int, sourceLinks, 0x0C));

            int sourceLinkCount = CSF_AT(int, sourceLinks, 0x08);
            auto* forwardLink = reinterpret_cast<std::uint8_t*>(
                CSF_AT(std::uint32_t, sourceLinks, 0x00)) + 8 * sourceLinkCount;
            CSF_AT(int, sourceLinks, 0x08) = sourceLinkCount + 1;
            CSF_AT(int, forwardLink, 0x04) = destinationIndex;
            CSF_AT(std::uint32_t, forwardLink, 0x00) = groupIndex;

            auto* destinationLinks = reinterpret_cast<std::uint8_t*>(
                CSF_AT(std::uint32_t, group, 0x38)) + 44 * destinationIndex + 28;
            if (CSF_AT(std::uint32_t, destinationLinks, 0x08) >=
                CSF_AT(std::uint32_t, destinationLinks, 0x04))
                ResizePairBuffer(destinationLinks, nullptr, CSF_AT(int, destinationLinks, 0x0C));

            int destinationLinkCount = CSF_AT(int, destinationLinks, 0x08);
            auto* reverseLink = reinterpret_cast<std::uint8_t*>(
                CSF_AT(std::uint32_t, destinationLinks, 0x00)) + 8 * destinationLinkCount;
            CSF_AT(int, destinationLinks, 0x08) = destinationLinkCount + 1;
            CSF_AT(std::uint32_t, reverseLink, 0x00) = groupIndex;
            CSF_AT(std::uint32_t, reverseLink, 0x04) = sourceIndex;
        }
    }

    auto* connections = FindProperty(reinterpret_cast<std::uint8_t*>(node), nullptr,
                                     const_cast<char*>(g_pszKey_CONEXIONES));
    if (connections)
    {
        const unsigned int connectionCount = MenuExtras_GetChildCount(connections, nullptr);
        for (unsigned int connectionIndex = 0; connectionIndex < connectionCount; ++connectionIndex)
        {
            auto* connection = reinterpret_cast<std::uint8_t*>(
                GetComponent(connections, nullptr, connectionIndex));
            int sourceGroupId = ReadIntegerAttribute(
                connection, nullptr, const_cast<char*>(g_pszKey_GRUPO_ORI));
            const int sourceGroupIndex = Table_FindIndex(self, nullptr, sourceGroupId);
            if (sourceGroupIndex == -1)
                continue;

            auto* sourceGroup = reinterpret_cast<std::uint8_t*>(self->field_0008)
                              + 72 * sourceGroupIndex;
            int sourcePointId = ReadIntegerAttribute(
                connection, nullptr, const_cast<char*>(g_pszKey_PUNTO_ORI));
            const unsigned int sourcePointIndex = S_00692b80_FindIndexOfValue(
                reinterpret_cast<S_00692b80*>(sourceGroup), nullptr, sourcePointId);
            if (sourcePointIndex == static_cast<unsigned int>(-1))
                continue;

            int destinationGroupId = ReadIntegerAttribute(
                connection, nullptr, const_cast<char*>(g_pszKey_GRUPO_DST));
            const int destinationGroupIndex = Table_FindIndex(self, nullptr, destinationGroupId);
            if (destinationGroupIndex == -1)
                continue;

            auto* destinationGroup = reinterpret_cast<std::uint8_t*>(self->field_0008)
                                   + 72 * destinationGroupIndex;
            int destinationPointId = ReadIntegerAttribute(
                connection, nullptr, const_cast<char*>(g_pszKey_PUNTO_DST));
            const int destinationPointIndex = S_00692b80_FindIndexOfValue(
                reinterpret_cast<S_00692b80*>(destinationGroup), nullptr, destinationPointId);
            if (destinationPointIndex == -1)
                continue;

            auto* sourceLinks = sourceGroup + 36;
            if (CSF_AT(std::uint32_t, sourceLinks, 0x08) >=
                CSF_AT(std::uint32_t, sourceLinks, 0x04))
                GrowControlBindingArray(sourceLinks, nullptr, CSF_AT(int, sourceLinks, 0x0C));

            int sourceLinkCount = CSF_AT(int, sourceLinks, 0x08);
            std::uint32_t sourceLinkData = CSF_AT(std::uint32_t, sourceLinks, 0x00);
            CSF_AT(int, sourceLinks, 0x08) = sourceLinkCount + 1;
            auto* forwardLink = reinterpret_cast<std::uint8_t*>(sourceLinkData)
                              + 12 * sourceLinkCount;
            CSF_AT(std::uint32_t, forwardLink, 0x00) = sourcePointIndex;
            CSF_AT(int, forwardLink, 0x04) = destinationGroupIndex;
            CSF_AT(int, forwardLink, 0x08) = destinationPointIndex;

            auto* destinationLinks = destinationGroup + 36;
            if (CSF_AT(std::uint32_t, destinationLinks, 0x08) >=
                CSF_AT(std::uint32_t, destinationLinks, 0x04))
                GrowControlBindingArray(destinationLinks, nullptr,
                                        CSF_AT(int, destinationLinks, 0x0C));

            int destinationLinkCount = CSF_AT(int, destinationLinks, 0x08);
            auto* reverseLink = reinterpret_cast<std::uint8_t*>(
                CSF_AT(std::uint32_t, destinationLinks, 0x00)) + 12 * destinationLinkCount;
            CSF_AT(int, destinationLinks, 0x08) = destinationLinkCount + 1;
            CSF_AT(int, reverseLink, 0x00) = destinationPointIndex;
            CSF_AT(int, reverseLink, 0x04) = sourceGroupIndex;
            CSF_AT(std::uint32_t, reverseLink, 0x08) = sourcePointIndex;
        }
    }
    return 1;
}

HOOK(0x0075E810, C_0086c288_LoadWaypointGraph, "auto");
