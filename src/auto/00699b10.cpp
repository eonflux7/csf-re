#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int16_t, __cdecl, CoreVariantValues_CopyWideString, 0x00695680, (char* dest, char* src));
CSF_FUNC(char, __fastcall, S_00698c10_ReadEntryKey, 0x00698D10, (void* self, void* edx, uint8_t* a2));
CSF_FUNC(int, __cdecl, MenuExtras_ParseDescriptionId, 0x00698D90, (const char* text));
CSF_FUNC(char, __fastcall, S_00698c10_ReadEntryDetails, 0x00698F00, (void* self, void* edx, void* a2));
CSF_FUNC(bool, __fastcall, S_00698c10_ReadEntryText, 0x006990B0, (void* self, void* edx, void* a2));
CSF_FUNC(void*, __fastcall, S_00699370_InsertDescription, 0x006998F0, (void* self, void* edx, uint32_t* a3));
CSF_FUNC(int, __cdecl, _tolower, 0x007B1905, (int C));

static char __fastcall S_00698c10_ReadDescriptionEntry(
    S_00698c10* self, void* /*edx*/, S_00699370* table)
{
    alignas(8) uint8_t textBytes[0x1C];
    alignas(8) uint8_t detailsBytes[0x1C];
    alignas(8) uint8_t entryBytes[0x8];
    // Reserve the full frame allowance and give unspecified trailing bytes
    // a deterministic zero value rather than scanning unrelated local storage.
    alignas(8) uint8_t keyBytes[0x48] = {};

    CSF_AT(uint32_t, textBytes, 0x18) = 7;
    CSF_AT(uint32_t, detailsBytes, 0x18) = 7;
    CSF_AT(int32_t, textBytes, 0x14) = 0;
    CSF_AT(uint16_t, textBytes, 0x4) = 0;
    CSF_AT(int32_t, detailsBytes, 0x14) = 0;
    CSF_AT(uint16_t, detailsBytes, 0x4) = 0;

    char result = S_00698c10_ReadEntryKey(self, nullptr, keyBytes);
    if (result)
    {
        char* key = reinterpret_cast<char*>(keyBytes);
        unsigned int length = 0;
        while (key[length])
            ++length;

        AssignStringRange_429B60(
            reinterpret_cast<uint8_t*>(self) + 0x20,
            nullptr, key, length);

        result = S_00698c10_ReadEntryText(self, nullptr, textBytes);
        if (result)
        {
            S_00698c10_ReadEntryDetails(self, nullptr, detailsBytes);

            if (*key)
            {
                char* character = key;
                do
                {
                    *character = static_cast<char>(
                        _tolower(static_cast<signed char>(*character)));
                } while (*++character);
            }

            const int descriptionId = MenuExtras_ParseDescriptionId(key);
            const uint32_t begin = table->field_0004;
            const uint32_t count = begin
                ? static_cast<uint32_t>(
                    static_cast<int32_t>(table->field_0008 - begin) >> 3)
                : 0;

            uint32_t index = 0;
            if (count)
            {
                const uint32_t* id = reinterpret_cast<const uint32_t*>(begin + 4);
                while (*id != static_cast<uint32_t>(descriptionId))
                {
                    ++index;
                    id += 2;
                    if (index >= count)
                        break;
                }
            }

            if (index >= count || index == UINT32_MAX)
            {
                const int allocation = CoreMemory_AllocUntagged(
                    2 * CSF_AT(uint32_t, textBytes, 0x14) + 2);
                CSF_AT(uint32_t, entryBytes, 0) =
                    static_cast<uint32_t>(allocation);

                char* source;
                if (CSF_AT(uint32_t, textBytes, 0x18) < 8)
                    source = reinterpret_cast<char*>(textBytes + 0x4);
                else
                    source = CSF_AT(char*, textBytes, 0x4);

                CoreVariantValues_CopyWideString(
                    reinterpret_cast<char*>(allocation), source);
                CSF_AT(int32_t, entryBytes, 0x4) = descriptionId;

                S_00699370_InsertDescription(
                    table, nullptr, reinterpret_cast<uint32_t*>(entryBytes));

                reinterpret_cast<void (__fastcall*)(
                    S_00698c10*, void*, void*, int)>(
                    reinterpret_cast<void**>(self->field_0000)[1])(
                        self, nullptr, detailsBytes, descriptionId);
            }
        }

        if (CSF_AT(uint32_t, detailsBytes, 0x18) >= 8 &&
            CSF_AT(void*, detailsBytes, 0x4))
        {
            Mem_FreeDebug(CSF_AT(void*, detailsBytes, 0x4));
        }
    }

    CSF_AT(uint32_t, detailsBytes, 0x18) = 7;
    CSF_AT(int32_t, detailsBytes, 0x14) = 0;
    CSF_AT(uint16_t, detailsBytes, 0x4) = 0;

    if (CSF_AT(uint32_t, textBytes, 0x18) >= 8 &&
        CSF_AT(void*, textBytes, 0x4))
    {
        Mem_FreeDebug(CSF_AT(void*, textBytes, 0x4));
    }

    return result;
}

HOOK(0x00699B10, S_00698c10_ReadDescriptionEntry, "auto");
