#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, AllocateAlignedBuffer, 0x00690EB0, (std::size_t size, int alignment, int option1, int option2));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, ResolveRwManager, 0x006B13B0, (int managerId, void* edx));
CSF_FUNC(int, __fastcall, CameraObject_Construct, 0x006D32F0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CameraObject_Setup, 0x006D3450, (void* self, void* edx, void* params, float value));
CSF_FUNC(double, __fastcall, RwManager_GetValue, 0x006F2860, (void* self, void* edx));
CSF_FUNC(char, __fastcall, WaterBase_Init, 0x006FE0C0, (void* self, void* edx, void* a2, char a3));

// Constant float data of the original image, read at run time (bound by address).
CSF_GLOBAL(std::uint32_t, g_WaterCameraNearClipBits, 0x00860DA4);
CSF_GLOBAL(std::uint32_t, g_WaterCameraFarClipBits, 0x00860DA8);
CSF_GLOBAL(std::uint32_t, g_PreviewCameraAngleBits, 0x00860DAC);

// Values the image stores at 0x860DA4 / 0x860DA8 (10.0f and 10000.0f); documentation only,
// the function always loads the globals above.
[[maybe_unused]] static constexpr std::uint32_t kWaterCameraNearClipBits = 0x41200000;
[[maybe_unused]] static constexpr std::uint32_t kWaterCameraFarClipBits = 0x461C4000;

static char __fastcall Water_Initialize(S_006fe0c0* self, void* /*edx*/, C_0085ec18* a2, int a3)
{
    char ok = WaterBase_Init(self, nullptr, a2, 0);
    self->field_00f8 = static_cast<uint32_t>(a3);
    self->field_0008 = static_cast<uint32_t>(reinterpret_cast<std::uintptr_t>(a2));
    self->field_00f0 = 0;
    self->field_00f4 = 0;

    void* mem = Mem_Alloc(228);
    int camera;
    if (mem)
        camera = CameraObject_Construct(mem, nullptr);
    else
        camera = 0;
    self->field_00d8 = static_cast<uint32_t>(camera);

    int globalInt = GetGlobalInt();
    int manager = ResolveRwManager(globalInt, nullptr);

    // Camera description block: 0x24 bytes on the original's frame.
    std::uint32_t params[9];
    params[4] = g_WaterCameraNearClipBits;
    params[7] = g_PreviewCameraAngleBits;
    std::memset(params, 0, 16);
    params[8] = 0;
    params[5] = g_WaterCameraFarClipBits;
    params[6] = 0;

    float value = static_cast<float>(RwManager_GetValue(reinterpret_cast<void*>(static_cast<std::uintptr_t>(manager)), nullptr));
    CameraObject_Setup(reinterpret_cast<void*>(static_cast<std::uintptr_t>(self->field_00d8)), nullptr, params, value);

    self->field_00c8 = static_cast<uint32_t>(AllocateAlignedBuffer(0x70, 64, 0, 0));
    self->field_00cc = 0;
    self->field_00d0 = 0;
    self->field_00ec = 0;
    self->field_00fc = CSF_AT(uint32_t, reinterpret_cast<std::uint8_t*>(static_cast<std::uintptr_t>(a3)), 0x98);

    void** vtbl = reinterpret_cast<void**>(static_cast<std::uintptr_t>(self->field_0000));
    if (ok) {
        ok = ((char(__fastcall*)(void*, void*))vtbl[0x30 / 4])(self, nullptr);
        if (ok) {
            vtbl = reinterpret_cast<void**>(static_cast<std::uintptr_t>(self->field_0000));
            ok = ((char(__fastcall*)(void*, void*))vtbl[0x2C / 4])(self, nullptr);
        }
    }

    self->field_00d4 = 1;
    if (!ok) {
        vtbl = reinterpret_cast<void**>(static_cast<std::uintptr_t>(self->field_0000));
        ((void(__fastcall*)(void*, void*))vtbl[0x24 / 4])(self, nullptr);
    }
    return ok;
}

HOOK(0x0070CE50, Water_Initialize, "auto");
