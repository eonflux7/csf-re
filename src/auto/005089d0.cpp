#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, S_00508240_ReserveEntries, 0x00508240,
         (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, S_005080e0_AddQuotedEntry, 0x00508810,
         (void* self, void* edx, void* a2));
CSF_FUNC(std::uint32_t, __cdecl, CloseSceneFile, 0x00697980, (void* file));
CSF_FUNC(void*, __cdecl, OpenSceneFile, 0x00698350,
         (const char* path, const char* mode, int flags));

static char __fastcall S_005080e0_LoadQuotedEntries(
    S_005080e0* self, void* /*edx*/, const char* path)
{
    alignas(8) std::uint8_t entryBytes[0xCC];
    std::uint16_t character = static_cast<std::uint16_t>(
        reinterpret_cast<std::uintptr_t>(path) >> 16);
    char success = 0;

    void* file = OpenSceneFile(path, CSF_STR(0x0083E130, "r"), 0);
    if (!file)
        return success;

    success = 1;
    const std::uint32_t capacity = CSF_AT(std::uint32_t, self, 0x58);
    if (capacity < 0x640)
    {
        (void)CSF_VTBL(0x00508240);
        S_00508240_ReserveEntries(
            reinterpret_cast<std::uint8_t*>(self) + 0x54,
            nullptr, 1600 - capacity);
    }

    do
    {
        if (((bool(__fastcall*)(void*, void*))
                 CSF_AT(void**, file, 0)[0x18 / 4])(file, nullptr))
            break;

        int length = 0;
        for (;;)
        {
            const std::uint32_t bytesRead =
                ((std::uint32_t(__fastcall*)(void*, void*, void*, int))
                     CSF_AT(void**, file, 0)[8 / 4])(
                    file, nullptr, &character, 2);
            const bool atEnd =
                ((bool(__fastcall*)(void*, void*))
                     CSF_AT(void**, file, 0)[0x18 / 4])(file, nullptr);

            success = bytesRead == 2;
            if (atEnd || bytesRead < 2)
                break;

            if (character == 34)
            {
                for (;;)
                {
                    const std::uint32_t quotedBytesRead =
                        ((std::uint32_t(__fastcall*)(void*, void*, void*, int))
                             CSF_AT(void**, file, 0)[8 / 4])(
                            file, nullptr, &character, 2);
                    const bool quotedAtEnd =
                        ((bool(__fastcall*)(void*, void*))
                             CSF_AT(void**, file, 0)[0x18 / 4])(file, nullptr);

                    success = quotedBytesRead == 2;
                    if (quotedAtEnd || character == 34 || quotedBytesRead < 2)
                        break;

                    CSF_AT(std::uint16_t, entryBytes, 2 * length) = character;
                    ++length;
                }
                break;
            }
        }

        CSF_AT(std::uint16_t, entryBytes, 2 * length) = 0;
        if (!success)
            break;

        success = S_005080e0_AddQuotedEntry(self, nullptr, entryBytes);
    } while (success);

    CloseSceneFile(file);
    return success;
}

HOOK(0x005089D0, S_005080e0_LoadQuotedEntries, "auto");
