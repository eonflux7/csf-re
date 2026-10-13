#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __fastcall, Hud_ResetWidgetState_42D6F0, 0x0042D6F0, (void* self, void*));
CSF_FUNC(unsigned int, __cdecl, Hud_WriteNormalizedScreenSize, 0x0042DE60, (float* out));
CSF_FUNC(int, __fastcall, CHud_LayoutDebugOverlayWidget, 0x0042FE00, (CHud* self, void*));
CSF_FUNC(const void*, __fastcall, CHud_InitializeConfiguredWidgets, 0x00431D10, (CHud* self, void*));
CSF_FUNC(uint32_t, __cdecl, MpSpawnPoints_RegisterObject, 0x00475850, (int a1));
CSF_FUNC(int, __fastcall, HudStringShow_AddFlags, 0x004F1CC0, (void* self, void* , int flags));
CSF_FUNC(int, __fastcall, CCamera_SetSubtitlePanelPosition, 0x004F1CE0, (void* self, void* , int subtitleX, int subtitleY));
CSF_FUNC(std::uint32_t*, __fastcall, C_0083f0ec_ConstructSubtitlePanel, 0x004F31A0, (C_0083f0ec* self, void*));
CSF_FUNC(int, __fastcall, C_0085e7b8_GetSceneRegionAt, 0x004FC130, (C_0085e7b8* self, void* , int index));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));
CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void*));
CSF_FUNC(int, __fastcall, C_00860894_Init, 0x006C45F0, (C_00860894* self, void* , int a2, int a3));
CSF_FUNC(int, __fastcall, C_008670f0_SetField0C, 0x006C4620, (C_008670f0* self, void* , int value));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_753A30, 0x00753A30, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(unsigned int, g_ScreenWidth, 0x00899B9C);
CSF_GLOBAL(unsigned int, g_ScreenHeight, 0x00899BA0);
CSF_GLOBAL(float, g_ColorWhite, 0x008A7D90);
CSF_GLOBAL(float, g_ColorWhite_G, 0x008A7D94);
CSF_GLOBAL(float, g_ColorWhite_B, 0x008A7D98);
CSF_GLOBAL(float, g_ColorWhite_A, 0x008A7D9C);
CSF_GLOBAL(float, g_ColorRed, 0x008A7DB0);
CSF_GLOBAL(float, g_DefaultColorGreen, 0x008A7DB4);
CSF_GLOBAL(float, g_DefaultColorBlue, 0x008A7DB8);
CSF_GLOBAL(float, g_DefaultColorAlpha, 0x008A7DBC);

static std::uint32_t Hud_EncodeFloat(float value)
{
    std::uint32_t bits;
    std::memcpy(&bits, &value, sizeof(bits));
    return bits;
}

