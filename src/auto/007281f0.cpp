#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int*, __cdecl, RwRasterCreate, 0x00627770,
         (int width, int height, int depth, int flags));
CSF_FUNC(std::uint32_t*, __fastcall, InsertRasterRecord, 0x00728160,
         (void* self, void* edx, int* record));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

CSF_GLOBAL(float, g_RasterByteScale, 0x00843C14);

static int* __fastcall C_008655e8_CreateTrackedRaster(
    C_008655e8* self, void* /*edx*/, int width, int height)
{
    int* raster = RwRasterCreate(width, height, 0, 0);
    if (raster)
    {
        int record[9];
        record[7] = 15;
        record[6] = 0;
        reinterpret_cast<std::uint8_t*>(record)[8] = 0;
        record[8] = 1;
        record[0] = reinterpret_cast<int>(raster);

        InsertRasterRecord(
            reinterpret_cast<std::uint8_t*>(self) + 0x44,
            nullptr, record);

        const std::int64_t byteCount = static_cast<std::int64_t>(
            static_cast<double>(CSF_AT(int, raster, 0x14))
            * static_cast<double>(g_RasterByteScale)
            * static_cast<double>(CSF_AT(int, raster, 0x10))
            * static_cast<double>(CSF_AT(int, raster, 0x0C)));

        self->field_0054 += static_cast<std::uint32_t>(byteCount);

        if (static_cast<std::uint32_t>(record[7]) >= 0x10)
        {
            void* block = reinterpret_cast<void*>(record[2]);
            if (block)
                Mem_FreeDebug(block);
        }
    }

    return raster;
}

HOOK(0x007281F0, C_008655e8_CreateTrackedRaster, "auto");
