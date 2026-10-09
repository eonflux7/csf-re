#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "globals.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(bool, __stdcall, IsPairRelationValid, 0x0045BB30, (int a1, int a2));
CSF_FUNC(bool, __fastcall, IsEntryFrameBlocked, 0x0049ED50, (void* self, void* edx, int a2));

static char __fastcall TryDispatchTimedOrder(void* self, void* /*edx*/, int /*a2 unused by original*/, int pairMode, uint32_t* argPack)
{
    // The original loads both argument records and their +4 blocks before testing any word.
    uint32_t* targetRecord = reinterpret_cast<uint32_t*>(argPack[1]);
    uint32_t* sourceRecord = reinterpret_cast<uint32_t*>(argPack[0]);
    uint32_t* sourceBlock = reinterpret_cast<uint32_t*>(sourceRecord[1]);
    // Source handle slot sits 4 bytes into the source block.
    uint32_t* sourceHandleSlot = sourceBlock + 1;

    const int32_t sourceWord = static_cast<int32_t>(sourceBlock[3]);

    // Target block: record's +4 pointer, handle slot 4 bytes in.
    uint32_t* targetBlock = reinterpret_cast<uint32_t*>(targetRecord[1]) + 1;

    uint32_t returnLowByte = static_cast<uint32_t>(sourceWord);
    if (sourceWord < 0)
        return static_cast<char>(returnLowByte);

    const int32_t targetWord = static_cast<int32_t>(targetBlock[2]);
    returnLowByte = static_cast<uint32_t>(targetWord);
    if (targetWord < 0)
        return static_cast<char>(returnLowByte);

    // Resolve the source entity; the slot address itself is the handle value.
    const int resolvedSource = CHandleTable_Resolve(g_pCurrentScene, nullptr,
                                                    static_cast<int>(reinterpret_cast<uintptr_t>(sourceHandleSlot)));
    returnLowByte = static_cast<uint32_t>(resolvedSource);
    if (resolvedSource == 0)
        return static_cast<char>(returnLowByte);

    void* sourceEntry = reinterpret_cast<void*>(static_cast<uintptr_t>(resolvedSource));
    // The source entity must carry a non-null object at +0x34.
    if (CSF_AT(uint32_t, sourceEntry, 0x34) == 0)
        return static_cast<char>(returnLowByte);

    // Resolve the target entity and check the pair relation.
    const int resolvedTarget = CHandleTable_Resolve(g_pCurrentScene, nullptr,
                                                    static_cast<int>(reinterpret_cast<uintptr_t>(targetBlock)));
    C_008342e8* targetEntry = reinterpret_cast<C_008342e8*>(static_cast<uintptr_t>(resolvedTarget));
    const bool pairValid = IsPairRelationValid(pairMode, resolvedTarget);
    returnLowByte = static_cast<uint32_t>(pairValid);
    if (!pairValid)
        return static_cast<char>(returnLowByte);

    // Frame value read from the third argument record: *(float*)(*(argPack[2] + 4) + 4).
    const uintptr_t frameRecordAddr = static_cast<uintptr_t>(reinterpret_cast<uint32_t*>(argPack[2])[1]);
    const float frameValue = *reinterpret_cast<const float*>(frameRecordAddr + 4);
    const uint32_t frameIndex = static_cast<uint32_t>(static_cast<int64_t>(frameValue));
    returnLowByte = frameIndex;
    if (frameIndex >= static_cast<uint32_t>(targetEntry->field_01a0))
        return static_cast<char>(returnLowByte);

    const bool frameBlocked = IsEntryFrameBlocked(targetEntry, nullptr, static_cast<int>(frameIndex));
    returnLowByte = static_cast<uint32_t>(frameBlocked);
    if (frameBlocked)
        return static_cast<char>(returnLowByte);

    // Command block: type 27, dispatch vtable, target block fields, frame index.
    uint32_t commandArgs[5];
    commandArgs[0] = 27;
    commandArgs[1] = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x0082D448)));
    commandArgs[2] = targetBlock[1];
    commandArgs[3] = targetBlock[2];
    commandArgs[4] = frameIndex;

    // Virtual call on the source entity's object at +0x34, slot 168 (42 * 4).
    void* commandObject = reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, sourceEntry, 0x34)));
    using CommandDispatchFn = int(__fastcall*)(void* self, void* edx, uint32_t* args);
    CommandDispatchFn dispatch = reinterpret_cast<CommandDispatchFn>(CSF_AT(void**, commandObject, 0)[42]);
    returnLowByte = static_cast<uint32_t>(dispatch(commandObject, nullptr, commandArgs));

    // Copy the packet's position pair into this object and clear the flag byte.
    CSF_AT(uint32_t, self, 0x60) = sourceHandleSlot[1];
    CSF_AT(uint32_t, self, 0x64) = sourceHandleSlot[2];
    CSF_AT(uint8_t, self, 0x82) = 0;

    return static_cast<char>(returnLowByte);
}

HOOK(0x00463930, TryDispatchTimedOrder, "auto");
