#include <cstdint>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CompareStringSubstring, 0x0042E200, (void* string, void* edx, unsigned int position, unsigned int count, const char* other, unsigned int otherCount));
CSF_FUNC(uint64_t, __cdecl, ScaleHudSizePair, 0x0042F3B0, (int* sizePair));
CSF_FUNC(void, __cdecl, Hud_RenderWidgetAtScaledRect, 0x0042F4F0, (void* widget, const int* rect));
CSF_FUNC(char, __fastcall, CGuiTacticalMap_AdjustLabelPosition, 0x00436C90, (void* self, void* edx, float* a2));
CSF_FUNC(uint32_t*, __fastcall, CGuiTacticalMap_GetLabelCoordinates, 0x00436D30, (void* self, void* edx, uint32_t* a2, float* a3));
CSF_FUNC(int, __fastcall, MissionInfo_ResetPageObjectsStub, 0x00436EB0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, CGuiTacticalMap_RenderSelectedMarker, 0x00437520, (void* self, void* edx, int* a2, float* a3));
CSF_FUNC(void, __fastcall, CGuiTacticalMap_RenderEntry, 0x004375D0, (void* self, void* edx, int a2, char a3, uint32_t* a4, uint32_t* a5, void** a6));
CSF_FUNC(unsigned int, __fastcall, CGuiTacticalMap_RenderBackground, 0x00437900, (void* self, void* edx));
CSF_FUNC(int*, __fastcall, CGuiTacticalMap_RenderOverlay, 0x00437B80, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetEntryCount, 0x0044BB80, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetEntryAt, 0x0044BBA0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(void*, __fastcall, UmsVirtualProcessor_GetDefaultDestination, 0x004FE4A0, (void* self, void*));
CSF_FUNC(int, __cdecl, Text_ConvertToDisplayEncoding, 0x00692580, (int a1, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(int, __stdcall, GfxSkin_ForwardToRwEngineSlot20, 0x006C7B40, (int a1, int a2));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);
CSF_GLOBAL(float, g_ColorChannelScale, 0x0082FEA8);
CSF_GLOBAL(float, g_TacticalMapPulseAngleScale, 0x00830A0C);
CSF_GLOBAL(int, g_CurrentSceneIndex, 0x0089972C);

static char __fastcall CGuiTacticalMap_Render(CGuiTacticalMap* self, void* /*edx*/)
{
    const char loaded = static_cast<char>(self->m_spritesLoaded);
    if (!loaded)
        return loaded;

    GetGlobalValue_6C5760();
    GfxSkin_ForwardToRwEngineSlot20(20, 1);
    GfxSkin_ForwardToRwEngineSlot20(6, 0);
    GfxSkin_ForwardToRwEngineSlot20(8, 0);
    GfxSkin_ForwardToRwEngineSlot20(29, 5);
    GfxSkin_ForwardToRwEngineSlot20(30, 0);
    GfxSkin_ForwardToRwEngineSlot20(10, 5);
    GfxSkin_ForwardToRwEngineSlot20(11, 6);

    if (self->field_0158)
        Hud_RenderWidgetAtScaledRect(
            reinterpret_cast<void*>(self->field_0158), nullptr);

    CGuiTacticalMap_RenderBackground(self, nullptr);
    if (self->field_017c == static_cast<uint32_t>(-1))
        MissionInfo_ResetPageObjectsStub(self, nullptr);

    alignas(8) uint8_t colorBytes[0x10];
    alignas(8) uint8_t positionBytes[0xC];
    alignas(8) uint8_t coordinateBytes[0x8];
    alignas(8) uint8_t sizePairBytes[0x8];
    alignas(8) uint8_t selectionBytes[0x8];

    auto* position = reinterpret_cast<float*>(positionBytes);
    auto* coordinates = reinterpret_cast<uint32_t*>(coordinateBytes);
    auto* selectedOut = reinterpret_cast<void**>(selectionBytes);
    uint32_t& index = CSF_AT(uint32_t, selectionBytes, 0x4);
    uint32_t& entryCount = CSF_AT(uint32_t, sizePairBytes, 0x4);

    std::memset(positionBytes, 0, sizeof(positionBytes));
    *selectedOut = nullptr;
    void* selectedMarker = nullptr;

    void* manager = reinterpret_cast<void*>(GetGlobalValue_8B0568());
    void* entries = UmsVirtualProcessor_GetDefaultDestination(manager, nullptr);
    entryCount = static_cast<uint32_t>(GetEntryCount(entries, nullptr));

    for (index = 0; index < entryCount; ++index)
    {
        const uint32_t entryIndex = index;
        manager = reinterpret_cast<void*>(GetGlobalValue_8B0568());
        entries = UmsVirtualProcessor_GetDefaultDestination(manager, nullptr);
        const int entryAddress = GetEntryAt(entries, nullptr, static_cast<int>(entryIndex));
        void* entry = reinterpret_cast<void*>(entryAddress);
        if (CSF_AT(int, entry, 0x1C) == g_CurrentSceneIndex)
        {
            CGuiTacticalMap_RenderEntry(
                self, nullptr, entryAddress,
                static_cast<char>(index == self->field_017c),
                reinterpret_cast<uint32_t*>(positionBytes), coordinates, selectedOut);
            selectedMarker = *selectedOut;
        }
    }

    CGuiTacticalMap_RenderOverlay(self, nullptr);
    if (selectedMarker)
    {
        auto* color = ((uint32_t* (__fastcall*)(void*, void*))
            CSF_AT(void**, selectedMarker, 0)[0x6C / 4])(selectedMarker, nullptr);
        const float angle = self->field_0174 * g_TacticalMapPulseAngleScale;
        std::memcpy(colorBytes, color, 3 * sizeof(uint32_t));
        CSF_AT(float, colorBytes, 0xC) = std::sin(angle) * g_ColorChannelScale;

        ((void (__fastcall*)(void*, void*, uint32_t*))
            CSF_AT(void**, selectedMarker, 0)[0x68 / 4])(
                selectedMarker, nullptr, reinterpret_cast<uint32_t*>(colorBytes));

        CGuiTacticalMap_RenderSelectedMarker(
            self, nullptr, reinterpret_cast<int*>(selectedMarker), position);

        const uint32_t selectedIndex = self->field_017c;
        manager = reinterpret_cast<void*>(GetGlobalValue_8B0568());
        entries = UmsVirtualProcessor_GetDefaultDestination(manager, nullptr);
        void* entry = reinterpret_cast<void*>(
            GetEntryAt(entries, nullptr, static_cast<int>(selectedIndex)));
        void* label = static_cast<uint8_t*>(entry) + 0x40;

        if (CompareStringSubstring(
                label, nullptr, 0, CSF_AT(uint32_t, entry, 0x54),
                reinterpret_cast<const char*>(&g_byte_82E65C), 0))
        {
            const char* text;
            if (CSF_AT(uint32_t, label, 0x18) < 0x10u)
                text = reinterpret_cast<const char*>(label) + 0x4;
            else
                text = reinterpret_cast<const char*>(CSF_AT(uint32_t, label, 0x4));

            const int displayText = Text_ConvertToDisplayEncoding(
                reinterpret_cast<int>(text), 12);
            void* labelWidget = reinterpret_cast<void*>(self->m_mapTexture);
            ((void (__fastcall*)(void*, void*, int))
                CSF_AT(void**, labelWidget, 0)[0x40 / 4])(
                    labelWidget, nullptr, displayText);

            CGuiTacticalMap_AdjustLabelPosition(self, nullptr, position);
            uint32_t* labelCoordinates = CGuiTacticalMap_GetLabelCoordinates(
                self, nullptr, coordinates, position);
            CSF_AT(uint32_t, sizePairBytes, 0) = labelCoordinates[0];
            CSF_AT(uint32_t, sizePairBytes, 0x4) = labelCoordinates[1] - 10;
            CSF_AT(uint32_t, sizePairBytes, 0) += 10;
            ScaleHudSizePair(reinterpret_cast<int*>(sizePairBytes));

            labelWidget = reinterpret_cast<void*>(self->m_mapTexture);
            ((void (__fastcall*)(void*, void*, int*))
                CSF_AT(void**, labelWidget, 0)[0x50 / 4])(
                    labelWidget, nullptr, reinterpret_cast<int*>(sizePairBytes));

            labelWidget = reinterpret_cast<void*>(self->m_mapTexture);
            ((void (__fastcall*)(void*, void*))
                CSF_AT(void**, labelWidget, 0)[0x8 / 4])(labelWidget, nullptr);
        }
    }

    GfxSkin_ForwardToRwEngineSlot20(20, 2);
    GfxSkin_ForwardToRwEngineSlot20(6, 1);
    GfxSkin_ForwardToRwEngineSlot20(8, 1);
    return static_cast<char>(GfxSkin_ForwardToRwEngineSlot20(29, 8));
}

HOOK(0x00437DA0, CGuiTacticalMap_Render, "auto");
