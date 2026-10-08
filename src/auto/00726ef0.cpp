#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, C_008655e8_FindEntryByName, 0x00726E30,
         (C_008655e8* self, void* edx, char* name));
CSF_FUNC(void, __fastcall, AddEntryReference, 0x006923B0,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, ReleaseEntryReference, 0x006923C0,
         (void* self, void* edx));
CSF_FUNC(void*, __cdecl, AllocateEntryMemory, 0x006910F0,
         (std::size_t size));
CSF_FUNC(void*, __fastcall, ConstructFontEntry, 0x0073B450,
         (void* self, void* edx, C_008655e8* owner));
CSF_FUNC(const char*, __cdecl, GetFileNameComponent, 0x00694A60,
         (const char* name));
CSF_FUNC(int, __cdecl, CompareNamesIgnoringCase, 0x007B0C41,
         (const char* left, const char* right));
CSF_FUNC(void*, __fastcall, GrowEntryArray, 0x0041E5F0,
         (void* self, void* edx, int growth));

static void* __fastcall C_008655e8_GetOrLoadFont(
    C_008655e8* self, void* /*edx*/, char* name)
{
    void* entry = C_008655e8_FindEntryByName(self, nullptr, name);
    if (entry)
    {
        AddEntryReference(entry, nullptr);
        return entry;
    }

    void* storage = AllocateEntryMemory(0x60);
    entry = storage ? ConstructFontEntry(storage, nullptr, self) : nullptr;

    reinterpret_cast<void (__fastcall*)(void*, void*, char*)>(
        CSF_AT(void**, entry, 0)[2])(entry, nullptr, name);

    const std::uint8_t loaded =
        reinterpret_cast<std::uint8_t (__fastcall*)(void*, void*)>(
            CSF_AT(void**, entry, 0)[1])(entry, nullptr);

    if (!loaded)
    {
        ReleaseEntryReference(entry, nullptr);
        return nullptr;
    }

    if (name)
    {
        const char* fileName = GetFileNameComponent(name);
        if (fileName &&
            CompareNamesIgnoringCase(
                fileName, CSF_STR(0x008655D8, "MGAME_01.FNT")) == 0)
        {
            reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, entry, 0)[12])(entry, nullptr, 1);
        }
    }

    if (self->field_003c >= self->field_0038)
    {
        GrowEntryArray(
            reinterpret_cast<std::uint8_t*>(self) + 0x34,
            nullptr, CSF_AT(int, self, 0x40));
    }

    const std::uint32_t count = self->field_003c;
    void** destination = CSF_AT(void**, self, 0x34) + count;
    self->field_003c = count + 1;
    *destination = entry;
    return entry;
}

HOOK(0x00726EF0, C_008655e8_GetOrLoadFont, "auto");
