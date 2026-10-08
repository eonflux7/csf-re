#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_RasterByteScale, 0x00843C14);

CSF_FUNC(char, __cdecl, NormalizeResourceName, 0x006949D0,
         (char* source, char* destination, unsigned int capacity));
CSF_FUNC(int, __fastcall, C_008655e8_FindRasterIndex, 0x00727020,
         (C_008655e8* self, void* edx, char* source));
CSF_FUNC(int*, __cdecl, LoadNamedRaster, 0x006E2EF0, (char* name));
CSF_FUNC(void*, __fastcall, AssignResourceString, 0x00429C60,
         (void* self, void* edx, char* text));
CSF_FUNC(std::uint32_t*, __fastcall, AppendElement36, 0x00728160,
         (void* self, void* edx, int* entry));
CSF_FUNC(int, __fastcall, ResetStringStorage_727250, 0x00727250,
         (int* self, void* edx));

static int* __fastcall C_008655e8_AcquireRaster(
    C_008655e8* self, void* /*edx*/, char* source)
{
    if (!source || !*source)
        return nullptr;

    char normalizedName[256] = {};
    NormalizeResourceName(source, normalizedName, 256);

    const int index =
        C_008655e8_FindRasterIndex(self, nullptr, normalizedName);

    if (index != -1)
    {
        const std::uint32_t offset =
            static_cast<std::uint32_t>(index) * 36;

        void* entry = reinterpret_cast<void*>(self->field_0048 + offset);
        ++CSF_AT(std::uint32_t, entry, 0x20);

        return CSF_AT(
            int*, reinterpret_cast<void*>(self->field_0048 + offset), 0);
    }

    int* raster = LoadNamedRaster(normalizedName);
    if (!raster)
        return nullptr;

    alignas(4) std::uint8_t entry[36];
    CSF_AT(std::uint32_t, entry, 0x1c) = 15;
    CSF_AT(std::uint32_t, entry, 0x18) = 0;
    CSF_AT(std::uint8_t, entry, 0x08) = 0;
    CSF_AT(std::uint32_t, entry, 0x20) = 1;
    CSF_AT(int*, entry, 0) = raster;

    AssignResourceString(entry + 4, nullptr, normalizedName);
    AppendElement36(
        reinterpret_cast<std::uint8_t*>(self) + 0x44,
        nullptr, reinterpret_cast<int*>(entry));

    double storageBytes =
        static_cast<double>(CSF_AT(std::int32_t, raster, 0x14));
    storageBytes *= static_cast<double>(g_RasterByteScale);
    storageBytes *= static_cast<double>(
        CSF_AT(std::int32_t, raster, 0x10));
    storageBytes *= static_cast<double>(
        CSF_AT(std::int32_t, raster, 0x0c));

    self->field_0054 += static_cast<std::uint32_t>(
        static_cast<std::int64_t>(storageBytes));

    ResetStringStorage_727250(reinterpret_cast<int*>(entry), nullptr);
    return raster;
}

HOOK(0x00728280, C_008655e8_AcquireRaster, "auto");
