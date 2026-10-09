#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Hud_ResetWidgetState, 0x00427590, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Hud_ResetPrimaryWidget, 0x00438720, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Hud_ResetSecondaryWidget, 0x00459300, (void* self, void* edx));
CSF_FUNC(int, __fastcall, HudDebugOverlay_Update, 0x004F2F90, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

static int __fastcall Hud_ResetWidgets(void* self, void* /*edx*/)
{
    Hud_ResetPrimaryWidget(CSF_AT(void*, self, 0x24C), nullptr);
    Hud_ResetSecondaryWidget(CSF_AT(void*, self, 0x248), nullptr);
    HudDebugOverlay_Update(CSF_AT(void*, self, 0x284), nullptr);

    void* widget = CSF_AT(void*, self, 0xC4);
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, widget, 0)[1])(widget, nullptr);

    void* buffer = CSF_AT(void*, self, 0x22C);
    CSF_AT(uint32_t, self, 0x230) = 0;
    CSF_AT(uint32_t, self, 0x234) = 0;
    if (buffer)
    {
        Mem_Free(buffer);
        CSF_AT(void*, self, 0x22C) = nullptr;
    }

    void* widgetState = CSF_AT(void*, self, 0x2CC);
    if (widgetState)
        Hud_ResetWidgetState(widgetState, nullptr);

    widget = CSF_AT(void*, self, 0x240);
    if (widget)
    {
        reinterpret_cast<int(__fastcall*)(void*, void*, uint8_t*)>(
            CSF_AT(void**, widget, 0)[0x3C / 4])(
                widget, nullptr, &g_byte_82E65C);
    }

    widget = CSF_AT(void*, self, 0x208);
    if (widget)
    {
        reinterpret_cast<int(__fastcall*)(void*, void*, uint8_t*)>(
            CSF_AT(void**, widget, 0)[0x3C / 4])(
                widget, nullptr, &g_byte_82E65C);
    }

    widget = CSF_AT(void*, self, 0x250);
    if (widget)
    {
        return reinterpret_cast<int(__fastcall*)(void*, void*, uint8_t*)>(
            CSF_AT(void**, widget, 0)[0x3C / 4])(
                widget, nullptr, &g_byte_82E65C);
    }
    return 0;
}

HOOK(0x0042E580, Hud_ResetWidgets, "auto");
