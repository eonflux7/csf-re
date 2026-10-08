#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_00714270_ComputeDoubledFieldOffset,
         0x00714270, (void* self, void* edx));

static void* MenuMpServerInfo_GetControlSize(void* control)
{
    return reinterpret_cast<void* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, control, 0)[0xA4 / 4])(control, nullptr);
}

static void* MenuMpServerInfo_GetControlPosition(void* control)
{
    return reinterpret_cast<void* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, control, 0)[0x9C / 4])(control, nullptr);
}

static int MenuMpServerInfo_SetControlPosition(
    void* control, std::uint32_t* position)
{
    return reinterpret_cast<int (__fastcall*)(void*, void*, std::uint32_t*)>(
        CSF_AT(void**, control, 0)[0x98 / 4])(control, nullptr, position);
}

static int MenuMpServerInfo_SetControlSize(
    void* control, std::uint32_t* size)
{
    return reinterpret_cast<int (__fastcall*)(void*, void*, std::uint32_t*)>(
        CSF_AT(void**, control, 0)[0xA0 / 4])(control, nullptr, size);
}

static int __fastcall CMenuMpServerInfo_UpdateStatisticsLayout(
    CMenuMpServerInfo* self, void* /*edx*/)
{
    if (self->field_01e4 == 0)
    {
        const std::uint32_t rowStep = static_cast<std::uint32_t>(
            C_00714270_ComputeDoubledFieldOffset(
                reinterpret_cast<void*>(self->field_0010), nullptr));
        const std::uint32_t verticalOffset =
            rowStep * (self->field_01f8 + 1u);

        void* sourcePosition = MenuMpServerInfo_GetControlPosition(
            reinterpret_cast<void*>(self->field_0010));

        std::uint32_t spectatorPosition[2];
        spectatorPosition[0] =
            CSF_AT(std::uint32_t, sourcePosition, 0x00);
        spectatorPosition[1] =
            CSF_AT(std::uint32_t, sourcePosition, 0x04) + verticalOffset;

        return MenuMpServerInfo_SetControlPosition(
            reinterpret_cast<void*>(self->field_001c), spectatorPosition);
    }

    // Preserve the original component reads, including unused coordinates.
    void* redSize = MenuMpServerInfo_GetControlSize(
        reinterpret_cast<void*>(self->field_0014));
    (void)CSF_AT(volatile std::uint32_t, redSize, 0x04);
    const std::int32_t redWidth =
        CSF_AT(std::int32_t, redSize, 0x00);

    void* redPosition = MenuMpServerInfo_GetControlPosition(
        reinterpret_cast<void*>(self->field_0014));
    const std::uint32_t redX =
        CSF_AT(std::uint32_t, redPosition, 0x00);
    (void)CSF_AT(volatile std::uint32_t, redPosition, 0x04);

    void* blueSize = MenuMpServerInfo_GetControlSize(
        reinterpret_cast<void*>(self->field_0018));
    const std::int32_t blueWidth =
        CSF_AT(std::int32_t, blueSize, 0x00);
    (void)CSF_AT(volatile std::uint32_t, blueSize, 0x04);

    void* bluePosition = MenuMpServerInfo_GetControlPosition(
        reinterpret_cast<void*>(self->field_0018));
    const std::uint32_t blueX =
        CSF_AT(std::uint32_t, bluePosition, 0x00);
    (void)CSF_AT(volatile std::uint32_t, bluePosition, 0x04);

    std::uint32_t sidePosition[2] = {0, 0};
    std::uint32_t sideSize[2] = {0, 0};
    std::uint32_t nextPosition[2];

    // Unsigned arithmetic preserves the original wrapping ADD/IMUL results.
    const std::uint32_t redRowStep = static_cast<std::uint32_t>(
        C_00714270_ComputeDoubledFieldOffset(
            reinterpret_cast<void*>(self->field_0014), nullptr));
    void* redControl = reinterpret_cast<void*>(self->field_0014);
    const std::uint32_t redVerticalOffset =
        redRowStep * (self->field_01fc + 1u);

    void* currentRedPosition =
        MenuMpServerInfo_GetControlPosition(redControl);
    nextPosition[0] =
        CSF_AT(std::uint32_t, currentRedPosition, 0x00);
    nextPosition[1] =
        CSF_AT(std::uint32_t, currentRedPosition, 0x04) + redVerticalOffset;

    MenuMpServerInfo_SetControlPosition(
        reinterpret_cast<void*>(self->field_0018), nextPosition);

    const std::uint32_t redHalfWidth =
        static_cast<std::uint32_t>(redWidth / 2);
    sidePosition[1] = nextPosition[1];
    sideSize[0] = redHalfWidth;
    sidePosition[0] = redHalfWidth + redX;
    sideSize[1] = 30;

    MenuMpServerInfo_SetControlPosition(
        reinterpret_cast<void*>(self->field_0028), sidePosition);
    MenuMpServerInfo_SetControlSize(
        reinterpret_cast<void*>(self->field_0028), sideSize);

    const std::uint32_t blueRowStep = static_cast<std::uint32_t>(
        C_00714270_ComputeDoubledFieldOffset(
            reinterpret_cast<void*>(self->field_0018), nullptr));
    const std::uint32_t blueVerticalOffset =
        blueRowStep * (self->field_0200 + 1u);

    void* currentBluePosition = MenuMpServerInfo_GetControlPosition(
        reinterpret_cast<void*>(self->field_0018));
    nextPosition[0] =
        CSF_AT(std::uint32_t, currentBluePosition, 0x00);
    nextPosition[1] =
        CSF_AT(std::uint32_t, currentBluePosition, 0x04) + blueVerticalOffset;

    MenuMpServerInfo_SetControlPosition(
        reinterpret_cast<void*>(self->field_001c), nextPosition);

    const std::uint32_t blueHalfWidth =
        static_cast<std::uint32_t>(blueWidth / 2);
    sidePosition[1] = nextPosition[1];
    sideSize[0] = blueHalfWidth;
    sidePosition[0] = blueHalfWidth + blueX;
    sideSize[1] = 30;

    MenuMpServerInfo_SetControlPosition(
        reinterpret_cast<void*>(self->field_002c), sidePosition);
    return MenuMpServerInfo_SetControlSize(
        reinterpret_cast<void*>(self->field_002c), sideSize);
}

HOOK(0x00530800, CMenuMpServerInfo_UpdateStatisticsLayout, "auto");
