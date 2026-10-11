#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, WeaponFirearms_BuildSpawnParameters, 0x00477240,
         (void* self, void* edx, int a2, int a3));
CSF_FUNC(void, __fastcall, MpSpawnPoints_CreateSpawn, 0x0048C370,
         (void* self, void* edx, uint32_t* position, uint32_t* direction,
          int count, float spread, uint32_t type, void* parameters, int direct));
CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0,
         (void* self, void* edx));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall C_008316d8_SpawnProjectile(
    C_008316d8* self, void* /*edx*/, int firingPoint)
{
    self->vSlotB0();

    uint32_t normalizedVectors[6] = {1065353216, 0, 0, 0, 0, 0};
    uint32_t position[3] = {0, 0, 0};
    uint32_t direction[3] = {0, 0, 0};

    using GetFiringVectors = void (__fastcall*)(
        void*, void*, int, uint32_t*, uint32_t*);
    reinterpret_cast<GetFiringVectors>(
        CSF_AT(void**, self, 0)[0xC8 / 4])(
            self, nullptr, firingPoint, position, direction);

    normalizedVectors[2] = direction[2];
    normalizedVectors[0] = direction[0];
    normalizedVectors[1] = direction[1];
    normalizedVectors[5] = position[2];
    normalizedVectors[3] = position[0];
    normalizedVectors[4] = position[1];
    NormalizeVector3(normalizedVectors, nullptr);

    alignas(4) uint8_t parameters[0x34] = {};
    WeaponFirearms_BuildSpawnParameters(
        parameters, nullptr,
        static_cast<int>(self->field_0004 + 28), firingPoint);

    const uint32_t projectileType = self->field_000c;
    void* spawnPoints = CSF_AT(void*, g_pCurrentScene, 0xA8);
    MpSpawnPoints_CreateSpawn(
        spawnPoints, nullptr, position, direction,
        1, 0.0f, projectileType, parameters, 1);

    return self->vSlot80();
}

HOOK(0x00477550, C_008316d8_SpawnProjectile, "auto");
