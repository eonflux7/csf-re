#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __cdecl, MissionScript_DestroyElementRange, 0x00469760, (int* first, int* last));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __fastcall, S_00692b80_GetChildCount, 0x00692B80, (S_00692b80* self, void*));
CSF_FUNC(S_00692b80*, __fastcall, S_00692b80_GetComponent, 0x00692BD0, (S_00692b80* self, void* , unsigned int index));
CSF_FUNC(uint8_t*, __fastcall, LookupNamedElement, 0x00693360, (uint8_t* self, void* edx, const char* name));
CSF_FUNC(int, __fastcall, ReadIntegerAttribute, 0x00693970, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0, (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));
CSF_FUNC(uint8_t*, __stdcall, SoundDefs_ReadSoundAttributes, 0x0075A720, (void* a1, int a2));
CSF_FUNC(unsigned int, __fastcall, SoundDefs_ReserveEntries, 0x0075B140, (void* self, void* edx, unsigned int a2));
CSF_FUNC(void*, __fastcall, SoundDefs_AppendEntry, 0x0075B680, (void* self, void* edx, int* a3));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);
CSF_GLOBAL(const char*, g_pszKey_LISTADATOS, 0x008A8CB0);
CSF_GLOBAL(const char*, g_pszKey_ID_8A8CB4, 0x008A8CB4);
CSF_GLOBAL(const char*, g_pszKey_NOMBRE_8A8CB8, 0x008A8CB8);

static char __fastcall SoundDefs_LoadSoundData(
    int* self, void* /*edx*/, S_00692b80* root)
{
    (void)CSF_VTBL(0x00692B80);

    auto* entries = reinterpret_cast<S_00692b80*>(
        LookupNamedElement(reinterpret_cast<uint8_t*>(root), nullptr,
                           g_pszKey_LISTADATOS));
    const unsigned int count =
        static_cast<unsigned int>(S_00692b80_GetChildCount(entries, nullptr));

    SoundDefs_ReserveEntries(CSF_AT(void*, self, 0x04), nullptr, count);

    for (unsigned int index = 0; index < count; ++index)
    {
        S_00692b80* node = S_00692b80_GetComponent(entries, nullptr, index);
        char nameBuffer[128];

        // Includes the contiguous default entry beginning at ebp-0x50.
        alignas(8) uint8_t entryBytes[0x50] = {};
        C_0082d410& entryName =
            *reinterpret_cast<C_0082d410*>(entryBytes + 0x28);

        CSF_AT(int, entryBytes, 0x00) = -1;
        CSF_AT(void*, entryBytes, 0x08) = nullptr;
        CSF_AT(int, entryBytes, 0x0C) = 0;
        CSF_AT(int, entryBytes, 0x10) = 0;
        CSF_AT(int, entryBytes, 0x14) = 100;
        CSF_AT(int, entryBytes, 0x18) = 1148846080;
        CSF_AT(int, entryBytes, 0x1C) = 1148846080;
        CSF_AT(uint8_t, entryBytes, 0x20) = 0;
        CSF_AT(uint8_t, entryBytes, 0x21) = 1;
        CSF_AT(uint8_t, entryBytes, 0x22) = 0;
        CSF_AT(uint8_t, entryBytes, 0x23) = 0;
        CSF_AT(uint8_t, entryBytes, 0x24) = 0;
        CSF_AT(uint8_t, entryBytes, 0x25) = 1;
        CSF_AT(uint8_t, entryBytes, 0x26) = 1;
        CSF_AT(uint8_t, entryBytes, 0x27) = 1;
        CSF_AT(unsigned int, entryBytes, 0x40) = 15;
        CSF_AT(int, entryBytes, 0x3C) = 0;
        reinterpret_cast<uint8_t*>(&entryName.field_0004)[0] = 0;

        AssignStringRange_429B60(
            &entryName, nullptr, reinterpret_cast<char*>(&g_byte_82E65C), 0);
        SoundDefs_AppendEntry(
            CSF_AT(void*, self, 0x04), nullptr,
            &CSF_AT(int, entryBytes, 0x00));

        auto* destination =
            CSF_AT(uint8_t*, CSF_AT(void*, self, 0x04), 0x08) - 68;
        const int id = ReadIntegerAttribute(
            reinterpret_cast<uint8_t*>(node), nullptr,
            const_cast<char*>(g_pszKey_ID_8A8CB4));
        CSF_AT(int, destination, 0x00) = id;

        MenuExtras_ReadText(
            reinterpret_cast<uint8_t*>(node), nullptr,
            const_cast<char*>(g_pszKey_NOMBRE_8A8CB8), nameBuffer, 128);

        destination =
            CSF_AT(uint8_t*, CSF_AT(void*, self, 0x04), 0x08) - 68;
        AssignStringRange_429B60(
            destination + 40, nullptr, nameBuffer,
            static_cast<unsigned int>(std::strlen(nameBuffer)));

        destination =
            CSF_AT(uint8_t*, CSF_AT(void*, self, 0x04), 0x08) - 68;
        SoundDefs_ReadSoundAttributes(
            node, static_cast<int>(reinterpret_cast<std::intptr_t>(destination)));

        if (CSF_AT(unsigned int, entryBytes, 0x40) >= 0x10 &&
            entryName.field_0004 != 0)
        {
            Mem_FreeDebug(reinterpret_cast<void*>(entryName.field_0004));
        }

        CSF_AT(unsigned int, entryBytes, 0x40) = 15;
        CSF_AT(int, entryBytes, 0x3C) = 0;
        reinterpret_cast<uint8_t*>(&entryName.field_0004)[0] = 0;

        if (CSF_AT(void*, entryBytes, 0x08))
        {
            MissionScript_DestroyElementRange(
                CSF_AT(int*, entryBytes, 0x08),
                CSF_AT(int*, entryBytes, 0x0C));
            if (CSF_AT(void*, entryBytes, 0x08))
                Mem_FreeDebug(CSF_AT(void*, entryBytes, 0x08));
        }

        CSF_AT(void*, entryBytes, 0x08) = nullptr;
        CSF_AT(int, entryBytes, 0x0C) = 0;
        CSF_AT(int, entryBytes, 0x10) = 0;
    }

    return 1;
}

HOOK(0x0075B760, SoundDefs_LoadSoundData, "auto");
