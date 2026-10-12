#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int32_t, __cdecl, GetGlobal8afc78, 0x0042D6D0, ());
CSF_FUNC(int, __fastcall, CHud_LookupValueByKey, 0x00431F10, (CHud* self, void* , int key));
CSF_FUNC(int, __fastcall, InitPoseScratchBuffer, 0x004582E0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, S_00458980_FindPoseIndex, 0x00458980,
         (void* self, void* edx, float* a2));
CSF_FUNC(char, __fastcall, S_00458980_RemovePoseByKey, 0x00458ED0,
         (void* self, void* edx, float* a2));
CSF_FUNC(void, __fastcall, S_00459ec0_AppendPose, 0x0045A030,
         (void* self, void* edx, void* a3));

CSF_GLOBAL(float, g_MissingMetricValue, 0x0082D4C4);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

// Retain the packet's listed reference; this address is also the lookup callee.
static const auto kPoseLookupReference = CSF_VTBL(0x00458980);

static void __fastcall S_00458980_ApplyPoseToTarget(
    S_00458980* self, void* /*edx*/, float* pose)
{
    const uint32_t targetsBegin = self->field_002c;
    if (!targetsBegin ||
        static_cast<int32_t>(self->field_0030 - targetsBegin) / 20 == 0)
        return;

    const int key = CSF_AT(int, pose, 0);
    CHud* hud = reinterpret_cast<CHud*>(GetGlobal8afc78());
    if (!CHud_LookupValueByKey(hud, nullptr, key))
    {
        S_00458980_RemovePoseByKey(self, nullptr, pose + 7);
        return;
    }

    int poseIndex = S_00458980_FindPoseIndex(self, nullptr, pose + 7);
    if (poseIndex == -1)
    {
        S_00459ec0_AppendPose(
            reinterpret_cast<uint8_t*>(self) + 0x18, nullptr, pose);

        const uint32_t posesBegin = self->field_001c;
        poseIndex = posesBegin
            ? static_cast<int32_t>(
                  CSF_AT(uint32_t, self, 0x20) - posesBegin) / 40 - 1
            : -1;
    }
    else
    {
        const uint32_t poseOffset = 40 * static_cast<uint32_t>(poseIndex);
        InitPoseScratchBuffer(
            reinterpret_cast<void*>(self->field_001c + poseOffset), nullptr);

        std::memcpy(
            reinterpret_cast<void*>(self->field_001c + poseOffset),
            pose, 0x28);
    }

    void* storedPose = reinterpret_cast<void*>(
        self->field_001c + 40 * static_cast<uint32_t>(poseIndex));
    float metric = CSF_AT(float, storedPose, 0x08);
    if (metric == g_fZero)
        metric = g_MissingMetricValue;
    CSF_AT(float, storedPose, 0x08) = metric;
}

HOOK(0x0045A6E0, S_00458980_ApplyPoseToTarget, "auto");
