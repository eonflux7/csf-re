#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int** __fastcall CGuiTextureListBox_RenderTextureCell(
    CGuiTextureListBox* self,
    void* /*edx*/,
    int* origin,
    std::uint32_t* extent,
    int inset,
    int rowIndex,
    int columnIndex)
{
    const std::uint32_t cellsAddress = CSF_AT(
        std::uint32_t,
        reinterpret_cast<void*>(self->field_01fc),
        static_cast<std::uint32_t>(rowIndex) * 16u + 4u);

    int** cell = reinterpret_cast<int**>(
        cellsAddress + static_cast<std::uint32_t>(columnIndex) * 4u);
    int* texture = *cell;
    if (!texture)
        return cell;

    std::uint32_t coordinates[2];
    const std::uint32_t originX = static_cast<std::uint32_t>(origin[0]);
    coordinates[1] = static_cast<std::uint32_t>(origin[1]);
    coordinates[0] = originX + static_cast<std::uint32_t>(inset);

    reinterpret_cast<void (__fastcall*)(void*, void*, std::uint32_t*)>(
        CSF_AT(void**, texture, 0)[0x28 / 4])(
            texture, nullptr, coordinates);

    const std::uint32_t width = extent[0];
    const std::uint32_t height = extent[1];
    coordinates[0] = width - static_cast<std::uint32_t>(inset);
    coordinates[1] = height;

    reinterpret_cast<void (__fastcall*)(void*, void*, std::uint32_t*)>(
        CSF_AT(void**, texture, 0)[0x38 / 4])(
            texture, nullptr, coordinates);

    const std::uint32_t* color =
        reinterpret_cast<const std::uint32_t* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, texture, 0)[0x6C / 4])(texture, nullptr);

    std::uint32_t savedColor[4];
    savedColor[0] = color[0];
    savedColor[1] = color[1];
    savedColor[2] = color[2];
    savedColor[3] = color[3];

    float alpha;
    float alphaMultiplier;
    std::memcpy(&alpha, &savedColor[3], sizeof(alpha));
    std::memcpy(&alphaMultiplier, &self->field_0208, sizeof(alphaMultiplier));

    const float adjustedAlpha = static_cast<float>(
        static_cast<double>(alpha) * static_cast<double>(alphaMultiplier));

    std::uint32_t adjustedColor[4];
    adjustedColor[0] = savedColor[0];
    adjustedColor[1] = savedColor[1];
    adjustedColor[2] = savedColor[2];
    std::memcpy(&adjustedColor[3], &adjustedAlpha, sizeof(adjustedAlpha));

    reinterpret_cast<void (__fastcall*)(void*, void*, std::uint32_t*)>(
        CSF_AT(void**, texture, 0)[0x68 / 4])(
            texture, nullptr, adjustedColor);

    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, texture, 0)[0x08 / 4])(texture, nullptr);

    return reinterpret_cast<int** (__fastcall*)(void*, void*, std::uint32_t*)>(
        CSF_AT(void**, texture, 0)[0x68 / 4])(
            texture, nullptr, savedColor);
}

HOOK(0x00730A40, CGuiTextureListBox_RenderTextureCell, "auto");
