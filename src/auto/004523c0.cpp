#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, InitializeActionRequest, 0x004011F0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, InitializeActionBounds, 0x0040FDA0, (void* self, void* edx, void* a2, int a3));
CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(uint32_t*, __fastcall, InitializeActionResult, 0x0044FAA0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(void, __fastcall, ProcessSelectionAction, 0x004520D0, (void* self, void* edx, int a2, int a3, int a4, int a5, float a6));
CSF_FUNC(int, __fastcall, ApplySelectionState, 0x0048E790, (void* self, void* edx, int a2));
CSF_FUNC(int*, __fastcall, GetSelectionState, 0x0048E7B0, (void* self, void* edx, int* a2));
CSF_FUNC(int, __fastcall, ClearSelectionState, 0x0048E830, (void* self, void* edx));
CSF_FUNC(int, __fastcall, IsField1BCNonNegative, 0x00492B10, (void* self, void*));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(float*, __fastcall, GetActionExtent, 0x006AE640, (void* self, void* edx, float* a2));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);

static void __fastcall S_00452280_UpdateControls(
    S_00452280* self, void* /*edx*/, int /*argument*/)
{
    const uint32_t index = self->field_0070;
    if (index == UINT32_MAX || index >= self->field_0068)
        return;

    void* candidate = CSF_AT(void**, self, 0x60)[index];
    if (!candidate)
        return;

    if (CSF_AT(int32_t, self, 0x1A0) < 0)
    {
        CSF_AT(int32_t, self, 0x19C) = -1;
        CSF_AT(int32_t, self, 0x1A0) = -1;
    }

    float elapsed;
    std::memcpy(&elapsed, &self->field_0008, sizeof(elapsed));
    // Unordered comparisons also proceed in the original.
    if (elapsed < g_HalfDurationScale)
        return;

    const uint32_t zero = 0;
    std::memcpy(&self->field_0008, &zero, sizeof(zero));

    if (static_cast<uint8_t>(IsField1BCNonNegative(candidate, nullptr)) ||
        CSF_AT(uint8_t, candidate, 0x228))
    {
        if (CSF_AT(int32_t, self, 0x1A0) >= 0)
        {
            const int resolved = CHandleTable_Resolve(
                self, nullptr,
                reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x198));
            if (resolved)
            {
                ClearSelectionState(reinterpret_cast<void*>(resolved), nullptr);
                CSF_AT(int32_t, self, 0x19C) = -1;
                CSF_AT(int32_t, self, 0x1A0) = -1;
            }
        }
        return;
    }

    alignas(4) uint8_t result[0x2C];
    // One contiguous request containing its embedded array.
    uint32_t request[(0x10 + 0x24) / sizeof(uint32_t)];
    alignas(4) uint8_t bounds[0x18];
    float extent[3];
    alignas(4) uint8_t candidateBounds[0xC];
    alignas(4) uint8_t candidateExtent[0x4];
    int selectionState = 0;
    int actionMode;

    InitializeActionResult(result, nullptr);
    InitializeActionRequest(request, nullptr);

    // The embedded array {data, capacity, count, growth} sits at +0x10 of the request.
    if (request[6] >= request[5])
    {
        (void)CSF_VTBL(0x0041E5F0);
        GrowButtonArray(&request[4], nullptr, static_cast<int32_t>(request[7]));
    }

    const uint32_t count = request[6];
    reinterpret_cast<void**>(request[4])[count] = candidate;
    request[6] = count + 1;
    request[0] = 1082;

    // These objects use buffer-taking slots, not CGuiControl's getter slots.
    void* extentSource =
        reinterpret_cast<void*(__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, candidate, 0)[0xAC / 4])(
                candidate, nullptr, candidateExtent);
    float* actionExtent = GetActionExtent(extentSource, nullptr, extent);
    void* actionBounds =
        reinterpret_cast<void*(__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, candidate, 0)[0xA4 / 4])(
                candidate, nullptr, candidateBounds);
    InitializeActionBounds(
        bounds, nullptr, actionBounds, reinterpret_cast<int>(actionExtent));

    actionMode = 5;
    ProcessSelectionAction(
        self, nullptr, reinterpret_cast<int>(result),
        reinterpret_cast<int>(request), reinterpret_cast<int>(bounds),
        reinterpret_cast<int>(&actionMode), 1000.0f);

    if (!CSF_AT(uint8_t, self, 0x180))
    {
        void* selected = CSF_AT(void*, result, 0x4);
        if (!selected ||
            CSF_AT(uint32_t, selected, 0x20) !=
                CSF_AT(uint32_t, self, 0x19C))
        {
            if (CSF_AT(int32_t, self, 0x1A0) >= 0)
            {
                const int resolved = CHandleTable_Resolve(
                    self, nullptr,
                    reinterpret_cast<int>(
                        reinterpret_cast<uint8_t*>(self) + 0x198));
                if (resolved)
                {
                    ClearSelectionState(reinterpret_cast<void*>(resolved), nullptr);
                    CSF_AT(int32_t, self, 0x19C) = -1;
                    CSF_AT(int32_t, self, 0x1A0) = -1;
                }
            }

            if (selected &&
                reinterpret_cast<char(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, selected, 0)[0x48 / 4])(
                        selected, nullptr))
            {
                int* state = GetSelectionState(selected, nullptr, &selectionState);
                ApplySelectionState(selected, nullptr, reinterpret_cast<int>(state));
                CSF_AT(uint32_t, self, 0x19C) =
                    CSF_AT(uint32_t, selected, 0x20);
                CSF_AT(uint32_t, self, 0x1A0) =
                    CSF_AT(uint32_t, selected, 0x24);
            }
        }
    }

    if (request[4])
        Mem_Free(reinterpret_cast<void*>(request[4]));
}

HOOK(0x004523C0, S_00452280_UpdateControls, "auto");
