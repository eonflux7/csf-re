#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));

static int __fastcall GameBddLoader_PositionAndShowControl(void* self, void* /*edx*/)
{
    int result = CSF_AT(int, self, 0x104);
    if (!result)
        return result;

    void* menu = MenuControls_ResolveObject(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    bool ready = reinterpret_cast<bool (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, menu, 0)[0x74 / 4])(menu, nullptr, 0);
    if (!ready)
        return 0;

    void* display = LookupObjectById(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    void* dimensions = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, display, 0)[0x18 / 4])(display, nullptr);
    int32_t width = CSF_AT(int32_t, dimensions, 0);

    display = LookupObjectById(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    dimensions = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, display, 0)[0x18 / 4])(display, nullptr);
    int32_t height = CSF_AT(int32_t, dimensions, 4);

    int32_t scaledHeight =
        static_cast<int32_t>(92 * static_cast<uint32_t>(height));
    int32_t scaledWidth =
        static_cast<int32_t>(8 * static_cast<uint32_t>(width));

    alignas(8) uint8_t positionBytes[0x8];
    CSF_AT(int32_t, positionBytes, 0) = scaledWidth / 100 + 140;
    CSF_AT(int32_t, positionBytes, 4) = scaledHeight / 100 - 50;

    void* control = CSF_AT(void*, self, 0x104);
    reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, control, 0)[0x50 / 4])(
            control, nullptr, positionBytes);

    control = CSF_AT(void*, self, 0x104);
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, control, 0)[8 / 4])(control, nullptr);

    menu = MenuControls_ResolveObject(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    return reinterpret_cast<int (__fastcall*)(void*, void*)>(
        CSF_AT(void**, menu, 0)[0x78 / 4])(menu, nullptr);
}

HOOK(0x0050E430, GameBddLoader_PositionAndShowControl, "auto");
