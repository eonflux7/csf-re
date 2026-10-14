#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, LookupSlotIndex, 0x007686E0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetSlotEntry, 0x00768740, (void* self, void* edx, int a2));

// Zeroes the 3-float vector at a1->+4, marks two indices invalid (-1),
// looks up the slot index for a key taken from a3, and if found copies the
// slot entry's vector into a1->+4. Returns the slot vector pointer, or -1.
static int32_t* __stdcall ResetVectorAndLoadSlotPosition(void* a1, void* /*a2*/, void* a3)
{
    void* object = CSF_AT(void*, a1, 4);
    CSF_AT(float, object, 4) = g_ZeroVectorX;
    CSF_AT(float, object, 8) = g_ZeroVectorY;
    CSF_AT(float, object, 12) = g_ZeroVectorZ;

    CSF_AT(int32_t, CSF_AT(void*, a1, 4), 0x10) = -1;
    CSF_AT(int32_t, CSF_AT(void*, a1, 4), 0x14) = -1;

    // Key comes from the object referenced by a3: [[a3] + 4] + 4.
    void* keyOwner = CSF_AT(void*, CSF_AT(void*, a3, 0), 4);

    void* context = reinterpret_cast<void*>(static_cast<intptr_t>(GetValue8B0554()));
    int32_t key = CSF_AT(int32_t, keyOwner, 4);
    void* lookupSelf = CSF_AT(void*, context, 0x1F90);
    int slotIndex = LookupSlotIndex(lookupSelf, nullptr, key);

    int32_t* result = reinterpret_cast<int32_t*>(static_cast<intptr_t>(slotIndex));
    if (slotIndex != -1) {
        void* context2 = reinterpret_cast<void*>(static_cast<intptr_t>(GetValue8B0554()));
        void* slotSelf = CSF_AT(void*, context2, 0x1F90);
        uint8_t* slotEntry = reinterpret_cast<uint8_t*>(
            static_cast<intptr_t>(GetSlotEntry(slotSelf, nullptr, slotIndex)));

        int32_t* sourceVector = reinterpret_cast<int32_t*>(slotEntry + 4);
        void* destination = CSF_AT(void*, a1, 4);
        CSF_AT(int32_t, destination, 4) = sourceVector[0];
        CSF_AT(int32_t, destination, 8) = sourceVector[1];
        CSF_AT(int32_t, destination, 12) = sourceVector[2];

        result = sourceVector;
    }
    return result;
}

HOOK(0x0045C820, ResetVectorAndLoadSlotPosition, "auto");
