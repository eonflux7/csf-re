#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(int, __fastcall, S_00692b80_GetChildCount, 0x00692B80, (S_00692b80* self, void*));
CSF_FUNC(S_00692b80*, __fastcall, S_00692b80_GetComponent, 0x00692BD0, (S_00692b80* self, void* , unsigned int index));
CSF_FUNC(int, __fastcall, S_00692b80_ParseIntegerComponent, 0x00692C40, (S_00692b80* self, void*));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(int, __fastcall, ReadIntegerAttribute, 0x00693970, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(double, __fastcall, ReadFloatAttribute, 0x00693990, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0, (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));
CSF_FUNC(int, __fastcall, DataNode_ReadVector3Property, 0x00693A60, (void* self, void* edx, char* String1, int a3));
CSF_FUNC(bool, __cdecl, Geometry_IsPolygonSimple, 0x006AFE10, (std::uint32_t* polygon));
CSF_FUNC(char, __fastcall, S_006b0320_ReleaseBuffers, 0x006B1010, (S_006b0320* self, void*));
CSF_FUNC(char, __fastcall, S_006b0320_TryApplyRecord, 0x006B1050, (S_006b0320* self, void* , uint32_t* record));
CSF_FUNC(const void*, __fastcall, Gfx2d_GrowPositionBuffer, 0x006D64A0, (void* self, void* , int growBy));
CSF_FUNC(uint32_t*, __fastcall, RpPrtStd_ResetEmitterSubObject703440, 0x00703440, (void* self, void* edx));
CSF_FUNC(double, __fastcall, SoundAreas_CalculateAreaRadius, 0x007706B0, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, SoundAreas_ReservePoints, 0x00770BD0, (void* self, void* edx, unsigned int a2));
CSF_FUNC(uint32_t*, __fastcall, SoundAreas_AppendPoint, 0x00770E00, (void* self, void* edx, uint32_t* a3));
CSF_FUNC(unsigned int, __fastcall, SoundAreas_ReserveAreas, 0x00771230, (void* self, void* edx, unsigned int a2));
CSF_FUNC(uint32_t*, __fastcall, SoundAreas_AppendArea, 0x00771700, (void* self, void* edx, uint32_t* a3));

CSF_GLOBAL(const char*, g_pszKey_AREAS, 0x008A9948);
CSF_GLOBAL(const char*, g_pszKey_PUNTOS_8A994C, 0x008A994C);
CSF_GLOBAL(const char*, g_pszKey_ID_8A9950, 0x008A9950);
CSF_GLOBAL(const char*, g_pszKey_TIPO_8A9954, 0x008A9954);
CSF_GLOBAL(const char*, g_pszKey_NOMBRE_8A9958, 0x008A9958);
CSF_GLOBAL(const char*, g_pszKey_HEIGHT_8A995C, 0x008A995C);
CSF_GLOBAL(const char*, g_pszKey_POS_8A9964, 0x008A9964);
CSF_GLOBAL(const char*, g_pszKey_REVERB_8A9970, 0x008A9970);
CSF_GLOBAL(const char*, g_pszKey_FLAGS_8A9974, 0x008A9974);
CSF_GLOBAL(const char*, g_pszKey_OCLUSION, 0x008A9978);
CSF_GLOBAL(const char*, g_pszKey_LIMITREVERB, 0x008A997C);

static char __fastcall C_00871914_ParseAreas(
    C_00871914* self, void* /*edx*/, S_00692b80* root)
{
    // The packet also identifies the child-count entry point as a vtable address.
    (void)CSF_VTBL(0x00692B80);

    auto* areas = reinterpret_cast<S_00692b80*>(
        FindProperty(reinterpret_cast<std::uint8_t*>(root), nullptr,
                     const_cast<char*>(g_pszKey_AREAS)));
    if (!areas)
        return 0;

    char text[256];
    std::memset(text, 0, sizeof(text));

    const unsigned int areaCount = S_00692b80_GetChildCount(areas, nullptr);
    void* collection = reinterpret_cast<std::uint8_t*>(self) + 4;
    SoundAreas_ReserveAreas(collection, nullptr, areaCount);

    // Preserve the contiguous record and the full room for its embedded objects.
    alignas(8) std::uint8_t recordBytes[0x9C];
    auto& name = *reinterpret_cast<C_0082d410*>(recordBytes + 0x04);
    auto& points = *reinterpret_cast<S_00770ce0*>(recordBytes + 0x20);
    auto* geometry = reinterpret_cast<S_006b0320*>(recordBytes + 0x30);
    auto* position = reinterpret_cast<std::uint32_t*>(recordBytes + 0x7C);
    auto* polygon = reinterpret_cast<std::uint32_t*>(recordBytes + 0x88);

    for (unsigned int areaIndex = 0; areaIndex < areaCount; ++areaIndex)
    {
        CSF_AT(unsigned int, &name, 0x18) = 15;
        CSF_AT(unsigned int, &name, 0x14) = 0;
        reinterpret_cast<char*>(&name.field_0004)[0] = 0;
        std::memset(&points.field_0004, 0, 12);
        RpPrtStd_ResetEmitterSubObject703440(geometry, nullptr);

        CSF_AT(int, geometry, 0x2C) = 0;
        CSF_AT(int, geometry, 0x30) = 1;
        CSF_AT(std::uint32_t, geometry, 0x28) = 1;
        CSF_AT(std::uint8_t, geometry, 0x34) = 0;
        CSF_AT(int, recordBytes, 0x00) = -1;
        CSF_AT(float, geometry, 0x20) = 0.0f;
        CSF_AT(float, geometry, 0x24) = 200.0f;

        auto* component = S_00692b80_GetComponent(areas, nullptr, areaIndex);
        auto* node = reinterpret_cast<std::uint8_t*>(component);
        CSF_AT(int, recordBytes, 0x00) =
            ReadIntegerAttribute(node, nullptr, const_cast<char*>(g_pszKey_ID_8A9950));

        auto* property = reinterpret_cast<S_00692b80*>(
            FindProperty(node, nullptr, const_cast<char*>(g_pszKey_FLAGS_8A9974)));
        if (property)
            CSF_AT(std::uint32_t, geometry, 0x28) =
                S_00692b80_ParseIntegerComponent(property, nullptr);

        property = reinterpret_cast<S_00692b80*>(
            FindProperty(node, nullptr, const_cast<char*>(g_pszKey_TIPO_8A9954)));
        if (property)
        {
            const int type = S_00692b80_ParseIntegerComponent(property, nullptr);
            CSF_AT(std::uint32_t, geometry, 0x28) |=
                std::uint32_t{1} << (static_cast<unsigned int>(type) & 31);
        }

        property = reinterpret_cast<S_00692b80*>(
            FindProperty(node, nullptr, const_cast<char*>(g_pszKey_OCLUSION)));
        if (property)
            CSF_AT(int, geometry, 0x30) =
                S_00692b80_ParseIntegerComponent(property, nullptr);

        property = reinterpret_cast<S_00692b80*>(
            FindProperty(node, nullptr, const_cast<char*>(g_pszKey_REVERB_8A9970)));
        if (property)
            CSF_AT(int, geometry, 0x2C) =
                S_00692b80_ParseIntegerComponent(property, nullptr);

        property = reinterpret_cast<S_00692b80*>(
            FindProperty(node, nullptr, const_cast<char*>(g_pszKey_LIMITREVERB)));
        if (property)
            CSF_AT(std::uint8_t, geometry, 0x34) =
                S_00692b80_ParseIntegerComponent(property, nullptr) > 0;

        MenuExtras_ReadText(node, nullptr,
                           const_cast<char*>(g_pszKey_NOMBRE_8A9958), text, 256);
        AssignStringRange_429B60(&name, nullptr, text,
                                static_cast<unsigned int>(std::strlen(text)));
        CSF_AT(float, geometry, 0x24) = static_cast<float>(
            ReadFloatAttribute(node, nullptr, const_cast<char*>(g_pszKey_HEIGHT_8A995C)));

        auto* pointNodes = reinterpret_cast<S_00692b80*>(
            FindProperty(node, nullptr, const_cast<char*>(g_pszKey_PUNTOS_8A994C)));
        if (pointNodes)
        {
            const unsigned int pointCount =
                S_00692b80_GetChildCount(pointNodes, nullptr);
            SoundAreas_ReservePoints(&points, nullptr, pointCount);
            std::memset(polygon, 0, 12);
            CSF_AT(int, polygon, 0x0C) = 10;
            if (pointCount)
                Gfx2d_GrowPositionBuffer(polygon, nullptr, pointCount);

            for (unsigned int pointIndex = 0; pointIndex < pointCount; ++pointIndex)
            {
                std::memset(position, 0, 12);
                auto* pointNode =
                    S_00692b80_GetComponent(pointNodes, nullptr, pointIndex);
                DataNode_ReadVector3Property(
                    pointNode, nullptr, const_cast<char*>(g_pszKey_POS_8A9964),
                    static_cast<int>(reinterpret_cast<std::uintptr_t>(position)));
                SoundAreas_AppendPoint(&points, nullptr, position);

                const std::uint32_t x = position[0];
                const std::uint32_t z = position[2];
                if (CSF_AT(unsigned int, polygon, 0x08) >=
                    CSF_AT(unsigned int, polygon, 0x04))
                {
                    Gfx2d_GrowPositionBuffer(
                        polygon, nullptr, CSF_AT(int, polygon, 0x0C));
                }

                auto* destination = reinterpret_cast<std::uint32_t*>(
                    CSF_AT(std::uint32_t, polygon, 0x00)) +
                    2 * CSF_AT(unsigned int, polygon, 0x08);
                ++CSF_AT(unsigned int, polygon, 0x08);
                destination[0] = x;
                destination[1] = z;
            }

            if (Geometry_IsPolygonSimple(polygon))
                S_006b0320_TryApplyRecord(geometry, nullptr, polygon);

            CSF_AT(float, geometry, 0x20) =
                static_cast<float>(SoundAreas_CalculateAreaRadius(recordBytes, nullptr));
            SoundAreas_AppendArea(
                collection, nullptr, reinterpret_cast<std::uint32_t*>(recordBytes));

            if (CSF_AT(std::uint32_t, polygon, 0x00))
            {
                CoreMemory_Free(reinterpret_cast<void*>(
                    CSF_AT(std::uint32_t, polygon, 0x00)));
                CSF_AT(std::uint32_t, polygon, 0x00) = 0;
            }
        }

        S_006b0320_ReleaseBuffers(geometry, nullptr);
        if (points.field_0004)
            Mem_FreeDebug(reinterpret_cast<void*>(points.field_0004));
        std::memset(&points.field_0004, 0, 12);

        if (CSF_AT(unsigned int, &name, 0x18) >= 0x10)
        {
            if (name.field_0004)
                Mem_FreeDebug(reinterpret_cast<void*>(name.field_0004));
        }
        CSF_AT(unsigned int, &name, 0x18) = 15;
        CSF_AT(unsigned int, &name, 0x14) = 0;
        reinterpret_cast<char*>(&name.field_0004)[0] = 0;
    }

    return 1;
}

HOOK(0x00771790, C_00871914_ParseAreas, "auto");
