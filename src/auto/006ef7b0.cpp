#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, WeaponFirearms_InitializeSceneObjectOptions, 0x004777D0, (uint32_t* self, void*));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GfxObject_SetFlagBitOnAllAttachments, 0x006B2F50, (void* self, void* , char enable));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(int, __fastcall, GfxSkin_SetHandleSceneRegion, 0x006C5900, (C_0085e7b8** self, void* , C_00862490* a2));
CSF_FUNC(unsigned int, __fastcall, GfxScene_ResetStreamingObject, 0x006EF440, (void* self, void* edx));

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(float, g_PositiveHalfRotationDegrees, 0x00830D08);

static char __fastcall C_008621c8_CreateStreamingSceneObjects(
    C_008621c8* self, void* /*edx*/)
{
    if (self->field_003c)
        GfxScene_ResetStreamingObject(self, nullptr);

    const uint32_t objectCount = self->field_0020;
    char succeeded = 1;
    uint32_t objectIndex = 0;
    const float angleStep = g_PositiveHalfRotationDegrees / static_cast<float>(objectCount);

    alignas(8) uint8_t optionsBytes[0xBC];
    alignas(8) uint8_t requestBytes[0x24];

    if (objectCount)
    {
        uint32_t* objectSlot = &self->field_0024;
        do
        {
            uint32_t* name = self->field_001c < 0x10
                ? &self->field_0008
                : reinterpret_cast<uint32_t*>(self->field_0008);

            CSF_AT(uint32_t*, requestBytes, 0x00) = name;
            CSF_AT(uint8_t, requestBytes, 0x04) = 0;
            CSF_AT(int, requestBytes, 0x08) = 0;
            CSF_AT(int, requestBytes, 0x0C) = 0;
            CSF_AT(int, requestBytes, 0x10) = 0;
            CSF_AT(int, requestBytes, 0x14) = 0;

            void* globalState = reinterpret_cast<void*>(GetGlobalInt());
            void* controller = reinterpret_cast<void*>(
                SceneLoader_GetSceneController(globalState, nullptr));

            void* object = ((void* (__fastcall*)(void*, void*, uint32_t**))
                CSF_AT(void**, controller, 0)[0x14 / 4])(
                    controller, nullptr,
                    reinterpret_cast<uint32_t**>(requestBytes));
            *objectSlot = reinterpret_cast<uint32_t>(object);

            if (object)
            {
                WeaponFirearms_InitializeSceneObjectOptions(
                    reinterpret_cast<uint32_t*>(optionsBytes), nullptr);

                uint32_t flags = CSF_AT(uint32_t, optionsBytes, 0);
                if (self->field_003d)
                    flags = (flags & 0xFFFFDE6F) | 0x110;
                else
                    flags = (flags & 0xFFFFDE6F) | 0x2110;
                CSF_AT(uint32_t, optionsBytes, 0) = flags & 0xFFFFFFBF;

                object = reinterpret_cast<void*>(*objectSlot);
                ((void (__fastcall*)(void*, void*, uint32_t*))
                    CSF_AT(void**, object, 0)[0x10 / 4])(
                        object, nullptr, reinterpret_cast<uint32_t*>(optionsBytes));

                GfxObject_SetFlagBitOnAllAttachments(
                    reinterpret_cast<void*>(*objectSlot), nullptr, 1);

                object = reinterpret_cast<void*>(*objectSlot);
                void* transform = ((void* (__fastcall*)(void*, void*))
                    CSF_AT(void**, object, 0)[0x18 / 4])(object, nullptr);

                const float angle = static_cast<float>(objectIndex) * angleStep;
                ((void (__fastcall*)(void*, void*, float))
                    CSF_AT(void**, transform, 0)[0x44 / 4])(
                        transform, nullptr, angle);

                if (CSF_AT(uint32_t, optionsBytes, 0x90) >= 0x10 &&
                    CSF_AT(void*, optionsBytes, 0x7C))
                    Mem_FreeDebug(CSF_AT(void*, optionsBytes, 0x7C));
                CSF_AT(uint32_t, optionsBytes, 0x90) = 15;
                CSF_AT(int, optionsBytes, 0x8C) = 0;
                CSF_AT(uint8_t, optionsBytes, 0x7C) = 0;

                if (CSF_AT(uint32_t, optionsBytes, 0x3C) >= 0x10 &&
                    CSF_AT(void*, optionsBytes, 0x28))
                    Mem_FreeDebug(CSF_AT(void*, optionsBytes, 0x28));
                CSF_AT(uint32_t, optionsBytes, 0x3C) = 15;
                CSF_AT(int, optionsBytes, 0x38) = 0;
                CSF_AT(uint8_t, optionsBytes, 0x28) = 0;

                if (CSF_AT(uint32_t, optionsBytes, 0x20) >= 0x10 &&
                    CSF_AT(void*, optionsBytes, 0x0C))
                    Mem_FreeDebug(CSF_AT(void*, optionsBytes, 0x0C));
                CSF_AT(uint32_t, optionsBytes, 0x20) = 15;
                CSF_AT(int, optionsBytes, 0x1C) = 0;
                CSF_AT(uint8_t, optionsBytes, 0x0C) = 0;
            }
            else
            {
                succeeded = 0;
            }

            ++objectSlot;
            ++objectIndex;
        } while (objectIndex < self->field_0020);
    }

    C_0085e7b8** sceneRegion = reinterpret_cast<C_0085e7b8**>(
        GetGlobalValue_6C5760());
    GfxSkin_SetHandleSceneRegion(
        sceneRegion, nullptr, reinterpret_cast<C_00862490*>(self));

    self->field_003c = 1;
    return succeeded;
}

HOOK(0x006EF7B0, C_008621c8_CreateStreamingSceneObjects, "auto");
