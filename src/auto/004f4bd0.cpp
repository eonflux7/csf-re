#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ConfigureSubtitlePanel, 0x004F1CC0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, SetSubtitlePanelPosition, 0x004F1CE0, (void* self, void* edx, float x, float y));
CSF_FUNC(void*, __fastcall, ConstructSubtitlePanel, 0x004F31A0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, C_0083f130_Construct, 0x004F46A0, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static C_0083f160* __fastcall C_0083f160_Construct(
    C_0083f160* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x0083F160);

    void* storage = Mem_Alloc(100);
    void* panel = storage ? ConstructSubtitlePanel(storage, nullptr) : nullptr;
    self->field_0004 = reinterpret_cast<uint32_t>(panel);

    ConfigureSubtitlePanel(panel, nullptr, 8);
    ConfigureSubtitlePanel(reinterpret_cast<void*>(self->field_0004), nullptr, 4);

    void* settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    int enabledDirection = 2 * (CSF_AT(uint8_t, settings, 0x18D8) != 0) - 1;
    CSF_AT(int, reinterpret_cast<void*>(self->field_0004), 0x34) = enabledDirection;

    settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0004), 0x54) =
        CSF_AT(uint32_t, settings, 0x18E4);
    CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_0004), 0x58) = 1;
    CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_0004), 0x59) = 1;

    storage = Mem_Alloc(100);
    panel = storage ? ConstructSubtitlePanel(storage, nullptr) : nullptr;
    self->field_0008 = reinterpret_cast<uint32_t>(panel);

    SetSubtitlePanelPosition(panel, nullptr, 0.0f, 0.9f);
    ConfigureSubtitlePanel(reinterpret_cast<void*>(self->field_0008), nullptr, 8);

    settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    enabledDirection = 2 * (CSF_AT(uint8_t, settings, 0x18D9) != 0) - 1;
    CSF_AT(int, reinterpret_cast<void*>(self->field_0008), 0x34) = enabledDirection;

    settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0008), 0x54) =
        CSF_AT(uint32_t, settings, 0x18E8);
    CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_0008), 0x58) = 1;
    CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_0008), 0x59) = 1;
    CSF_AT(int, reinterpret_cast<void*>(self->field_0008), 0x24) = 2;

    storage = Mem_Alloc(84);
    self->field_000c = storage
        ? reinterpret_cast<uint32_t>(C_0083f130_Construct(storage, nullptr))
        : 0;

    return self;
}

HOOK(0x004F4BD0, C_0083f160_Construct, "auto");
