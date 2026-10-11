#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(uint32_t, g_CharacterSelectState8B0694, 0x008B0694);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(void*, __fastcall, sub_554600, 0x00554600, (void* self, void* edx));
CSF_FUNC(char*, __fastcall, sub_6B29A0, 0x006B29A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_73BDA0, 0x0073BDA0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_747F60, 0x00747F60, (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, sub_748010, 0x00748010, (void* self, void* edx, char a2));

static uint8_t* AsBytePtr(uint32_t value)
{
    return reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(value));
}

// Returns the selected MP stats object (g_CharacterSelectState8B0694 + 4 - 0x308), or nullptr when the raw pointer is 0.
static uint8_t* GetSelectedMpStatsObject()
{
    uint32_t menuRaw = CSF_AT(uint32_t, AsBytePtr(g_CharacterSelectState8B0694), 4);
    if (menuRaw == 0)
        return nullptr;
    return AsBytePtr(menuRaw) - 0x308;
}

// Body of the code at 0x551860, reached from this function by a tail jump (ecx = self).
// Refreshes the spawn-point panel for both spawn handles.
static char RefreshSpawnPointPanel(S_00553680* self)
{
    uint8_t* selfBytes = reinterpret_cast<uint8_t*>(self);
    uint8_t* selectedMenu = GetSelectedMpStatsObject();

    // First spawn-point handle (this + 0x1404).
    uint32_t handleEntry = static_cast<uint32_t>(
        CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(selfBytes + 0x1404))));
    uint32_t eaxValue = handleEntry;
    if (eaxValue != 0)
    {
        eaxValue = CSF_AT(uint32_t, AsBytePtr(handleEntry), 0x18);
        uint8_t* object = eaxValue ? AsBytePtr(eaxValue) - 0x80 : nullptr;
        if (CSF_AT(uint8_t, object, 0x47) != 0)
        {
            sub_73BDA0(object, nullptr);
            sub_748010(selectedMenu, nullptr, 2);
            sub_747F60(GetSelectedMpStatsObject(), nullptr, 2);
        }
    }

    // Second spawn-point handle (this + 0x1410).
    handleEntry = static_cast<uint32_t>(
        CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(selfBytes + 0x1410))));
    eaxValue = handleEntry;
    if (eaxValue != 0)
    {
        eaxValue = CSF_AT(uint32_t, AsBytePtr(handleEntry), 0x18);
        uint8_t* object = eaxValue ? AsBytePtr(eaxValue) - 0x80 : nullptr;
        if (CSF_AT(uint8_t, object, 0x47) != 0)
        {
            sub_73BDA0(object, nullptr);
            sub_748010(selectedMenu, nullptr, 1);
            return sub_747F60(GetSelectedMpStatsObject(), nullptr, 1);
        }
    }
    return static_cast<char>(eaxValue);
}

static char __fastcall MpSpawn_ConfirmSelectedSpawnPoints(S_00553680* self, void* /*edx*/)
{
    uint32_t eaxValue = g_CharacterSelectState8B0694;
    if (eaxValue == 0)
        return static_cast<char>(eaxValue);

    uint8_t* state = AsBytePtr(g_CharacterSelectState8B0694);
    eaxValue = CSF_AT(uint32_t, state, 4);
    if (eaxValue == 0)
        return static_cast<char>(eaxValue);

    uint8_t* menu = AsBytePtr(eaxValue) - 0x308;
    if (menu == nullptr)
        return static_cast<char>(eaxValue);

    char* statusResult = sub_6B29A0(menu, nullptr);
    eaxValue = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(statusResult));
    if (static_cast<uint8_t>(statusResult[6]) != 3)
        return static_cast<char>(eaxValue);

    // Virtual call: slot 2 (offset 0x08) on the object embedded at menu + 0x308.
    uint8_t* confirmObj = menu + 0x308;
    char confirmed = reinterpret_cast<char(__fastcall*)(void*, void*)>(CSF_AT(void**, confirmObj, 0)[2])(confirmObj, nullptr);
    eaxValue = static_cast<uint8_t>(confirmed);
    if (confirmed == 0)
        return confirmed;

    sub_554600(self, nullptr);
    // Tail jump to 0x551860 (inlined as RefreshSpawnPointPanel); its result is this function's result.
    return RefreshSpawnPointPanel(self);
}

HOOK(0x00554DD0, MpSpawn_ConfirmSelectedSpawnPoints, "auto");
