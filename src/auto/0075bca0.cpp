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
CSF_FUNC(int, __fastcall, S_00692b80_ParseIntegerComponent, 0x00692C40, (S_00692b80* self, void*));
CSF_FUNC(uint8_t*, __fastcall, LookupNamedElement, 0x00693360, (uint8_t* self, void* edx, const char* name));
CSF_FUNC(int, __fastcall, ReadIntegerAttribute, 0x00693970, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0, (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));
CSF_FUNC(unsigned int, __fastcall, SoundDefs_ReserveEntries, 0x0075B140, (void* self, void* edx, unsigned int a2));
CSF_FUNC(void*, __fastcall, SoundDefs_AppendEntry, 0x0075B680, (void* self, void* edx, int* a3));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);
CSF_GLOBAL(const char*, g_pszKey_LISTADATOS, 0x008A8CB0);
CSF_GLOBAL(const char*, g_pszKey_ID_8A8CB4, 0x008A8CB4);
CSF_GLOBAL(const char*, g_pszKey_NOMBRE_8A8CB8, 0x008A8CB8);
CSF_GLOBAL(const char*, g_pszKey_NUM_WAVES_PS2, 0x008A8CE0);

static char __fastcall C_0086c1e0_LoadSoundEntries(
    C_0086c1e0* self, void* /*edx*/, S_00692b80* source)
{
    // The packet also classifies the child-count function address as a vtable.
    (void)CSF_VTBL(0x00692B80);

    auto* list = reinterpret_cast<S_00692b80*>(
        LookupNamedElement(reinterpret_cast<uint8_t*>(source), nullptr,
                           g_pszKey_LISTADATOS));
    const unsigned int childCount =
        static_cast<unsigned int>(S_00692b80_GetChildCount(list, nullptr));

    SoundDefs_ReserveEntries(
        reinterpret_cast<void*>(self->field_0004), nullptr, childCount);

    alignas(8) uint8_t nameBuffer[0x80];
    // Keep the ID and the remainder of the entry contiguous: AppendEntry
    // copies the complete 68-byte entry starting at the ID.
    alignas(8) uint8_t entryBytes[0x50]{};
    auto& temporaryName =
        *reinterpret_cast<C_0082d410*>(entryBytes + 40);

    for (unsigned int index = 0; index < childCount; ++index)
    {
        S_00692b80* component =
            S_00692b80_GetComponent(list, nullptr, index);

        CSF_AT(void*, entryBytes, 8) = nullptr;
        CSF_AT(int*, entryBytes, 12) = nullptr;
        CSF_AT(int, entryBytes, 16) = 0;
        CSF_AT(int, entryBytes, 20) = 100;
        CSF_AT(uint8_t, entryBytes, 32) = 0;
        CSF_AT(uint8_t, entryBytes, 33) = 1;
        CSF_AT(uint8_t, entryBytes, 34) = 0;
        CSF_AT(uint8_t, entryBytes, 35) = 0;
        CSF_AT(uint8_t, entryBytes, 36) = 0;
        CSF_AT(float, entryBytes, 24) = 1000.0f;
        CSF_AT(float, entryBytes, 28) = 1000.0f;
        CSF_AT(uint8_t, entryBytes, 37) = 1;
        CSF_AT(uint8_t, entryBytes, 38) = 1;
        CSF_AT(uint8_t, entryBytes, 39) = 1;
        CSF_AT(unsigned int, entryBytes, 64) = 15;
        CSF_AT(int, entryBytes, 60) = 0;
        CSF_AT(uint8_t, &temporaryName.field_0004, 0) = 0;
        CSF_AT(int, entryBytes, 0) = -1;

        AssignStringRange_429B60(
            &temporaryName, nullptr,
            reinterpret_cast<char*>(&g_byte_82E65C), 0);
        SoundDefs_AppendEntry(
            reinterpret_cast<void*>(self->field_0004), nullptr,
            &CSF_AT(int, entryBytes, 0));

        uint8_t* entry =
            CSF_AT(uint8_t*, reinterpret_cast<void*>(self->field_0004), 0x8) - 68;
        CSF_AT(int, entry, 0) = ReadIntegerAttribute(
            reinterpret_cast<uint8_t*>(component), nullptr,
            const_cast<char*>(g_pszKey_ID_8A8CB4));

        MenuExtras_ReadText(
            reinterpret_cast<uint8_t*>(component), nullptr,
            const_cast<char*>(g_pszKey_NOMBRE_8A8CB8),
            reinterpret_cast<char*>(nameBuffer), 128);

        entry =
            CSF_AT(uint8_t*, reinterpret_cast<void*>(self->field_0004), 0x8) - 68;
        AssignStringRange_429B60(
            reinterpret_cast<C_0082d410*>(entry + 40), nullptr,
            reinterpret_cast<char*>(nameBuffer),
            static_cast<unsigned int>(
                std::strlen(reinterpret_cast<char*>(nameBuffer))));

        auto* waveCountNode = reinterpret_cast<S_00692b80*>(
            LookupNamedElement(reinterpret_cast<uint8_t*>(component), nullptr,
                               g_pszKey_NUM_WAVES_PS2));
        int waveCount = 1;
        if (waveCountNode)
            waveCount = S_00692b80_ParseIntegerComponent(waveCountNode, nullptr);

        entry =
            CSF_AT(uint8_t*, reinterpret_cast<void*>(self->field_0004), 0x8) - 68;
        reinterpret_cast<void(__fastcall*)(
            C_0086c1e0*, void*, S_00692b80*, uint8_t*, int)>(
                self->vtbl[0x18 / 4])(
                    self, nullptr, component, entry, waveCount);

        if (CSF_AT(unsigned int, entryBytes, 64) >= 0x10 &&
            temporaryName.field_0004)
        {
            Mem_FreeDebug(
                reinterpret_cast<void*>(temporaryName.field_0004));
        }

        CSF_AT(unsigned int, entryBytes, 64) = 15;
        CSF_AT(int, entryBytes, 60) = 0;
        CSF_AT(uint8_t, &temporaryName.field_0004, 0) = 0;

        if (CSF_AT(void*, entryBytes, 8))
        {
            MissionScript_DestroyElementRange(
                CSF_AT(int*, entryBytes, 8),
                CSF_AT(int*, entryBytes, 12));
            if (CSF_AT(void*, entryBytes, 8))
                Mem_FreeDebug(CSF_AT(void*, entryBytes, 8));
        }
        CSF_AT(void*, entryBytes, 8) = nullptr;
        CSF_AT(int*, entryBytes, 12) = nullptr;
        CSF_AT(int, entryBytes, 16) = 0;
    }
    return 1;
}

HOOK(0x0075BCA0, C_0086c1e0_LoadSoundEntries, "auto");
