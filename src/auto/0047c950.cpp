#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(float*, __stdcall, AiAttackTasks_GenerateRandomPosition, 0x0047C7A0, (float* a1, int a2));
CSF_FUNC(uint32_t*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GetStateValue, 0x0050DD30, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, AppendThreeDwordRecord_75A3A0, 0x0075A3A0, (C_0085e7b8* self, void* , const uint32_t* record));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall AiAttackTasks_AppendRandomAttackPositions(
    void* self, void* /*edx*/, int attackData, void* handles,
    C_0085e7b8* output)
{
    alignas(8) uint8_t trailingPositionBytes[0xC];
    alignas(8) uint8_t adjustedPositionBytes[0xC];
    alignas(8) uint8_t leadingPositionBytes[0xC];
    alignas(8) uint8_t targetPositionBytes[0x10];

    auto* trailingPosition = reinterpret_cast<float*>(trailingPositionBytes);
    auto* leadingPosition = reinterpret_cast<float*>(leadingPositionBytes);
    auto* targetPosition = reinterpret_cast<uint32_t*>(targetPositionBytes);

    const uint32_t begin = CSF_AT(uint32_t, handles, 0x4);
    int handleCount = 0;
    if (begin)
    {
        const int32_t byteCount =
            static_cast<int32_t>(CSF_AT(uint32_t, handles, 0x8) - begin);
        handleCount = byteCount / 12;
    }

    if (CSF_AT(uint32_t, self, 0x80) && handleCount)
    {
        // The iteration counter shares the final stack buffer's fourth dword.
        targetPosition[3] = 0;
        do
        {
            float* position = AiAttackTasks_GenerateRandomPosition(
                leadingPosition, attackData);
            AppendThreeDwordRecord_75A3A0(
                output, nullptr, reinterpret_cast<const uint32_t*>(position));

            void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
            const int index = GetStateValue(state, nullptr, handleCount);
            void* scene = g_pCurrentScene;
            const uint32_t handle =
                CSF_AT(uint32_t, handles, 0x4) + 12 * static_cast<uint32_t>(index);
            auto* target = reinterpret_cast<C_008342e8*>(
                CHandleTable_Resolve(scene, nullptr, static_cast<int>(handle)));

            if (target)
            {
                const uint32_t* objectPosition =
                    GetObjectPositionVector(target, nullptr);
                targetPosition[0] = objectPosition[0];
                targetPosition[1] = objectPosition[1];
                targetPosition[2] = objectPosition[2];

                if (target->field_0170 & 0x2000)
                {
                    auto* adjustedPosition =
                        reinterpret_cast<uint32_t*>(adjustedPositionBytes);
                    objectPosition =
                        reinterpret_cast<uint32_t*(__fastcall*)(
                            void*, void*, uint32_t*)>(target->vtbl[0xA4 / 4])(
                                target, nullptr, adjustedPosition);
                    targetPosition[0] = objectPosition[0];
                    targetPosition[1] = objectPosition[1];
                    targetPosition[2] = objectPosition[2];
                }

                AppendThreeDwordRecord_75A3A0(output, nullptr, targetPosition);
            }

            position = AiAttackTasks_GenerateRandomPosition(
                trailingPosition, attackData);
            AppendThreeDwordRecord_75A3A0(
                output, nullptr, reinterpret_cast<const uint32_t*>(position));

            const uint32_t requestedCount = CSF_AT(uint32_t, self, 0x80);
            ++targetPosition[3];
            if (targetPosition[3] >= requestedCount)
                return static_cast<int>(targetPosition[3]);
        } while (true);
    }

    void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
    uint32_t remaining =
        static_cast<uint32_t>(GetStateValue(state, nullptr, 4)) + 2;
    int result = static_cast<int>(remaining);
    if (remaining)
    {
        do
        {
            float* position = AiAttackTasks_GenerateRandomPosition(
                trailingPosition, attackData);
            result = AppendThreeDwordRecord_75A3A0(
                output, nullptr, reinterpret_cast<const uint32_t*>(position));
            --remaining;
        } while (remaining);
    }
    return result;
}

HOOK(0x0047C950, AiAttackTasks_AppendRandomAttackPositions, "auto");
