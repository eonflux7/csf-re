#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(double, __fastcall, C_0082f9f8_ComputeNavigationMetric, 0x00420FD0, (void* self, void* edx, float* a2, unsigned int a3));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiTaskStates_ApplyTaskState, 0x004DB140, (void* self, void* edx, int a2));
CSF_FUNC(unsigned int, __fastcall, DispatchWithArgBlock, 0x004DD060, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

static int* __fastcall C_0082f9f8_SetNavigationDestination(
    C_0082f9f8* self, void* /*edx*/, int* destination, char mode)
{
    auto* context = reinterpret_cast<uint8_t*>(self->field_0008);
    CSF_AT(char, context, 517) = mode;

    context = reinterpret_cast<uint8_t*>(self->field_0008);
    auto* position = reinterpret_cast<uint32_t*>(
        GetObjectPositionVector(CSF_AT(void*, context, 20), nullptr));

    const uint32_t positionX = position[0];
    context = reinterpret_cast<uint8_t*>(self->field_0008);
    CSF_AT(uint32_t, context, 524) = positionX;
    CSF_AT(uint32_t, context, 528) = position[1];
    CSF_AT(uint32_t, context, 532) = position[2];

    context = reinterpret_cast<uint8_t*>(self->field_0008);
    CSF_AT(uint32_t, context, 536) = CSF_AT(uint32_t, context, 524);
    CSF_AT(uint32_t, context, 540) = CSF_AT(uint32_t, context, 528);
    CSF_AT(uint32_t, context, 544) = CSF_AT(uint32_t, context, 532);

    context = reinterpret_cast<uint8_t*>(self->field_0008);
    if (GetCachedObjectProvider(context + 604, nullptr))
    {
        context = reinterpret_cast<uint8_t*>(self->field_0008);
        auto* objectIdentity =
            static_cast<uint8_t*>(CSF_AT(void*, context, 20)) + 28;

        uint32_t argumentBlock[7];
        argumentBlock[0] = 2;
        argumentBlock[1] = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
        argumentBlock[2] = CSF_AT(uint32_t, objectIdentity, 4);
        argumentBlock[3] = CSF_AT(uint32_t, objectIdentity, 8);
        argumentBlock[4] = static_cast<uint32_t>(destination[0]);
        argumentBlock[5] = static_cast<uint32_t>(destination[1]);
        argumentBlock[6] = static_cast<uint32_t>(destination[2]);

        auto* manager = reinterpret_cast<uint8_t*>(GetValue8B0554());
        auto* dispatcher =
            static_cast<uint8_t*>(CSF_AT(void*, manager, 8152)) + 52;
        DispatchWithArgBlock(dispatcher, nullptr, argumentBlock);

        context = reinterpret_cast<uint8_t*>(self->field_0008);
        CSF_AT(uint32_t, context, 624) = 0;

        context = reinterpret_cast<uint8_t*>(self->field_0008);
        CSF_AT(int, context, 548) = destination[0];
        CSF_AT(int, context, 552) = destination[1];
        CSF_AT(int, context, 556) = destination[2];

        context = reinterpret_cast<uint8_t*>(self->field_0008);
        const double metric = C_0082f9f8_ComputeNavigationMetric(
            self, nullptr, reinterpret_cast<float*>(context + 524),
            CSF_AT(uint32_t, context, 624));
        context = reinterpret_cast<uint8_t*>(self->field_0008);
        CSF_AT(float, context, 572) = static_cast<float>(metric);

        context = reinterpret_cast<uint8_t*>(self->field_0008);
        CSF_AT(uint32_t, context, 24) |= 2u;
        return reinterpret_cast<int*>(AiTaskStates_ApplyTaskState(
            self, nullptr, reinterpret_cast<int>(&self->field_0014)));
    }

    if (CSF_AT(uint8_t, self, 0x7C))
    {
        context = reinterpret_cast<uint8_t*>(self->field_0008);
        void* object = CSF_AT(void*, context, 20);
        void* controller = CSF_AT(void*, object, 72);

        if (((bool(__fastcall*)(void*, void*, int))
                 CSF_AT(void**, controller, 0)[36 / 4])(
                controller, nullptr, 1))
        {
            context = reinterpret_cast<uint8_t*>(self->field_0008);
            object = CSF_AT(void*, context, 20);
            ((void(__fastcall*)(void*, void*, int, int, int, int))
                 CSF_AT(void**, object, 0)[140 / 4])(
                object, nullptr, 0, 0, -1, 0);
        }

        context = reinterpret_cast<uint8_t*>(self->field_0008);
        if (!CSF_AT(uint32_t, context, 28))
            CSF_AT(uint8_t, context, 628) = 0;

        context = reinterpret_cast<uint8_t*>(self->field_0008);
        auto* secondaryObject =
            static_cast<uint8_t*>(CSF_AT(void*, context, 20)) + 4;
        ((void(__fastcall*)(void*, void*))
             CSF_AT(void**, secondaryObject, 0)[16 / 4])(
            secondaryObject, nullptr);

        context = reinterpret_cast<uint8_t*>(self->field_0008);
        auto* flags = reinterpret_cast<int*>(context + 24);
        *flags |= 6;
        CSF_AT(uint8_t, self, 0x7C) = 0;
        return flags;
    }

    context = reinterpret_cast<uint8_t*>(self->field_0008);
    CSF_AT(uint32_t, context, 24) = 0;
    CSF_AT(uint8_t, self, 0x7C) = 0;
    return reinterpret_cast<int*>(context);
}

HOOK(0x00422A40, C_0082f9f8_SetNavigationDestination, "auto");
