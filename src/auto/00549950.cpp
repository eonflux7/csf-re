#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, CMenuVideoOptions_ParseResolutionPair, 0x00549840,
         (CMenuVideoOptions* self, void* edx, int selection,
          int* width, int* height));

// Original switch dispatch tables; the switches below express their behavior.
using ResolutionPresetDispatchTable = const std::uint32_t[4];
using QualityPresetDispatchTable = const std::uint32_t[5];
CSF_GLOBAL(ResolutionPresetDispatchTable, g_ResolutionPresetDispatch, 0x00549B20);
CSF_GLOBAL(QualityPresetDispatchTable, g_QualityPresetDispatch, 0x00549B30);

static int CMenuVideoOptions_ReadConfiguration(
    CMenuVideoOptions* self, unsigned slot)
{
    void* configuration = reinterpret_cast<void*>(self->field_0024);
    return reinterpret_cast<int (__fastcall*)(void*, void*)>(
        CSF_AT(void**, configuration, 0)[slot])(configuration, nullptr);
}

static int __fastcall CMenuVideoOptions_GetMatchingQualityPreset(
    CMenuVideoOptions* self, void* /*edx*/)
{
    void* resolutionControl = reinterpret_cast<void*>(self->field_0060);
    void** resolutionVtable = CSF_AT(void**, resolutionControl, 0);
    int width = 0;
    int height = 0;

    const int selectedIndex =
        reinterpret_cast<int (__fastcall*)(void*, void*)>(
            resolutionVtable[0x2AC / 4])(resolutionControl, nullptr);

    // The object pointer is reloaded, but the original vtable is retained.
    resolutionControl = reinterpret_cast<void*>(self->field_0060);
    const int resolutionSelection =
        reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
            resolutionVtable[0x2B4 / 4])(
                resolutionControl, nullptr, selectedIndex);

    CMenuVideoOptions_ParseResolutionPair(
        self, nullptr, resolutionSelection, &width, &height);

    int preset;
    if (width == 1024) {
        if (height != 768)
            return 0;
        preset = 3;
    } else if (width == 800) {
        if (height != 600)
            return 0;
        preset = 2;
    } else {
        if (width != 640 || height != 480)
            return 0;
        preset = 1;
    }

    switch (CMenuVideoOptions_ReadConfiguration(self, 0x60 / 4)) {
    case 0:
        if (preset != 1) return 0;
        break;
    case 1:
        if (preset != 2) return 0;
        break;
    case 2:
        if (preset != 3) return 0;
        break;
    default:
        break;
    }

    switch (CMenuVideoOptions_ReadConfiguration(self, 0x48 / 4)) {
    case 0:
        return 0;
    case 1:
        if (preset != 1) return 0;
        break;
    case 2:
        if (preset != 2) return 0;
        break;
    case 3:
        if (preset != 3) return 0;
        break;
    default:
        break;
    }

    switch (CMenuVideoOptions_ReadConfiguration(self, 0x70 / 4)) {
    case 0:
        if (preset != 1) return 0;
        break;
    case 1:
        if (preset != 2 && preset != 3) return 0;
        break;
    case 2:
        return 0;
    default:
        break;
    }

    switch (CMenuVideoOptions_ReadConfiguration(self, 0x50 / 4)) {
    case 0:
        if (preset != 1) return 0;
        break;
    case 1:
        if (preset != 2) return 0;
        break;
    case 2:
        if (preset != 3) return 0;
        break;
    default:
        break;
    }

    switch (CMenuVideoOptions_ReadConfiguration(self, 0x68 / 4)) {
    case 0:
        if (preset != 1) return 0;
        break;
    case 1:
        if (preset != 2) return 0;
        break;
    case 2:
        if (preset != 3) return 0;
        break;
    default:
        break;
    }

    switch (CMenuVideoOptions_ReadConfiguration(self, 0x40 / 4)) {
    case 0:
        if (preset != 1) return 0;
        break;
    case 1:
        if (preset != 2) return 0;
        break;
    case 2:
        if (preset != 3) return 0;
        break;
    case 3:
    case 4:
        return 0;
    default:
        break;
    }

    return preset;
}

HOOK(0x00549950, CMenuVideoOptions_GetMatchingQualityPreset, "auto");
