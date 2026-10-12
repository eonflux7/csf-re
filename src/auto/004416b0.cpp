#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Hud_CreateTextureWidget, 0x0042D750, (S_00692b80* properties, int layer));
CSF_FUNC(unsigned int, __fastcall, Campaign_DestroyScenarioRecord, 0x0043E360, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, Campaign_ResetScenarioRecord, 0x0043FA70, (int a1, int a2));
CSF_FUNC(char, __stdcall, Campaign_ParseScenarioRecord, 0x0043FED0, (void* a1, int a3));
CSF_FUNC(unsigned int, __fastcall, MenuScenarioSelect_InitializeStorage, 0x00440150, (void* self, void* edx, unsigned int count));
CSF_FUNC(int, __fastcall, Campaign_InitializeScenarioRecord, 0x00440C20, (int a1, int a2));
CSF_FUNC(uint32_t*, __fastcall, Campaign_AppendScenarioRecord, 0x004413B0, (void* self, void* edx, int a3));
CSF_FUNC(void*, __fastcall, MenuExtras_InitializeFile, 0x00692B10, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(int, __fastcall, MenuExtras_CloseFile, 0x00694380, (void* self, void* edx));
CSF_FUNC(char, __fastcall, MenuExtras_LoadFile, 0x00694770, (void* self, void* edx, const char* path));

CSF_GLOBAL(const char*, g_pszKey_LISTA_ESCENARIOS, 0x008996C4);
CSF_GLOBAL(const char*, g_pszKey_SCREENSHOT, 0x008996D4);

static char __fastcall S_00440ee0_ProcessSceneFile(
    S_00440ee0* self, void* /*edx*/, const char* path)
{
    alignas(8) std::uint8_t scenarioBytes[0xC4];
    alignas(8) std::uint8_t fileBytes[0x44];

    // These two addresses are also listed as vtable references in the packet.
    (void)CSF_VTBL(0x00692B10);
    MenuExtras_InitializeFile(fileBytes, nullptr);
    if (!MenuExtras_LoadFile(fileBytes, nullptr, path))
    {
        MenuExtras_CloseFile(fileBytes, nullptr);
        return 0;
    }

    auto* root = CSF_AT(std::uint8_t*, fileBytes, 0x00);
    char result = 1;
    Campaign_InitializeScenarioRecord(
        reinterpret_cast<int>(scenarioBytes), 0);

    auto* scenarios = FindProperty(
        root, nullptr, const_cast<char*>(g_pszKey_LISTA_ESCENARIOS));
    auto* storage = reinterpret_cast<std::uint8_t*>(self) + 0x30;

    std::uint32_t begin = CSF_AT(std::uint32_t, self, 0x34);
    std::int32_t count = 0;
    if (begin)
    {
        count = static_cast<std::int32_t>(
            CSF_AT(std::uint32_t, self, 0x38) - begin) / 196;
    }

    if (!begin || count == 0)
    {
        (void)CSF_VTBL(0x00692B80);
        const std::uint32_t childCount =
            MenuExtras_GetChildCount(scenarios, nullptr);
        if (childCount != 0)
        {
            MenuScenarioSelect_InitializeStorage(storage, nullptr, childCount);
            for (std::uint32_t index = 0; index < childCount; ++index)
            {
                Campaign_ResetScenarioRecord(
                    reinterpret_cast<int>(scenarioBytes), 0);
                auto* component = reinterpret_cast<void*>(
                    GetComponent(scenarios, nullptr, index));
                result = Campaign_ParseScenarioRecord(
                    component, reinterpret_cast<int>(scenarioBytes));
                if (result)
                {
                    CSF_AT(std::uint32_t, scenarioBytes, 0xBC) = index;
                    Campaign_AppendScenarioRecord(
                        storage, nullptr, reinterpret_cast<int>(scenarioBytes));
                }
            }
        }
    }
    else
    {
        begin = CSF_AT(std::uint32_t, self, 0x34);
        count = 0;
        if (begin)
        {
            count = static_cast<std::int32_t>(
                CSF_AT(std::uint32_t, self, 0x38) - begin) / 196;
        }

        if (count != 0)
        {
            std::uint32_t remaining = static_cast<std::uint32_t>(count);
            std::uint32_t offset = 0;
            do
            {
                auto* record = reinterpret_cast<std::uint8_t*>(
                    CSF_AT(std::uint32_t, self, 0x34)) + offset;
                if (CSF_AT(std::uint32_t, record, 0xB8) == 0)
                {
                    auto* component = reinterpret_cast<std::uint8_t*>(
                        GetComponent(scenarios, nullptr,
                            CSF_AT(std::uint32_t, record, 0xBC)));

                    // GetComponent may have changed the storage pointer.
                    auto* destination = reinterpret_cast<std::uint8_t*>(
                        CSF_AT(std::uint32_t, self, 0x34)) + offset;
                    auto* screenshot = FindProperty(
                        component, nullptr,
                        const_cast<char*>(g_pszKey_SCREENSHOT));
                    CSF_AT(int, destination, 0xB8) = Hud_CreateTextureWidget(
                        reinterpret_cast<S_00692b80*>(screenshot), 0);
                }
                offset += 196;
                --remaining;
            } while (remaining != 0);
        }
    }

    Campaign_DestroyScenarioRecord(scenarioBytes, nullptr);
    MenuExtras_CloseFile(fileBytes, nullptr);
    return result;
}

HOOK(0x004416B0, S_00440ee0_ProcessSceneFile, "auto");
