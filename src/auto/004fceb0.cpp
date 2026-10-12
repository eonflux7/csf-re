#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CommandoAnims_UpdateAuxiliaryAnimation, 0x00443350, (void* self, void* edx, float a2));
CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(void, __fastcall, CharacterModel_Update, 0x004F84C0, (void* self, void* , float deltaTime));
CSF_FUNC(void, __fastcall, DevConsole_UpdateProximityIntensity, 0x004F92A0, (void* self, void* , float deltaTime));
CSF_FUNC(void, __fastcall, C_0083f448_UpdateAnimationState, 0x004FC6C0, (void* self, void* edx, float a3));
CSF_FUNC(uint32_t, __fastcall, C_0083f448_UpdateAnimationEntries, 0x004FC740, (C_0083f448* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static char __fastcall C_0083f448_UpdateAnimations(
    C_0083f448* self, void* /*edx*/, int timeBits)
{
    float deltaTime;
    std::memcpy(&deltaTime, &timeBits, sizeof(deltaTime));

    C_0083f448_UpdateAnimationState(self, nullptr, deltaTime);
    C_0083f448_UpdateAnimationEntries(self, nullptr, timeBits);

    if (self->field_0074)
    {
        DevConsole_UpdateProximityIntensity(
            reinterpret_cast<void*>(self->field_0074), nullptr, deltaTime);
    }

    for (uint32_t index = 0; index < self->field_000c; ++index)
    {
        CharacterModel_Update(
            reinterpret_cast<void**>(self->field_0004)[index],
            nullptr, deltaTime);
    }

    for (uint32_t index = 0; index < self->field_001c; ++index)
    {
        void* object = reinterpret_cast<void*>(ResolveEntryLookupValue(
            reinterpret_cast<void**>(self->field_0014)[index], nullptr));

        ((void (__fastcall*)(void*, void*, int))
            CSF_AT(void**, object, 0)[0x5C / 4])(
                object, nullptr, timeBits);
    }

    if (self->field_0034)
    {
        void* object = reinterpret_cast<void*>(ResolveEntryLookupValue(
            reinterpret_cast<void*>(self->field_0034), nullptr));

        ((void (__fastcall*)(void*, void*, int))
            CSF_AT(void**, object, 0)[0x5C / 4])(
                object, nullptr, timeBits);
    }

    if (self->field_0078)
    {
        CommandoAnims_UpdateAuxiliaryAnimation(
            reinterpret_cast<void*>(self->field_0078), nullptr, deltaTime);
    }

    void* sceneState = CSF_AT(void*, g_pCurrentScene, 0xC0);
    if (!CSF_AT(uint8_t, sceneState, 9))
        return 0;

    uint32_t count = self->field_001c;
    for (uint32_t index = 0; index < count; ++index)
    {
        if (GetReferencedObject(
                reinterpret_cast<void**>(self->field_0014)[index],
                nullptr) == reinterpret_cast<void*>(1))
        {
            void* object = reinterpret_cast<void*>(ResolveEntryLookupValue(
                reinterpret_cast<void**>(self->field_0014)[index], nullptr));

            ((void (__fastcall*)(void*, void*, int))
                CSF_AT(void**, object, 0)[0x44 / 4])(
                    object, nullptr, 0);
        }

        count = self->field_001c;
    }

    return static_cast<char>(count);
}

HOOK(0x004FCEB0, C_0083f448_UpdateAnimations, "auto");
