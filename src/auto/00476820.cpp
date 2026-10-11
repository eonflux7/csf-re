#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __cdecl, SelectMovementRateBound, 0x0040A640, (float* a1, float* a2));
CSF_FUNC(char, __fastcall, C_008316d8_GetAttachmentMatrix, 0x00475DD0, (void* self, void* edx, int a2, void** a3));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(char, __fastcall, FxParticle_SetMode, 0x006CA480, (void* self, void* edx, int a2, char a3));
CSF_FUNC(int, __cdecl, FxParticleManager_GetParticleController, 0x006CA810, (int a1));
CSF_FUNC(int, __fastcall, SceneLoader_GetObjectRegistry, 0x00726C40, (void* self, void* edx));

CSF_GLOBAL(float, g_WeaponEffectRestartThreshold, 0x008316B0);

static void __fastcall C_008316d8_UpdateShellCasingEffect(
    C_008316d8* self, void* /*edx*/, float elapsed)
{
    if (self->field_0038 == 0 && self->field_0034 != 0xFFFFFFFF)
    {
        void* owner = reinterpret_cast<void*>(self->field_0004);
        void* entries =
            ((void* (__fastcall*)(void*, void*))
                CSF_AT(void**, owner, 0)[0x28 / 4])(owner, nullptr);
        void* attachment = nullptr;

        if (entries)
        {
            int index =
                ((int (__fastcall*)(void*, void*, int))
                    CSF_AT(void**, entries, 0)[0x98 / 4])(entries, nullptr, 3100);
            if (index != -1)
            {
                attachment =
                    ((void* (__fastcall*)(void*, void*, int))
                        CSF_AT(void**, entries, 0)[0x8C / 4])(entries, nullptr, index);
            }
        }

        if (!attachment)
        {
            entries = reinterpret_cast<void*>(self->vResolveEntryLookupValue());
            if (entries)
            {
                int index =
                    ((int (__fastcall*)(void*, void*, int))
                        CSF_AT(void**, entries, 0)[0x98 / 4])(entries, nullptr, 3100);
                if (index != -1)
                {
                    attachment =
                        ((void* (__fastcall*)(void*, void*, int))
                            CSF_AT(void**, entries, 0)[0x8C / 4])(entries, nullptr, index);
                }
            }
        }

        if (attachment)
        {
            owner = reinterpret_cast<void*>(self->field_0004);
            bool useDetachedMatrix =
                CSF_AT(std::uint32_t, owner, 0x50) == 0 &&
                CSF_AT(std::uint8_t, owner, 0x228) == 0;

            GetValue8B0554();
            void* manager = reinterpret_cast<void*>(GetValue8B0554());
            void* effectEntries = CSF_AT(void*, manager, 0x1FCC);
            void* effectEntry =
                ((void* (__fastcall*)(void*, void*, std::uint32_t))
                    CSF_AT(void**, effectEntries, 0)[0x8 / 4])(
                        effectEntries, nullptr, self->field_0034);

            void* scene = reinterpret_cast<void*>(GetGlobalInt());
            void* registry =
                reinterpret_cast<void*>(SceneLoader_GetObjectRegistry(scene, nullptr));
            void* effect =
                ((void* (__fastcall*)(void*, void*, void*))
                    CSF_AT(void**, registry, 0)[0x14 / 4])(
                        registry, nullptr,
                        reinterpret_cast<std::uint8_t*>(effectEntry) + 4);

            self->field_0038 = reinterpret_cast<std::uint32_t>(effect);
            self->field_003c = 0.0f;

            if (effect)
            {
                if (useDetachedMatrix)
                    FxParticle_SetMode(effect, nullptr, 4, 1);

                void* controller = reinterpret_cast<void*>(
                    FxParticleManager_GetParticleController(
                        static_cast<int>(self->field_0038)));
                if (controller)
                {
                    void* particle =
                        ((void* (__fastcall*)(void*, void*))
                            CSF_AT(void**, controller, 0)[0x2C / 4])(
                                controller, nullptr);
                    if (particle)
                    {
                        float lowerBound = 0.0f;
                        float upperBound[2] = {elapsed + elapsed, 0.0f};
                        CSF_AT(float, particle, 0x10) =
                            *SelectMovementRateBound(&lowerBound, upperBound);
                    }
                }

                if (useDetachedMatrix)
                {
                    void* matrix = RwMatrixCreate();
                    std::uint32_t ownsMatrix = 1;
                    C_008316d8_GetAttachmentMatrix(self, nullptr, 3100, &matrix);

                    effect = reinterpret_cast<void*>(self->field_0038);
                    ((void (__fastcall*)(void*, void*, void**))
                        CSF_AT(void**, effect, 0)[0x40 / 4])(
                            effect, nullptr, &matrix);

                    if (static_cast<std::uint8_t>(ownsMatrix))
                        RwMatrixDestroy(static_cast<std::uint32_t*>(matrix));
                }
                else
                {
                    effect = reinterpret_cast<void*>(self->field_0038);
                    void** effectVtable = CSF_AT(void**, effect, 0);
                    void* attachmentTransform =
                        ((void* (__fastcall*)(void*, void*))
                            CSF_AT(void**, attachment, 0)[0x18 / 4])(
                                attachment, nullptr);

                    ((void (__fastcall*)(void*, void*, void*))
                        effectVtable[0x44 / 4])(
                            reinterpret_cast<void*>(self->field_0038),
                            nullptr, attachmentTransform);
                }
            }
        }
    }

    void* effect = reinterpret_cast<void*>(self->field_0038);
    if (effect)
    {
        if (self->field_003c < g_WeaponEffectRestartThreshold)
        {
            ((void (__fastcall*)(void*, void*, int))
                CSF_AT(void**, effect, 0)[0x28 / 4])(effect, nullptr, 0);
        }
        self->field_003c = elapsed + elapsed;
    }
}

HOOK(0x00476820, C_008316d8_UpdateShellCasingEffect, "auto");
