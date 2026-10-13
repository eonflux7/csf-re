#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ApplyVideoPreset, 0x0050AF90, (void* self, void* edx, int preset));
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int*, __fastcall, QueryVideoOptions, 0x006DC7E0, (void* self, void* edx, int* a2, uint32_t* a3));

static int __fastcall S_0050af90_CycleVideoResolution(
    S_0050af90* self, void* /*edx*/)
{
    alignas(8) uint8_t queryBytes[0x14] = {};
    alignas(8) uint8_t optionsBytes[0x1C];

    // Default the otherwise uninitialized fallback to zero.
    CSF_AT(int, optionsBytes, 0x18) = 0;

    void* globalObject = reinterpret_cast<void*>(
        static_cast<uintptr_t>(GetGlobalInt()));
    void* videoObject = LookupObjectById(globalObject, nullptr);
    int* options = QueryVideoOptions(
        videoObject, nullptr,
        reinterpret_cast<int*>(optionsBytes),
        reinterpret_cast<uint32_t*>(queryBytes));

    int resolution = options[0];
    int enabled = options[1];
    int result = options[2];
    CSF_AT(int, optionsBytes, 0x10) = enabled;
    CSF_AT(int, optionsBytes, 0x14) = result;

    if (resolution != -1)
    {
        int preset = 1;
        if (resolution != 0)
            preset = CSF_AT(int, optionsBytes, 0x18);
        if (resolution == 1)
            preset = 2;
        if (resolution == 2)
            preset = 3;

        auto vtable = reinterpret_cast<void**>(
            static_cast<uintptr_t>(self->field_0000));
        reinterpret_cast<void(__fastcall*)(S_0050af90*, void*, int)>(
            vtable[0x1C / 4])(self, nullptr, preset);

        // The packet also tags the preset function's address as a vtable reference.
        (void)CSF_VTBL(0x0050AF90);
        ApplyVideoPreset(self, nullptr, preset);

        enabled = CSF_AT(int, optionsBytes, 0x10);
        result = CSF_AT(int, optionsBytes, 0x14);
    }

    if (enabled != -1)
    {
        auto vtable = reinterpret_cast<void**>(
            static_cast<uintptr_t>(self->field_0000));
        reinterpret_cast<void(__fastcall*)(S_0050af90*, void*, int)>(
            vtable[0x74 / 4])(self, nullptr, enabled ? 1 : 0);
        result = CSF_AT(int, optionsBytes, 0x14);
    }

    if (result == 0 || result == 1 || result == 2)
    {
        auto vtable = reinterpret_cast<void**>(
            static_cast<uintptr_t>(self->field_0000));
        return reinterpret_cast<int(__fastcall*)(S_0050af90*, void*, int)>(
            vtable[0x6C / 4])(self, nullptr, result);
    }

    return result;
}

HOOK(0x0050B5B0, S_0050af90_CycleVideoResolution, "auto");
