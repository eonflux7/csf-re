#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwRasterDestroy, 0x00627570,
         (std::uint32_t raster));
CSF_FUNC(std::uint32_t*, __cdecl, CopyStringRecordRange, 0x00727320,
         (std::uint32_t* first, std::uint32_t* last,
          std::uint32_t* destination));
CSF_FUNC(int, __cdecl, DestroyStringRecordRange, 0x00727370,
         (std::uint32_t* first, std::uint32_t* last));

CSF_GLOBAL(float, g_RasterByteScale, 0x00843C14);

static void __fastcall C_008655e8_ReleaseRaster(
    C_008655e8* self, void* /*edx*/, std::int32_t* raster)
{
    if (!raster)
        return;

    const std::uint32_t initialBegin = self->field_0048;
    std::uint32_t count = 0;
    if (initialBegin)
    {
        const std::uint32_t difference =
            CSF_AT(std::uint32_t, self, 0x4C) - initialBegin;
        count = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(difference) / 36);
    }

    if (!count)
        return;

    auto* cursor = reinterpret_cast<std::uint32_t*>(self->field_0048);
    std::uint32_t index = 0;
    while (CSF_AT(std::uint32_t, cursor, 0x00) !=
           reinterpret_cast<std::uintptr_t>(raster))
    {
        ++index;
        cursor += 9;
        if (index >= count)
            return;
    }

    const std::uint32_t recordOffset = index * 36u;
    auto* referenceCount = reinterpret_cast<std::uint32_t*>(
        self->field_0048 + recordOffset + 0x20u);
    --*referenceCount;

    auto* record = reinterpret_cast<std::uint32_t*>(
        self->field_0048 + recordOffset);
    if (CSF_AT(std::uint32_t, record, 0x20) != 0)
        return;

    double byteSize = static_cast<double>(
        CSF_AT(std::int32_t, raster, 0x14));
    byteSize *= static_cast<double>(g_RasterByteScale);
    byteSize *= static_cast<double>(
        CSF_AT(std::int32_t, raster, 0x10));
    byteSize *= static_cast<double>(
        CSF_AT(std::int32_t, raster, 0x0C));

    const auto releasedBytes = static_cast<std::int64_t>(byteSize);
    self->field_0054 -= static_cast<std::uint32_t>(releasedBytes);

    RwRasterDestroy(CSF_AT(std::uint32_t, record, 0x00));

    auto* destination = reinterpret_cast<std::uint32_t*>(
        self->field_0048 + recordOffset);
    auto* last = reinterpret_cast<std::uint32_t*>(
        CSF_AT(std::uint32_t, self, 0x4C));
    CopyStringRecordRange(destination + 9, last, destination);

    const std::uint32_t oldEnd = CSF_AT(std::uint32_t, self, 0x4C);
    DestroyStringRecordRange(
        reinterpret_cast<std::uint32_t*>(oldEnd - 36u),
        reinterpret_cast<std::uint32_t*>(oldEnd));

    CSF_AT(std::uint32_t, self, 0x4C) -= 36u;
}

HOOK(0x00727E30, C_008655e8_ReleaseRaster, "auto");
