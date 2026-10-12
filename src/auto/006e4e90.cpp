#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(void*, __cdecl, RwCameraBeginUpdate, 0x00628E50, (void* camera));
CSF_FUNC(int, __stdcall, GfxObjectInstances_RenderAtomicGeometry, 0x006E4530,
         (int a1, int a2, void* a3, int a4, int a5, uint32_t* a6));
CSF_FUNC(void, __fastcall, GfxObjectInstances_PrepareAtomicGeometry, 0x006E4B10,
         (void* self, void* edx, void* a2, uint32_t* a3, int a4, float* a5));
CSF_FUNC(int, __fastcall, GfxObjectInstances_TouchCachedEntry, 0x007013B0,
         (void* self, void* edx, int a2));

static bool __fastcall S_006e48a0_UpdateAtomicGeometry(
    S_006e48a0* self, void* /*edx*/, C_0085e7b8* object,
    uint32_t* instances, int mode)
{
    alignas(8) uint8_t geometryBytes[0x68];
    std::memset(geometryBytes + 0x30, 0, 48);

    GfxObjectInstances_PrepareAtomicGeometry(
        self, nullptr, object, instances, mode,
        reinterpret_cast<float*>(geometryBytes));

    float transition;
    if (((void*(__fastcall*)(void*, void*))
         object->vtbl[0x4C / 4])(object, nullptr))
    {
        void* controller = ((void*(__fastcall*)(void*, void*))
                            object->vtbl[0x4C / 4])(object, nullptr);
        transition = ((float(__fastcall*)(void*, void*))
                      CSF_AT(void**, controller, 0)[8 / 4])(controller, nullptr);
    }
    else
    {
        transition = g_TransitionOne;
    }

    float opacity;
    std::memcpy(&opacity, &self->field_0030, sizeof(opacity));
    float weight = transition * opacity;
    std::memcpy(geometryBytes + 0x60, &weight, sizeof(weight));
    if (!(weight > g_fZero))
        return false;

    void* cachedGeometry = nullptr;
    if (mode != 3)
    {
        uint32_t entryIndex = self->field_0020;
        if (entryIndex != uint32_t(-1))
        {
            auto* entries = reinterpret_cast<uint8_t*>(instances[19]);
            void* entry = entries + (entryIndex << 4);
            if (CSF_AT(S_006e48a0*, entry, 0) == self)
            {
                cachedGeometry = CSF_AT(void*, entry, 0x0C);
                CSF_AT(uint32_t, entry, 8) = instances[30];
                if (CSF_AT(uint32_t, entry, 4) != uint32_t(-1))
                {
                    // Retain the packet's reference annotation for this address.
                    (void)CSF_VTBL(0x007013B0);
                    GfxObjectInstances_TouchCachedEntry(
                        reinterpret_cast<uint8_t*>(instances) + 0x44,
                        nullptr, reinterpret_cast<int>(entry));
                }
            }
        }
        if (!cachedGeometry)
            return false;
    }

    void* globals = reinterpret_cast<void*>(GetGlobalValue_6C5760());
    void* camera = GetCachedObjectProvider(
        CSF_AT(void*, globals, 0xF0), nullptr);
    void* objectRegistry = reinterpret_cast<void*>(GetGlobalInt());
    void* renderObject = LookupObjectById(objectRegistry, nullptr);
    void* device = CSF_AT(void*, renderObject, 0x5C);

    CSF_AT(int, geometryBytes, 0x64) = -1205353044;
    ((int(__stdcall*)(void*, int, int))
     CSF_AT(void**, device, 0)[0xE4 / 4])(
        device, 195, CSF_AT(int, geometryBytes, 0x64));

    if (RwCameraBeginUpdate(camera))
    {
        GfxObjectInstances_RenderAtomicGeometry(
            reinterpret_cast<int>(camera),
            reinterpret_cast<int>(instances),
            cachedGeometry, mode, CSF_AT(int, geometryBytes, 0x60),
            reinterpret_cast<uint32_t*>(geometryBytes));
    }

    ((int(__stdcall*)(void*, int, int))
     CSF_AT(void**, device, 0)[0xE4 / 4])(device, 195, 0);
    return true;
}

HOOK(0x006E4E90, S_006e48a0_UpdateAtomicGeometry, "auto");
