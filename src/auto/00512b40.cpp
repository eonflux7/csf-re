#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __fastcall, CompareStringSubstring, 0x0042E200, (void* string, void* edx, unsigned int position, unsigned int count, const char* other, unsigned int otherCount));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(char, __fastcall, S_004ff0f0_SetRecordNameAndLoad, 0x00501A00, (S_004ff0f0* self, void* , C_0082d410* name));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(char, __cdecl, GetOptionsByteAt2, 0x006972F0, ());
CSF_FUNC(char, __cdecl, SceneLoader_SetFileOption, 0x00698600, (char a1));

CSF_GLOBAL(std::uint8_t, g_byte_82E65C, 0x0082E65C);

static char __fastcall CSaveGameManager_LoadSelectedSaveGame(
    CSaveGameManager* self, void* /*edx*/)
{
    const char savedOption = GetOptionsByteAt2();
    if (savedOption)
        SceneLoader_SetFileOption(0);

    alignas(8) std::uint8_t selectedNameBytes[0x20];
    CSF_AT(std::uint32_t, selectedNameBytes, 0x18) = 15;
    CSF_AT(std::uint32_t, selectedNameBytes, 0x14) = 0;
    CSF_AT(char, selectedNameBytes, 0x04) = 0;

    self->vSlot50(selectedNameBytes);

    const bool hasName = CompareStringSubstring(
        selectedNameBytes, nullptr, 0,
        CSF_AT(unsigned int, selectedNameBytes, 0x14),
        reinterpret_cast<const char*>(&g_byte_82E65C), 0) != 0;

    if (hasName)
    {
        AssignStringSubstring(
            reinterpret_cast<std::uint8_t*>(self) + 0x94,
            nullptr, selectedNameBytes, 0, -1);

        char* text = CSF_AT(char*, selectedNameBytes, 0x04);
        if (CSF_AT(std::uint32_t, selectedNameBytes, 0x18) < 0x10)
            text = reinterpret_cast<char*>(selectedNameBytes + 0x04);

        alignas(8) std::uint8_t recordNameBytes[0x1C];
        C_0082d410& recordName =
            *reinterpret_cast<C_0082d410*>(recordNameBytes);

        CSF_AT(std::uint32_t, recordNameBytes, 0x18) = 15;
        CSF_AT(std::uint32_t, recordNameBytes, 0x14) = 0;
        reinterpret_cast<char*>(&recordName.field_0004)[0] = 0;

        AssignStringRange_429B60(
            &recordName, nullptr, text,
            static_cast<unsigned int>(std::strlen(text)));

        S_004ff0f0* records =
            reinterpret_cast<S_004ff0f0*>(GetGlobalValue_8B0568());
        S_004ff0f0_SetRecordNameAndLoad(records, nullptr, &recordName);

        if (CSF_AT(std::uint32_t, recordNameBytes, 0x18) >= 0x10 &&
            recordName.field_0004)
        {
            Mem_FreeDebug(reinterpret_cast<void*>(recordName.field_0004));
        }
    }

    SceneLoader_SetFileOption(savedOption);

    if (CSF_AT(std::uint32_t, selectedNameBytes, 0x18) >= 0x10 &&
        CSF_AT(void*, selectedNameBytes, 0x04))
    {
        Mem_FreeDebug(CSF_AT(void*, selectedNameBytes, 0x04));
    }

    return hasName ? 1 : 0;
}

HOOK(0x00512B40, CSaveGameManager_LoadSelectedSaveGame, "auto");
