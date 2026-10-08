#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, MenuMessageBox_AdvanceActiveIndicator,
         0x00529E50, (void* self, void* edx));
CSF_FUNC(int, __fastcall, MenuMessageBox_ContinueUpdate,
         0x00529D30, (void* self, void* edx));

static int __fastcall MenuMessageBox_UpdateWhenControlsReady(
    void* self, void* /*edx*/)
{
    void* control = CSF_AT(void*, self, 0x884);
    const int ready =
        reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, control, 0)[0x1CC / 4])(control, nullptr);

    control = CSF_AT(void*, self, 0x884);
    uint32_t result = static_cast<uint32_t>(
        reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, control, 0)[0x1C0 / 4])(control, nullptr, 7));

    if (!ready || !result)
        return static_cast<int>(result);

    result = CSF_AT(uint32_t, self, 0x48);
    if (!result)
        return static_cast<int>(result);

    // The original replaces AL only, retaining the upper bytes of EAX.
    result = (result & 0xFFFFFF00u) | CSF_AT(uint8_t, self, 0x8A9);
    if (!(result & 0xFFu))
        return static_cast<int>(result);

    result = CSF_AT(uint32_t, self, 0x5C);
    if (!result)
        return static_cast<int>(result);

    result = CSF_AT(uint32_t, self, 0x60);
    if (!result)
        return static_cast<int>(result);

    result = CSF_AT(uint32_t, self, 0x64);
    if (!result)
        return static_cast<int>(result);

    void* completeObject = static_cast<uint8_t*>(self) - 0x10;
    MenuMessageBox_AdvanceActiveIndicator(completeObject, nullptr);
    return MenuMessageBox_ContinueUpdate(completeObject, nullptr);
}

HOOK(0x0052AA30, MenuMessageBox_UpdateWhenControlsReady, "auto");
