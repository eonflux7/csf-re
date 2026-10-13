#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpHAnimHierarchyGetMatrixArray, 0x0056CF80, (int a1));
CSF_FUNC(int, __cdecl, _rpAtomicResyncInterpolatedSphere, 0x0058EB40, (int a1));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, RenderHandle_FromObject, 0x006B2D50, (void* self, void* edx));
CSF_FUNC(int, __fastcall, S_006d69e0_TryResolveReferencedObject, 0x006D6E20, (void* self, void* edx, void* a2));
CSF_FUNC(int, __fastcall, GfxHAnim_ResetAnimationState, 0x006E9ED0, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, C_008630e0_RefreshTeamPlayerSelection, 0x006FF680, (void* self, void* edx));
CSF_FUNC(uint32_t*, __cdecl, GfxWater_UpdateRenderObjectFrame, 0x007FEFA0, (uint32_t* a1, int a2, int16_t a3));
CSF_FUNC(float*, __cdecl, Water_ApplyTransform, 0x00803770, (int a1, uint16_t a2));
CSF_FUNC(unsigned int, __cdecl, GfxWater_ApplyHierarchyMatrices, 0x0081CBB0, (int a1, int a2, int a3));
CSF_FUNC(int, __cdecl, GfxWater_UpdateRenderObjectBounds, 0x0081CDF0, (int a1));

static bool __fastcall C_00863600_InitializeWater(
    C_00863600* self, void* /*edx*/)
{
    int globalObject = GetGlobalInt();
    int renderHandle = RenderHandle_FromObject(
        reinterpret_cast<void*>(globalObject), nullptr);
    int renderObject = S_006d69e0_TryResolveReferencedObject(
        reinterpret_cast<void*>(renderHandle), nullptr,
        reinterpret_cast<void*>(self->field_002c));
    if (!renderObject)
        return false;

    void* resource = reinterpret_cast<void*>(self->field_0008);
    self->field_0004 = renderObject;
    self->field_0030 =
        reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, resource, 0)[0x18 / 4])(resource, nullptr);

    void* renderObjectPtr = reinterpret_cast<void*>(renderObject);
    CSF_AT(uint32_t, renderObjectPtr, 0x08) |= 0x10;

    resource = reinterpret_cast<void*>(self->field_0008);
    void* animation =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, resource, 0)[0x68 / 4])(resource, nullptr);

    alignas(8) uint8_t animationStateBytes[0x2C];
    CSF_AT(int, animationStateBytes, 0x20) =
        reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, animation, 0)[0x20 / 4])(animation, nullptr, 0);
    CSF_AT(float, animationStateBytes, 0x24) = static_cast<float>(
        reinterpret_cast<double (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, animation, 0)[0x34 / 4])(animation, nullptr, 0));

    CSF_AT(uint32_t, animationStateBytes, 0x00) = 0;
    CSF_AT(uint32_t, animationStateBytes, 0x04) = 0;
    CSF_AT(uint32_t, animationStateBytes, 0x08) = 0;
    CSF_AT(uint8_t, animationStateBytes, 0x0C) = 0;
    CSF_AT(uint8_t, animationStateBytes, 0x0D) = 0;
    CSF_AT(uint8_t, animationStateBytes, 0x0E) = 0;
    CSF_AT(float, animationStateBytes, 0x10) = 1.0f;
    CSF_AT(float, animationStateBytes, 0x14) = 1.0f;
    CSF_AT(float, animationStateBytes, 0x18) = 1.0f;
    CSF_AT(uint8_t, animationStateBytes, 0x1C) = 0;
    CSF_AT(uint8_t, animationStateBytes, 0x1D) = 1;

    reinterpret_cast<void (__fastcall*)(void*, void*, int, int, void*)>(
        CSF_AT(void**, animation, 0)[0x14 / 4])(
            animation, nullptr, 0, 0, animationStateBytes);
    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, animation, 0)[0x28 / 4])(animation, nullptr, 0);

    int hierarchy = CSF_AT(int, animation, 0x14);
    int matrices = RpHAnimHierarchyGetMatrixArray(hierarchy);
    // The hierarchy argument remains on the caller's stack as argument three.
    GfxWater_ApplyHierarchyMatrices(renderObject, matrices, hierarchy);

    CSF_AT(bool, animationStateBytes, 0x2B) =
        C_008630e0_RefreshTeamPlayerSelection(self, nullptr);

    int savedFrame = CSF_AT(int, animationStateBytes, 0x20);
    CSF_AT(float, animationStateBytes, 0x00) =
        CSF_AT(float, animationStateBytes, 0x24);
    reinterpret_cast<void (__fastcall*)(void*, void*, int, int, void*)>(
        CSF_AT(void**, animation, 0)[0x14 / 4])(
            animation, nullptr, 0, savedFrame, animationStateBytes);
    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, animation, 0)[0x28 / 4])(animation, nullptr, 0);

    if (!CSF_AT(bool, animationStateBytes, 0x2B))
    {
        self->vSlot24();
        return false;
    }

    int elementCount = CSF_AT(uint16_t,
        reinterpret_cast<void*>(self->field_0004), 0x0C);
    for (int index = 0; index < elementCount; ++index)
    {
        Water_ApplyTransform(
            static_cast<int>(self->field_0004),
            static_cast<uint16_t>(index));
    }

    GfxWater_UpdateRenderObjectBounds(static_cast<int>(self->field_0004));

    // Verified at 0x7FEFA0: all three arguments are loaded from the stack,
    // and both exits use plain ret. ECX is not an incoming object pointer.
    GfxWater_UpdateRenderObjectFrame(
        CSF_AT(uint32_t*, renderObjectPtr, 0x24), renderObject, 1);

    resource = reinterpret_cast<void*>(self->field_0008);
    uint32_t atomicCount = static_cast<uint32_t>(
        reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, resource, 0)[0x84 / 4])(resource, nullptr));

    for (uint32_t index = 0; index < atomicCount; ++index)
    {
        void* entry =
            reinterpret_cast<void* (__fastcall*)(void*, void*, uint32_t)>(
                CSF_AT(void**, resource, 0)[0x80 / 4])(
                    resource, nullptr, index);
        void* atomic = CSF_AT(void*, entry, 0x0C);
        if (atomic)
        {
            if (CSF_AT(uint8_t, atomic, 0x50) & 2)
                _rpAtomicResyncInterpolatedSphere(reinterpret_cast<int>(atomic));
            CSF_AT(float, atomic, 0x28) =
                CSF_AT(float, atomic, 0x28) + CSF_AT(float, atomic, 0x28);
        }
    }

    GfxHAnim_ResetAnimationState(animation, nullptr);
    return CSF_AT(bool, animationStateBytes, 0x2B);
}

HOOK(0x0070A860, C_00863600_InitializeWater, "auto");
