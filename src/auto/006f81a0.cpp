#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

using AtomicVisitorFn = int (__cdecl*)(int atomic, int data);

CSF_FUNC(int, __cdecl, RpClumpForAllAtomics, 0x0058EF00, (int clump, AtomicVisitorFn visitor, int data));
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ResolveRwManager, 0x006B13B0, (int managerId, void* edx));
CSF_FUNC(int, __fastcall, GfxScreenEffects_CreateManagedRaster, 0x006D0BA0, (void* self, void* edx, int a2, int a3, int a4, char a5, uint32_t* a6));
CSF_FUNC(int, __fastcall, SceneLoader_GetSectorEntry, 0x006D4720, (S_006d4450* self, void* , int sectorIndex));
CSF_FUNC(int, __fastcall, GfxTextureLoader_FindModelIndex, 0x006D58E0, (S_006d4450* self, void* , char* String2, int));

static char __fastcall GfxMaterials_CreateRasterAndBindModel(
    void* self, void* /*edx*/, char* modelName)
{
    char success = 1;

    LookupObjectById(reinterpret_cast<void*>(GetGlobalInt()), nullptr);

    uint32_t* rasterStorage = &CSF_AT(uint32_t, self, 0x64);
    int manager = ResolveRwManager(GetGlobalInt(), nullptr);
    int managedRaster = GfxScreenEffects_CreateManagedRaster(
        reinterpret_cast<void*>(manager), nullptr, 512, 512, 24, 1,
        rasterStorage);

    CSF_AT(int, self, 0x60) = managedRaster;
    if (managedRaster)
        CSF_AT(uint8_t, reinterpret_cast<void*>(*rasterStorage), 0x50) = 2;
    else
        success = 0;

    int accessor = GlobalInt_ResolveAccessor(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    auto* provider = static_cast<S_006d4450*>(
        GetCachedObjectProvider(reinterpret_cast<void*>(accessor), nullptr));

    if (!modelName)
        return success;

    int modelIndex = GfxTextureLoader_FindModelIndex(
        provider, nullptr, modelName, -1);
    int sectorEntry = SceneLoader_GetSectorEntry(provider, nullptr, modelIndex);

    if (CSF_AT(int, reinterpret_cast<void*>(sectorEntry), 0x28))
    {
        sectorEntry = SceneLoader_GetSectorEntry(provider, nullptr, modelIndex);
        void* clumpStorage = CSF_AT(
            void*, reinterpret_cast<void*>(sectorEntry), 0x20);
        int clump = CSF_AT(int, clumpStorage, 0);
        int raster = static_cast<int>(*rasterStorage);

        CSF_AT(int, self, 0x68) = clump;
        RpClumpForAllAtomics(
            clump,
            reinterpret_cast<AtomicVisitorFn>(CSF_ADDR_FN(0x006F8070)),
            raster);
    }

    return success;
}

HOOK(0x006F81A0, GfxMaterials_CreateRasterAndBindModel, "auto");
