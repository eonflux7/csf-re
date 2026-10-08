#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Video mode request block built on the stack by the original (0x30 bytes).
struct VideoModeRequest {
    uint32_t width;        // +0x00 (800)
    uint32_t height;       // +0x04 (600)
    uint32_t f08;          // +0x08 (2)
    float    brightness;   // +0x0C (0.5f)
    uint8_t  f10;          // +0x10 (1)
    uint8_t  f11;          // +0x11 (0)
    uint8_t  pad12[2];     // +0x12 (not written by the original)
    uint32_t f14;          // +0x14 (1)
    uint32_t f18;          // +0x18 (2)
    uint32_t f1C;          // +0x1C (1)
    uint32_t f20;          // +0x20 (1)
    uint32_t f24;          // +0x24 (1)
    uint32_t f28;          // +0x28 (1)
    uint32_t f2C;          // +0x2C (1)
};

CSF_FUNC(int, __fastcall, CMenuVideoOptions_FillSettingsSnapshot, 0x00549760, (void* self, void* edx, void* request));
CSF_FUNC(int, __fastcall, MenuVideo_IsSettingsDifferent, 0x00549C10, (void* self, void* edx, void* settings));
CSF_FUNC(int, __fastcall, CMenuVideoOptions_ApplyVideoSettings, 0x005496A0, (void* self, void* edx, void* settings));

static int __fastcall CMenuVideoOptions_ValidateAndApplyMode(CMenuVideoOptions* self, void* /*edx*/)
{
    VideoModeRequest request;
    request.width      = 800;
    request.height     = 600;
    request.f08        = 2;
    request.brightness = 0.5f;
    request.f10        = 1;
    request.f11        = 0;
    request.f14        = 1;
    request.f18        = 2;
    request.f1C        = 1;
    request.f20        = 1;
    request.f24        = 1;
    request.f28        = 1;
    request.f2C        = 1;

    CMenuVideoOptions_FillSettingsSnapshot(self, nullptr, &request);

    // Sub-object at +0x28 (address, not a loaded pointer).
    void* settingsSubObject = reinterpret_cast<uint8_t*>(self) + 0x28;

    int isValid = MenuVideo_IsSettingsDifferent(&request, nullptr, settingsSubObject);
    if (static_cast<uint8_t>(isValid) != 0)
        return CMenuVideoOptions_ApplyVideoSettings(self, nullptr, settingsSubObject);

    return isValid;
}

HOOK(0x00549CF0, CMenuVideoOptions_ValidateAndApplyMode, "auto");
