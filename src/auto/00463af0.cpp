#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int32_t, __cdecl, GetGlobalValue_51F740, 0x0051F740, ());
CSF_FUNC(int, __fastcall, ResolveGlobalEntryToOutput, 0x00520D60, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __fastcall, FindSubsystemSlotIndex, 0x007686E0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetSubsystemSlotRecord, 0x00768740, (void* self, void* edx, int a2));

static void* __stdcall ResolveEntryAndDispatch(
    int /*unusedFirstArg*/, int /*unusedSecondArg*/, int* requestRecord)
{
    void* requestNode = reinterpret_cast<void*>(requestRecord[1]);
    void* sourceObject = CSF_AT(void*, requestNode, 0x04);

    void* gameState = reinterpret_cast<void*>(GetValue8B0554());
    void* subsystem = CSF_AT(void*, gameState, 0x1F90);
    int slotIndex = FindSubsystemSlotIndex(
        subsystem, nullptr, CSF_AT(int, sourceObject, 0x04));

    if (slotIndex == -1)
        return reinterpret_cast<void*>(slotIndex);

    gameState = reinterpret_cast<void*>(GetValue8B0554());
    subsystem = CSF_AT(void*, gameState, 0x1F90);
    int* slotRecord = reinterpret_cast<int*>(
        GetSubsystemSlotRecord(subsystem, nullptr, slotIndex));

    // Local argument layout, not a game-object field layout.
    struct ResolveArguments {
        uint32_t unknown;
        uint32_t stringStorage[4];
        uint32_t stringLength;
        uint32_t stringCapacity;
        int32_t sentinel;
        int32_t requestValue;
        int32_t slotValue;
        int32_t sentinel2;
        int32_t sentinel3;
        uint8_t flag;
        uint8_t hasRequestValue;
        uint8_t hasSlotValue;
        uint8_t padding;
    };
    static_assert(sizeof(ResolveArguments) == 0x34);

    ResolveArguments args;
    void* keyNode = reinterpret_cast<void*>(requestRecord[0]);

    args.requestValue = -1;
    args.slotValue = -1;
    args.hasRequestValue = 0;
    args.hasSlotValue = 0;
    args.stringCapacity = 15;
    args.stringLength = 0;
    reinterpret_cast<uint8_t*>(args.stringStorage)[0] = 0;
    args.sentinel = -1;
    args.sentinel2 = -1;
    args.flag = 0;
    args.sentinel3 = -1;

    void* keyObject = CSF_AT(void*, keyNode, 0x04);
    args.requestValue = CSF_AT(int, keyObject, 0x04);
    args.slotValue = *slotRecord;
    args.hasRequestValue = 1;
    args.hasSlotValue = 1;

    void* globalValue = reinterpret_cast<void*>(GetGlobalValue_51F740());
    void* result = reinterpret_cast<void*>(ResolveGlobalEntryToOutput(
        globalValue, nullptr, reinterpret_cast<uint32_t*>(&args)));

    if (args.stringCapacity >= 0x10) {
        result = reinterpret_cast<void*>(args.stringStorage[0]);
        if (result)
            return reinterpret_cast<void*>(Mem_FreeDebug(result));
    }
    return result;
}

HOOK(0x00463AF0, ResolveEntryAndDispatch, "auto");
