#include <cstdint>
#include "globals.h"
#include "hook.h"

// Locates (or creates) the slot for the given index key and returns its address as an int.
CSF_FUNC(int, __fastcall, SceneLoader_LookupIndexSlot, 0x006B7E80, (void* self, void* edx, int* a2));

// Stores the value pointed to by a3 into the indexed slot selected by a2.
// The slot lookup runs on the sub-object embedded at +4 of this (the original's `add ecx, 4`).
static int* __fastcall SceneLoader_SetIndexedColor(void* self, void* /*edx*/, int a2, int* a3)
{
    int colorValue = *a3;
    void* indexSubObject = reinterpret_cast<uint8_t*>(self) + 4;
    int* slotResult = reinterpret_cast<int*>(SceneLoader_LookupIndexSlot(indexSubObject, nullptr, &a2));
    *slotResult = colorValue;
    return slotResult;
}

HOOK(0x006B7F70, SceneLoader_SetIndexedColor, "auto");
