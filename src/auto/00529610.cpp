#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160,
         (void* self, void* edx));
// Capture the residual EAX value: this callee is logically void.
CSF_FUNC(int, __fastcall, CGuiStatic_CopyStateToTarget, 0x00718230,
         (void* self, void* edx, int* target));

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(float, g_WidthReduction, 0x0082E0F0);
CSF_GLOBAL(float, g_VerticalInset, 0x0082FEBC);
CSF_GLOBAL(float, g_HorizontalInset, 0x008412FC);
CSF_GLOBAL(std::uint32_t, g_ScreenWidth, 0x00899B9C);
CSF_GLOBAL(std::uint32_t, g_ScreenHeight, 0x00899BA0);

static int __fastcall CMenuMessageBox_UpdateStaticLayout(
    CMenuMessageBox* self, void* /*edx*/)
{
    void* container = reinterpret_cast<void*>(self->field_0894);
    void* reference =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, container, 0)[0x1CC / 4])(container, nullptr);

    const std::uint32_t initialStatic = self->m_staticControl;
    if (!initialStatic || !reference)
        return static_cast<int>(initialStatic);

    void* display = LookupObjectById(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    void* dimensions =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, display, 0)[0x18 / 4])(display, nullptr);

    const double displayHeight = CSF_AT(std::int32_t, dimensions, 4);
    const std::int32_t heightSign = static_cast<std::int32_t>(g_ScreenHeight);
    double screenHeight = static_cast<std::int32_t>(g_ScreenHeight);
    if (heightSign < 0)
        screenHeight += g_UnsignedIntegerBias;
    const float verticalScale = static_cast<float>(displayHeight / screenHeight);

    display = LookupObjectById(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    dimensions =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, display, 0)[0x18 / 4])(display, nullptr);

    const double displayWidth = CSF_AT(std::int32_t, dimensions, 0);
    const std::int32_t widthSign = static_cast<std::int32_t>(g_ScreenWidth);
    double screenWidth = static_cast<std::int32_t>(g_ScreenWidth);
    if (widthSign < 0)
        screenWidth += g_UnsignedIntegerBias;
    const float horizontalScale = static_cast<float>(displayWidth / screenWidth);

    void* control = reinterpret_cast<void*>(self->m_staticControl);
    int* stateTarget =
        reinterpret_cast<int* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, control, 0)[0x110 / 4])(control, nullptr);

    int retainedHeight = 0;
    if (stateTarget) {
        retainedHeight =
            reinterpret_cast<int (__fastcall*)(void*, void*)>(
                CSF_AT(void**, stateTarget, 0)[0xA0 / 4])(stateTarget, nullptr);
    }

    std::uint32_t size[2] = {0, 0};
    std::uint32_t position[2] = {0, 0};

    void* referencePosition =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, reference, 0)[0x20 / 4])(reference, nullptr);
    const std::int64_t horizontalOffset = static_cast<std::int64_t>(
        static_cast<double>(horizontalScale) * g_HorizontalInset);
    position[0] = CSF_AT(std::uint32_t, referencePosition, 0)
                - static_cast<std::uint32_t>(horizontalOffset);

    referencePosition =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, reference, 0)[0x20 / 4])(reference, nullptr);
    const std::int64_t verticalOffset = static_cast<std::int64_t>(
        static_cast<double>(verticalScale) * g_VerticalInset);
    position[1] = CSF_AT(std::uint32_t, referencePosition, 4)
                - static_cast<std::uint32_t>(verticalOffset);
    size[1] = static_cast<std::uint32_t>(retainedHeight);

    void* referenceSize =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, reference, 0)[0x28 / 4])(reference, nullptr);
    const std::int64_t widthReduction = static_cast<std::int64_t>(
        static_cast<double>(horizontalScale) * g_WidthReduction);
    size[0] = CSF_AT(std::uint32_t, referenceSize, 0)
            - static_cast<std::uint32_t>(widthReduction);

    control = reinterpret_cast<void*>(self->m_staticControl);
    reinterpret_cast<void (__fastcall*)(void*, void*, std::uint32_t*)>(
        CSF_AT(void**, control, 0)[0x98 / 4])(control, nullptr, position);

    control = reinterpret_cast<void*>(self->m_staticControl);
    reinterpret_cast<void (__fastcall*)(void*, void*, std::uint32_t*)>(
        CSF_AT(void**, control, 0)[0xA0 / 4])(control, nullptr, size);

    control = reinterpret_cast<void*>(self->m_staticControl);
    return CGuiStatic_CopyStateToTarget(control, nullptr, stateTarget);
}

HOOK(0x00529610, CMenuMessageBox_UpdateStaticLayout, "auto");
