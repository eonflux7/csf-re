#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CPlayerCharacter_ApplyTransitionValue, 0x00492500, (void* self, void* edx, float a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static int __fastcall C_00833b88_SetTransitionState(
    C_00833b88* self, void* /*edx*/, int state)
{
    if (CSF_AT(int, self, 0x1C4) == state)
        return state;

    alignas(8) uint8_t vectorBytes[0x18] = {};
    bool applyTransition = true;
    float transitionValue = 0.0f;

    switch (state)
    {
    case 0:
    {
        auto* parameters =
            reinterpret_cast<C_00833908*>(self->field_002c);

        const float z = reinterpret_cast<const float&>(parameters->field_0168);
        const float y = reinterpret_cast<const float&>(parameters->field_0164);
        CSF_AT(float, vectorBytes, 0x00) =
            reinterpret_cast<const float&>(parameters->field_0160);
        CSF_AT(float, vectorBytes, 0x04) = y;
        CSF_AT(int, self, 0x1C4) = 0;
        CSF_AT(float, vectorBytes, 0x08) = z;
        transitionValue = reinterpret_cast<const float&>(parameters->field_00e4);
        break;
    }

    case 1:
    {
        void* globals = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
        CSF_AT(float, vectorBytes, 0x0C) = CSF_AT(float, globals, 0xA40);
        CSF_AT(float, vectorBytes, 0x10) = CSF_AT(float, globals, 0xA44);
        CSF_AT(float, vectorBytes, 0x14) = CSF_AT(float, globals, 0xA48);

        auto* parameters =
            reinterpret_cast<C_00833908*>(self->field_002c);
        const float z = CSF_AT(float, vectorBytes, 0x14) *
            reinterpret_cast<const float&>(parameters->field_0168);
        const float y = CSF_AT(float, vectorBytes, 0x10) *
            reinterpret_cast<const float&>(parameters->field_0164);
        const float x = CSF_AT(float, vectorBytes, 0x0C) *
            reinterpret_cast<const float&>(parameters->field_0160);

        CSF_AT(int, self, 0x1C4) = 1;
        CSF_AT(float, vectorBytes, 0x00) = x;
        CSF_AT(float, vectorBytes, 0x04) = y;
        CSF_AT(float, vectorBytes, 0x08) = z;
        transitionValue = reinterpret_cast<const float&>(parameters->field_00e8);
        break;
    }

    case 2:
    {
        void* globals = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
        CSF_AT(float, vectorBytes, 0x0C) = CSF_AT(float, globals, 0xA4C);
        CSF_AT(float, vectorBytes, 0x10) = CSF_AT(float, globals, 0xA50);
        CSF_AT(float, vectorBytes, 0x14) = CSF_AT(float, globals, 0xA54);

        auto* parameters =
            reinterpret_cast<C_00833908*>(self->field_002c);
        const float z = CSF_AT(float, vectorBytes, 0x14) *
            reinterpret_cast<const float&>(parameters->field_0168);
        const float y = CSF_AT(float, vectorBytes, 0x10) *
            reinterpret_cast<const float&>(parameters->field_0164);
        const float x = CSF_AT(float, vectorBytes, 0x0C) *
            reinterpret_cast<const float&>(parameters->field_0160);

        CSF_AT(int, self, 0x1C4) = 2;
        CSF_AT(float, vectorBytes, 0x00) = x;
        CSF_AT(float, vectorBytes, 0x04) = y;
        CSF_AT(float, vectorBytes, 0x08) = z;
        transitionValue = reinterpret_cast<const float&>(parameters->field_00ec);
        break;
    }

    default:
        applyTransition = false;
        break;
    }

    if (applyTransition)
    {
        CPlayerCharacter_ApplyTransitionValue(self, nullptr, transitionValue);

        void* owner = reinterpret_cast<void*>(self->field_004c);
        if (owner)
        {
            void* object = reinterpret_cast<void*>(
                C_0082dd78_DispatchReturnedObject(owner, nullptr));
            reinterpret_cast<void (__fastcall*)(void*, void*, float*)>(
                CSF_AT(void**, object, 0)[0x2C / 4])(
                    object, nullptr, reinterpret_cast<float*>(vectorBytes));
        }
    }

    void* secondaryObject = reinterpret_cast<uint8_t*>(self) + 0x04;
    return reinterpret_cast<int (__fastcall*)(void*, void*)>(
        reinterpret_cast<void**>(self->field_0004)[0x30 / 4])(
            secondaryObject, nullptr);
}

HOOK(0x004925E0, C_00833b88_SetTransitionState, "auto");
