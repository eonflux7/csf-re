#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CoverAttack_SetStance, 0x00492720, (void* self, void* , int newStance));
CSF_FUNC(int, __fastcall, C_00834050_SetActionMode, 0x00492760, (void* self, void* , int newActionMode));
CSF_FUNC(double, __fastcall, GetLinkedPositionWeight, 0x004AD850, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiCore_ApplyLinkedPosition, 0x004BAD20, (void* self, void* edx, int a2, char a3, uint32_t* a4, int a5, char a6));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, UnnamedApply_4DC180, 0x004DC180, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

static uint8_t* AiCore_GetLinkedObjectBase(C_0085e7b8* context)
{
    const int linkedValue = GetLinkedObjectValue_4DB5C0(context, nullptr);
    return linkedValue ? reinterpret_cast<uint8_t*>(linkedValue) - 4 : nullptr;
}

static int __stdcall AiCore_UpdateLinkedPosition(C_0085e7b8* context)
{
    uint8_t* linked = AiCore_GetLinkedObjectBase(context);
    if (CSF_AT(int, linked, 0x608) != -1 ||
        CSF_AT(int, linked, 0x60C) != -1)
    {
        linked = AiCore_GetLinkedObjectBase(context);
        const int mode = CSF_AT(int, linked, 0x604);
        bool setStance = mode == 3;
        if (!setStance && mode == 2)
        {
            const int action = CSF_AT(int, linked, 0x60C);
            setStance = action == 12 || action == 8 ||
                        action == 9 || action == 7;
        }

        if (setStance)
        {
            linked = AiCore_GetLinkedObjectBase(context);
            CoverAttack_SetStance(CSF_AT(void*, linked, 0x2C), nullptr, 3);

            linked = AiCore_GetLinkedObjectBase(context);
            C_00834050_SetActionMode(CSF_AT(void*, linked, 0x2C), nullptr, 1);
        }
    }

    uint8_t* positionSource = AiCore_GetLinkedObjectBase(context) + 0x64C;
    uint8_t* positionDestination =
        reinterpret_cast<uint8_t*>(GetLinkedObjectField10(context, nullptr)) + 0x14;
    CSF_AT(uint32_t, positionDestination, 0) = CSF_AT(uint32_t, positionSource, 0);
    CSF_AT(uint32_t, positionDestination, 4) = CSF_AT(uint32_t, positionSource, 4);
    CSF_AT(uint32_t, positionDestination, 8) = CSF_AT(uint32_t, positionSource, 8);

    linked = AiCore_GetLinkedObjectBase(context);
    UnnamedApply_4DC180(context, nullptr, reinterpret_cast<int>(linked + 0x1B0));

    uint8_t* weightObject = AiCore_GetLinkedObjectBase(context);
    uint8_t* stateObject = AiCore_GetLinkedObjectBase(context);
    const float weight =
        static_cast<float>(GetLinkedPositionWeight(weightObject + 0x604, nullptr));
    int weightBits;
    std::memcpy(&weightBits, &weight, sizeof(weightBits));

    uint32_t* position = reinterpret_cast<uint32_t*>(
        reinterpret_cast<uint8_t*>(GetLinkedObjectField10(context, nullptr)) + 0x14);
    void* provider = GetCachedObjectProvider(context, nullptr);

    return AiCore_ApplyLinkedPosition(
        stateObject + 0x1B0, nullptr, reinterpret_cast<int>(provider),
        1, position, weightBits, 1);
}

HOOK(0x004BF030, AiCore_UpdateLinkedPosition, "auto");
