#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(void*, __fastcall, DefaultDestination_Get, 0x004FE4A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, FindSlotIndexOfId, 0x007686E0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetSlotBaseAddress, 0x00768740, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, DestinationPostPayloadMessage, 0x0044D0C0, (void* self, void* edx, int a2, int a3, int* a4));

static void* AddressToPointer(uint32_t address) {
    return reinterpret_cast<void*>(static_cast<uintptr_t>(address));
}

static void* GetWrapperPayload(uint32_t wrapperAddress) {
    return CSF_AT(void*, AddressToPointer(wrapperAddress), 4);
}

static int ConvertFloatToInt64LowDword(float value) {
    const int64_t wide = static_cast<int64_t>(value);
    return static_cast<int>(static_cast<uint32_t>(wide));
}

static int __stdcall SendIndexedMessageToDefaultDestination(
    int /*unusedArg1*/, int /*unusedArg2*/, uint32_t* wrapperArgs) {
    void* keyRecord = GetWrapperPayload(wrapperArgs[1]);

    const int managerAddress = GetValue8B0554();
    const int keyId = CSF_AT(int, keyRecord, 4);
    void* subsystem = CSF_AT(void*, AddressToPointer(managerAddress), 0x1F90);
    const int slotIndex = FindSlotIndexOfId(subsystem, nullptr, keyId);
    if (slotIndex == -1) {
        return slotIndex;
    }

    void* flagWrapper = AddressToPointer(wrapperArgs[2]);
    int payload[2];
    uint8_t* payloadBytes = reinterpret_cast<uint8_t*>(payload);
    payloadBytes[0] = 0;
    payload[1] = 0;
    void* flagRecord = CSF_AT(void*, flagWrapper, 4);
    const uint8_t flag = CSF_AT(uint8_t, flagRecord, 1);
    void* magnitudeWrapper = AddressToPointer(wrapperArgs[3]);
    payloadBytes[0] = flag;
    void* magnitudeRecord = CSF_AT(void*, magnitudeWrapper, 4);
    payload[1] = ConvertFloatToInt64LowDword(CSF_AT(float, magnitudeRecord, 4));

    const int managerForSlotAddress = GetValue8B0554();
    void* scalarWrapper = AddressToPointer(wrapperArgs[0]);
    void* slotSubsystem = CSF_AT(void*, AddressToPointer(managerForSlotAddress), 0x1F90);
    void* scalarRecord = CSF_AT(void*, scalarWrapper, 4);
    const int slotRecordAddress = GetSlotBaseAddress(slotSubsystem, nullptr, slotIndex) + 4;
    const int scalarValue = ConvertFloatToInt64LowDword(CSF_AT(float, scalarRecord, 4));

    void* globalObject = AddressToPointer(GetGlobalValue_8B0568());
    void* destination = DefaultDestination_Get(globalObject, nullptr);
    return DestinationPostPayloadMessage(destination, nullptr, scalarValue, slotRecordAddress, payload);
}

HOOK(0x0045DAF0, SendIndexedMessageToDefaultDestination, "auto");
