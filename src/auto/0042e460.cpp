#include <cstdint>

#include "globals.h"
#include "hook.h"

CSF_FUNC(unsigned int, __cdecl, FillScreenOffsetPair, 0x0042DE60, (float* a1));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(int, __fastcall, TranslateMatrixHandle, 0x006ADBB0, (void* self, void* edx, float x, float y, float z));
CSF_FUNC(int, __fastcall, RotateMatrixHandle, 0x006AE210, (void* self, void* edx, float radians));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_DegreesToRadians, 0x00830670);

static int __cdecl Hud_DrawWidgetRotatedAroundCenter(int a1, float a2)
{
    if (!a1)
        return 0;

    void* widget = reinterpret_cast<void*>(static_cast<std::uintptr_t>(a1));

    // vtbl slot 44 (11): returns pointer to the widget's position pair
    int* position = reinterpret_cast<int* (__fastcall*)(void*, void*)>(CSF_AT(void**, widget, 0)[11])(widget, nullptr);
    // vtbl slot 60 (15): returns pointer to the widget's size pair
    int* size = reinterpret_cast<int* (__fastcall*)(void*, void*)>(CSF_AT(void**, widget, 0)[15])(widget, nullptr);

    float centerX = static_cast<float>(*size) * g_HalfDurationScale + static_cast<float>(*position);
    float centerY = static_cast<float>(size[1]) * g_HalfDurationScale + static_cast<float>(position[1]);

    float scratchPair[2];
    FillScreenOffsetPair(scratchPair);

    std::uint32_t* matrix = RwMatrixCreate();
    bool ownsMatrix = true;

    // Identity matrix (RW layout: right/up/at/pos rows)
    matrix[10] = 0x3F800000u;
    matrix[5] = 0x3F800000u;
    matrix[0] = 0x3F800000u;
    matrix[4] = 0;
    matrix[2] = 0;
    matrix[1] = 0;
    matrix[9] = 0;
    matrix[8] = 0;
    matrix[6] = 0;
    matrix[14] = 0;
    matrix[13] = 0;
    matrix[12] = 0;
    matrix[3] |= 0x20003u;

    // Move the pivot to the origin, rotate, then move back to the center.
    TranslateMatrixHandle(&matrix, nullptr, -centerX, -centerY, 0.0f);
    float radians = a2 * g_DegreesToRadians;
    RotateMatrixHandle(&matrix, nullptr, radians);
    TranslateMatrixHandle(&matrix, nullptr, centerX, centerY, 0.0f);

    // vtbl slot 96 (24): hands the transform matrix handle to the widget.
    reinterpret_cast<void (__fastcall*)(void*, void*, std::uint32_t**)>(CSF_AT(void**, widget, 0)[24])(widget, nullptr, &matrix);

    if (ownsMatrix)
        RwMatrixDestroy(matrix);

    return a1;
}

HOOK(0x0042E460, Hud_DrawWidgetRotatedAroundCenter, "auto");
