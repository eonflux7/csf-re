#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_004f2610_AddThumbnailObject, 0x004F2610, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Text_ConvertToDisplayEncoding, 0x00692580, (int a1, int a2));
CSF_FUNC(int, __cdecl, MenuMessageBox_TestEntryAgainstFlag, 0x00695500, (uint16_t* entry, char* flagByte));
CSF_FUNC(std::uint32_t*, __fastcall, MenuProgressInput_AssignText, 0x00696F90, (std::uint32_t* self, void* edx, std::int16_t* text));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

// The packet also labels this direct-call address as a vtable reference.
[[maybe_unused]] static const auto g_ThumbnailMethodReference = CSF_VTBL(0x004F2610);

static int __fastcall S_004f2610_UpdateThumbnailObjects(
    S_004f2610* self, void* /*edx*/)
{
    std::uint32_t entriesAddress = self->field_0008;
    if (!entriesAddress)
        return 0;

    int result = static_cast<std::int32_t>(
        self->field_000c - entriesAddress) / 124;
    if (!result)
        return result;

    entriesAddress = self->field_0008;
    result = entriesAddress
        ? static_cast<std::int32_t>(self->field_000c - entriesAddress) / 124
        : 0;

    auto* firstEntry = reinterpret_cast<std::uint8_t*>(self->field_0008);
    int groupFirst = CSF_AT(int, firstEntry, 0x6C);
    int groupSecond = CSF_AT(int, firstEntry, 0x70);
    std::uint32_t addedInGroup = 0;

    if (!result)
        return result;

    std::uint32_t remaining = static_cast<std::uint32_t>(result);
    std::uint32_t entryOffset = 0;

    do
    {
        auto* entry = reinterpret_cast<std::uint8_t*>(
            self->field_0008 + entryOffset);

        if (CSF_AT(int, entry, 0x6C) != groupFirst ||
            CSF_AT(int, entry, 0x70) != groupSecond)
        {
            groupFirst = CSF_AT(int, entry, 0x6C);
            groupSecond = CSF_AT(int, entry, 0x70);
            addedInGroup = 0;
        }

        if (addedInGroup < self->field_0054 &&
            CSF_AT(std::uint8_t, entry, 0x74))
        {
            alignas(8) std::uint8_t textBytes[0x30]{};
            CSF_AT(std::uint32_t, textBytes, 0x18) = 7;
            CSF_AT(std::uint32_t, textBytes, 0x14) = 0;
            CSF_AT(std::uint16_t, textBytes, 0x04) = 0;

            if (CSF_AT(std::uint32_t, entry, 0x30))
            {
                S_004f2610_AddThumbnailObject(
                    self, nullptr,
                    reinterpret_cast<int>(entry),
                    reinterpret_cast<int>(entry + 0x1C));
                ++addedInGroup;
            }

            const int sourceText =
                CSF_AT(std::uint32_t, entry, 0x18) < 0x10u
                ? reinterpret_cast<int>(entry + 4)
                : CSF_AT(int, entry, 0x04);

            auto* displayText = reinterpret_cast<std::int16_t*>(
                Text_ConvertToDisplayEncoding(
                    sourceText, CSF_AT(int, self, 0x20)));

            if (displayText &&
                MenuMessageBox_TestEntryAgainstFlag(
                    reinterpret_cast<uint16_t*>(displayText),
                    reinterpret_cast<char*>(&g_byte_82E65C)))
            {
                MenuProgressInput_AssignText(
                    reinterpret_cast<std::uint32_t*>(textBytes),
                    nullptr, displayText);
                S_004f2610_AddThumbnailObject(
                    self, nullptr,
                    reinterpret_cast<int>(entry),
                    reinterpret_cast<int>(textBytes));
                ++addedInGroup;
            }

            if (CSF_AT(std::uint32_t, textBytes, 0x18) >= 8)
            {
                void* block = CSF_AT(void*, textBytes, 0x04);
                if (block)
                    Mem_FreeDebug(block);
            }

            CSF_AT(std::uint32_t, textBytes, 0x18) = 7;
            CSF_AT(std::uint32_t, textBytes, 0x14) = 0;
            CSF_AT(std::uint16_t, textBytes, 0x04) = 0;
        }

        entryOffset += 124;
        --remaining;
    } while (remaining != 0);

    return 0;
}

HOOK(0x004F2930, S_004f2610_UpdateThumbnailObjects, "auto");
