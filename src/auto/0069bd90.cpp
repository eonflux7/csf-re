#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, TextDatabase_InitializeEntryFromPath, 0x00699C80, (void* self, void* edx, char* a3));
CSF_FUNC(int, __cdecl, TextDatabase_GetMaskBucketIndex, 0x0069B730, (unsigned int a1));
CSF_FUNC(void*, __fastcall, CTextDatabase_StorePathForMask, 0x0069BB10, (void* self, void* edx, char* a2, unsigned int a3));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

static void __fastcall CTextDatabase_SetTextDatabasePath(
    CTextDatabase* self, void* /*edx*/, unsigned int mask, char* path)
{
    if (!mask)
        return;

    CTextDatabase_StorePathForMask(self, nullptr, path, mask);

    unsigned int bucketIndex = 0;
    for (unsigned int remaining = mask >> 1; remaining; remaining >>= 1)
        ++bucketIndex;

    auto* savedEntries = reinterpret_cast<uint8_t*>(self->field_0008);
    if (CSF_AT(void*, savedEntries + 32 * bucketIndex, 0))
        return;

    // The original compares exactly one byte, not the entire string.
    if (static_cast<uint8_t>(path[0]) == g_byte_82E65C)
        return;

    const uint32_t entryOffset =
        static_cast<uint32_t>(TextDatabase_GetMaskBucketIndex(mask)) * 32;

    void* createdEntry =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, self, 0)[0xC / 4])(self, nullptr);

    CSF_AT(void*, savedEntries + entryOffset, 0) = createdEntry;

    auto* entryRecord =
        reinterpret_cast<uint8_t*>(self->field_0008) + entryOffset;
    void* entry = CSF_AT(void*, entryRecord, 0);
    if (!entry)
        return;

    char* storedPath;
    if (CSF_AT(uint32_t, entryRecord, 0x1C) < 0x10u)
        storedPath = reinterpret_cast<char*>(entryRecord + 8);
    else
        storedPath = CSF_AT(char*, entryRecord, 8);

    TextDatabase_InitializeEntryFromPath(entry, nullptr, storedPath);
}

HOOK(0x0069BD90, CTextDatabase_SetTextDatabasePath, "auto");