static int __fastcall CHud_ConstructInstance(CHud* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x00830A04);
    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0010 = 10;
    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0020 = 10;
    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_002c = 0;
    self->field_0030 = 10;
    self->field_0034 = 0;
    self->field_0038 = 0;
    self->field_003c = 0;
    self->field_0040 = 10;
    self->field_0044 = 0;
    self->field_0048 = 0;
    self->field_004c = 0;
    self->field_0050 = 10;
    self->field_0054 = 0;
    self->field_0058 = 0;
    self->field_005c = 0;
    self->field_0060 = 10;
    self->field_0064 = 0;
    self->field_0068 = 0;
    self->field_006c = 0;
    self->field_0070 = 10;
    self->field_0074 = 0;
    self->field_0078 = 0;
    self->field_007c = 0;
    self->field_0080 = 10;
    self->field_0084 = 0;
    self->field_0088 = 0;
    self->field_008c = 0;
    self->field_0090 = 10;
    self->field_0094 = 0;
    self->field_0098 = 0;
    self->field_00a0 = 0;
    self->field_00a4 = 0;
    self->field_00a8 = 0;
    self->field_00ac = 0;
    self->field_00b0 = 0;
    self->field_00b4 = 0;
    self->field_00b8 = 10;
    self->field_00c8 = 0;
    self->field_00cc = 0;
    self->field_00d0 = 0;
    self->field_00d4 = 10;
    self->field_00d8 = 0;
    self->field_00dc = 0;
    self->field_00e0 = 0;
    self->field_00e4 = 10;
    self->field_00f0 = 0;
    self->field_00f4 = 0;
    self->field_00f8 = 0;
    self->field_00fc = 0;
    self->field_0108 = 0;
    self->field_010c = 0;
    self->field_0110 = 0;
    self->field_0114 = 10;
    self->field_0140 = 0;
    self->field_0144 = 0;
    self->field_0148 = 0;
    self->field_014c = 10;
    self->field_015c = 0;
    self->field_0160 = 0;
    self->field_0164 = 0;
    self->field_01e8 = 0;
    self->field_01ec = 0;
    self->field_01f0 = 0;
    self->field_01f4 = 10;
    self->field_01f8 = 0;
    self->field_01fc = 0;
    self->field_0204 = 10;
    self->field_0200 = 0;
    CSF_AT(void*, self, 0x21C) = CSF_VTBL(0x0082D448);
    self->field_0220 = -1;
    self->field_0224 = -1;
    self->field_0238 = 10;
    self->field_022c = 0;
    self->field_0230 = 0;
    self->field_0234 = 0;
    self->field_0254 = 0;
    self->field_0258 = 0;
    self->field_0280 = 10;
    self->field_0274 = 0;
    self->field_0278 = 0;
    self->field_027c = 0;
    self->field_0294 = 10;
    self->field_0288 = 0;
    self->field_028c = 0;
    self->field_0290 = 0;
    self->m_colorNormal = 0;
    self->field_02a8 = 0;
    self->field_02ac = 0;
    self->field_02b0 = 1065353216;
    self->m_colorAlert = 0;
    self->field_02b8 = 0;
    self->field_02bc = 0;
    self->field_02c0 = 1065353216;
    Hud_ResetWidgetState_42D6F0(reinterpret_cast<std::uint8_t*>(self) + 0x2D4, nullptr);

    self->field_0118 = 0;
    self->field_0119 = 0;
    self->field_011a = 0;
    self->field_011b = 0;
    self->field_011c = 0;
    self->field_011d = 0;
    self->field_011e = 0;
    self->field_011f = 0;
    self->field_0120 = 0;
    self->field_0121 = 0;
    self->field_0122 = 0;
    self->field_0123 = 0;
    self->field_0124 = 0;
    self->field_0125 = 0;
    self->field_0126 = 0;
    self->field_0127 = 0;
    self->field_023d = 0;
    self->field_025c = 0;
    self->field_023c = 0;
    self->field_0128 = 0;
    self->field_012c = 0;
    self->field_00bc = 0;
    self->field_00e8 = 0;
    self->field_00ec = 0;
    self->field_0100 = 0;
    self->field_0104 = 0;
    self->field_0248 = 0;
    self->field_0298 = 0;
    self->field_029c = 0;
    self->m_textLabel = 0;
    self->field_0244 = 0;
    self->field_0208 = 0;
    self->field_020c = 0;
    self->field_0130 = 0;
    self->field_0134 = 0;
    self->field_0138 = 0;
    self->field_013c = 0;
    self->field_0250 = 0;
    self->field_02a0 = 1;
    self->field_02a1 = 1;
    self->field_02a2 = 1;
    self->m_colorNormal = Hud_EncodeFloat(g_ColorWhite);
    self->field_02a8 = Hud_EncodeFloat(g_ColorWhite_G);
    self->field_02ac = Hud_EncodeFloat(g_ColorWhite_B);
    self->field_02b0 = Hud_EncodeFloat(g_ColorWhite_A);
    self->m_colorAlert = Hud_EncodeFloat(g_ColorRed);
    self->field_02b8 = Hud_EncodeFloat(g_DefaultColorGreen);
    self->field_02bc = Hud_EncodeFloat(g_DefaultColorBlue);
    self->field_02c0 = Hud_EncodeFloat(g_DefaultColorAlpha);
    self->field_02c4 = 0;
    self->field_0234 = 0;
    MpSpawnPoints_RegisterObject(reinterpret_cast<int>(self));

    auto* renderHook = reinterpret_cast<C_00860178*>(CoreMemory_AllocUntagged(28));
    if (renderHook)
    {
        C_00860178_Construct(renderHook, nullptr);
        renderHook->vtbl = CSF_VTBL(0x00830918);
        C_00860894_Init(reinterpret_cast<C_00860894*>(
            reinterpret_cast<std::uint8_t*>(renderHook) + 8), nullptr, -1, 0);
        CSF_AT(CHud*, renderHook, 4) = self;
    }
    self->m_renderHook = reinterpret_cast<std::uint32_t>(renderHook);

    void* sceneManager = reinterpret_cast<void*>(GetGlobalValue_6C5760());
    renderHook = reinterpret_cast<C_00860178*>(self->m_renderHook);
    int sceneRegion = C_0085e7b8_GetSceneRegionAt(
        CSF_AT(C_0085e7b8*, sceneManager, 4), nullptr, 0);
    auto* regionBinding = reinterpret_cast<C_008670f0*(__fastcall*)(void*, void*)>(
        renderHook->vtbl[28 / 4])(renderHook, nullptr);
    C_008670f0_SetField0C(regionBinding, nullptr, sceneRegion);

    void* configuration = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    // The string includes its inline buffer, length and capacity.
    alignas(8) std::uint8_t labelNameBytes[0x34];
    CSF_AT(std::uint32_t, labelNameBytes, 0x18) = 15;
    CSF_AT(std::uint32_t, labelNameBytes, 0x14) = 0;
    CSF_AT(char, labelNameBytes, 4) = 0;
    AssignStringSubstring(labelNameBytes, nullptr,
        reinterpret_cast<std::uint8_t*>(configuration) + 6140, 0, -1);

    void* controls = MenuControls_ResolveObject(reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    const char* labelName = CSF_AT(std::uint32_t, labelNameBytes, 0x18) < 0x10
        ? reinterpret_cast<const char*>(labelNameBytes + 4)
        : CSF_AT(const char*, labelNameBytes, 4);
    self->m_textLabel = reinterpret_cast<std::uint32_t>(
        reinterpret_cast<void*(__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, controls, 0)[80 / 4])(controls, nullptr, labelName));

    alignas(8) std::uint8_t colorBytes[0x34];
    float* color = reinterpret_cast<float*>(colorBytes);
    alignas(8) std::uint8_t normalizedSizeBytes[8];
    float* normalizedSize = reinterpret_cast<float*>(normalizedSizeBytes);
    alignas(8) std::uint8_t sizeBytes[8];
    float* size = reinterpret_cast<float*>(sizeBytes);

    if (self->m_textLabel)
    {
        size[0] = 1.0f;
        size[1] = 1.0f;
        normalizedSize[0] = 0.0f;
        normalizedSize[1] = 0.0f;
        Hud_WriteNormalizedScreenSize(normalizedSize);
        void* label = reinterpret_cast<void*>(self->m_textLabel);
        size[0] *= normalizedSize[0];
        size[1] *= normalizedSize[1];
        reinterpret_cast<void(__fastcall*)(void*, void*, float*)>(
            CSF_AT(void**, label, 0)[152 / 4])(label, nullptr, size);

        label = reinterpret_cast<void*>(self->m_textLabel);
        color[0] = 255.0f;
        color[1] = 255.0f;
        color[2] = 0.0f;
        color[3] = 255.0f;
        reinterpret_cast<void(__fastcall*)(void*, void*, float*)>(
            CSF_AT(void**, label, 0)[96 / 4])(label, nullptr, color);
        label = reinterpret_cast<void*>(self->m_textLabel);
        reinterpret_cast<void(__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, label, 0)[60 / 4])(
                label, nullptr, CSF_STR(0x0083098C, "                              "));
    }

    controls = MenuControls_ResolveObject(reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    self->field_0244 = reinterpret_cast<std::uint32_t>(
        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, controls, 0)[72 / 4])(controls, nullptr));
    if (self->field_0244)
    {
        color[0] = 0.0f;
        color[1] = 0.0f;
        color[2] = 0.0f;
        color[3] = 128.0f;
        void* background = reinterpret_cast<void*>(self->field_0244);
        reinterpret_cast<void(__fastcall*)(void*, void*, float*)>(
            CSF_AT(void**, background, 0)[44 / 4])(background, nullptr, color);
    }

    self->field_024c = 0;
    int barHeight = static_cast<int>(static_cast<std::int64_t>(
        static_cast<double>(g_ScreenHeight) *
        CSF_AT(float, reinterpret_cast<void*>(GetGlobalValue_753A30()), 532)));

    controls = MenuControls_ResolveObject(reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    self->m_letterboxBottom = reinterpret_cast<std::uint32_t>(
        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, controls, 0)[72 / 4])(controls, nullptr));
    color[0] = 0.0f;
    color[1] = 0.0f;
    color[2] = 0.0f;
    color[3] = 255.0f;
    void* bar = reinterpret_cast<void*>(self->m_letterboxBottom);
    reinterpret_cast<void(__fastcall*)(void*, void*, float*)>(
        CSF_AT(void**, bar, 0)[44 / 4])(bar, nullptr, color);

    // These widget slots take integer coordinates, not floating-point vectors.
    alignas(8) std::uint8_t coordinatesBytes[8];
    auto* coordinates = reinterpret_cast<std::uint32_t*>(coordinatesBytes);
    bar = reinterpret_cast<void*>(self->m_letterboxBottom);
    coordinates[0] = -1;
    coordinates[1] = g_ScreenHeight - barHeight + 2;
    reinterpret_cast<void(__fastcall*)(void*, void*, std::uint32_t*)>(
        CSF_AT(void**, bar, 0)[28 / 4])(bar, nullptr, coordinates);
    coordinates[0] = g_ScreenWidth + 2;
    coordinates[1] = barHeight + 2;
    bar = reinterpret_cast<void*>(self->m_letterboxBottom);
    reinterpret_cast<void(__fastcall*)(void*, void*, std::uint32_t*)>(
        CSF_AT(void**, bar, 0)[36 / 4])(bar, nullptr, coordinates);

    controls = MenuControls_ResolveObject(reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    self->m_letterboxTop = reinterpret_cast<std::uint32_t>(
        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, controls, 0)[72 / 4])(controls, nullptr));
    color[0] = 0.0f;
    color[1] = 0.0f;
    color[2] = 0.0f;
    color[3] = 255.0f;
    bar = reinterpret_cast<void*>(self->m_letterboxTop);
    reinterpret_cast<void(__fastcall*)(void*, void*, float*)>(
        CSF_AT(void**, bar, 0)[44 / 4])(bar, nullptr, color);
    bar = reinterpret_cast<void*>(self->m_letterboxTop);
    coordinates[0] = -1;
    coordinates[1] = -1;
    reinterpret_cast<void(__fastcall*)(void*, void*, std::uint32_t*)>(
        CSF_AT(void**, bar, 0)[28 / 4])(bar, nullptr, coordinates);
    coordinates[0] = g_ScreenWidth + 2;
    bar = reinterpret_cast<void*>(self->m_letterboxTop);
    coordinates[1] = barHeight + 1;
    reinterpret_cast<void(__fastcall*)(void*, void*, std::uint32_t*)>(
        CSF_AT(void**, bar, 0)[36 / 4])(bar, nullptr, coordinates);

    auto* panelMemory = reinterpret_cast<C_0083f0ec*>(CoreMemory_AllocUntagged(100));
    self->m_subtitlePanel = panelMemory
        ? reinterpret_cast<std::uint32_t>(C_0083f0ec_ConstructSubtitlePanel(panelMemory, nullptr))
        : 0;
    HudStringShow_AddFlags(reinterpret_cast<void*>(self->m_subtitlePanel), nullptr, 4);
    HudStringShow_AddFlags(reinterpret_cast<void*>(self->m_subtitlePanel), nullptr, 8);
    CSF_AT(int, reinterpret_cast<void*>(self->m_subtitlePanel), 0x34) = -1;
    auto* subtitlePanel = reinterpret_cast<C_0083f0ec*>(self->m_subtitlePanel);
    subtitlePanel->field_0054 = CSF_AT(std::uint32_t,
        reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 6372);
    reinterpret_cast<C_0083f0ec*>(self->m_subtitlePanel)->field_0058 = 1;
    reinterpret_cast<C_0083f0ec*>(self->m_subtitlePanel)->field_0059 = 1;
    reinterpret_cast<C_0083f0ec*>(self->m_subtitlePanel)->field_0024 = 1;

    float subtitleBase = g_TransitionOne -
        CSF_AT(float, reinterpret_cast<void*>(GetGlobalValue_753A30()), 532);
    void* subtitleSettings = reinterpret_cast<void*>(GetGlobalValue_753A30());
    void* positioningSettings = reinterpret_cast<void*>(GetGlobalValue_753A30());
    float subtitleY = CSF_AT(float, positioningSettings, 536) *
        CSF_AT(float, subtitleSettings, 532) + subtitleBase;
    CCamera_SetSubtitlePanelPosition(reinterpret_cast<void*>(self->m_subtitlePanel),
        nullptr, 1033476506, static_cast<int>(Hud_EncodeFloat(subtitleY)));

    self->field_02cc = 0;
    self->field_02d0 = 0;
    self->field_0154 = 0;
    CHud_InitializeConfiguredWidgets(self, nullptr);
    CHud_LayoutDebugOverlayWidget(self, nullptr);
    if (CSF_AT(std::uint32_t, labelNameBytes, 0x18) >= 0x10 &&
        CSF_AT(void*, labelNameBytes, 4))
    {
        Mem_FreeDebug(CSF_AT(void*, labelNameBytes, 4));
    }
    return reinterpret_cast<int>(self);
}

HOOK(0x004355D0, CHud_ConstructInstance, "auto");
