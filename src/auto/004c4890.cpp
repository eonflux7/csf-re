#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_LevelScaleFactor, 0x0082E048);

CSF_FUNC(char, __fastcall, CoverAttack_CheckState_4ADAE0, 0x004ADAE0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, S_004ad990_TryBuildCombatDestination, 0x004B23D0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, S_004ad990_ResolveCombatTarget, 0x004B2450, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, S_004ad990_ApplyCombatOrder, 0x004B8D90, (S_004ad990* self, void* , int* order));
CSF_FUNC(int, __fastcall, S_004ad990_ApplyCombatDestination, 0x004B8EF0, (S_004ad990* self, void* , int destinationAddress));
CSF_FUNC(uint32_t*, __fastcall, S_004ad990_RefreshTrackedHandleTarget, 0x004B9030, (S_004ad990* self, void* , int handle));
CSF_FUNC(int, __fastcall, S_004ad990_ProcessInnerBlockAndCheckValid, 0x004BDA90, (S_004ad990* self, void* , int a2));
CSF_FUNC(char, __fastcall, S_004ad990_CheckCoverAttackBlocked, 0x004C4800, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_0083e9d0_FindMatchingPairLink, 0x004E7170, (C_0083e9d0* self, void* , int slot, int key));

static int __fastcall S_004ad990_UpdateCoverAttack(S_004ad990* self, void* /*edx*/)
{
    const uint32_t timeBits =
        CSF_AT(uint32_t, CSF_AT(void*, g_pCurrentScene, 0xB0), 0x08);
    float currentTime;
    std::memcpy(&currentTime, &timeBits, sizeof(currentTime));
    bool hasCandidate = false;

    if (self->field_002c == 14)
    {
        if (self->field_00a8)
        {
            const float deadline = self->field_00a0 + g_LevelScaleFactor;
            self->field_0058 = timeBits;
            if (deadline >= currentTime)
                return 1;
        }
        else if (self->field_009c < g_LevelScaleFactor)
        {
            return 1;
        }
    }

    auto* owner = reinterpret_cast<void*>(self->field_0000);
    auto* links = CSF_AT(C_0083e9d0*, owner, 0x3C);
    const char matchingLink =
        C_0083e9d0_FindMatchingPairLink(links, nullptr, 3, 14);

    if (self->field_00a0 + g_HalfDurationScale >= currentTime && !matchingLink)
    {
        return (self->field_002c != uint32_t(-1) ||
                self->field_0030 != uint32_t(-1)) &&
               !(self->field_009c > g_TransitionOne);
    }

    self->field_00a0 = currentTime;
    if (S_004ad990_CheckCoverAttackBlocked(self, nullptr))
        return 0;

    alignas(8) uint8_t orderBytes[0x14];
    alignas(8) uint8_t orderTargetBytes[0x20];
    alignas(8) uint8_t destinationBytes[0x08];
    alignas(8) uint8_t destinationTargetBytes[0x28];

    CSF_AT(void**, orderTargetBytes, 0x00) = CSF_VTBL(0x0082D448);
    CSF_AT(void**, destinationTargetBytes, 0x00) = CSF_VTBL(0x0082D448);
    CSF_AT(void**, orderBytes, 0x00) = CSF_VTBL(0x0083124C);
    CSF_AT(int, orderBytes, 0x0C) = -1;
    CSF_AT(int, orderBytes, 0x10) = -1;
    CSF_AT(int, orderTargetBytes, 0x04) = -1;
    CSF_AT(int, orderTargetBytes, 0x08) = -1;
    CSF_AT(int, orderTargetBytes, 0x0C) = 0;
    CSF_AT(int, orderTargetBytes, 0x10) = 0;
    CSF_AT(int, orderTargetBytes, 0x14) = 0;
    CSF_AT(int, orderBytes, 0x04) = -1;
    CSF_AT(int, orderBytes, 0x08) = -1;
    CSF_AT(int, orderTargetBytes, 0x18) = 2139095039;
    CSF_AT(int, orderTargetBytes, 0x1C) = 0;
    CSF_AT(int, destinationTargetBytes, 0x04) = -1;
    CSF_AT(int, destinationTargetBytes, 0x08) = -1;
    CSF_AT(int, destinationTargetBytes, 0x0C) = 0;
    CSF_AT(int, destinationTargetBytes, 0x10) = 0;
    CSF_AT(int, destinationTargetBytes, 0x14) = 0;
    CSF_AT(int, destinationBytes, 0x00) = -1;
    CSF_AT(int, destinationBytes, 0x04) = -1;
    CSF_AT(int, destinationTargetBytes, 0x18) = 2139095039;
    CSF_AT(int, destinationTargetBytes, 0x1C) = 0;

    bool orderAccepted = false;
    if (static_cast<uint8_t>(S_004ad990_ProcessInnerBlockAndCheckValid(
            self, nullptr, reinterpret_cast<int>(orderBytes))))
    {
        hasCandidate = true;
        orderAccepted = CoverAttack_CheckState_4ADAE0(
            self, nullptr, reinterpret_cast<int>(orderTargetBytes)) != 0;
    }

    if (!orderAccepted)
    {
        if (static_cast<uint8_t>(S_004ad990_TryBuildCombatDestination(
                self, nullptr, reinterpret_cast<int>(destinationBytes))) &&
            (!hasCandidate ||
             CoverAttack_CheckState_4ADAE0(
                 self, nullptr, reinterpret_cast<int>(destinationTargetBytes))))
        {
            CSF_AT(int, orderBytes, 0x04) = -1;
            CSF_AT(int, orderBytes, 0x08) = -1;
            CSF_AT(int, orderTargetBytes, 0x18) = 2139095039;
            CSF_AT(int, orderTargetBytes, 0x1C) = 0;
            hasCandidate = true;
        }
        else if (!hasCandidate)
        {
            return 0;
        }
    }

    uint8_t transitioning = 0;
    if (self->field_002c == 14 && self->field_009c < g_LevelScaleFactor)
    {
        if (CSF_AT(int, orderBytes, 0x08) == -1)
        {
            if (CSF_AT(int, destinationBytes, 0x04) != -1)
            {
                uint8_t* trackedHandle = reinterpret_cast<uint8_t*>(self) + 0x4C;
                if (self->field_0054 & 0x80000000)
                    trackedHandle = reinterpret_cast<uint8_t*>(self) + 0x34;

                if (CSF_AT(uint32_t, destinationTargetBytes, 0x04) !=
                    CSF_AT(uint32_t, trackedHandle, 0x04))
                {
                    auto* target = reinterpret_cast<void*>(
                        S_004ad990_ResolveCombatTarget(
                            self, nullptr, reinterpret_cast<int>(destinationTargetBytes)));
                    if (!target)
                        return hasCandidate;

                    const uint32_t targetState = CSF_AT(uint32_t, target, 0x2C);
                    if (targetState == uint32_t(-1))
                        return hasCandidate;
                    if (targetState != 14 || !CSF_AT(uint8_t, target, 0xA8))
                        return hasCandidate;
                    transitioning = 1;
                }
            }
        }
        else if (CSF_AT(int, orderBytes, 0x08) != 14)
        {
            const int32_t handleState = static_cast<int32_t>(self->field_0054);
            uint8_t* trackedHandle = reinterpret_cast<uint8_t*>(self) + 0x4C;
            if (handleState < 0)
                trackedHandle = reinterpret_cast<uint8_t*>(self) + 0x34;

            if (CSF_AT(int32_t, trackedHandle, 0x08) < 0 ||
                CSF_AT(uint32_t, orderTargetBytes, 0x04) !=
                    CSF_AT(uint32_t, trackedHandle, 0x04))
            {
                auto* target = reinterpret_cast<void*>(
                    S_004ad990_ResolveCombatTarget(
                        self, nullptr, reinterpret_cast<int>(orderTargetBytes)));
                if (!target || CSF_AT(uint32_t, target, 0x2C) == uint32_t(-1))
                    return hasCandidate;
                if (self->field_002c != 14 || !CSF_AT(uint8_t, target, 0xA8))
                    return hasCandidate;
                transitioning = 1;
            }
        }
    }

    if (CSF_AT(int, orderBytes, 0x08) != -1)
    {
        S_004ad990_ApplyCombatOrder(
            self, nullptr, reinterpret_cast<int*>(orderBytes));
        self->field_00a9 = transitioning;
    }
    else if (CSF_AT(int, destinationBytes, 0x04) != -1)
    {
        S_004ad990_ApplyCombatDestination(
            self, nullptr, reinterpret_cast<int>(destinationBytes));
        self->field_00a9 = transitioning;
    }

    const uint32_t state = self->field_002c;
    if (state == 5 || state == 11 || self->field_0030 == 11)
    {
        uint8_t* trackedHandle = reinterpret_cast<uint8_t*>(self) + 0x4C;
        if (self->field_0054 & 0x80000000)
            trackedHandle = reinterpret_cast<uint8_t*>(self) + 0x34;
        S_004ad990_RefreshTrackedHandleTarget(
            self, nullptr, reinterpret_cast<int>(trackedHandle));
    }
    return hasCandidate;
}

HOOK(0x004C4890, S_004ad990_UpdateCoverAttack, "auto");
