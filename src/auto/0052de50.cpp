#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(void*, __fastcall, Profiles_AppendStringRange, 0x0043EB30, (void* self, int a2, void* a3, unsigned int a4));
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(int, __cdecl, SoundMgr_StartActiveInstanceIfReady, 0x00521F10, ());
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, SelectionData_BuildFromIndex, 0x00698DF0, (int a1, int a2));

CSF_GLOBAL(std::uint32_t, g_BriefingExtensionWord, 0x00830FA0);

static void __fastcall CMenuScenarioSelect_SelectScenario(
    CMenuScenarioSelect* self, void* /*edx*/, int scenarioIndex)
{
    if (scenarioIndex == -1)
        return;

    GetGlobalValue_521BB0();
    SoundMgr_StartActiveInstanceIfReady();

    struct
    {
        alignas(8) std::uint8_t briefingPathBytes[0x1C];
        std::uint8_t scenarioNameBytes[0x8];
        std::uint32_t trailingZero;
    } storage;
    storage.trailingZero = 0;

    char* scenarioName =
        reinterpret_cast<char*>(storage.scenarioNameBytes);
    SelectionData_BuildFromIndex(
        scenarioIndex, reinterpret_cast<int>(scenarioName));

    C_0082d410& briefingPath =
        *reinterpret_cast<C_0082d410*>(storage.briefingPathBytes);

    CSF_AT(unsigned int, storage.briefingPathBytes, 0x18) = 15;
    CSF_AT(int, storage.briefingPathBytes, 0x14) = 0;
    *reinterpret_cast<std::uint8_t*>(&briefingPath.field_0004) = 0;

    AssignStringRange_429B60(
        &briefingPath, nullptr,
        const_cast<char*>(CSF_STR(0x00830FA8, "Sounds\\Scenes\\Briefings\\")),
        24);
    Profiles_AppendStringRange(
        &briefingPath, 0, scenarioName,
        static_cast<unsigned int>(std::strlen(scenarioName)));
    Profiles_AppendStringRange(
        &briefingPath, 0, &g_BriefingExtensionWord, 4);

    AssignStringRange_429B60(
        reinterpret_cast<std::uint8_t*>(self) + 0x78,
        nullptr, scenarioName,
        static_cast<unsigned int>(std::strlen(scenarioName)));

    const bool inlineStorage =
        CSF_AT(unsigned int, storage.briefingPathBytes, 0x18) < 0x10;
    self->field_0040 = 1;
    self->field_0094 = 0.0f;

    if (!inlineStorage && briefingPath.field_0004)
        Mem_FreeDebug(reinterpret_cast<void*>(briefingPath.field_0004));
}

HOOK(0x0052DE50, CMenuScenarioSelect_SelectScenario, "auto");
