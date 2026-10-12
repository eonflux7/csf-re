#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_HeadModelScale, 0x0083F380);
CSF_GLOBAL(float, g_WaterCameraNearClip, 0x00860DA4);
CSF_GLOBAL(float, g_WaterCameraFarClip, 0x00860DA8);

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(void, __fastcall, InvokeMemberVirtualSlot8, 0x004F5170, (void* self, void* edx, float scale));
CSF_FUNC(C_0083f1a0*, __fastcall, C_0083f1a0_InitializeHeadModel, 0x004F7E20, (C_0083f1a0* self, void* , int width, int height));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, GetGlobalValue_753A30, 0x00753A30, ());

static int __fastcall C_0083f448_AddHeadModel(
    C_0083f448* self, void* /*edx*/)
{
    // Clip defaults are 10.0f and 10000.0f. Preserve the runtime globals'
    // raw bits, including any changes made since initialization.
    int width = 0x41200000;
    int height = 0x461C4000;
    std::memcpy(&width, &g_WaterCameraNearClip, sizeof(width));
    std::memcpy(&height, &g_WaterCameraFarClip, sizeof(height));

    auto* globalState =
        reinterpret_cast<std::uint8_t*>(GetGlobalValue_753A30());
    float scale = CSF_AT(float, globalState, 0x13C) * g_HeadModelScale;

    auto* currentState =
        reinterpret_cast<std::uint8_t*>(GetValue8B0554());
    if (CSF_AT(std::uint32_t, currentState, 0x90C) != 0xBF800000)
    {
        scale = CSF_AT(float, currentState + 0x54, 0x8B8)
              * g_HeadModelScale;
    }

    auto* allocation = static_cast<C_0083f1a0*>(Mem_Alloc(312));
    C_0083f1a0* model = nullptr;
    if (allocation)
    {
        model = C_0083f1a0_InitializeHeadModel(
            allocation, nullptr, width, height);
    }

    InvokeMemberVirtualSlot8(model, nullptr, scale);

    if (self->field_000c >= self->field_0008)
    {
        (void)CSF_VTBL(0x0041E5F0);
        S_0041e5f0_GrowArrayCapacity(
            reinterpret_cast<S_0041e5f0*>(
                reinterpret_cast<std::uint8_t*>(self) + 0x04),
            nullptr,
            CSF_AT(int, self, 0x10));
    }

    const std::uint32_t index = self->field_000c;
    auto* destination =
        reinterpret_cast<C_0083f1a0**>(self->field_0004 + 4 * index);
    self->field_000c = index + 1;
    *destination = model;

    return static_cast<int>(self->field_000c - 1);
}

HOOK(0x004FC8B0, C_0083f448_AddHeadModel, "auto");
