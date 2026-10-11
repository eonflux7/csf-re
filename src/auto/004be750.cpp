#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, IsField1BCNonNegative, 0x00492B10, (void* self, void*));
CSF_FUNC(bool, __fastcall, AiCoverAttackTasks_TestMovementThreshold, 0x004AA4D0, (void* self, void* edx, float a2));
CSF_FUNC(int, __fastcall, UnnamedSetRangeParams_4BC3A0, 0x004BC3A0, (void* self, void* edx, float a2, float a3));
CSF_FUNC(int, __fastcall, AiCore_SetAngularRange, 0x004BC750, (void* self, void* edx, float a2, float a3, int a4));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GetStateValue, 0x0050DD30, (void* self, void* edx, int a2));

// Packet-listed vtable binding; the instruction at 0x004BE7D2 is a direct call.
static const auto g_MovementThresholdVtable = CSF_VTBL(0x004AA4D0);

static int __fastcall AiCore_ConfigureAttackRanges(char* self, void* /*edx*/, int target)
{
    void* link = self + 8;

    int linkedValue = GetLinkedObjectValue_4DB5C0(link, nullptr);
    void* linkedObject = linkedValue
        ? reinterpret_cast<void*>(static_cast<uint32_t>(linkedValue) - 4)
        : nullptr;

    bool configureAngles = static_cast<uint8_t>(
        IsField1BCNonNegative(CSF_AT(void*, linkedObject, 0x2C), nullptr)) == 0;

    linkedValue = GetLinkedObjectValue_4DB5C0(link, nullptr);
    linkedObject = linkedValue
        ? reinterpret_cast<void*>(static_cast<uint32_t>(linkedValue) - 4)
        : nullptr;

    bool movementThresholdMet = false;
    if (CSF_AT(int, CSF_AT(void*, linkedObject, 0x2C), 0x1D8) == 3)
    {
        linkedValue = GetLinkedObjectValue_4DB5C0(link, nullptr);
        void* movementObject = linkedValue
            ? reinterpret_cast<void*>(static_cast<uint32_t>(linkedValue) - 4)
            : nullptr;

        linkedValue = GetLinkedObjectValue_4DB5C0(
            reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(movementObject) + 8),
            nullptr);
        void* movementLinkedObject = linkedValue
            ? reinterpret_cast<void*>(static_cast<uint32_t>(linkedValue) - 4)
            : nullptr;

        void* owner = CSF_AT(void*, movementLinkedObject, 0x2C);
        void* movementData = CSF_AT(void*, owner, 0x3C);
        float threshold = CSF_AT(float, movementData, 0x34);
        movementThresholdMet = AiCoverAttackTasks_TestMovementThreshold(
            movementObject, nullptr, threshold);
    }

    void* range = reinterpret_cast<void*>(static_cast<uint32_t>(target) + 204);
    if (!movementThresholdMet && configureAngles)
    {
        void* globalState = reinterpret_cast<void*>(
            static_cast<uint32_t>(GetGlobalState_50D190()));
        int state = GetStateValue(globalState, nullptr, 9);

        switch (state)
        {
        case 0:
            AiCore_SetAngularRange(range, nullptr, -45.0f, -270.0f, 1140457472);
            break;
        case 1:
            AiCore_SetAngularRange(range, nullptr, 45.0f, 270.0f, 1140457472);
            break;
        case 2:
            AiCore_SetAngularRange(range, nullptr, -45.0f, -270.0f, 2139095039);
            break;
        case 3:
            AiCore_SetAngularRange(range, nullptr, -45.0f, -180.0f, 2139095039);
            break;
        case 4:
            AiCore_SetAngularRange(range, nullptr, 45.0f, 180.0f, 2139095039);
            break;
        case 5:
            AiCore_SetAngularRange(range, nullptr, -45.0f, -120.0f, 1140457472);
            UnnamedSetRangeParams_4BC3A0(range, nullptr, 2.0f, 4.0f);
            AiCore_SetAngularRange(range, nullptr, -45.0f, -270.0f, 1140457472);
            break;
        case 6:
            AiCore_SetAngularRange(range, nullptr, 45.0f, 120.0f, 1140457472);
            UnnamedSetRangeParams_4BC3A0(range, nullptr, 2.0f, 4.0f);
            AiCore_SetAngularRange(range, nullptr, 45.0f, 270.0f, 1140457472);
            break;
        case 7:
            AiCore_SetAngularRange(range, nullptr, -45.0f, -120.0f, 1140457472);
            UnnamedSetRangeParams_4BC3A0(range, nullptr, 2.0f, 4.0f);
            AiCore_SetAngularRange(range, nullptr, -45.0f, -270.0f, 2139095039);
            break;
        default:
            AiCore_SetAngularRange(range, nullptr, 45.0f, 120.0f, 1140457472);
            UnnamedSetRangeParams_4BC3A0(range, nullptr, 2.0f, 4.0f);
            AiCore_SetAngularRange(range, nullptr, 45.0f, 270.0f, 2139095039);
            break;
        }
    }

    return UnnamedSetRangeParams_4BC3A0(range, nullptr, 2.0f, 4.0f);
}

HOOK(0x004BE750, AiCore_ConfigureAttackRanges, "auto");
