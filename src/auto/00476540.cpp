#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, C_008316d8_CopyEntryMatrix, 0x00475DD0, (C_008316d8* self, void* , int entryId, void** output));
CSF_FUNC(void, __stdcall, C_0085ec18_NullSub, 0x004B2A20, (int));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, WeaponFirearms_IsContextActiveImpl, 0x006C8620, (float* self, void*));
CSF_FUNC(int, __fastcall, WeaponFirearms_UpdateContextFromObject, 0x006C8740, (void* self, void* , void* object, float duration));
CSF_FUNC(void, __fastcall, S_006c8800_UpdateWeaponContext, 0x006C8840, (S_006c8800* self, void* , float elapsedTime));
CSF_FUNC(char, __fastcall, FxParticle_SetMode, 0x006CA480, (C_0086754c* self, void* , int requestedMode, char enableFlag));
CSF_FUNC(int, __fastcall, SceneLoader_GetObjectRegistry, 0x00726C40, (void* self, void* edx));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static int __fastcall C_008316d8_UpdateWeaponEffects(
    C_008316d8* self, void* /*edx*/, float elapsedTime)
{
    if (self->field_0030)
    {
        int elapsedBits;
        std::memcpy(&elapsedBits, &elapsedTime, sizeof(elapsedBits));
        C_0085ec18_NullSub(elapsedBits);
    }

    int result = static_cast<int>(self->vGetPageCountForContexts());
    if (result)
    {
        auto* context =
            reinterpret_cast<float*>(self->vGetPageCountForContexts());
        if (static_cast<std::uint8_t>(
                WeaponFirearms_IsContextActiveImpl(context, nullptr)) &&
            self->vSlotD8())
        {
            void* source = reinterpret_cast<void*>(self->vSlotD8());
            void* object =
                ((void*(__fastcall*)(void*, void*))
                    CSF_AT(void**, source, 0)[0x18 / 4])(source, nullptr);

            void* currentContext =
                reinterpret_cast<void*>(self->vGetPageCountForContexts());
            WeaponFirearms_UpdateContextFromObject(
                currentContext, nullptr, object, 0.0f);
        }

        auto* currentContext =
            reinterpret_cast<S_006c8800*>(self->vGetPageCountForContexts());
        S_006c8800_UpdateWeaponContext(currentContext, nullptr, elapsedTime);
    }

    void* effect = reinterpret_cast<void*>(self->field_0038);
    if (effect)
    {
        if (self->field_003c > g_fZero)
        {
            float remainingTime = self->field_003c - elapsedTime;
            self->field_003c = remainingTime;
            if (remainingTime <= g_fZero)
            {
                ((void(__fastcall*)(void*, void*, int))
                    CSF_AT(void**, effect, 0)[0x2C / 4])(
                        effect, nullptr, 0);
            }
        }
        else
        {
            if (((bool(__fastcall*)(void*, void*))
                    CSF_AT(void**, effect, 0)[0x34 / 4])(effect, nullptr))
            {
                void* scene = reinterpret_cast<void*>(GetGlobalInt());
                void* registry = reinterpret_cast<void*>(
                    SceneLoader_GetObjectRegistry(scene, nullptr));
                ((void(__fastcall*)(void*, void*, void*))
                    CSF_AT(void**, registry, 0)[0x18 / 4])(
                        registry, nullptr,
                        reinterpret_cast<void*>(self->field_0038));

                self->field_0038 = 0;
                self->field_003c = 0.0f;
            }
        }

        result = static_cast<int>(self->field_0038);
        if (result)
        {
            void* owner = reinterpret_cast<void*>(self->field_0004);
            char enableFlag = 0;
            if (!CSF_AT(std::uint32_t, owner, 0x50) &&
                !CSF_AT(std::uint8_t, owner, 0x228))
            {
                enableFlag = 1;
                alignas(8) std::uint8_t matrixBytes[0x8];
                CSF_AT(void*, matrixBytes, 0) = RwMatrixCreate();
                CSF_AT(char, matrixBytes, 0x4) = 1;

                C_008316d8_CopyEntryMatrix(
                    self, nullptr, 3100,
                    reinterpret_cast<void**>(matrixBytes));

                void* currentEffect =
                    reinterpret_cast<void*>(self->field_0038);
                ((void(__fastcall*)(void*, void*, void**))
                    CSF_AT(void**, currentEffect, 0)[0x40 / 4])(
                        currentEffect, nullptr,
                        reinterpret_cast<void**>(matrixBytes));

                if (CSF_AT(char, matrixBytes, 0x4))
                {
                    RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(
                        CSF_AT(void*, matrixBytes, 0)));
                }
            }

            return FxParticle_SetMode(
                reinterpret_cast<C_0086754c*>(self->field_0038),
                nullptr, 4, enableFlag);
        }
    }
    return result;
}

HOOK(0x00476540, C_008316d8_UpdateWeaponEffects, "auto");
