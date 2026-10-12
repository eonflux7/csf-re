#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(double, __fastcall, C_00864508_GetField9c, 0x006D59F0, (C_00864508* self, void*));
CSF_FUNC(bool, __fastcall, GfxSkin_GetListBegin, 0x006DA400, (void* self, void* edx, int* a2));
CSF_FUNC(int, __fastcall, GfxSkin_InsertProvider, 0x006DA440, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(uint32_t*, __fastcall, GfxSkin_InsertSortedProvider, 0x006DA550, (void* self, void* edx, uint32_t* a2));

CSF_GLOBAL(float, g_CoverAttackFarDistanceSquared, 0x0082D4C0);

// The packet also identifies the array-growth address as a vtable reference.
static const auto kArrayGrowthVtableReference = CSF_VTBL(0x0041E5F0);

static void __fastcall C_0085e7b8_RegisterObjectProvider(
    C_0085e7b8* self, void* /*edx*/, C_0085e7b8* provider, int context)
{
    if (CSF_AT(int, self, 0x34) <= 0 ||
        CSF_AT(uint32_t, self, 0xC8) == 4)
        return;

    if (!GetCachedObjectProvider(provider, nullptr))
        return;

    if (!((bool(__fastcall*)(void*, void*))
          CSF_AT(void**, provider, 0)[0x40 / 4])(provider, nullptr))
        return;

    if (!((bool(__fastcall*)(void*, void*))
          CSF_AT(void**, provider, 0)[0x38 / 4])(provider, nullptr))
        return;

    if (!CSF_AT(void*, provider, 0x7C))
        return;

    void* globalState = reinterpret_cast<void*>(GetGlobalInt());
    void* scene = reinterpret_cast<void*>(
        SceneLoader_GetSceneController(globalState, nullptr));
    C_00864508* controller = CSF_AT(C_00864508*, scene, 0xBC);

    void* providerObject =
        ((void*(__fastcall*)(void*, void*))
         CSF_AT(void**, provider, 0)[0x18 / 4])(provider, nullptr);
    void* distanceSettings =
        ((void*(__fastcall*)(void*, void*))
         CSF_AT(void**, providerObject, 0)[0x10 / 4])(
            providerObject, nullptr);

    double sceneDistance = C_00864508_GetField9c(controller, nullptr);
    if (sceneDistance >= CSF_AT(float, distanceSettings, 0x4))
        return;

    if (((bool(__fastcall*)(void*, void*))
         CSF_AT(void**, provider, 0)[0xA8 / 4])(provider, nullptr))
    {
        if (CSF_AT(uint32_t, self, 0x2C) >=
            CSF_AT(uint32_t, self, 0x28))
        {
            S_0041e5f0_GrowArrayCapacity(
                reinterpret_cast<S_0041e5f0*>(
                    reinterpret_cast<uint8_t*>(self) + 0x24),
                nullptr, CSF_AT(int, self, 0x30));
        }

        uint32_t count = CSF_AT(uint32_t, self, 0x2C);
        C_0085e7b8** destination =
            CSF_AT(C_0085e7b8**, self, 0x24) + count;
        CSF_AT(uint32_t, self, 0x2C) = count + 1;
        *destination = provider;
        return;
    }

    void* registration = CSF_AT(void*, provider, 0x7C);
    CSF_AT(uint32_t, registration, 0x24) = 0;

    alignas(8) uint8_t entryBytes[0xC];
    alignas(8) uint8_t iteratorBytes[0x8];

    CSF_AT(C_0085e7b8*, entryBytes, 0) = provider;
    void* position =
        ((void*(__fastcall*)(void*, void*))
         CSF_AT(void**, provider, 0)[0x28 / 4])(provider, nullptr);

    float objectX = CSF_AT(float, position, 0);
    float objectY = CSF_AT(float, position, 0x4);
    float objectZ = CSF_AT(float, position, 0x8);
    void* camera = CSF_AT(void*, reinterpret_cast<void*>(context), 0x4);
    float cameraY = CSF_AT(float, camera, 0x44);
    float cameraX = CSF_AT(float, camera, 0x40);
    float cameraZ = CSF_AT(float, camera, 0x48);
    CSF_AT(float, entryBytes, 0x8) = cameraX;

    uint32_t epoch = CSF_AT(uint32_t, self, 0xD0);
    bool alreadyRegistered = CSF_AT(uint32_t, registration, 0x28) == epoch;

    float dx = cameraX - objectX;
    float dy = cameraY - objectY;
    float dz = cameraZ - objectZ;
    CSF_AT(float, entryBytes, 0x4) = dz * dz + dy * dy + dx * dx;

    CSF_AT(int, iteratorBytes, 0) = -1;
    CSF_AT(uint32_t, iteratorBytes, 0x4) = 0;

    if (alreadyRegistered)
    {
        if (CSF_AT(float, entryBytes, 0x4) < g_CoverAttackFarDistanceSquared)
        {
            GfxSkin_GetListBegin(
                reinterpret_cast<uint8_t*>(self) + 0xD8, nullptr,
                reinterpret_cast<int*>(iteratorBytes));
        }
        else
        {
            GfxSkin_GetListBegin(
                reinterpret_cast<uint8_t*>(self) + 0xF0, nullptr,
                reinterpret_cast<int*>(iteratorBytes));
        }

        GfxSkin_InsertProvider(
            iteratorBytes, nullptr, reinterpret_cast<uint32_t*>(&provider));
        CSF_AT(uint32_t, registration, 0x2C) =
            CSF_AT(uint32_t, self, 0xD0) + 1;
        return;
    }

    CSF_AT(float, registration, 0x30) =
        CSF_AT(uint32_t, registration, 0x2C) == epoch ? 0.0f : 1.0f;

    GfxSkin_GetListBegin(
        &self->field_000c, nullptr, reinterpret_cast<int*>(iteratorBytes));

    int index = CSF_AT(int, iteratorBytes, 0);
    if (index != -1)
    {
        void* list = CSF_AT(void*, iteratorBytes, 0x4);
        do
        {
            uint8_t* node = index == -1
                ? nullptr
                : CSF_AT(uint8_t*, list, 0) +
                    16 * static_cast<uint32_t>(index);

            if (CSF_AT(float, node, 0x4) >= CSF_AT(float, entryBytes, 0x4))
                break;

            if (index != -1)
            {
                node = CSF_AT(uint8_t*, list, 0) +
                    16 * static_cast<uint32_t>(index);
                index = CSF_AT(int, node, 0x8);
            }
        } while (index != -1);

        CSF_AT(int, iteratorBytes, 0) = index;
    }

    GfxSkin_InsertSortedProvider(
        iteratorBytes, nullptr, reinterpret_cast<uint32_t*>(entryBytes));
    CSF_AT(uint32_t, registration, 0x2C) =
        CSF_AT(uint32_t, self, 0xD0) + 1;
}

HOOK(0x006DA700, C_0085e7b8_RegisterObjectProvider, "auto");
