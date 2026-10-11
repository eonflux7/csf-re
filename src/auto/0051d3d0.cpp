#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __cdecl, FxParticleManager_HasNamedEffect, 0x00437D60, (unsigned int* a1, char* a2));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(unsigned int, __fastcall, S_0051c1b0_GrowEntries, 0x0051C1B0, (void* self, void* edx, int newCapacity));
CSF_FUNC(uint32_t*, __fastcall, S_0051c1b0_AppendEntry, 0x0051CB50, (S_0051c1b0* self, void* , uint32_t* source, int requestedCapacity));
CSF_FUNC(bool, __stdcall, FxParticleManager_CanEmitNamedEffect, 0x0051CFE0, (void* a1));
CSF_FUNC(void*, __fastcall, FxParticleManager_InitializeParticleParameters, 0x0051D170, (void* self, void* , const std::uint32_t* firstPair, const std::uint32_t* secondPair, int parameter, const std::uint32_t* firstVector, const std::uint32_t* secondVector));
CSF_FUNC(void, __fastcall, FxParticleManager_EmitAttachmentEffect, 0x0051D210, (void* self, void* edx, int a3, int a4, void* a5));
CSF_FUNC(int, __fastcall, CMenuSaveGame_ResetStringStorage, 0x00544310, (void* self, void*));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (C_00833b88* self, void*));
CSF_FUNC(int, __fastcall, GfxMatrix_SetFourRowsAndUpdate, 0x006ADA10, (S_006ada10* self, void* , uint32_t* a2, uint32_t* a3, uint32_t* a4, uint32_t* a5));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, ObjectAttachment_HasSoundTrigger, 0x006B8140, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetInnerByteFlag, 0x006B81E0, (void* self, void*));
CSF_FUNC(int, __cdecl, SceneLoader_GetObjectAttachment, 0x006B87A0, (int a1));
CSF_FUNC(int, __fastcall, SceneLoader_GetObjectRegistry, 0x00726C40, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_SoundActivityThreshold, 0x0082E114);
CSF_GLOBAL(std::uint8_t, g_byte_82E65C, 0x0082E65C);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static void FxParticleManager_BuildEffectMatrix(
    void* request, S_006ada10* matrix, float* perpendicular,
    float* crossProduct, bool directionFirst)
{
    perpendicular[0] = 0.0f;
    perpendicular[1] = 0.0f;
    perpendicular[2] = 0.0f;

    matrix->field_0000 = reinterpret_cast<std::uint32_t>(RwMatrixCreate());
    CSF_AT(std::uint8_t, matrix, 0x4) = 1;

    if (CSF_AT(float, request, 0x40) == g_fZero &&
        CSF_AT(float, request, 0x44) == g_fZero)
    {
        perpendicular[0] = g_fZero;
        perpendicular[2] = CSF_AT(float, request, 0x44);
        perpendicular[1] = -CSF_AT(float, request, 0x48);
    }
    else
    {
        perpendicular[1] = CSF_AT(float, request, 0x40);
        perpendicular[0] = -CSF_AT(float, request, 0x44);
        perpendicular[2] = g_fZero;
    }

    NormalizeVector3(reinterpret_cast<C_00833b88*>(perpendicular), nullptr);

    crossProduct[0] =
        perpendicular[1] * CSF_AT(float, request, 0x48) -
        perpendicular[2] * CSF_AT(float, request, 0x44);
    crossProduct[1] =
        perpendicular[2] * CSF_AT(float, request, 0x40) -
        perpendicular[0] * CSF_AT(float, request, 0x48);
    crossProduct[2] =
        perpendicular[0] * CSF_AT(float, request, 0x44) -
        perpendicular[1] * CSF_AT(float, request, 0x40);

    auto* direction = reinterpret_cast<std::uint32_t*>(
        static_cast<std::uint8_t*>(request) + 0x40);
    auto* position = reinterpret_cast<std::uint32_t*>(
        static_cast<std::uint8_t*>(request) + 0x34);
    auto* normal = reinterpret_cast<std::uint32_t*>(perpendicular);
    auto* cross = reinterpret_cast<std::uint32_t*>(crossProduct);

    // These call addresses are also classified as vtable references in the packet.
    (void)CSF_VTBL(0x006ADA10);
    GfxMatrix_SetFourRowsAndUpdate(
        matrix, nullptr, directionFirst ? direction : normal,
        directionFirst ? normal : direction, cross, position);
}

