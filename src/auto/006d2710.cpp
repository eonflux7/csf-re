#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint8_t*, __fastcall, InputBinding_Construct, 0x004022A0, (S_004022a0* self, void* , int bindingId));
CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void*));
CSF_FUNC(char, __fastcall, C_00860e68_RegisterLight, 0x006D1DB0, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(uint32_t*, __fastcall, GfxLights_ConstructLightState, 0x006D2640, (void* self, void* edx));
CSF_FUNC(char, __fastcall, GfxLights_InitializeLightState, 0x006DAEF0, (void* self, void* edx));
CSF_FUNC(int*, __fastcall, GfxLights_ConstructLightObject, 0x006FD400, (void* self, void* edx, int a2, int a3));

CSF_GLOBAL(float, g_ColorBlack, 0x008A7DA0);
CSF_GLOBAL(float, g_ColorBlack_G, 0x008A7DA4);
CSF_GLOBAL(float, g_ColorBlack_B, 0x008A7DA8);
CSF_GLOBAL(float, g_ColorBlack_A, 0x008A7DAC);

static int __fastcall C_00860e68_Construct(C_00860e68* self, void* /*edx*/)
{
    C_00860178_Construct(reinterpret_cast<C_00860178*>(self), nullptr);
    CSF_AT(void**, self, 0x04) = CSF_VTBL(0x008304D8);
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00860E68);
    CSF_AT(void**, self, 0x04) = CSF_VTBL(0x00860E64);

    self->field_0028 = 1065353216;
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;

    self->field_0034 = 0;
    self->field_0038 = 0;
    self->field_003c = 0;
    self->field_0040 = 10;
    self->field_0044 = 0;
    self->field_0048 = 0;
    self->field_004c = 0;
    self->field_0050 = 10;
    self->field_0054 = 0;
    self->field_0058 = 0;
    self->field_005c = 0;
    self->field_0060 = 10;
    self->field_0064 = 0;
    self->field_0068 = 0;
    self->field_006c = 0;
    self->field_0070 = 10;
    self->field_0074 = 0;
    self->field_0078 = 0;
    self->field_007c = 0;
    self->field_0080 = 10;
    self->field_0088 = 0;
    self->field_008c = 0;
    self->field_0090 = 0;
    self->field_0094 = 10;

    void* stateStorage = Mem_Alloc(272);
    uint32_t* lightState = stateStorage
        ? GfxLights_ConstructLightState(stateStorage, nullptr)
        : nullptr;
    self->field_0008 = reinterpret_cast<uint32_t>(lightState);
    GfxLights_InitializeLightState(lightState, nullptr);

    void* bindingStorage = Mem_Alloc(8);
    uint8_t* binding = bindingStorage
        ? InputBinding_Construct(
            reinterpret_cast<S_004022a0*>(bindingStorage), nullptr,
            reinterpret_cast<int>(self))
        : nullptr;
    self->field_000c = reinterpret_cast<uint32_t>(binding);
    self->field_0018 = 0;
    self->field_0019 = 0;

    uint32_t capacity = self->field_0038;
    if (capacity < 2)
        S_0041e5f0_GrowArrayCapacity(
            reinterpret_cast<S_0041e5f0*>(&self->field_0034), nullptr,
            static_cast<int>(2 - capacity));

    capacity = self->field_0048;
    if (capacity < 0x12C)
        S_0041e5f0_GrowArrayCapacity(
            reinterpret_cast<S_0041e5f0*>(&self->field_0044), nullptr,
            static_cast<int>(300 - capacity));

    capacity = self->field_0058;
    if (capacity < 0xC8)
        S_0041e5f0_GrowArrayCapacity(
            reinterpret_cast<S_0041e5f0*>(&self->field_0054), nullptr,
            static_cast<int>(200 - capacity));

    capacity = self->field_0078;
    if (capacity < 0x20)
        S_0041e5f0_GrowArrayCapacity(
            reinterpret_cast<S_0041e5f0*>(&self->field_0074), nullptr,
            static_cast<int>(32 - capacity));

    self->field_0098 = 0;
    self->field_009c = 0;
    self->field_00a0 = 0;

    void* firstLightStorage = Mem_Alloc(132);
    int* firstLight = firstLightStorage
        ? GfxLights_ConstructLightObject(
            firstLightStorage, nullptr, 0, reinterpret_cast<int>(self))
        : nullptr;
    self->field_0084 = reinterpret_cast<uint32_t>(firstLight);
    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, firstLight, 0)[0x20 / 4])(firstLight, nullptr, 3);
    C_00860e68_RegisterLight(
        self, nullptr, reinterpret_cast<uint32_t*>(firstLight));
    self->field_002c = reinterpret_cast<uint32_t>(firstLight);

    void* secondLightStorage = Mem_Alloc(132);
    int* secondLight = secondLightStorage
        ? GfxLights_ConstructLightObject(
            secondLightStorage, nullptr, 0, reinterpret_cast<int>(self))
        : nullptr;
    self->field_0084 = reinterpret_cast<uint32_t>(secondLight);
    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, secondLight, 0)[0x20 / 4])(secondLight, nullptr, 1);
    C_00860e68_RegisterLight(
        self, nullptr, reinterpret_cast<uint32_t*>(secondLight));
    self->field_0030 = reinterpret_cast<uint32_t>(secondLight);

    self->field_001a = 0;
    std::memcpy(&self->field_001c, &g_ColorBlack, sizeof(float));
    std::memcpy(&self->field_0020, &g_ColorBlack_G, sizeof(float));
    std::memcpy(&self->field_0024, &g_ColorBlack_B, sizeof(float));
    std::memcpy(&self->field_0028, &g_ColorBlack_A, sizeof(float));
    self->field_0084 = 0;
    return reinterpret_cast<int>(self);
}

HOOK(0x006D2710, C_00860e68_Construct, "auto");
