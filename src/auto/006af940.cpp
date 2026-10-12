#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(const void*, __cdecl, RwImageSetPath, 0x00625270, (const void* path));
CSF_FUNC(unsigned int, __cdecl, SceneLoader_CopyMapFilename, 0x006948E0, (char* a1, char* a2, unsigned int a3));
CSF_FUNC(char*, __cdecl, Hud_ExtractResourcePath, 0x00694960, (char* a1, uint8_t* a2, unsigned int a3));

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);

[[maybe_unused]] static const auto g_ArrayGrowthVtableReference =
    CSF_VTBL(0x0041E5F0);

static char __fastcall C_0082d410_LoadDisplayTiles(
    C_0082d410* self, void* /*edx*/, int descriptorAddress)
{
    auto* descriptor = reinterpret_cast<uint8_t*>(descriptorAddress);
    alignas(8) uint8_t filenameBytes[0x100];
    alignas(8) uint8_t pathBytes[0x104];
    alignas(8) uint8_t rectangleBytes[0x2C];

    pathBytes[0] = 0;
    char* inlineFilename = reinterpret_cast<char*>(descriptor + 4);
    char* resourceFilename = inlineFilename;
    if (CSF_AT(uint32_t, descriptor, 0x18) >= 0x10u)
        resourceFilename = CSF_AT(char*, descriptor, 4);

    Hud_ExtractResourcePath(resourceFilename, pathBytes, 256);
    RwImageSetPath(pathBytes);

    char* mapFilename = inlineFilename;
    if (CSF_AT(uint32_t, descriptor, 0x18) >= 0x10u)
        mapFilename = CSF_AT(char*, descriptor, 4);

    SceneLoader_CopyMapFilename(
        mapFilename, reinterpret_cast<char*>(filenameBytes), 256);

    uint32_t column = 0;
    uint32_t row = 0;
    uint32_t columns = 0;
    float columnStep = 0.0f;
    float rowStep = 0.0f;
    bool firstTile = true;
    bool loading = true;

    for (;;)
    {
        if (!loading)
            return 0;

        void* provider = CSF_AT(void*, self, 0x20);
        void* tile =
            reinterpret_cast<void*(__fastcall*)(void*, void*, uint8_t*)>(
                CSF_AT(void**, provider, 0)[0x38 / 4])(
                    provider, nullptr, filenameBytes);

        if (tile)
        {
            if (firstTile)
            {
                uint32_t* dimensions =
                    reinterpret_cast<uint32_t*(__fastcall*)(void*, void*)>(
                        CSF_AT(void**, tile, 0)[0x5C / 4])(tile, nullptr);
                uint32_t tileSize = CSF_AT(uint32_t, descriptor, 0x24);

                columns = dimensions[0] / tileSize;
                column = 0;
                row = 0;
                columnStep = g_TransitionOne / static_cast<float>(columns);
                uint32_t rows = dimensions[1] / tileSize;
                rowStep = g_TransitionOne / static_cast<float>(rows);
                firstTile = false;
            }

            float* rectangle = reinterpret_cast<float*>(rectangleBytes);
            rectangle[0] = static_cast<float>(column) * columnStep;
            rectangle[1] = static_cast<float>(row) * rowStep;
            ++column;
            rectangle[2] = static_cast<float>(column) * columnStep;
            uint32_t nextRow = row + 1;
            rectangle[3] = static_cast<float>(nextRow) * rowStep;

            reinterpret_cast<void(__fastcall*)(void*, void*, float*)>(
                CSF_AT(void**, tile, 0)[0x4C / 4])(
                    tile, nullptr, rectangle);
            reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, tile, 0)[0x38 / 4])(
                    tile, nullptr, descriptor + 0x1C);

            if (self->field_0008 >= self->field_0004)
            {
                S_0041e5f0_GrowArrayCapacity(
                    reinterpret_cast<S_0041e5f0*>(self), nullptr,
                    CSF_AT(int, self, 0x0C));
            }

            uint32_t index = self->field_0008;
            auto* destination =
                reinterpret_cast<void**>(self->vtbl) + index;
            self->field_0008 = index + 1;
            *destination = tile;

            if (column >= columns)
            {
                row = nextRow;
                column = 0;
            }
        }
        else
        {
            loading = false;
        }

        uint32_t tileIndex = column + columns * row;
        if (tileIndex >= 0x40)
            return static_cast<char>(tileIndex);
    }
}

HOOK(0x006AF940, C_0082d410_LoadDisplayTiles, "auto");
