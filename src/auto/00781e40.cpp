#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// ---------------------------------------------------------------------------
// Callees (bound by address)
// ---------------------------------------------------------------------------
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(void*, __fastcall, MenuExtras_InitializeFile, 0x00692B10, (void* self, void* edx));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(int, __fastcall, ReadIntegerProperty, 0x00693910, (std::uint8_t* self, void* edx, char* name));
CSF_FUNC(int, __fastcall, ReadIntegerAttribute, 0x00693970, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0, (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));
CSF_FUNC(int, __fastcall, MenuExtras_CloseFile, 0x00694380, (void* self, void* edx));
CSF_FUNC(char, __fastcall, MenuExtras_LoadFile, 0x00694770, (void* self, void* edx, const char* path));
CSF_FUNC(char, __cdecl, Path_ReplaceExtension, 0x006947D0, (char* Str, int a2, char* a3));
CSF_FUNC(bool, __cdecl, Path_Normalize, 0x00697470, (char* a1));

CSF_FUNC(int, __fastcall, CLevelData_Reset, 0x0077D5F0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, CLevelData_LoadV1to3, 0x00778D50, (void* self, void* edx, void* a2));
CSF_FUNC(char, __fastcall, CLevelData_LoadV4, 0x007793D0, (void* self, void* edx, void* a2));
CSF_FUNC(char, __fastcall, CLevelData_LoadV5, 0x00779AD0, (void* self, void* edx, void* a2));
CSF_FUNC(char, __fastcall, CLevelData_LoadV6, 0x0077A2D0, (void* self, void* edx, void* a2));
CSF_FUNC(char, __fastcall, CLevelData_LoadV7, 0x0077AAE0, (void* self, void* edx, void* a2));
CSF_FUNC(char, __fastcall, CLevelData_LoadV8, 0x0077B230, (void* self, void* edx, void* a2));
CSF_FUNC(char, __fastcall, CLevelData_LoadV9, 0x0077B9A0, (void* self, void* edx, void* a2));
CSF_FUNC(char, __fastcall, CLevelData_LoadV10, 0x0077C120, (void* self, void* edx, void* a2));
CSF_FUNC(char, __fastcall, CLevelData_LoadV11, 0x0077C920, (void* self, void* edx, void* a2));
CSF_FUNC(char, __fastcall, CLevelData_LoadV12, 0x0077D730, (void* self, void* edx, void* a2));
CSF_FUNC(char, __fastcall, CLevelData_LoadV13, 0x0077E240, (void* self, void* edx, void* a2));
CSF_FUNC(char, __fastcall, CLevelData_LoadV14, 0x0077EEC0, (void* self, void* edx, void* a2));
CSF_FUNC(char, __fastcall, CLevelData_LoadV15to16, 0x0077FB70, (void* self, void* edx, void* a2));
CSF_FUNC(char, __fastcall, CLevelData_LoadV17, 0x00780B90, (void* self, void* edx, void* a2));
CSF_FUNC(char, __fastcall, DstData_LoadFile, 0x00782550, (void* self, void* edx, int a2));

// ---------------------------------------------------------------------------
// Keyword globals
// ---------------------------------------------------------------------------
CSF_GLOBAL(char*, g_pszKey_VERSION_game_data_defs, 0x008A99E8);
CSF_GLOBAL(char*, g_pszKey_MALLA_NAVEGACION, 0x008A99FC);
CSF_GLOBAL(char*, g_pszKey_MAPA_SECTORES, 0x008A9A00);
CSF_GLOBAL(char*, g_pszKey_MALLA_DUMMIES, 0x008A9A04);
CSF_GLOBAL(char*, g_pszKey_MALLA_AREAS, 0x008A9A08);
CSF_GLOBAL(char*, g_pszKey_MALLA_LUCES, 0x008A9A0C);
CSF_GLOBAL(char*, g_pszKey_MALLA_SCENE_OBJS, 0x008A9A10);
CSF_GLOBAL(char*, g_pszKey_PUNTUACION_MAXIMA, 0x008A9B54);
CSF_GLOBAL(char*, g_pszKey_PUNTUACION_MINIMA, 0x008A9B58);

// Inline string copy (the original has strcpy expanded inline).
static void CopyString(char* destination, const char* source)
{
    std::memcpy(destination, source, std::strlen(source) + 1);
}

// Calls a mesh sub-object's "load from property node" virtual.
static char CallMeshLoad(void* mesh, int slot, std::uint8_t* node, int version)
{
    return ((char(__fastcall*)(void*, void*, std::uint8_t*, int))CSF_AT(void**, mesh, 0)[slot])(mesh, nullptr, node, version);
}

