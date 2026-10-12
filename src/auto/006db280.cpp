#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, RwCameraEndUpdate, 0x00628E40, (void* camera));
CSF_FUNC(void*, __cdecl, RwCameraBeginUpdate, 0x00628E50, (void* camera));
CSF_FUNC(int, __fastcall, ResolveClumpManager, 0x00652020, (int managerId, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(void, __fastcall, ApplyCameraTransform, 0x006DB0D0, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, S_006e48a0_UpdateAtomicGeometry, 0x006E4E90, (S_006e48a0* self, void* , C_0085e7b8* object, uint32_t* instances, int mode));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_ProgressScale, 0x0082E03C);

static int __fastcall AtomicGeometry_ProcessPendingUpdates(void* self, void* /*edx*/)
{
    if (CSF_AT(int32_t, self, 0x34) <= 0)
        return 1;

    const int mode = CSF_AT(int, self, 0xC8);
    if (mode == 4)
        return 1;

    ++CSF_AT(uint32_t, self, 0xD0);
    if (mode != 3)
    {
        ApplyCameraTransform(static_cast<uint8_t*>(self) + 0x44, nullptr);
        ApplyCameraTransform(static_cast<uint8_t*>(self) + 0x84, nullptr);
    }

    void* globalObject = reinterpret_cast<void*>(GetGlobalValue_6C5760());
    void* camera = GetCachedObjectProvider(
        CSF_AT(void*, globalObject, 0xF0), nullptr);
    RwCameraEndUpdate(camera);

    for (int i = 0; i < CSF_AT(int, self, 0x2C); ++i)
    {
        void* object = CSF_AT(void*, CSF_AT(void*, self, 0x24), 4 * i);
        void* instance =
            reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, object, 0)[0xB0 / 4])(object, nullptr);
        if (instance)
        {
            void* geometry = CSF_AT(void*, instance, 0x7C);
            if (geometry)
                CSF_AT(void*, geometry, 0x24) = object;
        }
    }

    int budget = CSF_AT(int, self, 0x34);
    switch (CSF_AT(int, self, 0xC8))
    {
    case 1:
        budget = static_cast<int32_t>(2u * static_cast<uint32_t>(budget)) / 3;
        break;
    case 2:
        budget /= 2;
        break;
    }

    int updated = 0;
    void* clumpManager = reinterpret_cast<void*>(
        ResolveClumpManager(GetGlobalInt(), nullptr));
    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, clumpManager, 0)[0x80 / 4])(clumpManager, nullptr, 1);

    if (CSF_AT(uint32_t, self, 0xE0) != 0)
    {
        int index = CSF_AT(int, self, 0xE8);
        while (index != -1)
        {
            if (updated >= budget)
                break;

            const uint32_t offset = 12 * static_cast<uint32_t>(index);
            C_0085e7b8* object =
                CSF_AT(C_0085e7b8*, CSF_AT(void*, self, 0xD8), offset);
            S_006e48a0* geometry = CSF_AT(S_006e48a0*, object, 0x7C);
            if (geometry)
            {
                const float progress =
                    CSF_AT(float, self, 0xD4) + CSF_AT(float, geometry, 0x30);
                CSF_AT(float, geometry, 0x30) =
                    progress > g_TransitionOne ? 1.0f : progress;

                if (S_006e48a0_UpdateAtomicGeometry(
                        geometry, nullptr, object,
                        static_cast<uint32_t*>(self), CSF_AT(int, self, 0xC8)))
                {
                    ++updated;
                    CSF_AT(uint32_t, geometry, 0x28) =
                        CSF_AT(uint32_t, self, 0xD0);
                }
            }
            index = CSF_AT(int, CSF_AT(void*, self, 0xD8), offset + 4);
        }
    }
    CSF_AT(uint32_t, self, 0xE8) = 0;
    CSF_AT(uint32_t, self, 0xEC) = 0;
    CSF_AT(uint32_t, self, 0xE0) = 0;

    if (CSF_AT(uint32_t, self, 0xF8) != 0)
    {
        int index = CSF_AT(int, self, 0x100);
        while (index != -1)
        {
            if (updated >= budget)
                break;

            const uint32_t offset = 12 * static_cast<uint32_t>(index);
            C_0085e7b8* object =
                CSF_AT(C_0085e7b8*, CSF_AT(void*, self, 0xF0), offset);
            S_006e48a0* geometry = CSF_AT(S_006e48a0*, object, 0x7C);
            if (geometry)
            {
                const float progress =
                    CSF_AT(float, geometry, 0x30) - CSF_AT(float, self, 0xD4);
                CSF_AT(float, geometry, 0x30) =
                    progress > g_fZero ? progress : 0.0f;

                if (S_006e48a0_UpdateAtomicGeometry(
                        geometry, nullptr, object,
                        static_cast<uint32_t*>(self), CSF_AT(int, self, 0xC8)))
                {
                    ++updated;
                    if (CSF_AT(float, geometry, 0x30) > g_ProgressScale)
                        CSF_AT(uint32_t, geometry, 0x28) =
                            CSF_AT(uint32_t, self, 0xD0);
                }
            }
            index = CSF_AT(int, CSF_AT(void*, self, 0xF0), offset + 4);
        }
    }
    CSF_AT(uint32_t, self, 0x100) = 0;
    CSF_AT(uint32_t, self, 0x104) = 0;
    CSF_AT(uint32_t, self, 0xF8) = 0;

    if (CSF_AT(uint32_t, self, 0x14) != 0)
    {
        int index = CSF_AT(int, self, 0x1C);
        while (index != -1)
        {
            if (updated >= CSF_AT(int, self, 0x10C))
                break;

            const uint32_t offset = 16 * static_cast<uint32_t>(index);
            void* entry = static_cast<uint8_t*>(CSF_AT(void*, self, 0x0C)) + offset;
            C_0085e7b8* object = CSF_AT(C_0085e7b8*, entry, 0);
            S_006e48a0* geometry = CSF_AT(S_006e48a0*, object, 0x7C);
            if (geometry &&
                CSF_AT(uint32_t, geometry, 0x28) != CSF_AT(uint32_t, self, 0xD0))
            {
                const float progress =
                    CSF_AT(float, self, 0xD4) + CSF_AT(float, geometry, 0x30);
                CSF_AT(float, geometry, 0x30) =
                    progress > g_TransitionOne ? 1.0f : progress;

                if (S_006e48a0_UpdateAtomicGeometry(
                        geometry, nullptr, CSF_AT(C_0085e7b8*, entry, 0),
                        static_cast<uint32_t*>(self), CSF_AT(int, self, 0xC8)))
                {
                    ++updated;
                    CSF_AT(uint32_t, geometry, 0x28) =
                        CSF_AT(uint32_t, self, 0xD0);
                }
            }
            index = CSF_AT(int, CSF_AT(void*, self, 0x0C), offset + 8);
        }
    }

    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, clumpManager, 0)[0x80 / 4])(clumpManager, nullptr, 0);

    const int result = RwCameraBeginUpdate(camera) != nullptr;
    CSF_AT(uint32_t, self, 0xD4) = 0;
    return result;
}

HOOK(0x006DB280, AtomicGeometry_ProcessPendingUpdates, "auto");
