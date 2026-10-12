#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0082ddf8_UpdateSceneEffect, 0x0040EC10, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, InitializeSceneEffectParameters, 0x0040ED30, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_0082ddf8_UpdateMatrixEffect, 0x0040EE80, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_0082ddf8_UpdateClumpEffect, 0x0040EED0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ResolveClumpManager, 0x00652020, (int managerId, void* edx));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(char, __fastcall, IsEffectSourceActive, 0x0070ECB0, (void* self, void* edx));

CSF_GLOBAL(float, g_ColorWhite, 0x008A7D90);

static char __fastcall C_0082ddf8_SynchronizeEffects(C_0082ddf8* self, void* /*edx*/)
{
    alignas(8) std::uint8_t parameters[0x28] = {};

    const std::uint8_t matrixActive =
        (self->field_0014 & 1) != 0 &&
        IsEffectSourceActive(reinterpret_cast<void*>(self->field_001c), nullptr);

    CSF_AT(std::uint8_t, parameters, 0x26) =
        (self->field_0014 & 2) != 0 &&
        IsEffectSourceActive(reinterpret_cast<void*>(self->field_001c), nullptr) &&
        self->field_0010 != 0;

    CSF_AT(std::uint8_t, parameters, 0x27) =
        (self->field_0014 & 4) != 0 &&
        IsEffectSourceActive(reinterpret_cast<void*>(self->field_001c), nullptr);

    if (matrixActive != self->field_0018)
    {
        self->field_0018 = matrixActive;
        if (matrixActive)
        {
            void* matrixEffect = Mem_Alloc(16);
            if (matrixEffect)
            {
                CSF_AT(std::uint32_t*, matrixEffect, 0x0) = RwMatrixCreate();
                CSF_AT(std::uint8_t, matrixEffect, 0x4) = 1;
                CSF_AT(float, matrixEffect, 0x8) = 100.0f;
                CSF_AT(float, matrixEffect, 0xC) = 10000.0f;
            }
            self->field_0020 = reinterpret_cast<std::uint32_t>(matrixEffect);
            C_0082ddf8_UpdateMatrixEffect(self, nullptr);
        }
        else
        {
            void* matrixEffect = reinterpret_cast<void*>(self->field_0020);
            if (matrixEffect)
            {
                if (CSF_AT(std::uint8_t, matrixEffect, 0x4))
                    RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixEffect, 0x0));
                Mem_Free(matrixEffect);
                self->field_0020 = 0;
            }
        }
    }

    const std::uint8_t sceneActive = CSF_AT(std::uint8_t, parameters, 0x26);
    if (sceneActive != self->field_0019)
    {
        self->field_0019 = sceneActive;
        if (sceneActive)
        {
            void* sceneController = reinterpret_cast<void*>(
                SceneLoader_GetSceneController(reinterpret_cast<void*>(GetGlobalInt()), nullptr));
            void* effectManager =
                ((void*(__fastcall*)(void*, void*))CSF_AT(void**, sceneController, 0)[0x84 / 4])(
                    sceneController, nullptr);

            InitializeSceneEffectParameters(parameters, nullptr);
            CSF_AT(std::uint32_t, parameters, 0x0) = self->field_0010;
            CSF_AT(float, parameters, 0x4) = 100.0f;
            CSF_AT(float, parameters, 0x20) = 10000.0f;
            self->field_0024 =
                ((std::uint32_t(__fastcall*)(void*, void*, void*))
                    CSF_AT(void**, effectManager, 0)[0])(
                        effectManager, nullptr, parameters);
            C_0082ddf8_UpdateSceneEffect(self, nullptr);
        }
        else
        {
            void* sceneController = reinterpret_cast<void*>(
                SceneLoader_GetSceneController(reinterpret_cast<void*>(GetGlobalInt()), nullptr));
            void* effectManager =
                ((void*(__fastcall*)(void*, void*))CSF_AT(void**, sceneController, 0)[0x84 / 4])(
                    sceneController, nullptr);
            ((void(__fastcall*)(void*, void*, std::uint32_t*))
                CSF_AT(void**, effectManager, 0)[0x4 / 4])(
                    effectManager, nullptr, &self->field_0024);
        }
    }

    char result = static_cast<char>(CSF_AT(std::uint8_t, parameters, 0x27));
    if (CSF_AT(std::uint8_t, parameters, 0x27) != self->field_001a)
    {
        self->field_001a = CSF_AT(std::uint8_t, parameters, 0x27);
        void* clumpManager = reinterpret_cast<void*>(ResolveClumpManager(GetGlobalInt(), nullptr));
        result = static_cast<char>(CSF_AT(std::uint8_t, parameters, 0x27));
        if (CSF_AT(std::uint8_t, parameters, 0x27))
        {
            if (!self->field_0028)
            {
                self->field_0028 =
                    ((std::uint32_t(__fastcall*)(void*, void*, int, int))
                        CSF_AT(void**, clumpManager, 0)[0x28 / 4])(
                            clumpManager, nullptr, 2, 3);
            }

            void* clumpEffect = reinterpret_cast<void*>(self->field_0028);
            ((void(__fastcall*)(void*, void*, float*))
                CSF_AT(void**, clumpEffect, 0)[0x34 / 4])(
                    clumpEffect, nullptr, &g_ColorWhite);

            clumpEffect = reinterpret_cast<void*>(self->field_0028);
            ((void(__fastcall*)(void*, void*, float))
                CSF_AT(void**, clumpEffect, 0)[0x3C / 4])(
                    clumpEffect, nullptr, 1500.0f);
            return C_0082ddf8_UpdateClumpEffect(self, nullptr);
        }

        const std::uint32_t clumpEffect = self->field_0028;
        result = static_cast<char>(clumpEffect);
        if (clumpEffect)
        {
            result = static_cast<char>(
                ((int(__fastcall*)(void*, void*, std::uint32_t))
                    CSF_AT(void**, clumpManager, 0)[0x2C / 4])(
                        clumpManager, nullptr, clumpEffect));
            self->field_0028 = 0;
        }
    }
    return result;
}

HOOK(0x0040F150, C_0082ddf8_SynchronizeEffects, "auto");
