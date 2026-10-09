#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));

CSF_GLOBAL(unsigned int, g_ScreenWidth, 0x00899B9C);
CSF_GLOBAL(unsigned int, g_ScreenHeight, 0x00899BA0);

static void Hud_SetWidgetPosition(void* widget, int* position)
{
    ((void(__fastcall*)(void*, void*, int*))
        CSF_AT(void**, widget, 0)[7])(widget, nullptr, position);
}

static int Hud_SetWidgetSize(void* widget, int* size)
{
    return ((int(__fastcall*)(void*, void*, int*))
        CSF_AT(void**, widget, 0)[9])(widget, nullptr, size);
}

static int __fastcall Hud_CreateCenteredBorder(char* self, void* /*edx*/, int index)
{
    char* border = self + 32u * static_cast<std::uint32_t>(index);

    for (int i = 0; i < 8; ++i)
    {
        void* controls = MenuControls_ResolveObject(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr);
        void* widget = ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, controls, 0)[18])(controls, nullptr);

        CSF_AT(void*, border, 0x168 + 4 * i) = widget;
        float color[4] = {0.0f, 0.0f, 0.0f, 255.0f};
        ((void(__fastcall*)(void*, void*, float*))
            CSF_AT(void**, widget, 0)[11])(widget, nullptr, color);
    }

    char* entry = CSF_AT(char*, self, 0x15C)
        + 48u * static_cast<std::uint32_t>(index);
    void* content = CSF_AT(void*, entry, 0x1C);
    if (!content)
        return static_cast<int>(reinterpret_cast<std::uintptr_t>(entry));

    unsigned int screenHeight = g_ScreenHeight;
    unsigned int screenWidth = g_ScreenWidth;
    int* contentSize = ((int*(__fastcall*)(void*, void*))
        CSF_AT(void**, content, 0)[15])(content, nullptr);

    int centerX = static_cast<std::int32_t>(
        screenWidth - static_cast<unsigned int>(contentSize[0])) / 2;
    int centerY = static_cast<std::int32_t>(
        screenHeight - static_cast<unsigned int>(contentSize[1])) / 2;

    int insetSize[2] = {centerX + 8, centerY + 8};
    int cornerPosition[2] = {-1, -1};
    int scratch[2];
    int position[2];

    Hud_SetWidgetPosition(CSF_AT(void*, border, 0x168), cornerPosition);
    Hud_SetWidgetSize(CSF_AT(void*, border, 0x168), insetSize);

    scratch[0] = centerX - 2;
    cornerPosition[1] = scratch[0];
    scratch[1] = -1;
    Hud_SetWidgetPosition(CSF_AT(void*, border, 0x16C), scratch);

    scratch[0] = contentSize[0];
    scratch[1] = insetSize[1];
    Hud_SetWidgetSize(CSF_AT(void*, border, 0x16C), scratch);

    scratch[0] = centerX + contentSize[0] - 4;
    scratch[1] = -1;
    Hud_SetWidgetPosition(CSF_AT(void*, border, 0x170), scratch);
    Hud_SetWidgetSize(CSF_AT(void*, border, 0x170), insetSize);

    position[1] = centerY - 2;
    scratch[1] = centerY - 2;
    position[0] = -1;
    Hud_SetWidgetPosition(CSF_AT(void*, border, 0x174), position);

    position[0] = insetSize[0];
    position[1] = contentSize[1];
    Hud_SetWidgetSize(CSF_AT(void*, border, 0x174), position);

    position[0] = centerX + contentSize[0] - 4;
    position[1] = scratch[1];
    Hud_SetWidgetPosition(CSF_AT(void*, border, 0x178), position);

    position[0] = insetSize[0];
    position[1] = contentSize[1];
    Hud_SetWidgetSize(CSF_AT(void*, border, 0x178), position);

    position[1] = contentSize[1] + centerY - 4;
    position[0] = -1;
    Hud_SetWidgetPosition(CSF_AT(void*, border, 0x17C), position);
    Hud_SetWidgetSize(CSF_AT(void*, border, 0x17C), insetSize);

    position[0] = cornerPosition[1];
    position[1] = contentSize[1] + centerY - 4;
    char* bottomEdge = self
        + 32u * static_cast<std::uint32_t>(index) + 384;
    Hud_SetWidgetPosition(CSF_AT(void*, bottomEdge, 0), position);

    position[0] = contentSize[0];
    position[1] = insetSize[1];
    Hud_SetWidgetSize(CSF_AT(void*, bottomEdge, 0), position);

    int contentHeight = contentSize[1];
    position[0] = centerX + contentSize[0] - 4;
    position[1] = contentHeight + centerY - 4;
    Hud_SetWidgetPosition(CSF_AT(void*, border, 0x184), position);
    return Hud_SetWidgetSize(CSF_AT(void*, border, 0x184), insetSize);
}

HOOK(0x00433910, Hud_CreateCenteredBorder, "auto");
