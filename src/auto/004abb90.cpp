#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(uint32_t*, __fastcall, AiCoverAttackTasks_GetPositionData, 0x004A9CA0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, LookupEntryIndex, 0x004DC5F0, (void* self, void* edx, float* a2));
CSF_FUNC(int, __fastcall, ResolveEntryStateRecord, 0x004DC6E0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_GLOBAL(int, g_AiSearchCostThisFrame, 0x008B044C);

// Returns nonzero when the candidate's entry state still matches the expected owner
// and state value; increments the per-frame search cost counter.
static int __fastcall S_004abb90_IsCandidateMatchingState(S_004abb90* self, void* /*edx*/, int a2)
{
    bool mismatch = false;

    int ownerValue = GetValue8B0554();
    uint8_t* owner = reinterpret_cast<uint8_t*>(ownerValue);
    uint8_t* listBase = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, owner, 0x1fd8)) + 0x44;

    uint32_t* positionData = AiCoverAttackTasks_GetPositionData(
        reinterpret_cast<uint8_t*>(self) + 0x38, nullptr);
    int entryIndex = LookupEntryIndex(listBase, nullptr, reinterpret_cast<float*>(positionData));

    if (entryIndex != -1) {
        uint32_t* stateRecord = reinterpret_cast<uint32_t*>(
            ResolveEntryStateRecord(listBase, nullptr, entryIndex));

        int linkedValue = GetLinkedObjectValue_4DB5C0(
            reinterpret_cast<void*>(static_cast<uintptr_t>(a2)), nullptr);
        uint8_t* linkedObj = linkedValue ? reinterpret_cast<uint8_t*>(linkedValue - 4) : nullptr;
        uint8_t* expectedOwner = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, linkedObj, 0x2c));

        mismatch = stateRecord[2] != CSF_AT(uint32_t, expectedOwner, 0x20) || stateRecord[0] != 2;
    }

    ++g_AiSearchCostThisFrame;
    return mismatch ? 0 : 1;
}

HOOK(0x004ABB90, S_004abb90_IsCandidateMatchingState, "auto");
