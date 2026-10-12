#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static float* __fastcall CharacterModel_GetPosition(
    void* self, void* /*edx*/, float* output)
{
    alignas(8) uint8_t temporaryBytes[0x18];
    float positionX = 0.0f;
    float positionY = 0.0f;
    float positionZ = 0.0f;

    if ((CSF_AT(uint32_t, self, 0x2E0) & 0x2000) != 0)
    {
        void* attachedObject = reinterpret_cast<void*>(CHandleTable_Resolve(
            g_pCurrentScene, nullptr,
            static_cast<int>(reinterpret_cast<uintptr_t>(
                static_cast<uint8_t*>(self) + 0x1B4))));

        void* model =
            ((void*(__fastcall*)(void*, void*))
                CSF_AT(void**, attachedObject, 0)[0x28 / 4])(
                attachedObject, nullptr);

        int attachmentIndex =
            ((int(__fastcall*)(void*, void*, int))
                CSF_AT(void**, model, 0)[0x98 / 4])(
                model, nullptr, 3000);

        if (attachmentIndex != -1)
        {
            model =
                ((void*(__fastcall*)(void*, void*))
                    CSF_AT(void**, attachedObject, 0)[0x28 / 4])(
                    attachedObject, nullptr);

            void* attachment =
                ((void*(__fastcall*)(void*, void*, int))
                    CSF_AT(void**, model, 0)[0x8C / 4])(
                    model, nullptr, attachmentIndex);

            void* transform =
                ((void*(__fastcall*)(void*, void*))
                    CSF_AT(void**, attachment, 0)[0x18 / 4])(
                    attachment, nullptr);

            float* position =
                ((float*(__fastcall*)(void*, void*))
                    CSF_AT(void**, transform, 0)[0x10 / 4])(
                    transform, nullptr);

            positionX = position[0];
            positionY = position[1];
            positionZ = position[2];
        }
    }
    else
    {
        float* position =
            ((float*(__fastcall*)(void*, void*, void*))
                CSF_AT(void**, self, 0)[0x34 / 4])(
                self, nullptr, temporaryBytes);

        positionX = position[0];
        positionY = position[1];
        positionZ = position[2];

        void* poseProvider = CSF_AT(void*, self, 0x16C);
        uint8_t* pose =
            ((uint8_t*(__fastcall*)(void*, void*))
                CSF_AT(void**, poseProvider, 0)[0x70 / 4])(
                poseProvider, nullptr);

        void* model =
            ((void*(__fastcall*)(void*, void*))
                CSF_AT(void**, self, 0)[0x28 / 4])(
                self, nullptr);

        void* transform =
            ((void*(__fastcall*)(void*, void*))
                CSF_AT(void**, model, 0)[0x18 / 4])(
                model, nullptr);

        float* offset =
            ((float*(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, transform, 0)[0xA4 / 4])(
                transform, nullptr, temporaryBytes, pose + 24);

        positionX = positionX + offset[0];
        positionY = positionY + offset[1];
        positionZ = positionZ + offset[2];
    }

    output[0] = positionX;
    output[1] = positionY;
    output[2] = positionZ;
    return output;
}

HOOK(0x00494C30, CharacterModel_GetPosition, "auto");