static void __fastcall C_00840ee0_CreateImpactEffects(
    C_00840ee0* self, void* /*edx*/, int requestAddress,
    S_0051c1b0* output)
{
    auto* request = reinterpret_cast<std::uint8_t*>(requestAddress);

    void* target = nullptr;
    if (CSF_AT(int, request, 0x1C) >= 0)
    {
        target = reinterpret_cast<void*>(CHandleTable_Resolve(
            g_pCurrentScene, nullptr,
            reinterpret_cast<int>(request + 0x14)));
    }

    C_008342e8* source = nullptr;
    if (CSF_AT(int, request, 0x10) >= 0)
    {
        source = reinterpret_cast<C_008342e8*>(CHandleTable_Resolve(
            g_pCurrentScene, nullptr,
            reinterpret_cast<int>(request + 0x8)));
    }

    int priority = 10;
    if (source && !source->field_0050)
        priority = 20;

    output->field_0008 = 0;

    int objectId = 0;
    bool emitSurfaceEffects;
    bool emitNamedEffect;
    if (CSF_AT(int, request, 0x18) == -1)
    {
        emitSurfaceEffects = true;
        emitNamedEffect = FxParticleManager_CanEmitNamedEffect(source);
        objectId = CSF_AT(int, request, 0x58);
    }
    else
    {
        emitSurfaceEffects =
            (CSF_AT(std::uint32_t, target, 0x170) & 0x2000) != 0x2000;
        emitNamedEffect = false;

        void* owner =
            ((void* (__fastcall*)(void*, void*))
                CSF_AT(void**, target, 0)[0x28 / 4])(target, nullptr);
        if (owner && CSF_AT(int, request, 0x68) != -1)
        {
            void* child =
                ((void* (__fastcall*)(void*, void*, int))
                    CSF_AT(void**, owner, 0)[0x80 / 4])(
                    owner, nullptr, CSF_AT(int, request, 0x68));
            objectId = CSF_AT(int, child, 0xC);
        }
    }

    void* material = nullptr;
    if (CSF_AT(int, request, 0x5C) != -1)
    {
        void* globals = reinterpret_cast<void*>(GetValue8B0554());
        void* materialTable = CSF_AT(void*, globals, 0x1FD4);
        material =
            ((void* (__fastcall*)(void*, void*, int))
                CSF_AT(void**, materialTable, 0)[0x8 / 4])(
                materialTable, nullptr, CSF_AT(int, request, 0x5C));
    }

    void* attachment = nullptr;
    if (objectId)
    {
        attachment = reinterpret_cast<void*>(
            SceneLoader_GetObjectAttachment(objectId));
        if (attachment &&
            static_cast<std::uint8_t>(
                ObjectAttachment_HasSoundTrigger(attachment, nullptr)) &&
            !CSF_AT(std::uint8_t,
                reinterpret_cast<void*>(GetGlobalState_50D190()), 0xD0))
        {
            FxParticleManager_EmitAttachmentEffect(
                self, nullptr, requestAddress, objectId, output);
            emitNamedEffect = false;
        }
    }

    alignas(8) std::uint8_t matrixBytes[0xC];
    auto& matrix = *reinterpret_cast<S_006ada10*>(matrixBytes);
    alignas(8) std::uint8_t perpendicularBytes[0x10];
    auto* perpendicular = reinterpret_cast<float*>(perpendicularBytes);
    alignas(8) std::uint8_t crossProductBytes[0x14];
    auto* crossProduct = reinterpret_cast<float*>(crossProductBytes);

    // The descriptor and its output handle occupy adjacent stack regions.
    alignas(8) std::uint8_t entryBytes[0x28];
    auto* entry = reinterpret_cast<std::uint32_t*>(entryBytes);

    if (emitSurfaceEffects)
    {
        FxParticleManager_BuildEffectMatrix(
            request, &matrix, perpendicular, crossProduct, false);

        void* definitions = CSF_AT(
            void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FCC);

        entry[1] = reinterpret_cast<std::uint32_t>(CSF_VTBL(0x0082D448));
        entry[2] = -1;
        entry[3] = -1;
        entry[0] = 0;
        entry[4] = -1;
        entry[5] = -1;

        bool skipMaterialEffect = false;
        if (self->field_0008 != static_cast<std::uint32_t>(-1))
        {
            if (!material)
            {
                skipMaterialEffect = true;
            }
            else
            {
                void* globals =
                    reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
                if (CSF_AT(std::uint32_t, material, 0) !=
                    CSF_AT(std::uint32_t, globals, 0x904))
                {
                    void* definition =
                        ((void* (__fastcall*)(void*, void*, std::uint32_t))
                            CSF_AT(void**, definitions, 0)[0x8 / 4])(
                            definitions, nullptr, self->field_0008);

                    void* factory = reinterpret_cast<void*>(self->field_0004);
                    void* effect =
                        ((void* (__fastcall*)(void*, void*, void*, int,
                                             std::uint32_t*))
                            CSF_AT(void**, factory, 0)[0x8 / 4])(
                            factory, nullptr,
                            static_cast<std::uint8_t*>(definition) + 0x4,
                            priority, entry + 4);
                    if (effect)
                    {
                        ((void (__fastcall*)(void*, void*, S_006ada10*))
                            CSF_AT(void**, effect, 0)[0x40 / 4])(
                            effect, nullptr, &matrix);
                        ((void (__fastcall*)(void*, void*, int))
                            CSF_AT(void**, effect, 0)[0x28 / 4])(
                            effect, nullptr, 0);
                        entry[5] = self->field_0008;
                        entry[0] = reinterpret_cast<std::uint32_t>(effect);
                        S_0051c1b0_AppendEntry(output, nullptr, entry, 0);
                    }
                }
            }
        }

        if (!skipMaterialEffect && material)
        {
            int materialEffect = CSF_AT(int, material, 0x28);
            if (materialEffect != -1)
            {
                int effectId =
                    ((int (__fastcall*)(void*, void*, int))
                        CSF_AT(void**, definitions, 0)[0x10 / 4])(
                        definitions, nullptr, materialEffect);
                if (effectId != -1)
                {
                    void* definition =
                        ((void* (__fastcall*)(void*, void*, int))
                            CSF_AT(void**, definitions, 0)[0x8 / 4])(
                            definitions, nullptr, effectId);
                    void* factory = reinterpret_cast<void*>(self->field_0004);
                    void* effect =
                        ((void* (__fastcall*)(void*, void*, void*, int,
                                             std::uint32_t*))
                            CSF_AT(void**, factory, 0)[0x8 / 4])(
                            factory, nullptr,
                            static_cast<std::uint8_t*>(definition) + 0x4,
                            priority, entry + 4);
                    if (effect)
                    {
                        ((void (__fastcall*)(void*, void*, S_006ada10*))
                            CSF_AT(void**, effect, 0)[0x40 / 4])(
                            effect, nullptr, &matrix);
                        ((void (__fastcall*)(void*, void*, int))
                            CSF_AT(void**, effect, 0)[0x28 / 4])(
                            effect, nullptr, 0);
                        entry[0] = reinterpret_cast<std::uint32_t>(effect);
                        entry[5] = effectId;
                        S_0051c1b0_AppendEntry(output, nullptr, entry, 0);
                    }
                }
            }
        }

        entry[1] = reinterpret_cast<std::uint32_t>(CSF_VTBL(0x0082D410));
        if (CSF_AT(std::uint8_t, &matrix, 0x4))
            RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(matrix.field_0000));
    }

    if (material && emitNamedEffect &&
        !CSF_AT(std::uint8_t, request, 0x70) &&
        CSF_AT(int, request, 0x5C) != -1)
    {
        auto* effectName = reinterpret_cast<unsigned int*>(
            static_cast<std::uint8_t*>(material) + 0x30);
        if (static_cast<std::uint8_t>(FxParticleManager_HasNamedEffect(
                effectName, reinterpret_cast<char*>(&g_byte_82E65C))))
        {
            void* globals = reinterpret_cast<void*>(GetValue8B0554());
            void* parameterTable = CSF_AT(void*, globals, 0x1FC0);
            void* parameters =
                ((void* (__fastcall*)(void*, void*, int))
                    CSF_AT(void**, parameterTable, 0)[0x8 / 4])(
                    parameterTable, nullptr, CSF_AT(int, request, 0x24));

            FxParticleManager_InitializeParticleParameters(
                nullptr, nullptr,
                reinterpret_cast<const std::uint32_t*>(
                    static_cast<std::uint8_t*>(parameters) + 0x140),
                reinterpret_cast<const std::uint32_t*>(
                    static_cast<std::uint8_t*>(parameters) + 0x148),
                1092616192,
                reinterpret_cast<const std::uint32_t*>(request + 0x34),
                reinterpret_cast<const std::uint32_t*>(request + 0x40));

            alignas(8) std::uint8_t nameBytes[0x54];
            auto& name = *reinterpret_cast<C_0082d410*>(nameBytes);
            AssignStringSubstring(&name, nullptr, effectName, 0, -1);

            if (attachment && GetInnerByteFlag(attachment, nullptr))
            {
                void* global = reinterpret_cast<void*>(GetGlobalInt());
                void* registry = reinterpret_cast<void*>(
                    SceneLoader_GetObjectRegistry(global, nullptr));
                ((void (__fastcall*)(void*, void*, int, C_0082d410*))
                    CSF_AT(void**, registry, 0)[0x38 / 4])(
                    registry, nullptr, objectId, &name);
            }
            else
            {
                void* global = reinterpret_cast<void*>(GetGlobalInt());
                void* registry = reinterpret_cast<void*>(
                    SceneLoader_GetObjectRegistry(global, nullptr));
                ((void (__fastcall*)(void*, void*, C_0082d410*))
                    CSF_AT(void**, registry, 0)[0x3C / 4])(
                    registry, nullptr, &name);
            }
            CMenuSaveGame_ResetStringStorage(&name, nullptr);
        }
    }

    if (target && (CSF_AT(std::uint32_t, target, 0x170) & 0x2000) != 0)
    {
        std::uint32_t effectCount = self->field_0014;
        bool emitObjectEffects = effectCount != 0;
        if (!CSF_AT(std::uint32_t, target, 0x50))
        {
            if (!effectCount || !CSF_AT(std::uint8_t, target, 0x228))
                emitObjectEffects = false;
        }

        void* soundObject = CSF_AT(void*, target, 0x38);
        double activity =
            ((double (__fastcall*)(void*, void*))
                CSF_AT(void**, soundObject, 0)[0xC4 / 4])(
                soundObject, nullptr);

        if (activity <= g_SoundActivityThreshold && emitObjectEffects)
        {
            void* definitions = CSF_AT(
                void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FCC);
            std::uint32_t count = self->field_0014;
            entry[2] = -1;
            entry[3] = -1;
            entry[4] = -1;
            entry[5] = -1;
            entry[1] = reinterpret_cast<std::uint32_t>(CSF_VTBL(0x0082D448));
            entry[0] = 0;

            if (count)
            {
                std::uint32_t index = 0;
                do
                {
                    entry[5] =
                        reinterpret_cast<std::uint32_t*>(
                            self->field_000c)[index];
                    void* definition =
                        ((void* (__fastcall*)(void*, void*, int))
                            CSF_AT(void**, definitions, 0)[0x8 / 4])(
                            definitions, nullptr, static_cast<int>(entry[5]));
                    void* factory = reinterpret_cast<void*>(self->field_0004);
                    void* effect =
                        ((void* (__fastcall*)(void*, void*, void*, int,
                                             std::uint32_t*))
                            CSF_AT(void**, factory, 0)[0x8 / 4])(
                            factory, nullptr,
                            static_cast<std::uint8_t*>(definition) + 0x4,
                            priority, entry + 4);
                    if (effect)
                    {
                        FxParticleManager_BuildEffectMatrix(
                            request, &matrix, perpendicular, crossProduct, true);
                        ((void (__fastcall*)(void*, void*, S_006ada10*))
                            CSF_AT(void**, effect, 0)[0x40 / 4])(
                            effect, nullptr, &matrix);
                        ((void (__fastcall*)(void*, void*, int))
                            CSF_AT(void**, effect, 0)[0x28 / 4])(
                            effect, nullptr, 0);

                        bool hasRoom = output->field_0008 < output->field_0004;
                        entry[0] = reinterpret_cast<std::uint32_t>(effect);
                        if (!hasRoom)
                        {
                            (void)CSF_VTBL(0x0051C1B0);
                            S_0051c1b0_GrowEntries(
                                output, nullptr, output->field_000c);
                        }

                        std::uint32_t outputIndex = output->field_0008;
                        auto* destination = reinterpret_cast<std::uint32_t*>(
                            output->field_0000 + 24 * outputIndex);
                        output->field_0008 = outputIndex + 1;
                        destination[0] = entry[0];
                        destination[2] = entry[2];
                        destination[3] = entry[3];
                        destination[4] = entry[4];
                        destination[5] = entry[5];

                        if (CSF_AT(std::uint8_t, &matrix, 0x4))
                        {
                            RwMatrixDestroy(
                                reinterpret_cast<std::uint32_t*>(
                                    matrix.field_0000));
                        }
                    }
                    ++index;
                }
                while (index < self->field_0014);
            }
        }
    }
}

HOOK(0x0051D3D0, C_00840ee0_CreateImpactEffects, "auto");
