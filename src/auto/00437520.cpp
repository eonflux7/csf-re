#include <cstdint>
#include "globals.h"
#include <cstring>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int32_t, __cdecl, GetGlobal8afc78, 0x0042D6D0, ());
CSF_FUNC(uint32_t*, __fastcall, CHud_CopyBoundsFromSource, 0x0042E150, (CHud* self, void* , int* a2));
CSF_FUNC(int, __fastcall, CHud_AttachPanelsToTarget, 0x0042E1A0, (CHud* self, void* , int a2));
CSF_FUNC(void, __cdecl, Hud_RenderWidgetAtScaledRect, 0x0042F4F0, (void* widget, const int* rect));
CSF_FUNC(char, __fastcall, CGuiTacticalMap_IsPositionOutsideMap, 0x00436C90, (void* self, void* edx, float* a2));
CSF_FUNC(uint32_t*, __fastcall, CGuiTacticalMap_ConvertPositionToMapCoordinates, 0x00436D30, (void* self, void* edx, uint32_t* a2, float* a3));
CSF_FUNC(int*, __fastcall, CGuiTacticalMap_HandleOutsideMapPosition, 0x00437390, (void* self, void* edx, float* a2));

static void __fastcall CGuiTacticalMap_RenderWidgetAtPosition(
    CGuiTacticalMap* self, void* /*edx*/, int* widget, float* position)
{
    if (!widget)
        return;

    alignas(8) uint8_t positionBytes[0x18];
    float* worldPosition = reinterpret_cast<float*>(positionBytes);
    float* fallbackPosition = reinterpret_cast<float*>(positionBytes + 0xC);
    uint32_t* mapCoordinates = reinterpret_cast<uint32_t*>(positionBytes + 0x10);

    std::memcpy(worldPosition, position, 0xC);
    std::memcpy(fallbackPosition, position, 0xC);

    if (CGuiTacticalMap_IsPositionOutsideMap(self, nullptr, worldPosition))
    {
        CGuiTacticalMap_HandleOutsideMapPosition(self, nullptr, fallbackPosition);
        return;
    }

    CGuiTacticalMap_ConvertPositionToMapCoordinates(
        self, nullptr, mapCoordinates, worldPosition);

    CHud_CopyBoundsFromSource(
        reinterpret_cast<CHud*>(GetGlobal8afc78()), nullptr, widget);

    reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t*)>(
        CSF_AT(void**, widget, 0)[0x28 / 4])(
            widget, nullptr, mapCoordinates);

    Hud_RenderWidgetAtScaledRect(widget, nullptr);

    CHud_AttachPanelsToTarget(
        reinterpret_cast<CHud*>(GetGlobal8afc78()),
        nullptr,
        reinterpret_cast<int>(widget));
}

HOOK(0x00437520, CGuiTacticalMap_RenderWidgetAtPosition, "auto");
