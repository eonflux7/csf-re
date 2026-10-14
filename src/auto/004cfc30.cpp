#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int archiveAddress, S_00691860* string));
CSF_FUNC(int, __fastcall, SceneObject_ResetAttachedColorOverride, 0x004CF850, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiAutonomous_SerializeBaseState, 0x004D19C0, (void* self, void* edx, void* a2));

static int __fastcall S_0041f0a0_SerializeSceneObjectState(
    S_0041f0a0* self, void* /*edx*/, S_0041f0a0* archive)
{
    bool initialError = false;
    if (CSF_AT(uint8_t, archive, 0x00) == 0)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        initialError =
            !reinterpret_cast<bool(__fastcall*)(void*, void*, int, S_0041f0a0*)>(
                CSF_AT(void**, stream, 0x00)[0x80 / 4])(
                stream, nullptr, 0, archive);
    }

    const uint32_t savedContext = CSF_AT(uint32_t, archive, 0x1C);
    CSF_AT(uint8_t, archive, 0x00) = initialError;
    AiAutonomous_SerializeBaseState(self, nullptr, archive);

    bool canSerialize = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (canSerialize)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, S_0041f0a0*)>(
                CSF_AT(void**, stream, 0x00)[0x48 / 4])(
                stream, nullptr, &self->field_0040, archive) == -1;
    }

    canSerialize = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (canSerialize)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        Archive_SerializeString(
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x44),
            reinterpret_cast<S_00691860*>(archive));
    }

    canSerialize = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (canSerialize)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, S_0041f0a0*)>(
                CSF_AT(void**, stream, 0x00)[0x48 / 4])(
                stream, nullptr,
                reinterpret_cast<uint8_t*>(self) + 0x3C, archive) == -1;
    }

    if (CSF_AT(uint8_t, archive, 0x14) == 0 &&
        CSF_AT(uint8_t, archive, 0x00) == 0)
    {
        SceneObject_ResetAttachedColorOverride(
            reinterpret_cast<uint8_t*>(self) - 4, nullptr);
    }

    canSerialize = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = savedContext;
    if (canSerialize)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, S_0041f0a0*)>(
                CSF_AT(void**, stream, 0x00)[0x7C / 4])(
                stream, nullptr, archive);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, archive, 0x00) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, archive, 0x00) = 1;
    return 1;
}

HOOK(0x004CFC30, S_0041f0a0_SerializeSceneObjectState, "auto");
