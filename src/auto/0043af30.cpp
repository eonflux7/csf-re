#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// ---------------------------------------------------------------------------
// 0x0043AF30  C_00830e5c::RenderRadarWidget   (minimap, .MINIMAPA_RADAR_MP)
//
// Draws one radar widget centred on the radar rectangle (+ the item's offset).
// Depending on the item flags the widget is rotated by its own heading, by the
// radar rotation (self+0x48), or by both.
//
// Item layout (as used here):
//   +0x00 void*  widget
//   +0x04 float  heading in degrees (-1.0f = "no heading")
//   +0x08 float  offsetX
//   +0x0C float  offsetY
//   +0x10 bool   rotates with its own heading
//   +0x11 bool   rotates with the radar
// ---------------------------------------------------------------------------

CSF_FUNC(unsigned int, __cdecl, Hud_WriteNormalizedScreenSize, 0x0042DE60, (float* out));
CSF_FUNC(void, __cdecl, Hud_RenderWidgetAtScaledRect, 0x0042F4F0, (void* widget, const int* rect));
CSF_FUNC(int, __cdecl, RwV3dTransformPoints, 0x00623040, (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __fastcall, GfxMatrix_Scale, 0x006ADB60, (void* self, void* , int a2, int a3, int a4));
CSF_FUNC(int, __fastcall, GfxMatrix_Translate, 0x006ADBB0, (void* self, void* , int a2, int a3, int a4));
CSF_FUNC(int, __fastcall, GfxMatrix_RotateAroundAxisZ, 0x006AE210, (int* self, void* , float angle));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);   // 0.5f
CSF_GLOBAL(float, g_RadarDegToRad, 0x00830D1C);       // pi / 180

// The matrix helpers take their float arguments as raw 32-bit stack slots.
static int FloatArg(float value)
{
    int raw;
    std::memcpy(&raw, &value, sizeof(raw));
    return raw;
}

