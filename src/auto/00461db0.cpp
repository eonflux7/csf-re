#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

static void* IntAsPointer(int value) {
    return reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(value)));
}

static int AddressAsInt(void* address) {
    return static_cast<int>(reinterpret_cast<uintptr_t>(address));
}

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(char, __fastcall, IsGlobalValueAvailable, 0x00521C00, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SearchGlobalValueNearPosition, 0x00524150, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, LookupTargetCellId, 0x007686E0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetCellRecordForId, 0x00768740, (void* self, void* edx, int a2));

struct SearchDescriptor {
    int requestFlag;
    int mode;
    int targetId;
    int sentinel0C;
    int sourceId;
    int zeroField14;
    int posX;
    int posY;
    int posZ;
    void** vtable;
    int sentinel28;
    int sentinel2C;
    int zeroField30;
    uint8_t flag34;
    uint8_t flag35;
    uint8_t flag36;
    uint8_t pad37;
};
static_assert(sizeof(SearchDescriptor) == 0x38);
static_assert(offsetof(SearchDescriptor, vtable) == 0x24);

struct SearchPosition {
    int x;
    int y;
    int z;
    float w;
};

static int __fastcall RequestPathToTargetPosition(
    void* self, void* /*edx*/,
    int /*firstStackArgUnused*/, int /*secondStackArgUnused*/,
    int* requestArgs) {
    SearchDescriptor descriptor;
    descriptor.mode = 1;
    descriptor.flag35 = 1;
    descriptor.requestFlag = 1;

    int rootRecord = requestArgs[0];
    descriptor.posX = 0;
    descriptor.posY = 0;
    descriptor.posZ = 0;
    descriptor.vtable = CSF_VTBL(0x0082D448);
    descriptor.sentinel28 = -1;
    descriptor.sentinel2C = -1;
    descriptor.sourceId = 0;
    descriptor.zeroField14 = 0;
    descriptor.flag34 = 0;
    descriptor.targetId = -1;
    descriptor.sentinel0C = -1;
    descriptor.zeroField30 = 0;
    descriptor.flag36 = 0;

    int rootChild = CSF_AT(int, IntAsPointer(rootRecord), 0x04);
    int targetId = CSF_AT(int, IntAsPointer(rootChild), 0x04);
    if (targetId == -1) {
        return targetId;
    }

    int sourceRecord = requestArgs[2];
    descriptor.targetId = targetId;
    int sourceChild = CSF_AT(int, IntAsPointer(sourceRecord), 0x04);
    descriptor.sourceId = CSF_AT(int, IntAsPointer(sourceChild), 0x04);
    int targetRecord = CSF_AT(int, IntAsPointer(requestArgs[1]), 0x04);

    int worldRoot = GetValue8B0554();
    int targetIdArg = CSF_AT(int, IntAsPointer(targetRecord), 0x04);
    int cellService = CSF_AT(int, IntAsPointer(worldRoot), 0x1F90);
    int locatedCellId = LookupTargetCellId(IntAsPointer(cellService), nullptr, targetIdArg);
    if (locatedCellId == -1) {
        return locatedCellId;
    }

    int worldRootAgain = GetValue8B0554();
    int cellServiceAgain = CSF_AT(int, IntAsPointer(worldRootAgain), 0x1F90);
    void* cellRecord = IntAsPointer(
        GetCellRecordForId(IntAsPointer(cellServiceAgain), nullptr, locatedCellId));
    descriptor.posX = CSF_AT(int, cellRecord, 0x04);
    descriptor.posY = CSF_AT(int, cellRecord, 0x08);
    descriptor.posZ = CSF_AT(int, cellRecord, 0x0C);
    descriptor.mode = 4;

    int globalValueForCheck = GetGlobalValue_521BB0();
    char isAvailable = IsGlobalValueAvailable(IntAsPointer(globalValueForCheck), nullptr);
    if (!isAvailable) {
        return isAvailable;
    }

    int fourthComponentChild = CSF_AT(int, IntAsPointer(requestArgs[3]), 0x04);
    SearchPosition position;
    position.w = CSF_AT(float, IntAsPointer(fourthComponentChild), 0x04);
    position.x = descriptor.posX;
    position.y = descriptor.posY;
    position.z = descriptor.posZ;

    int globalValueForSearch = GetGlobalValue_521BB0();
    int searchResult = SearchGlobalValueNearPosition(
        IntAsPointer(globalValueForSearch), nullptr,
        AddressAsInt(&descriptor), AddressAsInt(&position));

    CSF_AT(int, self, 0x7C) = searchResult;
    if (searchResult != 0) {
        CSF_AT(int, self, 0x18) = 8;
        CSF_AT(uint8_t, self, 0x82) = 0;
    }
    return searchResult;
}

HOOK(0x00461DB0, RequestPathToTargetPosition, "auto");
