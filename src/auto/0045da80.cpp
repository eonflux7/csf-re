#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(void*, __fastcall, DefaultDestination_Get, 0x004FE4A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetDestinationEntryCount, 0x0044BB80, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetDestinationEntryAt, 0x0044BBA0, (void* self, void* edx, int index));

static uint8_t* AddressToBytePtr(int address) {
    return reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(static_cast<uint32_t>(address)));
}

// Scans the destination's entries for one whose key (+0x38) matches the float value
// read through a3. On the first match, stores (entry.+0x3C == 1) into the byte at
// (*(a1+4)) + 1 and returns that flag. If no entry matches, returns the low byte of
// the entry count.
static char __stdcall SetFlagForMatchingDestinationEntry(int sourceRef, int /*a2*/, int valueRef) {
    uint8_t* destination = static_cast<uint8_t*>(
        DefaultDestination_Get(AddressToBytePtr(GetGlobalValue_8B0568()), nullptr));

    uint8_t* valueHolder = CSF_AT(uint8_t*, AddressToBytePtr(valueRef), 0);
    uint8_t* valueNode = CSF_AT(uint8_t*, valueHolder, 4);
    float valueFloat = CSF_AT(float, valueNode, 4);
    int targetKey = static_cast<int>(static_cast<uint64_t>(valueFloat));

    uint8_t* sourceObject = AddressToBytePtr(sourceRef);
    uint32_t index = 0;
    while (true) {
        int entryCount = GetDestinationEntryCount(destination, nullptr);
        if (index >= static_cast<uint32_t>(entryCount)) {
            return static_cast<char>(entryCount);
        }

        uint8_t* entry = AddressToBytePtr(
            GetDestinationEntryAt(destination, nullptr, static_cast<int>(index)));
        if (CSF_AT(int, entry, 0x38) == targetKey) {
            int flag = CSF_AT(int, entry, 0x3C) == 1;
            uint8_t* flagTarget = CSF_AT(uint8_t*, sourceObject, 4);
            CSF_AT(uint8_t, flagTarget, 1) = static_cast<uint8_t>(flag);
            return static_cast<char>(flag);
        }
        ++index;
    }
}

HOOK(0x0045DA80, SetFlagForMatchingDestinationEntry, "auto");
