#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int32_t, __cdecl, GetGlobal8afc78, 0x0042D6D0, ());
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, InitPoseScratchBuffer, 0x004582E0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, ApplyPoseToTarget, 0x0045A6E0, (void* self, void* edx, float* a2));
CSF_FUNC(int, __fastcall, FindSlotIndexForKey, 0x007686E0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetRecordBySlotIndex, 0x00768740, (void* self, void* edx, int a2));

// Reads three component words from the argument pack, looks up a record for the
// keyed slot, copies three of its dwords, and applies the pose to the global target.
// Buffer layout (matches the original stack layout):
//   [0] = truncated first component, [2] = third component, [4] = second component,
//   [7..9] = copy of the three dwords of the looked-up record.
static void __stdcall ApplyIndexedRecordPoseToGameTarget(int unusedFirstArg, int unusedSecondArg, uint32_t* argPack)
{
    // entry = *(*argPack + 4)
    const uint32_t entryRef = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(argPack[0]), 0x4);

    const uint32_t gameStateA = static_cast<uint32_t>(GetValue8B0554());
    const uint32_t entryKey = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(entryRef), 0x4);
    const uint32_t recordTable = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(gameStateA), 0x1F90);
    const int slotIndex = FindSlotIndexForKey(reinterpret_cast<void*>(recordTable), nullptr,
                                              static_cast<int>(entryKey));

    if (slotIndex != -1)
    {
        const uint32_t poseTarget = CSF_AT(uint32_t,
                                           reinterpret_cast<uint8_t*>(GetGlobal8afc78()), 0x248);
        if (poseTarget != 0)
        {
            uint32_t poseWords[10];
            InitPoseScratchBuffer(poseWords, nullptr);

            uint32_t* recordCopy = poseWords + 7;
            recordCopy[0] = 0;
            recordCopy[1] = 0;
            recordCopy[2] = 0;

            const uint32_t gameStateB = static_cast<uint32_t>(GetValue8B0554());
            const uint32_t recordTableB = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(gameStateB), 0x1F90);
            const uint32_t recordAddress =
                static_cast<uint32_t>(GetRecordBySlotIndex(reinterpret_cast<void*>(recordTableB),
                                                           nullptr, slotIndex)) + 4u;

            recordCopy[0] = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(recordAddress), 0x0);
            recordCopy[1] = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(recordAddress), 0x4);
            recordCopy[2] = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(recordAddress), 0x8);

            // First component: float read, truncated to integer, stored as raw bits in slot 0.
            const uint32_t firstHolder = argPack[1];
            const uint32_t firstSub = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(firstHolder), 0x4);
            const float firstValue = CSF_AT(float, reinterpret_cast<uint8_t*>(firstSub), 0x4);
            poseWords[0] = static_cast<uint32_t>(static_cast<int64_t>(firstValue));

            // Second component: raw dword, stored in slot 4.
            const uint32_t secondHolder = argPack[2];
            const uint32_t secondSub = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(secondHolder), 0x4);
            poseWords[4] = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(secondSub), 0x4);

            // Third component: raw dword, stored in slot 2.
            const uint32_t thirdHolder = argPack[3];
            const uint32_t thirdSub = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(thirdHolder), 0x4);
            poseWords[2] = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(thirdSub), 0x4);

            ApplyPoseToTarget(reinterpret_cast<void*>(poseTarget), nullptr,
                              reinterpret_cast<float*>(poseWords));
        }
    }
}

HOOK(0x0045D940, ApplyIndexedRecordPoseToGameTarget, "auto");
