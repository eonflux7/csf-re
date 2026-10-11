#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(unsigned int, __fastcall, FxParticleManager_GrowSpawnResults, 0x0051C1B0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetObjectRegistry, 0x00726C40, (void* self, void* edx));
CSF_FUNC(int, __cdecl, IsResolvedEntryActive, 0x00755880, (int a1));

CSF_GLOBAL(float, g_fReturnedConstant_0082E030, 0x0082E030);

struct SpawnMetadata
{
    void** vtbl;
    int attachmentA;
    int attachmentB;
    int token;
    std::uint32_t effectKey;
};

static void FxParticleManager_AppendSpawnResult(
    S_0051c1b0* results, void* effect, const SpawnMetadata& metadata)
{
    if (results->field_0008 >= results->field_0004)
    {
        // The packet also identifies this callee address as a vtable reference.
        (void)CSF_VTBL(0x0051C1B0);
        FxParticleManager_GrowSpawnResults(
            results, nullptr, static_cast<int>(results->field_000c));
    }

    const std::uint32_t index = results->field_0008;
    auto* entry = reinterpret_cast<std::uint8_t*>(
        results->field_0000 + 24 * index);
    results->field_0008 = index + 1;
    CSF_AT(void*, entry, 0) = effect;
    CSF_AT(int, entry, 8) = metadata.attachmentA;
    CSF_AT(int, entry, 12) = metadata.attachmentB;
    CSF_AT(int, entry, 16) = metadata.token;
    CSF_AT(std::uint32_t, entry, 20) = metadata.effectKey;
}

