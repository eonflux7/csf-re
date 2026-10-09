#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, GetClipDurationTicks, 0x00672AC0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetFrameIndexForTicks, 0x00471ED0, (void* self, void* edx, unsigned int a2));
CSF_FUNC(uint8_t*, __fastcall, LookupFrameByKey, 0x00472B20, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, LookupFrameByIndex, 0x00472B60, (void* self, void* edx, int a2));

static int __fastcall SelectFrameForClipTime(void* self, void* /*edx*/, C_008313f8* frameSet, int /*unusedArg*/, uint32_t* requestArgs)
{
    // requestArgs[0] -> key record; dword at +8 is the frame table index,
    // bit 23 selects the scene-level table instead of the owner's table.
    uint8_t* keyRecord = reinterpret_cast<uint8_t*>(requestArgs[0]);
    uint32_t frameIndexWord = *CSF_AT(uint32_t*, keyRecord, 0x8);
    uint32_t frameTableOwner = *CSF_AT(uint32_t*, self, 0x20);
    if ((frameIndexWord & 0x800000u) != 0)
    {
        frameTableOwner = *CSF_AT(uint32_t*,
            reinterpret_cast<uint8_t*>(*CSF_AT(uint32_t*, g_pCurrentScene, 0xBC)), 0x38);
        frameIndexWord &= ~0x800000u;
    }

    uint8_t* frameTable = reinterpret_cast<uint8_t*>(
        *CSF_AT(uint32_t*, reinterpret_cast<uint8_t*>(frameTableOwner), 0x8));
    uint8_t* frameEntry = frameTable + 12u * frameIndexWord;

    // requestArgs[1] -> P; *(P+4) -> Q; float at Q+4 is the elapsed time.
    uint8_t* requestPair = reinterpret_cast<uint8_t*>(requestArgs[1]);
    uint8_t* timeRecord = reinterpret_cast<uint8_t*>(*CSF_AT(uint32_t*, requestPair, 0x4));
    float elapsedTime = *CSF_AT(float*, timeRecord, 0x4);

    int64_t elapsedTicks = static_cast<int64_t>(elapsedTime);
    uint32_t elapsedTicksLow = static_cast<uint32_t>(elapsedTicks);
    uint32_t elapsedTicksHigh = static_cast<uint32_t>(static_cast<uint64_t>(elapsedTicks) >> 32);

    void* entryObject = reinterpret_cast<void*>(*CSF_AT(uint32_t*, frameEntry, 0x4));
    uint32_t clipDurationTicks = static_cast<uint32_t>(
        GetClipDurationTicks(entryObject, reinterpret_cast<void*>(static_cast<uintptr_t>(elapsedTicksHigh))));

    if (elapsedTicksLow < clipDurationTicks)
    {
        int frameIndexFromTicks = GetFrameIndexForTicks(
            reinterpret_cast<void*>(*CSF_AT(uint32_t*, frameEntry, 0x4)), nullptr, elapsedTicksLow);
        if (frameIndexFromTicks != 0)
        {
            return reinterpret_cast<int>(LookupFrameByIndex(frameSet, nullptr, frameIndexFromTicks));
        }
    }

    uint8_t* clipObject = reinterpret_cast<uint8_t*>(*CSF_AT(uint32_t*, frameEntry, 0x4));
    int fallbackFrameKey = static_cast<int>(*CSF_AT(uint32_t*, clipObject, 0x4));
    return reinterpret_cast<int>(LookupFrameByKey(frameSet, nullptr, fallbackFrameKey));
}

HOOK(0x004645C0, SelectFrameForClipTime, "auto");
