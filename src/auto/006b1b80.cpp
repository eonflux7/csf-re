#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __fastcall, GetAccountSelectionIndex, 0x004A2FC0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AppMain_GetDisplayRaster, 0x00534FA0, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(C_008608e8*, __cdecl, Skin_GetSingleton, 0x006C74C0, ());
CSF_FUNC(char, __fastcall, C_00860bb8_InitStreamingState, 0x006CB330, (C_00860bb8* self, void*));
CSF_FUNC(int, __fastcall, C_00860bb8_Construct, 0x006CBF30, (C_00860bb8* self, void*));
CSF_FUNC(bool, __fastcall, InitGfxCollisionShapeTables, 0x006CF710, (uint8_t* self, void*));
CSF_FUNC(int, __fastcall, C_00860cbc_Construct, 0x006CF830, (C_00860cbc* self, void*));
CSF_FUNC(unsigned int*, __fastcall, C_00860db0_Construct, 0x006D0B20, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_00861120_Initialize, 0x006D6300, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_00861120_Construct, 0x006D6530, (void* self, void* edx));
CSF_FUNC(uint32_t*, __cdecl, GetResourceScreen, 0x006DB7A0, ());
CSF_FUNC(char, __fastcall, C_00861988_Initialize, 0x006DF810, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, C_00861988_Construct, 0x006DFF90, (void* self, void* edx));
CSF_FUNC(char, __fastcall, InitializeAuxiliaryResource, 0x006E2D10, (void* self, void* edx));
CSF_FUNC(bool, __cdecl, InitializeResourceSubsystem, 0x006E2E60, ());
CSF_FUNC(uint32_t*, __cdecl, GetSharedResource, 0x006E3CD0, ());
CSF_FUNC(C_008655e8*, __fastcall, C_008655e8_InitializeCameraResources, 0x00728000, (C_008655e8* self, void* , int x, int y, int width, int height));
CSF_FUNC(uint8_t, __fastcall, GlobalFlag_IsSet, 0x007A7B50, (void* ecxArg, void* edx));

