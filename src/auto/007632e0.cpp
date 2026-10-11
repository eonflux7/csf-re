#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, CharacterDefs_ReleaseRecord, 0x00511040,
         (void* self, void* edx));
CSF_FUNC(void**, __fastcall, CharacterDefs_EraseRecordRange, 0x00762E50,
         (void* self, void* edx, void** a2, char* a3, int a4));
CSF_FUNC(int, __fastcall, CharacterDefs_InsertRecordCopies, 0x00762F80,
         (void* self, void* edx, char* a1, unsigned int a3, int arg8));

struct CharacterDefsRecordValue
{
    std::uint8_t bytes[556];
};

static int __fastcall CharacterDefs_ResizeRecords(
    void* self, void* /*edx*/, unsigned int requestedCount,
    CharacterDefsRecordValue value)
{
    const std::uint32_t begin = CSF_AT(std::uint32_t, self, 0x4);
    unsigned int currentCount = 0;
    if (begin)
    {
        currentCount = static_cast<unsigned int>(
            static_cast<std::int32_t>(
                CSF_AT(std::uint32_t, self, 0x8) - begin) / 556);
    }

    if (currentCount >= requestedCount)
    {
        if (begin)
        {
            const std::uint32_t end = CSF_AT(std::uint32_t, self, 0x8);
            const unsigned int count = static_cast<unsigned int>(
                static_cast<std::int32_t>(end - begin) / 556);

            if (requestedCount < count)
            {
                CharacterDefs_EraseRecordRange(
                    self, nullptr,
                    reinterpret_cast<void**>(&requestedCount),
                    reinterpret_cast<char*>(begin + 556 * requestedCount),
                    static_cast<int>(end));
            }
        }
    }
    else
    {
        unsigned int count = 0;
        if (begin)
        {
            count = static_cast<unsigned int>(
                static_cast<std::int32_t>(
                    CSF_AT(std::uint32_t, self, 0x8) - begin) / 556);
        }

        CharacterDefs_InsertRecordCopies(
            self, nullptr,
            reinterpret_cast<char*>(CSF_AT(std::uint32_t, self, 0x8)),
            requestedCount - count,
            reinterpret_cast<int>(&value));
    }

    return CharacterDefs_ReleaseRecord(&value, nullptr);
}

HOOK(0x007632E0, CharacterDefs_ResizeRecords, "auto");
