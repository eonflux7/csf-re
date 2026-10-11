#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_LevelScaleFactor, 0x0082E048);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, IsDistinctNonSentinelValue, 0x0048E0D0, (void* self, void* , std::int32_t candidate));
CSF_FUNC(int, __fastcall, IsStateCompatible, 0x0048EF90, (void* self, void* , int requestedState));
CSF_FUNC(int, __fastcall, AiCore_IsTargetRecordValid, 0x004B2290, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, AiCore_BuildTargetReference, 0x004C40F0, (void* self, void* edx, uint32_t* arg0, int a3, int a4));
CSF_FUNC(char*, __fastcall, Task_GetTargetNameBuffer, 0x004D1510, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GameConfig_IsResolvedEntryActive, 0x00755880, (int entry));
CSF_FUNC(int, __cdecl, GameConfig_IsSceneObjectRemovable, 0x007558C0, (int objectType));

static uint32_t* __fastcall S_004bd310_GetIndirectTarget(
    S_004bd310* self, void* /*edx*/, uint32_t* output, int handle)
{
    uint32_t targetId = static_cast<uint32_t>(-1);
    uint32_t targetGeneration = static_cast<uint32_t>(-1);
    uint32_t targetTime = CSF_AT(
        uint32_t, CSF_AT(void*, g_pCurrentScene, 0xB0), 8);
    alignas(8) uint8_t referenceBytes[0x24];

    if (CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0000), 0x20)
        == CSF_AT(uint32_t, reinterpret_cast<void*>(handle), 4))
        goto Finish;

    {
        void* entry = reinterpret_cast<void*>(
            CHandleTable_Resolve(g_pCurrentScene, nullptr, handle));
        if (!entry)
            goto Finish;

        if (CSF_AT(uint32_t, entry, 0x170) & 0x2000)
        {
            if (!static_cast<uint8_t>(IsStateCompatible(
                    reinterpret_cast<void*>(self->field_0000), nullptr,
                    CSF_AT(int, entry, 0x78))))
            {
                targetId = CSF_AT(uint32_t, entry, 0x20);
                targetGeneration = CSF_AT(uint32_t, entry, 0x24);
                goto Finish;
            }

            void* actor = CSF_AT(void*, entry, 0x34);
            if (!actor)
                goto Finish;
            if (((int (__fastcall*)(void*, void*))
                    CSF_AT(void**, actor, 0)[0x98 / 4])(actor, nullptr) != 1)
                goto Finish;

            auto* record = reinterpret_cast<S_004ad990*>(
                static_cast<uint8_t*>(CSF_AT(
                    void*, CSF_AT(void*, entry, 0x34), 0x30C)) + 0x5DC);
            if (!static_cast<uint8_t>(
                    AiCore_IsTargetRecordValid(record, nullptr)))
                goto Finish;

            int recordState = static_cast<int32_t>(record->field_0054);
            uint8_t* recordHandle = reinterpret_cast<uint8_t*>(record)
                + (recordState < 0 ? 0x34 : 0x4C);
            if (CSF_AT(int, recordHandle, 8) < 0)
                goto Finish;

            recordHandle = reinterpret_cast<uint8_t*>(record)
                + (recordState < 0 ? 0x34 : 0x4C);
            if (CSF_AT(uint32_t, recordHandle, 4)
                == CSF_AT(uint32_t,
                    reinterpret_cast<void*>(self->field_0000), 0x20))
                goto Finish;
            if (recordState < 0)
                goto Finish;

            float recordTime;
            std::memcpy(&recordTime, &record->field_0058, sizeof(recordTime));
            if (!(recordTime + g_LevelScaleFactor >
                  CSF_AT(float, CSF_AT(void*, g_pCurrentScene, 0xB0), 8)))
                goto Finish;

            uint8_t* recentHandle = reinterpret_cast<uint8_t*>(record) + 0x4C;
            void* recentEntry = reinterpret_cast<void*>(CHandleTable_Resolve(
                g_pCurrentScene, nullptr, reinterpret_cast<int>(recentHandle)));
            if (!recentEntry)
                goto Finish;

            uint32_t* reference;
            if (CSF_AT(uint32_t, recentEntry, 0x170) & 0x2000)
            {
                if (static_cast<uint8_t>(IsStateCompatible(
                        reinterpret_cast<void*>(self->field_0000), nullptr,
                        CSF_AT(int, recentEntry, 0x78)))
                    && record->field_002c == 10)
                {
                    reference = AiCore_BuildTargetReference(
                        self, nullptr, reinterpret_cast<uint32_t*>(referenceBytes),
                        reinterpret_cast<int>(
                            static_cast<uint8_t*>(recentEntry) + 0x1C), 2);
                    targetId = reference[1];
                    targetGeneration = reference[2];
                    targetTime = record->field_0058;
                    goto Finish;
                }

                reference = reinterpret_cast<uint32_t*>(
                    reinterpret_cast<uint8_t*>(record)
                    + (static_cast<int32_t>(record->field_0054) < 0
                        ? 0x34 : 0x4C));
            }
            else
            {
                if (!static_cast<uint8_t>(GameConfig_IsSceneObjectRemovable(
                        CSF_AT(int, recentEntry, 0x50))))
                {
                    targetTime = record->field_0058;
                    goto Finish;
                }

                reference = AiCore_BuildTargetReference(
                    self, nullptr, reinterpret_cast<uint32_t*>(referenceBytes),
                    reinterpret_cast<int>(
                        static_cast<uint8_t*>(recentEntry) + 0x1C), 3);
            }

            targetId = reference[1];
            targetGeneration = reference[2];
            targetTime = record->field_0058;
        }
        else
        {
            if (!static_cast<uint8_t>(GameConfig_IsResolvedEntryActive(
                    CSF_AT(int, entry, 0x50))))
                goto Finish;

            void* task = CSF_AT(void*, entry, 0x34);
            if (!task)
                goto Finish;
            if (!*Task_GetTargetNameBuffer(task, nullptr))
                goto Finish;

            char* buffer = Task_GetTargetNameBuffer(
                CSF_AT(void*, entry, 0x34), nullptr);
            char* taskHandle = buffer + 16;
            if (CSF_AT(int, buffer, 0x18) < 0)
                goto Finish;

            void* taskTarget = reinterpret_cast<void*>(CHandleTable_Resolve(
                g_pCurrentScene, nullptr, reinterpret_cast<int>(taskHandle)));
            if (!taskTarget)
                goto Finish;
            if (!static_cast<uint8_t>(IsDistinctNonSentinelValue(
                    reinterpret_cast<void*>(self->field_0000), nullptr,
                    CSF_AT(int32_t, taskTarget, 0x78))))
                goto Finish;

            targetId = CSF_AT(uint32_t, taskHandle, 4);
            targetGeneration = CSF_AT(uint32_t, taskHandle, 8);
        }
    }

Finish:
    output[1] = targetId;
    *reinterpret_cast<void***>(output) = CSF_VTBL(0x0082D448);
    output[2] = targetGeneration;
    output[3] = targetTime;
    return output;
}

HOOK(0x004BD370, S_004bd310_GetIndirectTarget, "auto");
