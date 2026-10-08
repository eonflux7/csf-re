#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, MenuProgressInput_GetGlobalContext, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuProgressInput_GetDisplay,
         0x00652160, (void* self, void* edx));

// Capture the residual EAX value: the original layout routine returns it,
// although this callee's logical interface is void.
CSF_FUNC(int, __fastcall, MenuProgressInput_CopyStateToTarget,
         0x00718230, (void* self, void* edx, int* target));

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(float, g_LayoutInset, 0x0082FEBC);
CSF_GLOBAL(float, g_LayoutWidthAdjustment, 0x00833E44);
CSF_GLOBAL(std::uint32_t, g_ScreenWidth, 0x00899B9C);
CSF_GLOBAL(std::uint32_t, g_ScreenHeight, 0x00899BA0);

static void* C_008414e8_GetLayoutRow(C_008414e8* self, unsigned index)
{
    switch (index) {
    case 0: return reinterpret_cast<void*>(self->field_0024);
    case 1: return reinterpret_cast<void*>(self->field_0028);
    case 2: return reinterpret_cast<void*>(self->field_002c);
    default: return reinterpret_cast<void*>(self->field_0030);
    }
}

static int C_008414e8_SubtractCoordinate(int coordinate, std::uint32_t amount)
{
    return static_cast<int>(static_cast<std::uint32_t>(coordinate) - amount);
}

static int __fastcall C_008414e8_LayoutInputRows(
    C_008414e8* self, void* /*edx*/)
{
    void* context = reinterpret_cast<void*>(
        MenuProgressInput_GetGlobalContext());
    void* display = MenuProgressInput_GetDisplay(context, nullptr);
    int* dimensions =
        reinterpret_cast<int* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, display, 0)[0x18 / 4])(display, nullptr);

    const double displayHeight = dimensions[1];
    const std::uint32_t heightSign = g_ScreenHeight;
    double screenHeight = static_cast<std::int32_t>(g_ScreenHeight);
    if (static_cast<std::int32_t>(heightSign) < 0)
        screenHeight += static_cast<double>(g_UnsignedIntegerBias);
    const float heightScale = static_cast<float>(displayHeight / screenHeight);

    context = reinterpret_cast<void*>(MenuProgressInput_GetGlobalContext());
    display = MenuProgressInput_GetDisplay(context, nullptr);
    dimensions =
        reinterpret_cast<int* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, display, 0)[0x18 / 4])(display, nullptr);

    const double displayWidth = dimensions[0];
    const std::uint32_t widthSign = g_ScreenWidth;
    double screenWidth = static_cast<std::int32_t>(g_ScreenWidth);
    if (static_cast<std::int32_t>(widthSign) < 0)
        screenWidth += static_cast<double>(g_UnsignedIntegerBias);

    void* anchor = reinterpret_cast<void*>(self->field_0034);
    int size[2] = {0, 0};
    int position[2] = {0, 0};
    const float widthScale = static_cast<float>(displayWidth / screenWidth);

    int* anchorPosition =
        reinterpret_cast<int* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, anchor, 0)[0x20 / 4])(anchor, nullptr);

    const std::uint32_t horizontalInset = static_cast<std::uint32_t>(
        static_cast<std::int64_t>(
            static_cast<double>(widthScale) * g_LayoutInset));

    const int anchorX = anchorPosition[0];
    anchor = reinterpret_cast<void*>(self->field_0034);
    position[0] = C_008414e8_SubtractCoordinate(anchorX, horizontalInset);

    const std::uint32_t verticalInset = static_cast<std::uint32_t>(
        static_cast<std::int64_t>(
            static_cast<double>(heightScale) * g_LayoutInset));

    anchorPosition =
        reinterpret_cast<int* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, anchor, 0)[0x20 / 4])(anchor, nullptr);
    const int anchorY = anchorPosition[1];
    anchor = reinterpret_cast<void*>(self->field_0034);
    position[1] = C_008414e8_SubtractCoordinate(anchorY, verticalInset);

    int* anchorSize =
        reinterpret_cast<int* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, anchor, 0)[0x28 / 4])(anchor, nullptr);

    const std::uint32_t widthAdjustment = static_cast<std::uint32_t>(
        static_cast<std::int64_t>(
            static_cast<double>(widthScale) * g_LayoutWidthAdjustment));
    size[0] = C_008414e8_SubtractCoordinate(anchorSize[0], widthAdjustment);

    int result;
    for (unsigned index = 0; index != 4; ++index) {
        void* row = C_008414e8_GetLayoutRow(self, index);
        reinterpret_cast<void (__fastcall*)(void*, void*, int*)>(
            CSF_AT(void**, row, 0)[0x98 / 4])(row, nullptr, position);

        row = C_008414e8_GetLayoutRow(self, index);
        void* source =
            reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, row, 0)[0x110 / 4])(row, nullptr);
        const int rowHeight =
            reinterpret_cast<int (__fastcall*)(void*, void*)>(
                CSF_AT(void**, source, 0)[0xA0 / 4])(source, nullptr);

        row = C_008414e8_GetLayoutRow(self, index);
        size[1] = rowHeight;
        reinterpret_cast<void (__fastcall*)(void*, void*, int*)>(
            CSF_AT(void**, row, 0)[0xA0 / 4])(row, nullptr, size);

        row = C_008414e8_GetLayoutRow(self, index);
        source =
            reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, row, 0)[0x110 / 4])(row, nullptr);

        row = C_008414e8_GetLayoutRow(self, index);
        result = MenuProgressInput_CopyStateToTarget(
            row, nullptr, reinterpret_cast<int*>(source));

        position[1] = static_cast<int>(
            static_cast<std::uint32_t>(position[1]) +
            static_cast<std::uint32_t>(size[1]) - verticalInset);
    }
    return result;
}

HOOK(0x0052C890, C_008414e8_LayoutInputRows, "auto");
