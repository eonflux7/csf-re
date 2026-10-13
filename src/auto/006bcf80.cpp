#include <cstdint>
#include "globals.h"
#include "hook.h"

// Bindings to original functions, bound by address.
CSF_FUNC(int, __cdecl, GfxClump_GetObjectIndex, 0x006BCE10, (int a1));
CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectAttachment, 0x006B87A0, (int objectIndex));
CSF_FUNC(int, __fastcall, C_6B81C0_GetFirstItem, 0x006B81C0, (void* self, void*));
CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));

// Searches the clump entry at self + 16 * entryIndex for an object whose attachment
// name matches nameToFind (case-insensitive). Returns the matching list index, or -1.
static int __fastcall C_008605d8_FindNamedClump(uint8_t* self, void* /*edx*/, int entryIndex, const char* nameToFind)
{
    uint8_t* entry = self + 16 * entryIndex;
    int matchedIndex = -1;
    uint32_t count = CSF_AT(uint32_t, entry, 0x34);

    for (uint32_t i = 0; i < count; ++i)
    {
        if (matchedIndex != -1)
            break;

        // The index list is re-read on every iteration, as the original does.
        uint32_t* objectIndexes = CSF_AT(uint32_t*, entry, 0x2C);
        int objectHandle = GfxClump_GetObjectIndex(static_cast<int>(objectIndexes[i]));
        if (objectHandle == 0)
        {
            continue;
        }

        int attachment = GfxObjectProps_GetObjectAttachment(objectHandle);
        int firstItem = C_6B81C0_GetFirstItem(
            reinterpret_cast<void*>(static_cast<uintptr_t>(attachment)), nullptr);

        uint8_t* item = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(firstItem));
        const char* itemName;
        if (CSF_AT(uint32_t, item, 0x18) < 0x10u)
            itemName = reinterpret_cast<const char*>(item + 4);   // inline buffer
        else
            itemName = CSF_AT(const char*, item, 4);              // heap pointer

        if (itemName && nameToFind && __stricmp(itemName, nameToFind) == 0)
            matchedIndex = static_cast<int>(i);
    }

    return matchedIndex;
}

HOOK(0x006BCF80, C_008605d8_FindNamedClump, "auto");
