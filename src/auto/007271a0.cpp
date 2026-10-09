#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwTexDictionaryForAllTextures, 0x006316E0, (int dictionary, int (__cdecl* callback)(uint32_t*, int), int context));
CSF_FUNC(int, __cdecl, UpdateObjectValueTotal, 0x00726D00, (uint32_t* texture, int context));

CSF_GLOBAL(float, g_RasterByteScale, 0x00843C14);
CSF_GLOBAL(std::uint32_t, g_ObjectValueTotal, 0x008BBF34);

static int __fastcall C_008655e8_UpdateTextureValueTotal(
    C_008655e8* self, void* /*edx*/)
{
    std::uint32_t total = 0;
    const int dictionary = static_cast<int>(self->field_002c);
    g_ObjectValueTotal = 0;
    RwTexDictionaryForAllTextures(dictionary, UpdateObjectValueTotal, 0);

    const std::uint32_t begin = self->field_0048;
    g_ObjectValueTotal = 0;
    std::uint32_t remaining = begin
        ? static_cast<std::uint32_t>(
              static_cast<std::int32_t>(self->field_004c - begin) / 36)
        : 0;
    std::uint32_t result = remaining;

    if (remaining != 0)
    {
        auto* entry = reinterpret_cast<std::uint8_t*>(self->field_0048);
        do
        {
            auto* raster = reinterpret_cast<void*>(
                CSF_AT(std::uint32_t, entry, 0));

            const double value =
                static_cast<double>(CSF_AT(std::int32_t, raster, 0x14))
                * g_RasterByteScale
                * static_cast<double>(CSF_AT(std::int32_t, raster, 0x10))
                * static_cast<double>(CSF_AT(std::int32_t, raster, 0x0C));

            result = static_cast<std::uint32_t>(
                static_cast<std::int64_t>(value));
            total += result;
            entry += 36;
            --remaining;
        } while (remaining != 0);

        g_ObjectValueTotal = total;
    }

    return static_cast<int>(result);
}

HOOK(0x007271A0, C_008655e8_UpdateTextureValueTotal, "auto");
