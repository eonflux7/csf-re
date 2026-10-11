#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, CharActionFsm_GetProviderFrameObject, 0x006C0500, (int a1));
CSF_FUNC(int, __cdecl, SceneLoader_GetObjectAttachment, 0x006B87A0, (int a1));
CSF_FUNC(int, __fastcall, C_6B81C0_GetFirstItem, 0x006B81C0, (void* self, void*));
CSF_FUNC(int, __fastcall, CompareStringSubstring, 0x0042E200, (void* string, void* edx, unsigned int position, unsigned int count, const char* other, unsigned int otherCount));

// Looks up the first item of the scene attachment for actor a1, compares its name
// against the name stored in the search-state block a2, and on a match records a1
// in the state block (offset 0x1C). Returns 0 on match, otherwise a1.
static int __cdecl SceneLoader_MatchAttachedNameAndRecord(int a1, void* a2)
{
    int providerFrame = CharActionFsm_GetProviderFrameObject(a1);
    int objectAttachment = SceneLoader_GetObjectAttachment(providerFrame);
    int firstItem = C_6B81C0_GetFirstItem(reinterpret_cast<void*>(objectAttachment), nullptr);
    if (firstItem == 0)
        return a1;

    uint32_t itemNameLength = CSF_AT(uint32_t, reinterpret_cast<void*>(firstItem), 0x14);
    if (itemNameLength == 0)
        return a1;

    // Short names (<16 chars) live inline at offset 4; longer ones are a pointer stored there.
    const char* searchText;
    if (CSF_AT(uint32_t, a2, 0x18) < 0x10u)
        searchText = reinterpret_cast<const char*>(reinterpret_cast<uint8_t*>(a2) + 4);
    else
        searchText = reinterpret_cast<const char*>(CSF_AT(uint32_t, a2, 0x4));

    int result = CompareStringSubstring(reinterpret_cast<void*>(firstItem), nullptr, 0u,
                                        itemNameLength, searchText,
                                        CSF_AT(uint32_t, a2, 0x14));
    if (result != 0)
        return a1;

    CSF_AT(int, a2, 0x1C) = a1;
    return result;
}

HOOK(0x006CCE70, SceneLoader_MatchAttachedNameAndRecord, "auto");
