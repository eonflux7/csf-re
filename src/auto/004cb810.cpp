#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __fastcall, EventQueue_TryConsumeRecord, 0x00449230, (void* self, void* , const void* incomingRecord));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, AiCoverAttackTasks_GetPositionData, 0x004A9CA0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, S_004abb90_CheckCandidate, 0x004ABB90, (void* self, void* edx, int a2));
CSF_FUNC(bool, __fastcall, S_004abb90_CheckUnassignedCandidate, 0x004ABC10, (void* self, void* edx, int a2));
CSF_FUNC(bool, __fastcall, S_004abb90_CheckAssignedCandidate, 0x004ABC90, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, AiCore_IsTargetPositionValid, 0x004AEDD0, (S_004aa310* self, void* , int a2));
CSF_FUNC(int, __fastcall, StaticGuard_CanEngageTarget, 0x004AEE30, (S_004aa310* self, void* , S_004abb90* a2));
CSF_FUNC(uint32_t, __fastcall, S_004abb90_FindClosestPositionIndex, 0x004B0770, (S_004abb90* self, void* , int object));
CSF_FUNC(bool, __fastcall, S_004abb90_CheckSearchCandidate, 0x004B57E0, (void* self, void* edx, int a2));
CSF_FUNC(bool, __fastcall, S_004abb90_CheckUnassignedSearchCandidate, 0x004B5A70, (void* self, void* edx, float a2));
CSF_FUNC(char, __fastcall, S_004abb90_ValidateSearchCandidate, 0x004B5C30, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, S_004abb90_CommitSearchCandidate, 0x004B5D20, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, AiSearchOrders_BuildSearchPositions, 0x004CB580, (void* self, void* edx, float a2, float a3, int* a4));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, FindActiveEntryIndexByKey, 0x004DC5B0, (void* self, void* , void* keyObject));
CSF_FUNC(int, __fastcall, ResolveEntryStateRecord, 0x004DC6E0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

CSF_GLOBAL(float, g_SearchPositionTolerance, 0x0083AB24);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(int, g_AiSearchCostThisFrame, 0x008B044C);

// The packet also classifies the direct call at this address as a vtable reference.
static const auto kCandidateCheckTable = CSF_VTBL(0x004ABB90);

static void* AiSearchOrders_GetLinkedActor(int object)
{
    const int linked = GetLinkedObjectValue_4DB5C0(
        reinterpret_cast<void*>(object), nullptr);
    return linked ? reinterpret_cast<void*>(linked - 4) : nullptr;
}

static float AiSearchOrders_FloatFromBits(uint32_t bits)
{
    float value;
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}

static char __fastcall S_004abb90_AdvanceSearchSweep(
    S_004abb90* self, void* /*edx*/, int object)
{
    void* actor = AiSearchOrders_GetLinkedActor(object);
    void* actorObject = CSF_AT(void*, actor, 0x2C);
    void* queue = CSF_AT(void*, g_pCurrentScene, 0xC4);
    if (!EventQueue_TryConsumeRecord(
            queue, nullptr, static_cast<uint8_t*>(actorObject) + 0x1C))
        return 0;

    void* positionActor = AiSearchOrders_GetLinkedActor(object);
    auto* targetActor = static_cast<S_004aa310*>(
        AiSearchOrders_GetLinkedActor(object));
    const int position = GetObjectPositionVector(
        CSF_AT(void*, positionActor, 0x2C), nullptr);
    const int valid = AiCore_IsTargetPositionValid(targetActor, nullptr, position);
    if (static_cast<uint8_t>(valid) == 0)
    {
        self->field_0004 = 2;
        return static_cast<char>(valid);
    }

    if (self->field_0024)
    {
        self->field_0024 = 0;
        actor = AiSearchOrders_GetLinkedActor(object);
        const float searchParameter =
            AiSearchOrders_FloatFromBits(self->field_001c);
        void* settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
        const float radius = CSF_AT(float, settings, 0xAB4);
        if (!AiSearchOrders_BuildSearchPositions(
                actor, nullptr, searchParameter, radius,
                reinterpret_cast<int*>(reinterpret_cast<uint8_t*>(self) + 0x28)))
        {
            actor = AiSearchOrders_GetLinkedActor(object);
            ++CSF_AT(uint32_t, actor, 0x710);
            self->field_0008 = 1084227584;
            self->field_0004 = 2;
            return static_cast<char>(reinterpret_cast<uintptr_t>(actor));
        }
    }

    void* services = reinterpret_cast<void*>(GetValue8B0554());
    void* entryManager = CSF_AT(void*, services, 0x1FD8);
    int savedState = 0;
    int entryIndex = -1;

    if (!CSF_AT(uint8_t, self, 0x14))
    {
        actor = AiSearchOrders_GetLinkedActor(object);
        void* key = static_cast<uint8_t*>(CSF_AT(void*, actor, 0x2C)) + 0x1C;
        void* entries = static_cast<uint8_t*>(entryManager) + 0x44;
        entryIndex = FindActiveEntryIndexByKey(entries, nullptr, key);
        if (entryIndex != -1)
        {
            void* record = reinterpret_cast<void*>(
                ResolveEntryStateRecord(entries, nullptr, entryIndex));
            savedState = CSF_AT(int, record, 0);
            record = reinterpret_cast<void*>(
                ResolveEntryStateRecord(entries, nullptr, entryIndex));
            CSF_AT(int, record, 0) = 0;
        }
    }

    while (self->field_0004 == 1 &&
           static_cast<uint32_t>(g_AiSearchCostThisFrame) < 0xA)
    {
        if (CSF_AT(uint32_t, self, 0x30) == 0)
        {
            actor = AiSearchOrders_GetLinkedActor(object);
            ++CSF_AT(uint32_t, actor, 0x710);
            self->field_0008 = 1084227584;
            self->field_0004 = 2;
            continue;
        }

        const uint32_t closest =
            S_004abb90_FindClosestPositionIndex(self, nullptr, object);
        const uint32_t byteOffset = 20 * closest;
        uint8_t* candidate =
            CSF_AT(uint8_t*, self, 0x28) + byteOffset;

        CSF_AT(uint32_t, self, 0x38) = CSF_AT(uint32_t, candidate, 0);
        CSF_AT(uint32_t, self, 0x3C) = CSF_AT(uint32_t, candidate, 4);
        CSF_AT(uint32_t, self, 0x40) = CSF_AT(uint32_t, candidate, 8);
        CSF_AT(uint32_t, self, 0x44) = CSF_AT(uint32_t, candidate, 0xC);
        CSF_AT(uint32_t, self, 0x48) = CSF_AT(uint32_t, candidate, 0x10);

        auto* selected = reinterpret_cast<S_004abb90*>(
            reinterpret_cast<uint8_t*>(self) + 0x38);
        targetActor = static_cast<S_004aa310*>(
            AiSearchOrders_GetLinkedActor(object));

        if (static_cast<uint8_t>(
                StaticGuard_CanEngageTarget(targetActor, nullptr, selected)) &&
            static_cast<uint8_t>(
                S_004abb90_CheckCandidate(self, nullptr, object)) &&
            S_004abb90_CheckSearchCandidate(self, nullptr, object))
        {
            actor = AiSearchOrders_GetLinkedActor(object);
            bool accepted;
            if (CSF_AT(int, actor, 0x6D8) < 0)
            {
                accepted =
                    S_004abb90_CheckUnassignedSearchCandidate(
                        self, nullptr,
                        AiSearchOrders_FloatFromBits(static_cast<uint32_t>(object))) &&
                    S_004abb90_CheckUnassignedCandidate(self, nullptr, object) &&
                    S_004abb90_ValidateSearchCandidate(self, nullptr, object);
            }
            else
            {
                accepted =
                    S_004abb90_CheckAssignedCandidate(self, nullptr, object);
            }

            if (accepted &&
                S_004abb90_CommitSearchCandidate(self, nullptr, object))
            {
                actor = AiSearchOrders_GetLinkedActor(object);
                void* currentPosition = reinterpret_cast<void*>(
                    GetObjectPositionVector(CSF_AT(void*, actor, 0x2C), nullptr));
                const float currentX = CSF_AT(float, currentPosition, 0);
                const float currentY = CSF_AT(float, currentPosition, 4);
                const float currentZ = CSF_AT(float, currentPosition, 8);

                actor = AiSearchOrders_GetLinkedActor(object);
                if (CSF_AT(uint32_t, actor, 0x6C8) &&
                    CSF_AT(int, actor, 0x6C0) != -1 &&
                    CSF_AT(int, actor, 0x6C4) != -1)
                {
                    GetLinkedObjectValue_4DB5C0(
                        reinterpret_cast<void*>(object), nullptr);
                }

                uint32_t* selectedPosition =
                    AiCoverAttackTasks_GetPositionData(selected, nullptr);
                const float dx = currentX - CSF_AT(float, selectedPosition, 0);
                const float dy = currentY - CSF_AT(float, selectedPosition, 4);
                const float dz = currentZ - CSF_AT(float, selectedPosition, 8);
                const float distance = std::sqrt(dx * dx + dy * dy + dz * dz);
                const bool alreadyThere = !(distance > g_SearchPositionTolerance);

                actor = AiSearchOrders_GetLinkedActor(object);
                const uint32_t completed = CSF_AT(uint32_t, actor, 0x714) + 1;
                ++CSF_AT(uint32_t, actor, 0x710);
                CSF_AT(uint32_t, actor, 0x714) = completed;
                self->field_0004 = alreadyThere ? 2 : 3;
                self->field_0008 = 1084227584;
            }
        }

        const uint32_t remaining = CSF_AT(uint32_t, self, 0x30) - 1;
        CSF_AT(uint32_t, self, 0x30) = remaining;
        if (closest != remaining)
        {
            uint8_t* destination = CSF_AT(uint8_t*, self, 0x28) + byteOffset;
            _memcpy(destination, destination + 20, 20 * (remaining - closest));
        }
    }

    if (entryIndex != -1)
    {
        void* record = reinterpret_cast<void*>(ResolveEntryStateRecord(
            static_cast<uint8_t*>(entryManager) + 0x44, nullptr, entryIndex));
        CSF_AT(int, record, 0) = savedState;
        return static_cast<char>(reinterpret_cast<uintptr_t>(record));
    }
    return static_cast<char>(entryIndex);
}

HOOK(0x004CB810, S_004abb90_AdvanceSearchSweep, "auto");
