#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Original callees, bound by address.
CSF_FUNC(int, __cdecl, CharActionFsm_GetProviderFrameObject, 0x006C0500, (int a1));
CSF_FUNC(int, __cdecl, SceneLoader_GetObjectAttachment, 0x006B87A0, (int a1));
CSF_FUNC(int, __fastcall, C_6B81C0_GetFirstItem, 0x006B81C0, (void* self, void*));
CSF_FUNC(int, __fastcall, CompareStringSubstring, 0x0042E200,
         (void* string, void* edx, unsigned int position, unsigned int count,
          const char* other, unsigned int otherCount));
CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0,
         (S_0041e5f0* self, void*, int additionalCapacity));

// Vtable of S_0041e5f0, referenced by the original at the GrowArrayCapacity call site.
static auto* const kGrowableIntListVtbl = CSF_VTBL(0x0041E5F0);

// Appends `sceneObject` to the list at `list` when the item's name matches `nameBuf`.
// Returns sceneObject unchanged.
static int __cdecl Scene_AddClumpIfNameMatches(int sceneObject, uint32_t* nameQuery)
{
    int providerFrameObject = CharActionFsm_GetProviderFrameObject(sceneObject);
    int objectAttachment = SceneLoader_GetObjectAttachment(providerFrameObject);
    int firstItem = C_6B81C0_GetFirstItem(reinterpret_cast<void*>(objectAttachment), nullptr);
    if (!firstItem)
        return sceneObject;

    uint32_t itemNameLength = CSF_AT(uint32_t, firstItem, 0x14);
    if (!itemNameLength)
        return sceneObject;

    // Short strings are stored inline at +4; long ones via the pointer at +4.
    const char* queryText;
    if (CSF_AT(uint32_t, nameQuery, 0x18) < 0x10u)
        queryText = reinterpret_cast<const char*>(nameQuery + 1);
    else
        queryText = reinterpret_cast<const char*>(nameQuery[1]);

    uint32_t queryLength = CSF_AT(uint32_t, nameQuery, 0x14);
    int mismatch = CompareStringSubstring(reinterpret_cast<void*>(firstItem), nullptr,
                                          0u, itemNameLength,
                                          queryText, queryLength);
    if (!mismatch)
    {
        S_0041e5f0* clumpList = reinterpret_cast<S_0041e5f0*>(CSF_AT(uint32_t, nameQuery, 0x1C));
        if (clumpList->field_0008 >= clumpList->field_0004)
            S_0041e5f0_GrowArrayCapacity(clumpList, nullptr, clumpList->field_000c);

        uint32_t slotIndex = clumpList->field_0008;
        int* slots = reinterpret_cast<int*>(clumpList->field_0000);
        clumpList->field_0008 = slotIndex + 1;
        slots[slotIndex] = sceneObject;
    }
    return sceneObject;
}

HOOK(0x006CCEE0, Scene_AddClumpIfNameMatches, "auto");
