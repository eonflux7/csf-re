#include <cstdint>
#include <cstddef>
#include <cmath>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

CSF_FUNC(uint32_t*, __fastcall, BuildQuerySegment, 0x0040FB60, (uint32_t* self, void* , const uint32_t* first, const uint32_t* second));
CSF_FUNC(int, __fastcall, GetSceneCandidateList, 0x00445F00, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GatherSceneCandidates, 0x00446DC0, (void* self, void* edx, int a2, void* a3));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, C_0083eb70_TestQuerySegment, 0x004E5A30, (void* self, void* edx, float* a3, uint32_t* a4));
CSF_FUNC(bool, __fastcall, C_0083eb70_HasCandidate, 0x004E5B40, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_0083eb70_FindCandidateIndex, 0x004E6E00, (void* self, void* edx, int a2));
CSF_FUNC(float*, __fastcall, Vec3_NormalizeHelper, 0x006AB830, (void* self, void* edx, float a2));

static uint8_t* __fastcall C_0083eb70_ProcessSceneCandidates(
    C_0083eb70* self, void* /*edx*/)
{
    auto* candidates = reinterpret_cast<S_00446340*>(
        GetSceneCandidateList(CSF_AT(void*, g_pCurrentScene, 0xB0), nullptr));

    void* manager = CSF_AT(void*, g_pCurrentScene, 0xB0);
    auto* owner = reinterpret_cast<C_00833908*>(self->field_0008);
    const int ownerPositionAddress =
        reinterpret_cast<int>(reinterpret_cast<uint8_t*>(owner) + 0x1C);

    if (static_cast<uint8_t>(GatherSceneCandidates(
            manager, nullptr, ownerPositionAddress, candidates)) == 0)
    {
        auto* result = CSF_AT(uint8_t*, g_pCurrentScene, 0xB0);
        result[232] = 0;
        return result;
    }

    alignas(8) uint8_t originBytes[0x1C];
    owner = reinterpret_cast<C_00833908*>(self->field_0008);
    ((void(__fastcall*)(void*, void*, float*))
        CSF_AT(void**, owner, 0)[0xA4 / 4])(
            owner, nullptr, reinterpret_cast<float*>(originBytes));

    uint32_t remaining = candidates->field_0008;
    if (remaining == 0)
    {
        auto* result = static_cast<uint8_t*>(g_pCurrentScene);
        CSF_AT(uint8_t, CSF_AT(void*, g_pCurrentScene, 0xB0), 232) = 0;
        return result;
    }

    uint32_t entryOffset = 0;
    do
    {
        auto* entry = reinterpret_cast<uint8_t*>(candidates->field_0000)
                    + entryOffset;
        owner = reinterpret_cast<C_00833908*>(self->field_0008);

        // The notification contains an embedded candidate descriptor at +0x14.
        alignas(8) uint8_t notificationBytes[0x14 + 0x20];
        uint8_t* descriptor = notificationBytes + 0x14;

        CSF_AT(void**, notificationBytes, 0) = CSF_VTBL(0x0083124C);
        CSF_AT(int, notificationBytes, 0x08) = -1;
        CSF_AT(int, notificationBytes, 0x0C) = -1;
        CSF_AT(int, notificationBytes, 0x10) = -1;
        CSF_AT(void**, descriptor, 0) = CSF_VTBL(0x0082D448);
        CSF_AT(int, descriptor, 0x04) = -1;
        CSF_AT(int, descriptor, 0x08) = -1;
        CSF_AT(float, descriptor, 0x0C) = 0.0f;
        CSF_AT(uint32_t, descriptor, 0x10) = 0;
        CSF_AT(uint32_t, descriptor, 0x14) = 0;
        CSF_AT(int, notificationBytes, 0x04) = -1;
        CSF_AT(float, descriptor, 0x18) = FLT_MAX;
        CSF_AT(uint32_t, descriptor, 0x1C) = 0;

        CSF_AT(int, notificationBytes, 0x08) = CSF_AT(int, entry, 0);
        if (CSF_AT(uint32_t, entry, 0x08) != owner->field_0020)
        {
            const uint32_t knownCount = self->field_0098;
            CSF_AT(uint32_t, descriptor, 0x04) = CSF_AT(uint32_t, entry, 0x08);
            CSF_AT(uint32_t, descriptor, 0x08) = CSF_AT(uint32_t, entry, 0x0C);
            CSF_AT(int, notificationBytes, 0x0C) = 1;

            bool alreadyKnown = false;
            if (knownCount != 0)
            {
                auto* known = reinterpret_cast<uint8_t*>(self->field_0090) + 4;
                for (uint32_t index = 0; index < knownCount; ++index, known += 12)
                {
                    if (CSF_AT(uint32_t, known, 0)
                        == CSF_AT(uint32_t, descriptor, 0x04))
                    {
                        alreadyKnown = true;
                        break;
                    }
                }
            }

            if (!alreadyKnown &&
                !C_0083eb70_HasCandidate(
                    self, nullptr, reinterpret_cast<int>(descriptor)))
            {
                CSF_AT(int, notificationBytes, 0x04) =
                    C_0083eb70_FindCandidateIndex(
                        self, nullptr, CSF_AT(int, notificationBytes, 0x08));

                if (CSF_AT(int, notificationBytes, 0x04) != -1)
                {
                    alignas(8) uint8_t directionBytes[0x0C];
                    float* direction = reinterpret_cast<float*>(directionBytes);
                    direction[0] = CSF_AT(float, entry, 0x10)
                                 - CSF_AT(float, originBytes, 0);
                    direction[1] = CSF_AT(float, entry, 0x14)
                                 - CSF_AT(float, originBytes, 4);
                    direction[2] = CSF_AT(float, entry, 0x18)
                                 - CSF_AT(float, originBytes, 8);
                    const float length = std::sqrt(
                        direction[2] * direction[2] +
                        direction[1] * direction[1] +
                        direction[0] * direction[0]);
                    Vec3_NormalizeHelper(direction, nullptr, length);
                    const float distance = length - CSF_AT(float, entry, 0x1C);

                    bool accepted = !(distance > g_fZero);
                    if (!accepted)
                    {
                        alignas(8) uint8_t endpointBytes[0x10];
                        CSF_AT(float, endpointBytes, 0) =
                            direction[0] * distance + CSF_AT(float, originBytes, 0);
                        CSF_AT(float, endpointBytes, 4) =
                            direction[1] * distance + CSF_AT(float, originBytes, 4);
                        CSF_AT(float, endpointBytes, 8) =
                            direction[2] * distance + CSF_AT(float, originBytes, 8);

                        alignas(8) uint8_t segmentBytes[0x20];
                        uint32_t* segment = BuildQuerySegment(
                            reinterpret_cast<uint32_t*>(segmentBytes), nullptr,
                            reinterpret_cast<const uint32_t*>(originBytes),
                            reinterpret_cast<const uint32_t*>(endpointBytes));
                        accepted = C_0083eb70_TestQuerySegment(
                            self, nullptr, reinterpret_cast<float*>(segment), nullptr);
                    }

                    if (accepted)
                    {
                        CSF_AT(uint32_t, descriptor, 0x1C) =
                            CSF_AT(uint32_t,
                                CSF_AT(void*, g_pCurrentScene, 0xB0), 8);
                        CSF_AT(uint32_t, descriptor, 0x0C) =
                            CSF_AT(uint32_t, entry, 0x10);
                        CSF_AT(uint32_t, descriptor, 0x10) =
                            CSF_AT(uint32_t, entry, 0x14);
                        CSF_AT(uint32_t, descriptor, 0x14) =
                            CSF_AT(uint32_t, entry, 0x18);

                        float* position = GetObjectPositionVector(
                            reinterpret_cast<void*>(self->field_0008), nullptr);
                        const float dx = position[0] - CSF_AT(float, entry, 0x10);
                        const float dy = position[1] - CSF_AT(float, entry, 0x14);
                        const float dz = position[2] - CSF_AT(float, entry, 0x18);
                        CSF_AT(float, descriptor, 0x18) =
                            dz * dz + dy * dy + dx * dx;
                        self->vSlotB4(reinterpret_cast<int>(notificationBytes));
                    }
                }
            }
        }

        CSF_AT(void**, descriptor, 0) = CSF_VTBL(0x0082D410);
        CSF_AT(void**, notificationBytes, 0) = CSF_VTBL(0x0082D410);
        entryOffset += 36;
        --remaining;
    } while (remaining != 0);

    CSF_AT(uint8_t, CSF_AT(void*, g_pCurrentScene, 0xB0), 232) = 0;
    return nullptr;
}

HOOK(0x004E79F0, C_0083eb70_ProcessSceneCandidates, "auto");
