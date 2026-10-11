#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, S_00413570_GrowEventRecords, 0x00413570, (S_00413570* self, void* , int additionalCapacity));
CSF_FUNC(char, __fastcall, AiCoverAttackTasks_TestAttackPosition, 0x004B2E20, (void* self, void* edx, float* a2, int a3));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_TestCoverPosition, 0x004E5C70, (C_0083e9d0* self, void* , float* arg0, int a2, int a4));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Retain the symbolic vtable reference identified by the packet.
[[maybe_unused]] static const auto g_EventRecordsVtable = CSF_VTBL(0x00413570);

static uint32_t* __fastcall S_00413570_CollectValidScenePositions(
    S_00413570* self, void* /*edx*/, int linkedObjectHandle)
{
    CSF_AT(uint32_t, self, 0x28) = 0;

    auto* scene = static_cast<uint8_t*>(g_pCurrentScene);
    auto* sceneList = scene + 0x60;
    const uint32_t sceneCount = CSF_AT(uint32_t, scene, 0x68);

    for (uint32_t index = 0; index < sceneCount; ++index)
    {
        void* candidate = CSF_AT(void*, CSF_AT(void*, sceneList, 0), index * 4);
        if (!candidate)
            continue;

        int linkedValue = GetLinkedObjectValue_4DB5C0(
            reinterpret_cast<void*>(linkedObjectHandle), nullptr);
        auto* actor = linkedValue
            ? reinterpret_cast<uint8_t*>(static_cast<uint32_t>(linkedValue)) - 4
            : nullptr;

        bool accepted = false;
        if (!CSF_AT(uint32_t, actor, 0x6C8) ||
            CSF_AT(int, actor, 0x6C0) == -1 ||
            CSF_AT(int, actor, 0x6C4) == -1)
        {
            linkedValue = GetLinkedObjectValue_4DB5C0(
                reinterpret_cast<void*>(linkedObjectHandle), nullptr);
            actor = linkedValue
                ? reinterpret_cast<uint8_t*>(static_cast<uint32_t>(linkedValue)) - 4
                : nullptr;

            void* linkedOwner = CSF_AT(void*, actor, 0x2C);
            auto* coverTester = CSF_AT(C_0083e9d0*, linkedOwner, 0x3C);
            accepted = static_cast<uint8_t>(
                AiCoverAttackTasks_TestCoverPosition(
                    coverTester, nullptr,
                    reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(self) + 0x14),
                    reinterpret_cast<int>(static_cast<uint8_t*>(candidate) + 0x1C),
                    -1)) != 0;
        }
        else
        {
            linkedValue = GetLinkedObjectValue_4DB5C0(
                reinterpret_cast<void*>(linkedObjectHandle), nullptr);
            actor = linkedValue
                ? reinterpret_cast<uint8_t*>(static_cast<uint32_t>(linkedValue)) - 4
                : nullptr;

            if (CSF_AT(uint32_t, actor, 0x6C8) &&
                CSF_AT(int, actor, 0x6C0) != -1 &&
                CSF_AT(int, actor, 0x6C4) != -1)
            {
                accepted = AiCoverAttackTasks_TestAttackPosition(
                    actor, nullptr, reinterpret_cast<float*>(actor + 0x6B4),
                    reinterpret_cast<int>(static_cast<uint8_t*>(candidate) + 0x1C)) != 0;
            }
        }

        if (accepted)
        {
            auto* records = reinterpret_cast<uint8_t*>(self) + 0x20;
            if (CSF_AT(uint32_t, self, 0x28) >= CSF_AT(uint32_t, self, 0x24))
            {
                S_00413570_GrowEventRecords(
                    reinterpret_cast<S_00413570*>(records), nullptr,
                    CSF_AT(int, records, 0x0C));
            }

            const uint32_t recordIndex = CSF_AT(uint32_t, records, 0x08);
            auto* recordData = CSF_AT(uint8_t*, records, 0);
            CSF_AT(uint32_t, records, 0x08) = recordIndex + 1;
            auto* record = recordData + 12 * recordIndex;
            CSF_AT(uint32_t, record, 0x04) = CSF_AT(uint32_t, candidate, 0x20);
            CSF_AT(uint32_t, record, 0x08) = CSF_AT(uint32_t, candidate, 0x24);
            self->field_0004 = 3;
        }
    }

    return sceneCount
        ? reinterpret_cast<uint32_t*>(sceneCount)
        : reinterpret_cast<uint32_t*>(sceneList);
}

HOOK(0x004C3B20, S_00413570_CollectValidScenePositions, "auto");
