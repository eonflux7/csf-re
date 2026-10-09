#include <cstdint>
#include "globals.h"
#include "hook.h"

// Vector constants used when the lookup fails.
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, FindVectorTrackIndex, 0x0075CE20, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetVectorTrackRecord, 0x0075CF00, (void* self, void* edx, int a2));

static uint8_t* AddressToBytes(int address) {
    return reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(static_cast<uint32_t>(address)));
}

static int __stdcall ResolveVectorSampleFromTable(int outputHolderAddr, int /*unused*/, uint32_t* requestArgs)
{
    // v3 = *(*a3 + 4)
    uint8_t* requestHeader = AddressToBytes(static_cast<int>(requestArgs[0]));
    uint8_t* requestPayload = AddressToBytes(static_cast<int>(CSF_AT(uint32_t, requestHeader, 4)));

    // v5 = FindVectorTrackIndex(*(GetValue8B0554() + 8152), *(v3 + 4))
    const int gameStateAddr = GetValue8B0554();
    void* trackManager = CSF_AT(void*, AddressToBytes(gameStateAddr), 8152);
    const int trackKey = static_cast<int>(CSF_AT(uint32_t, requestPayload, 4));
    const int sampleIndex = FindVectorTrackIndex(trackManager, nullptr, trackKey);

    if (sampleIndex == -1) {
        // Not found: zero vector, id and value set to -1; result is the output object pointer.
        uint32_t outputObjectAddr = CSF_AT(uint32_t, AddressToBytes(outputHolderAddr), 4);
        uint8_t* outputObject = AddressToBytes(static_cast<int>(outputObjectAddr));
        CSF_AT(float, outputObject, 4) = g_ZeroVectorX;
        CSF_AT(float, outputObject, 8) = g_ZeroVectorY;
        CSF_AT(float, outputObject, 12) = g_ZeroVectorZ;
        CSF_AT(int, outputObject, 16) = -1;
        outputObjectAddr = CSF_AT(uint32_t, AddressToBytes(outputHolderAddr), 4);
        CSF_AT(int, AddressToBytes(static_cast<int>(outputObjectAddr)), 20) = -1;
        return static_cast<int>(outputObjectAddr);
    }

    // v7 = GetVectorTrackRecord(*(GetValue8B0554() + 8152), sampleIndex)
    const int tableOwnerAddr = GetValue8B0554();
    void* tableManager = CSF_AT(void*, AddressToBytes(tableOwnerAddr), 8152);
    uint8_t* sampleRecord = AddressToBytes(GetVectorTrackRecord(tableManager, nullptr, sampleIndex));

    // v8 = 44 * (unsigned)*(float*)(*(a3[1] + 4) + 4)
    uint8_t* stepHolder = AddressToBytes(static_cast<int>(requestArgs[1]));
    uint8_t* stepSource = AddressToBytes(static_cast<int>(CSF_AT(uint32_t, stepHolder, 4)));
    const float stepCoordinate = CSF_AT(float, stepSource, 4);
    const int32_t stepIndex = static_cast<int32_t>(stepCoordinate);
    const uint32_t strideOffset = 44u * static_cast<uint32_t>(stepIndex);

    // Copy the 3-float vector at table[+0x38] + offset + 4 into output+4.
    const uint32_t tableBase = CSF_AT(uint32_t, sampleRecord, 0x38);
    uint8_t* vectorSource = AddressToBytes(static_cast<int>(tableBase + strideOffset + 4u));
    uint32_t outputObjectAddr = CSF_AT(uint32_t, AddressToBytes(outputHolderAddr), 4);
    uint8_t* outputObject = AddressToBytes(static_cast<int>(outputObjectAddr));
    CSF_AT(uint32_t, outputObject, 4) = CSF_AT(uint32_t, vectorSource, 0);
    CSF_AT(uint32_t, outputObject, 8) = CSF_AT(uint32_t, vectorSource, 4);
    CSF_AT(uint32_t, outputObject, 12) = CSF_AT(uint32_t, vectorSource, 8);

    // output+16 = first dword of the record; output+20 = first dword of the entry.
    CSF_AT(uint32_t, outputObject, 16) = CSF_AT(uint32_t, sampleRecord, 0);
    const uint32_t entryBase = CSF_AT(uint32_t, sampleRecord, 0x38) + strideOffset;
    const uint32_t entryValue = CSF_AT(uint32_t, AddressToBytes(static_cast<int>(entryBase)), 0);
    CSF_AT(uint32_t, outputObject, 20) = entryValue;
    return static_cast<int>(entryValue);
}

HOOK(0x00463160, ResolveVectorSampleFromTable, "auto");
