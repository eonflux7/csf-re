#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CGuiControl_ProcessDisplayFlags_453660, 0x00453660, (CGuiControl* self, void* , S_0041e5f0* a2, float* a3, int* a4, float a5));
CSF_FUNC(int, __fastcall, IsStateCompatible, 0x0048EF90, (void* self, void* , int requestedState));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

CSF_GLOBAL(float, g_fReturnedConstant_0082E030, 0x0082E030);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static char __fastcall S_004b1050_HasCompatibleNearbyObject(
    S_004b1050* self, void* /*edx*/, int linkedObject)
{
    alignas(8) uint8_t filterBytes[0x34];
    alignas(8) uint8_t queryBytes[0x10];
    alignas(8) uint8_t resultBytes[0x18] = {};

    CSF_AT(uint32_t, queryBytes, 0x00) = self->field_0014;
    CSF_AT(float, queryBytes, 0x04) =
        self->field_0018 + g_fReturnedConstant_0082E030;
    CSF_AT(uint32_t, queryBytes, 0x08) = self->field_001c;
    CSF_AT(float, queryBytes, 0x0C) = 1000.0f;

    CSF_AT(int, resultBytes, 0x0C) = 10;
    CSF_AT(int, filterBytes, 0x1C) = 10;
    CSF_AT(int, filterBytes, 0x04) = -1;
    CSF_AT(int, filterBytes, 0x08) = -1;
    CSF_AT(int, filterBytes, 0x0C) = -1;
    CSF_AT(int, filterBytes, 0x2C) = -1;
    CSF_AT(int, filterBytes, 0x10) = 0;
    CSF_AT(int, filterBytes, 0x14) = 0;
    CSF_AT(int, filterBytes, 0x18) = 0;
    CSF_AT(int, filterBytes, 0x20) = 0;
    CSF_AT(int, filterBytes, 0x24) = 0;
    CSF_AT(int, filterBytes, 0x28) = 0;
    CSF_AT(int, filterBytes, 0x30) = 0;
    CSF_AT(int, filterBytes, 0x00) = 8466;

    CGuiControl_ProcessDisplayFlags_453660(
        static_cast<CGuiControl*>(g_pCurrentScene), nullptr,
        reinterpret_cast<S_0041e5f0*>(resultBytes),
        reinterpret_cast<float*>(queryBytes),
        reinterpret_cast<int*>(filterBytes), 0.0f);

    CSF_AT(uint32_t, resultBytes, 0x10) =
        CSF_AT(uint32_t, resultBytes, 0x08);

    uint32_t index = 0;
    if (CSF_AT(uint32_t, resultBytes, 0x08) != 0)
    {
        do
        {
            void* candidate = CSF_AT(void**, resultBytes, 0x00)[index];
            int linkedValue = GetLinkedObjectValue_4DB5C0(
                reinterpret_cast<void*>(static_cast<uintptr_t>(linkedObject)),
                nullptr);

            void* linkedBase = linkedValue
                ? reinterpret_cast<void*>(static_cast<uintptr_t>(linkedValue) - 4)
                : nullptr;

            int requestedState = CSF_AT(int, candidate, 0x78);
            void* stateController = CSF_AT(void*, linkedBase, 0x2C);
            if (static_cast<uint8_t>(
                    IsStateCompatible(stateController, nullptr, requestedState)) != 0)
            {
                CSF_AT(char, resultBytes, 0x17) = 1;
                break;
            }
            ++index;
        } while (index < CSF_AT(uint32_t, resultBytes, 0x10));
    }

    if (CSF_AT(void*, filterBytes, 0x10))
    {
        Mem_Free(CSF_AT(void*, filterBytes, 0x10));
        CSF_AT(uint32_t, filterBytes, 0x10) = 0;
    }
    if (CSF_AT(void*, resultBytes, 0x00))
        Mem_Free(CSF_AT(void*, resultBytes, 0x00));

    return CSF_AT(char, resultBytes, 0x17);
}

HOOK(0x004B1050, S_004b1050_HasCompatibleNearbyObject, "auto");
