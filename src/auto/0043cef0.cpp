#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, ClampFloatToBounds, 0x00406B60, (float* value, float* lowerBound, float* upperBound));
CSF_FUNC(void, __cdecl, Hud_ReadRectFromProperties, 0x0042DF00, (S_00692b80* a1, int* a2));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(int, __fastcall, ReadIntegerProperty, 0x00693910, (std::uint8_t* self, void* edx, char* name));
CSF_FUNC(double, __fastcall, ReadFloatProperty, 0x00693940, (std::uint8_t* self, void* edx, char* name));

CSF_GLOBAL(const char*, g_pszKey_NUM_VERTEX, 0x00899698);
CSF_GLOBAL(const char*, g_pszKey_SOLID, 0x008996AC);
CSF_GLOBAL(const char*, g_pszKey_TEX_W, 0x008996B0);
CSF_GLOBAL(const char*, g_pszKey_TEX_H, 0x008996B4);

static char __fastcall MinimapRadar_LoadFromComponents(void* self, void* /*edx*/, S_00692b80* desc)
{
    std::uint8_t* base = reinterpret_cast<std::uint8_t*>(self);

    std::uint32_t rectComp = GetComponent(reinterpret_cast<unsigned char*>(desc), nullptr, 0);
    if (!rectComp)
        return 0;
    Hud_ReadRectFromProperties(reinterpret_cast<S_00692b80*>(rectComp), reinterpret_cast<int*>(base + 0x24));

    std::uint8_t* props = reinterpret_cast<std::uint8_t*>(
        GetComponent(reinterpret_cast<unsigned char*>(desc), nullptr, 1));
    if (!props)
        return 0;

    int numVertex = ReadIntegerProperty(props, nullptr, const_cast<char*>(g_pszKey_NUM_VERTEX));
    CSF_AT(int, base, 0x20) = numVertex;
    if (numVertex < 5)
        numVertex = 5;
    CSF_AT(int, base, 0x20) = numVertex;

    void** mesh = CSF_AT(void**, base, 0x1C);
    ((void(__fastcall*)(void*, void*, int, int))CSF_AT(void**, mesh, 0)[0x2C / 4])(
        mesh, nullptr, 2 * numVertex + 1, 9 * numVertex);

    mesh = CSF_AT(void**, base, 0x1C);
    ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, mesh, 0)[0x20 / 4])(mesh, nullptr, 1);

    float* solid = reinterpret_cast<float*>(base + 0x3C);
    *solid = static_cast<float>(ReadFloatProperty(props, nullptr, const_cast<char*>(g_pszKey_SOLID)));
    float upper = 1.0f;
    float lower = 0.0f;
    ClampFloatToBounds(solid, &lower, &upper);

    CSF_AT(int, base, 0x34) = ReadIntegerProperty(props, nullptr, const_cast<char*>(g_pszKey_TEX_W));
    CSF_AT(int, base, 0x38) = ReadIntegerProperty(props, nullptr, const_cast<char*>(g_pszKey_TEX_H));
    return 1;
}

HOOK(0x0043CEF0, MinimapRadar_LoadFromComponents, "auto");