static char __fastcall C_00840ec0_SpawnEventEffects(
    C_00840ec0* self, void* /*edx*/, int argument, S_0051c1b0* results)
{
    auto* event = reinterpret_cast<void*>(argument);
    results->field_0008 = 0;

    const std::uint32_t eventType = CSF_AT(std::uint32_t, event, 0x04);
    std::uint32_t* effectList = nullptr;
    void* attachmentOwner = nullptr;
    bool hasEffects = false;
    bool spawnGroundEffects = false;

    // The original reserves 0x1C bytes at the position passed to the callee.
    float position[7] = {};

    if (eventType == 14)
    {
        if (static_cast<std::uint8_t>(
                IsResolvedEntryActive(CSF_AT(int, event, 0x24))) != 0)
        {
            effectList = &self->field_0078;
            hasEffects = self->field_0080 != 0;
        }
        else if (CSF_AT(std::uint8_t, event, 0x5C) != 0)
        {
            effectList = &self->field_00a8;
            hasEffects = self->field_00b0 != 0;
            spawnGroundEffects = true;
        }
        else
        {
            switch (CSF_AT(std::uint32_t, event, 0x24))
            {
            case 8:
                effectList = &self->field_0008;
                hasEffects = self->field_0010 != 0;
                break;
            case 9:
                effectList = &self->field_0018;
                hasEffects = self->field_0020 != 0;
                break;
            case 0xA:
                effectList = &self->field_0028;
                hasEffects = self->field_0030 != 0;
                break;
            case 0xE:
                hasEffects = self->field_0040 != 0;
                attachmentOwner = CSF_AT(void*, event, 0x40);
                effectList = &self->field_0038;
                break;
            case 0xF:
                attachmentOwner = CSF_AT(void*, event, 0x40);
                hasEffects = self->field_0050 != 0;
                effectList = &self->field_0048;
                break;
            case 0x10:
                hasEffects = self->field_0060 != 0;
                attachmentOwner = CSF_AT(void*, event, 0x40);
                effectList = &self->field_0058;
                break;
            case 0x11:
                effectList = &self->field_0068;
                hasEffects = self->field_0070 != 0;
                break;
            default:
                break;
            }
        }

        position[0] = CSF_AT(float, event, 0x34);
        position[1] = CSF_AT(float, event, 0x38);
        position[2] = CSF_AT(float, event, 0x3C);
    }
    else if (eventType == 15)
    {
        hasEffects = self->field_0090 != 0;
        effectList = &self->field_0088;
        position[0] = CSF_AT(float, event, 0x34);
        position[1] = CSF_AT(float, event, 0x38);
        position[2] = CSF_AT(float, event, 0x3C);
    }
    else if (eventType == 4)
    {
        hasEffects = self->field_0030 != 0;
        effectList = &self->field_0028;
        position[0] = CSF_AT(float, event, 0x40)
                    * g_fReturnedConstant_0082E030
                    + CSF_AT(float, event, 0x34);
        position[1] = CSF_AT(float, event, 0x44)
                    * g_fReturnedConstant_0082E030
                    + CSF_AT(float, event, 0x38);
        position[2] = CSF_AT(float, event, 0x48)
                    * g_fReturnedConstant_0082E030
                    + CSF_AT(float, event, 0x3C);
    }
    else
    {
        return static_cast<char>(eventType);
    }

    if (hasEffects)
    {
        SpawnMetadata metadata{
            CSF_VTBL(0x0082D448), -1, -1, -1,
            static_cast<std::uint32_t>(-1)
        };

        void* definitions = CSF_AT(
            void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FCC);

        for (std::uint32_t index = 0; index < effectList[2]; ++index)
        {
            metadata.effectKey = CSF_AT(
                std::uint32_t, reinterpret_cast<void*>(effectList[0]), 4 * index);

            void* definition =
                ((void* (__fastcall*)(void*, void*, std::uint32_t))
                    CSF_AT(void**, definitions, 0)[8 / 4])(
                        definitions, nullptr, metadata.effectKey);
            void* owner = attachmentOwner;
            void* effect;

            if (owner)
            {
                void* registry = reinterpret_cast<void*>(
                    SceneLoader_GetObjectRegistry(
                        reinterpret_cast<void*>(GetGlobalInt()), nullptr));
                effect =
                    ((void* (__fastcall*)(void*, void*, void*))
                        CSF_AT(void**, registry, 0)[20 / 4])(
                            registry, nullptr,
                            static_cast<std::uint8_t*>(definition) + 4);
            }
            else
            {
                void* factory = reinterpret_cast<void*>(self->field_0004);
                effect =
                    ((void* (__fastcall*)(void*, void*, void*, int, int*))
                        CSF_AT(void**, factory, 0)[8 / 4])(
                            factory, nullptr,
                            static_cast<std::uint8_t*>(definition) + 4,
                            20, &metadata.token);
            }

            if (effect)
            {
                if (owner)
                {
                    void* attachment =
                        ((void* (__fastcall*)(void*, void*))
                            CSF_AT(void**, owner, 0)[40 / 4])(owner, nullptr);
                    void** effectVtable = CSF_AT(void**, effect, 0);
                    int attachmentHandle =
                        ((int (__fastcall*)(void*, void*))
                            CSF_AT(void**, attachment, 0)[24 / 4])(
                                attachment, nullptr);
                    ((void (__fastcall*)(void*, void*, int))
                        effectVtable[68 / 4])(
                            effect, nullptr, attachmentHandle);
                    metadata.attachmentA = CSF_AT(int, owner, 0x20);
                    metadata.attachmentB = CSF_AT(int, owner, 0x24);
                }
                else
                {
                    ((void (__fastcall*)(void*, void*, float*, void*))
                        CSF_AT(void**, effect, 0)[60 / 4])(
                            effect, nullptr, position,
                            reinterpret_cast<void*>(CSF_ADDR_DATA(0x008A7DEC)));
                }

                ((void (__fastcall*)(void*, void*, int))
                    CSF_AT(void**, effect, 0)[40 / 4])(effect, nullptr, 0);
                FxParticleManager_AppendSpawnResult(results, effect, metadata);
            }
        }
    }

    if (!spawnGroundEffects)
        return 0;

    SpawnMetadata metadata{
        CSF_VTBL(0x0082D448), -1, -1, -1,
        static_cast<std::uint32_t>(-1)
    };
    float groundPosition[3] = {position[0], position[1], position[2]};
    void* heightSource = CSF_AT(
        void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FDC);
    groundPosition[1] = CSF_AT(float, heightSource, 0x30);
    void* definitions = CSF_AT(
        void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FCC);

    std::uint32_t index = 0;
    for (; index < self->field_00a0; ++index)
    {
        metadata.effectKey = CSF_AT(
            std::uint32_t, reinterpret_cast<void*>(self->field_0098), 4 * index);

        void* definition =
            ((void* (__fastcall*)(void*, void*, std::uint32_t))
                CSF_AT(void**, definitions, 0)[8 / 4])(
                    definitions, nullptr, metadata.effectKey);
        void* factory = reinterpret_cast<void*>(self->field_0004);
        void* effect =
            ((void* (__fastcall*)(void*, void*, void*, int, int*))
                CSF_AT(void**, factory, 0)[8 / 4])(
                    factory, nullptr,
                    static_cast<std::uint8_t*>(definition) + 4,
                    20, &metadata.token);

        if (effect)
        {
            ((void (__fastcall*)(void*, void*, float*))
                CSF_AT(void**, effect, 0)[56 / 4])(
                    effect, nullptr, groundPosition);
            ((void (__fastcall*)(void*, void*, int))
                CSF_AT(void**, effect, 0)[40 / 4])(effect, nullptr, 0);
            FxParticleManager_AppendSpawnResult(results, effect, metadata);
        }
    }

    return static_cast<char>(index);
}

HOOK(0x0051CBB0, C_00840ec0_SpawnEventEffects, "auto");