static uint32_t* __fastcall C_00860130_Construct(
    C_00860130* self, void* /*edx*/, int screenArgument,
    int configurationAddress, uint8_t* successOut)
{
    self->vtbl = CSF_VTBL(0x00860130);
    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0010 = 0;
    auto* resourceArray = reinterpret_cast<S_0041e5f0*>(&self->field_0008);
    self->field_0014 = 10;
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_0028 = 1065353216;
    self->field_002c = 1065353216;
    self->field_0030 = -971227136;
    self->field_0034 = 0;
    self->field_0038 = 0;
    self->field_003c = 0;
    self->field_0040 = 0;
    self->field_0044 = 1065353216;
    self->field_0048 = 1065353216;
    self->field_004c = -971227136;
    self->field_007c = 0;
    self->field_0080 = 0;
    self->field_0084 = 0;
    self->field_0088 = 10;
    self->field_0074 = 0;
    self->field_0078 = 0;
    self->field_0058 = 0;
    self->field_0054 = 0;
    self->field_005c = 0;
    self->field_0064 = 0;
    self->field_0068 = 0;
    self->field_006c = 0;
    self->field_0060 = 0;
    self->field_008c = 0;

    uint32_t* screen = GetResourceScreen();
    self->field_0070 = reinterpret_cast<uint32_t>(screen);
    char success =
        reinterpret_cast<char(__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, screen, 0)[0x10 / 4])(
                screen, nullptr, screenArgument, configurationAddress);

    if (success)
    {
        success = InitializeResourceSubsystem();
        if (success)
        {
            auto* cameraStorage =
                static_cast<C_008655e8*>(Mem_Alloc(132));
            C_008655e8* camera = nullptr;
            if (cameraStorage)
            {
                auto* cameraScreen =
                    reinterpret_cast<CMenuScreen*>(self->field_0070);
                int* firstDimensions =
                    reinterpret_cast<int*(__fastcall*)(void*, void*)>(
                        cameraScreen->vtbl[0x18 / 4])(cameraScreen, nullptr);
                int* secondDimensions =
                    reinterpret_cast<int*(__fastcall*)(void*, void*)>(
                        cameraScreen->vtbl[0x18 / 4])(cameraScreen, nullptr);
                const int height = CSF_AT(int, secondDimensions, 0x4);
                const int width = CSF_AT(int, firstDimensions, 0);
                const int y = GetAccountSelectionIndex(
                    reinterpret_cast<void*>(self->field_0070), nullptr);
                const int x = AppMain_GetDisplayRaster(
                    reinterpret_cast<void*>(self->field_0070), nullptr);
                camera = C_008655e8_InitializeCameraResources(
                    cameraStorage, nullptr, x, y, width, height);
            }
            self->field_0074 = reinterpret_cast<uint32_t>(camera);

            const uint32_t capacity = self->field_000c;
            if (capacity < 0xC8)
            {
                // These two call addresses are also classified as vtables
                // in the supplied reference inventory.
                (void)CSF_VTBL(0x0041E5F0);
                S_0041e5f0_GrowArrayCapacity(
                    resourceArray, nullptr, 200 - capacity);
            }
            self->field_0014 = 100;
            Skin_GetSingleton();
            self->field_0078 = 0;

            void* flagStorage = Mem_Alloc(52);
            unsigned int* flagResource = flagStorage
                ? C_00860db0_Construct(flagStorage, nullptr) : nullptr;
            self->field_0060 = reinterpret_cast<uint32_t>(flagResource);
            GlobalFlag_IsSet(flagResource, nullptr);

            auto* collisionStorage =
                static_cast<C_00860cbc*>(Mem_Alloc(112));
            const uint32_t collisionResource = collisionStorage
                ? C_00860cbc_Construct(collisionStorage, nullptr) : 0;
            self->field_0058 = collisionResource;
            InitGfxCollisionShapeTables(
                reinterpret_cast<uint8_t*>(collisionResource), nullptr);

            void* resourceStorage = Mem_Alloc(100);
            const uint32_t resource = resourceStorage
                ? C_00861988_Construct(resourceStorage, nullptr) : 0;
            self->field_0054 = resource;
            C_00861988_Initialize(
                reinterpret_cast<void*>(resource), nullptr,
                reinterpret_cast<uint32_t*>(
                    static_cast<uint32_t>(configurationAddress) + 44));

            auto* streamingStorage =
                static_cast<C_00860bb8*>(Mem_Alloc(248));
            auto* streamingResource = streamingStorage
                ? reinterpret_cast<C_00860bb8*>(
                    C_00860bb8_Construct(streamingStorage, nullptr))
                : nullptr;
            self->field_005c =
                reinterpret_cast<uint32_t>(streamingResource);
            C_00860bb8_InitStreamingState(streamingResource, nullptr);

            self->field_0064 =
                reinterpret_cast<uint32_t>(GetSharedResource());

            void* configuredStorage = Mem_Alloc(264);
            const uint32_t configuredResource = configuredStorage
                ? C_00861120_Construct(configuredStorage, nullptr) : 0;
            self->field_0068 = configuredResource;
            C_00861120_Initialize(
                reinterpret_cast<void*>(configuredResource), nullptr,
                static_cast<int>(
                    static_cast<uint32_t>(configurationAddress) + 52));

            void* auxiliaryResource = Mem_Alloc(64);
            if (auxiliaryResource)
            {
                CSF_AT(void**, auxiliaryResource, 0) = CSF_VTBL(0x00860120);
                CSF_AT(uint8_t, auxiliaryResource, 0x4) = 0;
                CSF_AT(uint32_t, auxiliaryResource, 0x8) = 0;
                CSF_AT(uint32_t, auxiliaryResource, 0xC) = 2139095039;
                CSF_AT(uint32_t, auxiliaryResource, 0x10) = 0;
            }
            self->field_006c =
                reinterpret_cast<uint32_t>(auxiliaryResource);
            (void)CSF_VTBL(0x006E2D10);
            InitializeAuxiliaryResource(auxiliaryResource, nullptr);
            self->field_0004 = 0;
        }
    }

    if (successOut)
        *successOut = static_cast<uint8_t>(success);
    self->field_0050 = 1;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x006B1B80, C_00860130_Construct, "auto");
