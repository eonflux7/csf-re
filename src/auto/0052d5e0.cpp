#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, MenuMessageBox_ResolveEntryByKey, 0x00692440,
         (int value, int category));
CSF_FUNC(int, __fastcall, C_00720DF0_ForwardCallToChildVtblSlot16, 0x00720DF0,
         (void* self, void* edx, int value));

CSF_GLOBAL(float, g_ProgressScale, 0x0082E03C);

static int __fastcall CMenuProgressBar_UpdateProgress(
    CMenuProgressBar* self, void* /*edx*/, int /*unused*/)
{
    int result = static_cast<int>(self->field_0010);
    if (result == 0)
        return result;

    const int initialState = GetGlobalState_50D190();
    void* child = reinterpret_cast<void*>(self->field_0010);
    const float progress = CSF_AT(
        float, reinterpret_cast<void*>(initialState), 0xFC);

    void* geometry =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, child, 0)[0x1CC / 4])(child, nullptr);

    void* currentExtent =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, geometry, 0)[0x28 / 4])(geometry, nullptr);

    const double scaledProgress =
        static_cast<double>(progress) * static_cast<double>(g_ProgressScale);
    const uint32_t preservedExtent = CSF_AT(uint32_t, currentExtent, 0x04);
    const int64_t extent = static_cast<int64_t>(
        scaledProgress * static_cast<int32_t>(self->field_0014));

    uint32_t newExtent[2];
    child = reinterpret_cast<void*>(self->field_0010);
    newExtent[0] = static_cast<uint32_t>(extent);
    newExtent[1] = preservedExtent;

    geometry =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, child, 0)[0x1CC / 4])(child, nullptr);

    reinterpret_cast<void (__fastcall*)(void*, void*, uint32_t*)>(
        CSF_AT(void**, geometry, 0)[0x24 / 4])(
            geometry, nullptr, newExtent);

    result = GetGlobalState_50D190();
    if (self->field_0018 !=
        CSF_AT(uint32_t, reinterpret_cast<void*>(result), 0xF8))
    {
        const int refreshedState = GetGlobalState_50D190();
        const uint32_t displayValue = CSF_AT(
            uint32_t, reinterpret_cast<void*>(refreshedState), 0xF8);
        self->field_0018 = displayValue;

        const int resolvedValue =
            MenuMessageBox_ResolveEntryByKey(static_cast<int>(displayValue), 8);

        return C_00720DF0_ForwardCallToChildVtblSlot16(
            reinterpret_cast<void*>(self->field_0010), nullptr, resolvedValue);
    }

    return result;
}

HOOK(0x0052D5E0, CMenuProgressBar_UpdateProgress, "auto");