static char __fastcall C_00830e5c_RenderRadarWidget(C_00830e5c* self, void* /*edx*/, void* item)
{
    const int32_t left = static_cast<int32_t>(self->field_0024);
    const int32_t top = static_cast<int32_t>(self->field_0028);
    const int32_t width = static_cast<int32_t>(self->field_002c - self->field_0024);
    const int32_t height = static_cast<int32_t>(self->field_0030 - self->field_0028);

    const float centerX = static_cast<float>(width) * g_HalfDurationScale + static_cast<float>(left);
    const float centerY = static_cast<float>(height) * g_HalfDurationScale + static_cast<float>(top);

    float widgetPos[2];
    widgetPos[0] = centerX + CSF_AT(float, item, 0x8);
    widgetPos[1] = centerY + CSF_AT(float, item, 0xC);

    void* widget = CSF_AT(void*, item, 0x0);
    ((void(__fastcall*)(void*, void*, float*))CSF_AT(void**, widget, 0)[0x24 / 4])(widget, nullptr, widgetPos);

    // Scoped matrix wrapper: { RwMatrix* matrix; bool owned; }
    alignas(8) uint8_t gfxMatrix[8];
    CSF_AT(std::uint32_t*, gfxMatrix, 0x0) = RwMatrixCreate();
    CSF_AT(uint8_t, gfxMatrix, 0x4) = 1;

    // Identity
    CSF_AT(float, CSF_AT(std::uint32_t*, gfxMatrix, 0x0), 0x28) = 1.0f;
    CSF_AT(float, CSF_AT(std::uint32_t*, gfxMatrix, 0x0), 0x14) = 1.0f;
    CSF_AT(float, CSF_AT(std::uint32_t*, gfxMatrix, 0x0), 0x00) = 1.0f;
    CSF_AT(float, CSF_AT(std::uint32_t*, gfxMatrix, 0x0), 0x10) = 0.0f;
    CSF_AT(float, CSF_AT(std::uint32_t*, gfxMatrix, 0x0), 0x08) = 0.0f;
    CSF_AT(float, CSF_AT(std::uint32_t*, gfxMatrix, 0x0), 0x04) = 0.0f;
    CSF_AT(float, CSF_AT(std::uint32_t*, gfxMatrix, 0x0), 0x24) = 0.0f;
    CSF_AT(float, CSF_AT(std::uint32_t*, gfxMatrix, 0x0), 0x20) = 0.0f;
    CSF_AT(float, CSF_AT(std::uint32_t*, gfxMatrix, 0x0), 0x18) = 0.0f;
    CSF_AT(float, CSF_AT(std::uint32_t*, gfxMatrix, 0x0), 0x38) = 0.0f;
    CSF_AT(float, CSF_AT(std::uint32_t*, gfxMatrix, 0x0), 0x34) = 0.0f;
    CSF_AT(float, CSF_AT(std::uint32_t*, gfxMatrix, 0x0), 0x30) = 0.0f;
    CSF_AT(uint32_t, CSF_AT(std::uint32_t*, gfxMatrix, 0x0), 0x0C) |= 0x20003u;

    widget = CSF_AT(void*, item, 0x0);
    ((void(__fastcall*)(void*, void*, void*))CSF_AT(void**, widget, 0)[0x60 / 4])(widget, nullptr, gfxMatrix);

    float screenScale[2];
    screenScale[0] = 0.0f;
    screenScale[1] = 0.0f;
    Hud_WriteNormalizedScreenSize(screenScale);

    // The original tests the raw heading against the bit pattern of -1.0f.
    const bool hasHeading = CSF_AT(uint32_t, item, 0x4) != 0xBF800000u;

    if (hasHeading && CSF_AT(uint8_t, item, 0x10) && !CSF_AT(uint8_t, item, 0x11))
    {
        // Rotate around the widget position by the item's own heading.
        const float heading = CSF_AT(float, item, 0x4);
        GfxMatrix_Translate(gfxMatrix, nullptr, FloatArg(-widgetPos[0]), FloatArg(-widgetPos[1]), 0);
        GfxMatrix_RotateAroundAxisZ(reinterpret_cast<int*>(gfxMatrix), nullptr, -heading * g_RadarDegToRad);
        GfxMatrix_Scale(gfxMatrix, nullptr, FloatArg(screenScale[0]), FloatArg(screenScale[1]), 0);
        GfxMatrix_Translate(gfxMatrix, nullptr,
                            FloatArg(screenScale[0] * widgetPos[0]),
                            FloatArg(screenScale[1] * widgetPos[1]), 0);

        widget = CSF_AT(void*, item, 0x0);
        ((void(__fastcall*)(void*, void*, void*))CSF_AT(void**, widget, 0)[0x60 / 4])(widget, nullptr, gfxMatrix);
        widget = CSF_AT(void*, item, 0x0);
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, widget, 0)[0x8 / 4])(widget, nullptr);
    }
    else if (hasHeading && CSF_AT(uint8_t, item, 0x10) && CSF_AT(uint8_t, item, 0x11))
    {
        // Own heading around the widget position, then the radar rotation around the radar centre.
        const float heading = CSF_AT(float, item, 0x4);
        GfxMatrix_Translate(gfxMatrix, nullptr, FloatArg(-widgetPos[0]), FloatArg(-widgetPos[1]), 0);
        GfxMatrix_RotateAroundAxisZ(reinterpret_cast<int*>(gfxMatrix), nullptr, -heading * g_RadarDegToRad);
        GfxMatrix_Translate(gfxMatrix, nullptr, FloatArg(widgetPos[0]), FloatArg(widgetPos[1]), 0);
        GfxMatrix_Translate(gfxMatrix, nullptr, FloatArg(-centerX), FloatArg(-centerY), 0);
        GfxMatrix_RotateAroundAxisZ(reinterpret_cast<int*>(gfxMatrix), nullptr, self->field_0048 * g_RadarDegToRad);
        GfxMatrix_Scale(gfxMatrix, nullptr, FloatArg(screenScale[0]), FloatArg(screenScale[1]), 0);
        GfxMatrix_Translate(gfxMatrix, nullptr,
                            FloatArg(screenScale[0] * centerX),
                            FloatArg(screenScale[1] * centerY), 0);

        widget = CSF_AT(void*, item, 0x0);
        ((void(__fastcall*)(void*, void*, void*))CSF_AT(void**, widget, 0)[0x60 / 4])(widget, nullptr, gfxMatrix);
        widget = CSF_AT(void*, item, 0x0);
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, widget, 0)[0x8 / 4])(widget, nullptr);
    }
    else if (CSF_AT(uint8_t, item, 0x10) || CSF_AT(uint8_t, item, 0x11))
    {
        // Only the position follows the radar rotation; the widget itself is drawn unrotated.
        GfxMatrix_Translate(gfxMatrix, nullptr, FloatArg(-centerX), FloatArg(-centerY), 0);
        GfxMatrix_RotateAroundAxisZ(reinterpret_cast<int*>(gfxMatrix), nullptr, self->field_0048 * g_RadarDegToRad);
        GfxMatrix_Translate(gfxMatrix, nullptr, FloatArg(centerX), FloatArg(centerY), 0);

        float sourcePoint[3];
        float rotatedPoint[3];
        sourcePoint[0] = widgetPos[0];
        sourcePoint[1] = widgetPos[1];
        sourcePoint[2] = 0.0f;
        RwV3dTransformPoints(static_cast<int>(reinterpret_cast<intptr_t>(rotatedPoint)),
                             static_cast<int>(reinterpret_cast<intptr_t>(sourcePoint)),
                             1,
                             static_cast<int>(reinterpret_cast<intptr_t>(CSF_AT(std::uint32_t*, gfxMatrix, 0x0))));

        float rotatedPos[2];
        rotatedPos[0] = rotatedPoint[0];
        rotatedPos[1] = rotatedPoint[1];

        widget = CSF_AT(void*, item, 0x0);
        ((void(__fastcall*)(void*, void*, float*))CSF_AT(void**, widget, 0)[0x24 / 4])(widget, nullptr, rotatedPos);
        Hud_RenderWidgetAtScaledRect(CSF_AT(void*, item, 0x0), nullptr);
    }
    else
    {
        Hud_RenderWidgetAtScaledRect(CSF_AT(void*, item, 0x0), nullptr);
    }

    // Scoped matrix destructor.
    char result = static_cast<char>(CSF_AT(uint8_t, gfxMatrix, 0x4));
    if (result)
        return static_cast<char>(RwMatrixDestroy(CSF_AT(std::uint32_t*, gfxMatrix, 0x0)));
    return result;
}

HOOK(0x0043AF30, C_00830e5c_RenderRadarWidget, "auto");
