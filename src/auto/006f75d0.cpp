#include <cstdint>
#include <cstddef>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_6B81C0_GetFirstItem, 0x006B81C0, (void* self, void*));
CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectAttachment, 0x006B87A0, (int objectIndex));
CSF_FUNC(int, __cdecl, GfxFrames_GetProviderFrameObject, 0x006C0500, (int clump));
CSF_FUNC(double, __fastcall, C_Unknown_GetValue, 0x006C2050, (void* self, void*));
CSF_FUNC(int, __fastcall, AccessorState_ResolveSlot, 0x006C8EE0, (void* self, void* , int a2));
CSF_FUNC(char, __fastcall, GfxAnimInterpolator_InitializeHierarchy, 0x00708180, (void* self, void* , int clump, int animation, int , char attachHierarchy));
CSF_FUNC(std::uint32_t*, __fastcall, C_00863430_Construct, 0x007084E0, (C_00863430* self, void*));

static char __fastcall C_00862900_StartAnimationAtTime(
    C_00862900* self, void* /*edx*/, int slotIndex, float* time)
{
    char result = 0;

    do
    {
        if (slotIndex == -1)
            break;

        void* global = reinterpret_cast<void*>(GetGlobalInt());
        void* accessor = reinterpret_cast<void*>(
            GlobalInt_ResolveAccessor(global, nullptr));
        void* state = ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, accessor, 0)[0x14 / 4])(accessor, nullptr);
        void* animation = reinterpret_cast<void*>(
            AccessorState_ResolveSlot(state, nullptr, slotIndex));

        if (!animation || self->field_000c)
            break;

        C_00863430* storage = reinterpret_cast<C_00863430*>(
            CoreMemory_AllocUntagged(28));
        std::uint32_t* interpolator = storage
            ? C_00863430_Construct(storage, nullptr)
            : nullptr;

        const std::uint32_t hierarchyAnimation = self->field_0010;
        const std::uint32_t clump = self->field_0004;
        self->field_000c = reinterpret_cast<std::uint32_t>(interpolator);

        result = GfxAnimInterpolator_InitializeHierarchy(
            interpolator, nullptr, static_cast<int>(clump),
            static_cast<int>(hierarchyAnimation), 0, 1);
        if (!result)
            break;

        const float duration = static_cast<float>(
            C_Unknown_GetValue(animation, nullptr));
        if (*time > duration)
        {
            int frameObject = GfxFrames_GetProviderFrameObject(
                static_cast<int>(self->field_0004));
            void* attachment = reinterpret_cast<void*>(
                GfxObjectProps_GetObjectAttachment(frameObject));
            if (attachment)
                C_6B81C0_GetFirstItem(attachment, nullptr);

            *time = static_cast<float>(std::fmod(
                static_cast<double>(*time), static_cast<double>(duration)));
        }

        void* currentInterpolator =
            reinterpret_cast<void*>(self->field_000c);
        result = static_cast<char>(
            ((int (__fastcall*)(void*, void*, int, void*, float*))
                CSF_AT(void**, currentInterpolator, 0)[0x14 / 4])(
                    currentInterpolator, nullptr, 0, animation, time));
    } while (false);

    if (!result)
    {
        void* interpolator = reinterpret_cast<void*>(self->field_000c);
        if (interpolator)
        {
            ((void (__fastcall*)(void*, void*, int))
                CSF_AT(void**, interpolator, 0)[0x4 / 4])(
                    interpolator, nullptr, 1);
            self->field_000c = 0;
        }
    }

    return result;
}

HOOK(0x006F75D0, C_00862900_StartAnimationAtTime, "auto");
