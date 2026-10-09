#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, Hud_NeedsWidgetReset, 0x0042D980, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Hud_ApplyWidgetRectangle, 0x0042E950, (void* self, void* edx, int a2, int a3, int a4, int a5));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static void* Hud_GetPrimaryWidget(void* self)
{
    return CSF_AT(void*, CSF_AT(void*, self, 0x4), 0x4);
}

static void* Hud_GetTrailingWidget(void* self)
{
    return CSF_AT(void*, CSF_AT(void*, self, 0x4), 0x8);
}

static int Hud_GetWidgetValue(void* widget, unsigned slot)
{
    return reinterpret_cast<int (__fastcall*)(void*, void*)>(
        CSF_AT(void**, widget, 0)[slot])(widget, nullptr);
}

static int* Hud_GetWidgetPair(void* widget, unsigned slot)
{
    return reinterpret_cast<int* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, widget, 0)[slot])(widget, nullptr);
}

static int* Hud_ReadWidgetRectangle(void* widget, int* rectangle)
{
    return reinterpret_cast<int* (__fastcall*)(void*, void*, int*)>(
        CSF_AT(void**, widget, 0)[22])(widget, nullptr, rectangle);
}

static void Hud_WriteWidgetValues(void* widget, unsigned slot, int* values)
{
    reinterpret_cast<void (__fastcall*)(void*, void*, int*)>(
        CSF_AT(void**, widget, 0)[slot])(widget, nullptr, values);
}

static char __fastcall Hud_UpdateDelayedWidgetBar(void* self, void* /*edx*/, float elapsed)
{
    char result = CSF_AT(char, self, 0x118);
    if (!result)
        return result;

    void* context = reinterpret_cast<void*>(GetSceneMenuContext(g_pCurrentScene, nullptr));
    int currentValue = 0;
    int maximumValue = 0;
    int width = 0;

    if (context)
    {
        currentValue = Hud_GetWidgetValue(CSF_AT(void*, context, 0x38), 40);
        maximumValue = Hud_GetWidgetValue(CSF_AT(void*, context, 0x38), 33);
        if (maximumValue != 0)
        {
            width = currentValue * CSF_AT(int, self, 0x130) / maximumValue;
            if (width < 0)
                return static_cast<char>(width);
        }
    }

    int primarySize[2];
    {
        int* values = Hud_GetWidgetPair(Hud_GetPrimaryWidget(self), 15);
        primarySize[0] = values[0];
        primarySize[1] = values[1];
    }
    primarySize[0] = width;
    Hud_WriteWidgetValues(Hud_GetPrimaryWidget(self), 14, primarySize);

    int primaryRectangle[4];
    Hud_ReadWidgetRectangle(Hud_GetPrimaryWidget(self), primaryRectangle);
    primaryRectangle[2] = primaryRectangle[0]
        + width * CSF_AT(int, self, 0x134) / CSF_AT(int, self, 0x130);
    Hud_WriteWidgetValues(Hud_GetPrimaryWidget(self), 21, primaryRectangle);

    int primaryPosition[2];
    {
        int* values = Hud_GetWidgetPair(Hud_GetPrimaryWidget(self), 11);
        primaryPosition[0] = values[0];
        primaryPosition[1] = values[1];
    }

    int trailingSize[2];
    {
        int* values = Hud_GetWidgetPair(Hud_GetTrailingWidget(self), 15);
        trailingSize[0] = values[0];
        trailingSize[1] = values[1];
    }

    int trailingPosition[2];
    {
        int* values = Hud_GetWidgetPair(Hud_GetTrailingWidget(self), 11);
        trailingPosition[0] = values[0];
        trailingPosition[1] = values[1];
    }

    int trailingRectangle[4];
    Hud_ReadWidgetRectangle(Hud_GetTrailingWidget(self), trailingRectangle);

    if (currentValue > CSF_AT(int, self, 0x218)
        || Hud_NeedsWidgetReset(self, nullptr))
    {
        Hud_ApplyWidgetRectangle(self, nullptr,
            primaryPosition[0] + primarySize[0], 0,
            primaryRectangle[2], primaryRectangle[2]);
        CSF_AT(int, self, 0x20C) = 0;
        CSF_AT(int, self, 0x214) = 0;
        CSF_AT(int, self, 0x210) = 0;
        CSF_AT(int, self, 0x218) = currentValue;
    }

    result = static_cast<char>(trailingPosition[0] + trailingSize[0]);
    if (primaryPosition[0] + primarySize[0]
        < trailingPosition[0] + trailingSize[0])
    {
        int previousValue = CSF_AT(int, self, 0x218);
        if (previousValue > currentValue)
        {
            int lostWidth = CSF_AT(int, self, 0x130)
                * (previousValue - currentValue) / maximumValue;
            void* widget = Hud_GetTrailingWidget(self);
            int* size = Hud_GetWidgetPair(widget, 15);
            int rectangle[4];
            int* bounds = Hud_ReadWidgetRectangle(widget, rectangle);

            Hud_ApplyWidgetRectangle(self, nullptr,
                primarySize[0] + primaryPosition[0],
                lostWidth + size[0], primaryRectangle[2], bounds[2]);

            int* updatedSize = Hud_GetWidgetPair(Hud_GetTrailingWidget(self), 15);
            CSF_AT(int, self, 0x210) = updatedSize[0];

            widget = Hud_GetTrailingWidget(self);
            int* firstBounds = Hud_ReadWidgetRectangle(widget, rectangle);
            int otherRectangle[4];
            int right = Hud_ReadWidgetRectangle(widget, otherRectangle)[2];
            CSF_AT(int, self, 0x214) = right - firstBounds[0];
            CSF_AT(int, self, 0x218) = currentValue;
            CSF_AT(int, self, 0x20C) = 0;
        }

        CSF_AT(float, self, 0x20C) = elapsed + CSF_AT(float, self, 0x20C);
        void* settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
        float duration = CSF_AT(float, settings, 0xA64);
        if (duration > CSF_AT(float, self, 0x20C))
        {
            double remaining = (duration - CSF_AT(float, self, 0x20C)) / duration;
            result = static_cast<char>(Hud_ApplyWidgetRectangle(self, nullptr,
                primarySize[0] + primaryPosition[0],
                static_cast<int>(CSF_AT(int, self, 0x210) * remaining),
                primaryRectangle[2],
                static_cast<int>(CSF_AT(int, self, 0x214) * remaining
                    + trailingRectangle[0])));
        }
        else
        {
            result = static_cast<char>(Hud_ApplyWidgetRectangle(self, nullptr,
                primarySize[0] + primaryPosition[0], 0,
                primaryRectangle[2], primaryRectangle[2]));
            CSF_AT(int, self, 0x20C) = 0;
        }
    }

    return result;
}

HOOK(0x00430D20, Hud_UpdateDelayedWidgetBar, "auto");
