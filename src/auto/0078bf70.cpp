#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __fastcall, S_00692b80_ParseIntegerComponent, 0x00692C40, (S_00692b80* self, void*));
CSF_FUNC(char, __fastcall, CopyAttributeText, 0x00692D40, (std::uint8_t* attribute, void* edx, char* buffer, int capacity));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0, (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));
CSF_FUNC(int, __cdecl, CharacterDefs_FindParticleSystemIndex, 0x00761F30, (char* String1));

CSF_FUNC(bool, __stdcall, NetPerfCounters_ParseParticleType0Settings, 0x0078B500, (void* a1, int a2));
CSF_FUNC(void, __stdcall, NetPerfCounters_ParseParticleType1Settings, 0x0078B650, (void* a1, int a2));
CSF_FUNC(int, __stdcall, NetPerfCounters_ParseParticleType2Settings, 0x0078B760, (void* a1, int a2));
CSF_FUNC(void, __stdcall, NetPerfCounters_ParseParticleType4Settings, 0x0078B8E0, (void* a1, int a2));
CSF_FUNC(void, __stdcall, NetPerfCounters_ParseParticleType5Settings, 0x0078BA10, (void* a1, int a2));

CSF_GLOBAL(const char*, g_pszKey_TIPO_net_perf_counters, 0x008A9DB0);
CSF_GLOBAL(const char*, g_pszKey_FILE_net_perf_counters, 0x008A9DB4);
CSF_GLOBAL(const char*, g_pszKey_DUMMY_net_perf_counters, 0x008A9DB8);
CSF_GLOBAL(const char*, g_pszKey_PRIORITY_net_perf_counters, 0x008A9DBC);
CSF_GLOBAL(const char*, g_pszKey_REFLEXION, 0x008A9DC0);
CSF_GLOBAL(const char*, g_pszKey_BACKPLANE, 0x008A9DC4);
CSF_GLOBAL(const char*, g_pszKey_LOOP_net_perf_counters, 0x008A9DCC);
CSF_GLOBAL(const char*, g_pszKey_AUTOSTART, 0x008A9DD0);
CSF_GLOBAL(const char*, g_pszKey_QUALITY_CFG, 0x008A9E60);

static void __fastcall S_0078bf70_ReadParticleSystemDefinition(
    S_0078bf70* /*self*/, void* /*edx*/, S_00692b80* properties, int definitionAddress)
{
    alignas(8) std::uint8_t bufferBytes[0x104];
    char* buffer = reinterpret_cast<char*>(bufferBytes);
    std::memset(buffer, 0, 256);

    auto* source = reinterpret_cast<std::uint8_t*>(properties);
    auto* definition = reinterpret_cast<std::uint8_t*>(definitionAddress);

    MenuExtras_ReadText(source, nullptr,
        const_cast<char*>(g_pszKey_TIPO_net_perf_counters), buffer, 256);
    CSF_AT(int, definition, 0x04) = CharacterDefs_FindParticleSystemIndex(buffer);

    auto* property = FindProperty(source, nullptr,
        const_cast<char*>(g_pszKey_FILE_net_perf_counters));
    if (property)
    {
        CopyAttributeText(property, nullptr, buffer, 256);
        AssignStringRange_429B60(definition + 0x08, nullptr, buffer,
            static_cast<unsigned int>(std::strlen(buffer)));
    }

    property = FindProperty(source, nullptr,
        const_cast<char*>(g_pszKey_DUMMY_net_perf_counters));
    if (property)
    {
        CopyAttributeText(property, nullptr, buffer, 256);
        AssignStringRange_429B60(definition + 0x24, nullptr, buffer,
            static_cast<unsigned int>(std::strlen(buffer)));
    }

    property = FindProperty(source, nullptr,
        const_cast<char*>(g_pszKey_PRIORITY_net_perf_counters));
    if (property)
        CSF_AT(int, definition, 0x40) = S_00692b80_ParseIntegerComponent(
            reinterpret_cast<S_00692b80*>(property), nullptr);

    property = FindProperty(source, nullptr, const_cast<char*>(g_pszKey_REFLEXION));
    if (property)
        CSF_AT(std::uint8_t, definition, 0x44) =
            S_00692b80_ParseIntegerComponent(
                reinterpret_cast<S_00692b80*>(property), nullptr) != 0;

    property = FindProperty(source, nullptr, const_cast<char*>(g_pszKey_BACKPLANE));
    if (property)
        CSF_AT(std::uint8_t, definition, 0x45) =
            S_00692b80_ParseIntegerComponent(
                reinterpret_cast<S_00692b80*>(property), nullptr) != 0;

    property = FindProperty(source, nullptr,
        const_cast<char*>(g_pszKey_LOOP_net_perf_counters));
    if (property)
        CSF_AT(std::uint8_t, definition, 0x48) =
            S_00692b80_ParseIntegerComponent(
                reinterpret_cast<S_00692b80*>(property), nullptr) != 0;

    property = FindProperty(source, nullptr, const_cast<char*>(g_pszKey_AUTOSTART));
    if (property)
        CSF_AT(std::uint8_t, definition, 0x47) =
            S_00692b80_ParseIntegerComponent(
                reinterpret_cast<S_00692b80*>(property), nullptr) != 0;

    property = FindProperty(source, nullptr, const_cast<char*>(g_pszKey_QUALITY_CFG));
    if (property)
        CSF_AT(int, definition, 0x4C) = S_00692b80_ParseIntegerComponent(
            reinterpret_cast<S_00692b80*>(property), nullptr);
    else
        CSF_AT(int, definition, 0x4C) = 0;

    switch (CSF_AT(int, definition, 0x04))
    {
    case 0:
        NetPerfCounters_ParseParticleType0Settings(properties, definitionAddress);
        break;
    case 1:
        NetPerfCounters_ParseParticleType1Settings(properties, definitionAddress);
        break;
    case 2:
        NetPerfCounters_ParseParticleType2Settings(properties, definitionAddress);
        break;
    case 4:
        NetPerfCounters_ParseParticleType4Settings(properties, definitionAddress);
        break;
    case 5:
        NetPerfCounters_ParseParticleType5Settings(properties, definitionAddress);
        break;
    default:
        return;
    }
}

HOOK(0x0078BF70, S_0078bf70_ReadParticleSystemDefinition, "auto");
