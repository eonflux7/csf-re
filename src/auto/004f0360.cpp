#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, TextDatabase_DestroyEntryRange, 0x004F01A0,
         (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, TextDatabase_ShiftEntries, 0x004F02C0,
         (int a1, int a2, int a3));
CSF_FUNC(int, __fastcall, TextDatabase_ValidateEntry, 0x00770570,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, TextDatabase_ProcessEntry, 0x00770830,
         (void* self, void* edx));
CSF_FUNC(char, __fastcall, TextDatabase_LoadEntryList, 0x00771AF0,
         (void* self, void* edx, uint8_t* a2, int a3));

static char __fastcall C_0083eee4_LoadAndValidateEntries(
    C_0083eee4* self, void* /*edx*/, int data, int options)
{
    self->field_002c = 0;
    self->field_0030 = 0x800000;

    const char result = TextDatabase_LoadEntryList(
        self, nullptr, reinterpret_cast<uint8_t*>(data), options);
    if (!result)
        return result;

    const int processingOptions = static_cast<int>(
        (static_cast<uint32_t>(options) & 0x00FFFFFF) |
        (static_cast<uint32_t>(static_cast<uint8_t>(result)) << 24));

    uint32_t entryCount = self->field_0008;
    if (entryCount)
    {
        entryCount = static_cast<uint32_t>(
            static_cast<int32_t>(self->field_000c - entryCount) / 104);
    }

    const uint32_t lastIndex = entryCount - 1;
    if (static_cast<int32_t>(lastIndex) < 0)
        return result;

    uint32_t offset = 104 * lastIndex;
    uint32_t remaining = lastIndex + 1;
    do
    {
        void* entry = reinterpret_cast<void*>(self->field_0008 + offset);
        if (static_cast<uint8_t>(
                TextDatabase_ValidateEntry(entry, nullptr)) != 0)
        {
            TextDatabase_ProcessEntry(entry, nullptr);
            if (!self->field_002c &&
                (CSF_AT(uint8_t, entry, 0x58) & 2) != 0)
            {
                self->field_002c = 1;
                const float endTime =
                    CSF_AT(float, entry, 0x54) +
                    CSF_AT(float, entry, 0x50);
                std::memcpy(&self->field_0030, &endTime, sizeof(endTime));
            }
        }
        else
        {
            const uint32_t destination = self->field_0008 + offset;
            TextDatabase_ShiftEntries(
                static_cast<int>(destination + 104),
                static_cast<int>(self->field_000c),
                static_cast<int>(destination));

            const uint32_t end = self->field_000c;
            TextDatabase_DestroyEntryRange(
                static_cast<int>(end - 104),
                static_cast<int>(end),
                reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 4),
                processingOptions);

            self->field_000c -= 104;
        }

        offset -= 104;
        --remaining;
    } while (remaining != 0);

    return result;
}

HOOK(0x004F0360, C_0083eee4_LoadAndValidateEntries, "auto");
