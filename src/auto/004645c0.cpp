#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetFrameIndexForTicks, 0x00471ED0, (void* self, void* edx, unsigned int a2));
CSF_FUNC(uint8_t*, __fastcall, LookupFrameByKey, 0x00472B20, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, ProcessEntryPair, 0x00472B60, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, ResolveSceneEntryValue, 0x00672AC0, (void* self, void* edx));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Picks the animation frame for a clip-table entry from the elapsed time.
// If the time is below the entry's scene value and maps to a nonzero frame index,
// returns the pair-processed frame; otherwise returns the frame keyed by the entry's clip object.
static int __fastcall ScriptCommands_SelectFrameForClipTime(void* self, void* /*edx*/, C_008313f8* a2, int /*a3*/, uint32_t* a4)
{
    // a4[0] -> request object; the dword at +8 is the frame selector
    uint8_t* requestObject = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a4[0]));
    uint32_t frameSelector = CSF_AT(uint32_t, requestObject, 0x08);

    uint8_t* clipOwner = CSF_AT(uint8_t*, self, 0x20);
    if ((frameSelector & 0x800000u) != 0)
    {
        // Flag bit set: the clip owner comes from the current scene
        uint8_t* sceneObject = static_cast<uint8_t*>(g_pCurrentScene);
        uint8_t* sceneChild = CSF_AT(uint8_t*, sceneObject, 0xBC);
        clipOwner = CSF_AT(uint8_t*, sceneChild, 0x38);
        frameSelector &= 0xFF7FFFFFu;
    }

    uint8_t* clipTable = CSF_AT(uint8_t*, clipOwner, 0x08);
    uint8_t* clipEntry = reinterpret_cast<uint8_t*>(reinterpret_cast<uintptr_t>(clipTable) + 12u * frameSelector);

    // a4[1] -> time request -> holder whose +4 is the elapsed time as a float
    uint8_t* timeRequest = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a4[1]));
    uint8_t* timeHolder = CSF_AT(uint8_t*, timeRequest, 0x04);
    float elapsedTime = CSF_AT(float, timeHolder, 0x04);

    // Truncate to 64-bit ticks: low dword is the tick index, high dword is passed on
    uint64_t elapsedTicks = static_cast<uint64_t>(static_cast<int64_t>(elapsedTime));
    uint32_t ticksLow = static_cast<uint32_t>(elapsedTicks);
    uint32_t ticksHigh = static_cast<uint32_t>(elapsedTicks >> 32);

    int sceneEntryValue = ResolveSceneEntryValue(
        CSF_AT(void*, clipEntry, 0x04),
        reinterpret_cast<void*>(static_cast<uintptr_t>(ticksHigh)));

    if (ticksLow < static_cast<uint32_t>(sceneEntryValue))
    {
        int frameIndex = GetFrameIndexForTicks(CSF_AT(void*, clipEntry, 0x04), nullptr, ticksLow);
        if (frameIndex != 0)
        {
            return static_cast<int>(reinterpret_cast<uintptr_t>(ProcessEntryPair(a2, nullptr, frameIndex)));
        }
    }

    // Fallback: frame keyed by the entry's clip object
    int clipKey = CSF_AT(int, CSF_AT(void*, clipEntry, 0x04), 0x04);
    return static_cast<int>(reinterpret_cast<uintptr_t>(LookupFrameByKey(a2, nullptr, clipKey)));
}

HOOK(0x4645C0, ScriptCommands_SelectFrameForClipTime, "auto");