// Loads a versioned level data definition file.
static char __fastcall CLevelData_Load(CLevelData* self, void* /*edx*/, const char* path, int /*unused*/)
{
    alignas(4) std::uint8_t file[0x3C];
    char sectorMapPath[256];
    char dstPath[256];

    CLevelData_Reset(self, nullptr);

    MenuExtras_InitializeFile(file, nullptr);
    char ok = MenuExtras_LoadFile(file, nullptr, path);
    if (!ok)
    {
        MenuExtras_CloseFile(file, nullptr);
        return 0;
    }

    std::uint8_t* root = CSF_AT(std::uint8_t*, file, 0x0);
    int version = ReadIntegerAttribute(root, nullptr, g_pszKey_VERSION_game_data_defs);
    if (version > 17)
    {
        MenuExtras_CloseFile(file, nullptr);
        return 0;
    }

    CSF_AT(int, self, 0x1F38) = ReadIntegerProperty(root, nullptr, g_pszKey_PUNTUACION_MAXIMA);
    if (ReadIntegerProperty(root, nullptr, g_pszKey_PUNTUACION_MINIMA))
        CSF_AT(int, self, 0x1F3C) = ReadIntegerProperty(root, nullptr, g_pszKey_PUNTUACION_MINIMA);

    std::uint8_t* navNode = FindProperty(root, nullptr, g_pszKey_MALLA_NAVEGACION);
    if (navNode)
        ok = CallMeshLoad(reinterpret_cast<void*>(self->m_navMesh), 9, navNode, version);

    MenuExtras_ReadText(root, nullptr, g_pszKey_MAPA_SECTORES, sectorMapPath, 256);
    if (!sectorMapPath[0])
    {
        // Default sector map: same path as the definition file with a "sec" extension.
        CopyString(sectorMapPath, path);
        const std::uint32_t secExtension = 6514035; // "sec\0"
        std::memcpy(sectorMapPath + std::strlen(sectorMapPath) - 3, &secExtension, sizeof(secExtension));
        Path_Normalize(sectorMapPath);
    }
    AssignStringRange_429B60((std::uint8_t*)self + 0x1A7C, nullptr, sectorMapPath,
                             (unsigned int)std::strlen(sectorMapPath));

    std::uint8_t* dummiesNode = FindProperty(root, nullptr, g_pszKey_MALLA_DUMMIES);
    if (ok && dummiesNode)
        ok = CallMeshLoad(reinterpret_cast<void*>(self->m_dummyMesh), 1, dummiesNode, version);

    std::uint8_t* areasNode = FindProperty(root, nullptr, g_pszKey_MALLA_AREAS);
    if (ok && areasNode)
        ok = CallMeshLoad(reinterpret_cast<void*>(self->m_areaMesh), 1, areasNode, version);

    std::uint8_t* lightsNode = FindProperty(root, nullptr, g_pszKey_MALLA_LUCES);
    if (ok && lightsNode)
    {
        void* lightMesh = reinterpret_cast<void*>(self->m_lightMesh);
        if (lightMesh)
            ok = CallMeshLoad(lightMesh, 1, lightsNode, version);
    }

    std::uint8_t* sceneObjsNode = FindProperty(root, nullptr, g_pszKey_MALLA_SCENE_OBJS);
    if (ok && sceneObjsNode)
    {
        void* sceneObjMesh = reinterpret_cast<void*>(self->m_sceneObjMesh);
        if (sceneObjMesh)
            ok = CallMeshLoad(sceneObjMesh, 1, sceneObjsNode, version);
    }

    if (!ok)
    {
        CLevelData_Reset(self, nullptr);
        MenuExtras_CloseFile(file, nullptr);
        return 0;
    }

    std::memset(dstPath, 0, sizeof(dstPath));
    CopyString(dstPath, path);
    Path_ReplaceExtension(dstPath, 256, const_cast<char*>(CSF_STR(0x00872400, ".dst")));
    DstData_LoadFile(reinterpret_cast<void*>(self->m_dstData), nullptr, (int)dstPath);

    char result = 0;
    switch (version)
    {
    case 1:
    case 2:
    case 3:
        result = CLevelData_LoadV1to3(self, nullptr, root);
        break;
    case 4:
        result = CLevelData_LoadV4(self, nullptr, root);
        break;
    case 5:
        result = CLevelData_LoadV5(self, nullptr, root);
        break;
    case 6:
        result = CLevelData_LoadV6(self, nullptr, root);
        break;
    case 7:
        result = CLevelData_LoadV7(self, nullptr, root);
        break;
    case 8:
        result = CLevelData_LoadV8(self, nullptr, root);
        break;
    case 9:
        result = CLevelData_LoadV9(self, nullptr, root);
        break;
    case 10:
        result = CLevelData_LoadV10(self, nullptr, root);
        break;
    case 11:
        result = CLevelData_LoadV11(self, nullptr, root);
        break;
    case 12:
        result = CLevelData_LoadV12(self, nullptr, root);
        break;
    case 13:
        result = CLevelData_LoadV13(self, nullptr, root);
        break;
    case 14:
        result = CLevelData_LoadV14(self, nullptr, root);
        break;
    case 15:
    case 16:
        result = CLevelData_LoadV15to16(self, nullptr, root);
        break;
    case 17:
        result = CLevelData_LoadV17(self, nullptr, root);
        break;
    default:
        break;
    }

    MenuExtras_CloseFile(file, nullptr);
    return result;
}

HOOK(0x00781E40, CLevelData_Load, "auto");
