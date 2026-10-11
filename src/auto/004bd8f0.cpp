#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, SceneObj_IsInactive, 0x0048E100, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneObject_PassesFirstCheck, 0x0048E120, (void* self, void* edx));
CSF_FUNC(char, __fastcall, CoverAttack_CheckState_4ADAE0, 0x004ADAE0, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, AiCore_CopyCandidate, 0x004B8FD0, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, PairLink_PrepareForRebind, 0x004E55A0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, DecrementByteCounter, 0x004E55B0, (void* self, void*));
CSF_FUNC(char, __fastcall, PairLink_TestEndpoints, 0x004E5B60, (void* self, void* edx, int a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static bool __fastcall S_004ad990_SelectBestCandidate(
    S_004ad990* self, void* /*edx*/, uint32_t* candidates, float* selected)
{
    bool selectedValid = false;
    PairLink_PrepareForRebind(
        CSF_AT(void*, reinterpret_cast<void*>(self->field_0000), 0x3C),
        nullptr);

    const uint32_t count = candidates[2];
    for (uint32_t index = 0; index < count; ++index)
    {
        uint32_t* candidate = CSF_AT(uint32_t**, candidates, 0)[index];

        if (CSF_AT(int32_t, candidate, 0x1C) < 0)
        {
            const int32_t selectedPriority = CSF_AT(int32_t, selected, 0x08);
            if ((selectedPriority == -1 ||
                 selectedPriority < CSF_AT(int32_t, candidate, 0x08)) &&
                !selectedValid)
            {
                CSF_AT(uint32_t, selected, 0x04) = CSF_AT(uint32_t, candidate, 0x04);
                CSF_AT(uint32_t, selected, 0x08) = CSF_AT(uint32_t, candidate, 0x08);
                CSF_AT(uint32_t, selected, 0x0C) = CSF_AT(uint32_t, candidate, 0x0C);
                CSF_AT(uint32_t, selected, 0x10) = CSF_AT(uint32_t, candidate, 0x10);
                CSF_AT(uint32_t, selected, 0x18) = CSF_AT(uint32_t, candidate, 0x18);
                CSF_AT(uint32_t, selected, 0x1C) = CSF_AT(uint32_t, candidate, 0x1C);
                CSF_AT(uint32_t, selected, 0x20) = CSF_AT(uint32_t, candidate, 0x20);
                CSF_AT(uint32_t, selected, 0x24) = CSF_AT(uint32_t, candidate, 0x24);
                CSF_AT(uint32_t, selected, 0x28) = CSF_AT(uint32_t, candidate, 0x28);
                CSF_AT(uint32_t, selected, 0x2C) = CSF_AT(uint32_t, candidate, 0x2C);
                CSF_AT(uint32_t, selected, 0x30) = CSF_AT(uint32_t, candidate, 0x30);
            }
            continue;
        }

        auto* handle = reinterpret_cast<uint8_t*>(candidate) + 0x14;
        void* sceneObject = reinterpret_cast<void*>(
            CHandleTable_Resolve(g_pCurrentScene, nullptr, reinterpret_cast<int>(handle)));
        if (!sceneObject)
            continue;

        const int32_t selectedPriority = CSF_AT(int32_t, selected, 0x08);
        if (selectedPriority != -1 &&
            selectedPriority >= CSF_AT(int32_t, candidate, 0x08))
            continue;

        void* checker = CSF_AT(
            void*, reinterpret_cast<void*>(self->field_0000), 0x34);
        if (!((bool(__fastcall*)(void*, void*, void*))
              CSF_AT(void**, checker, 0)[0xB4 / 4])(checker, nullptr, handle))
            continue;

        const bool closer =
            CSF_AT(float, selected, 0x2C) > CSF_AT(float, candidate, 0x2C);

        const bool sceneEligible =
            static_cast<uint8_t>(SceneObj_IsInactive(sceneObject, nullptr)) == 0 &&
            static_cast<uint8_t>(SceneObject_PassesFirstCheck(sceneObject, nullptr)) == 0;

        const bool candidateValid =
            CoverAttack_CheckState_4ADAE0(
                self, nullptr, reinterpret_cast<int>(handle)) != 0 &&
            sceneEligible;

        if (!selectedValid || (candidateValid && closer))
        {
            if (PairLink_TestEndpoints(
                    CSF_AT(void*, reinterpret_cast<void*>(self->field_0000), 0x3C),
                    nullptr,
                    reinterpret_cast<int>(
                        reinterpret_cast<uint8_t*>(candidate) + 0x0C)))
            {
                AiCore_CopyCandidate(selected, nullptr, candidate);
                selectedValid = candidateValid;
            }
        }
    }

    DecrementByteCounter(
        CSF_AT(void*, reinterpret_cast<void*>(self->field_0000), 0x3C),
        nullptr);
    return selectedValid;
}

HOOK(0x004BD8F0, S_004ad990_SelectBestCandidate, "auto");
