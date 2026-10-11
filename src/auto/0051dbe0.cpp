#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GameConfig_IsResolvedEntryActive, 0x00755880, (int entry));

CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

static float* __stdcall FxParticleManager_GetAttachmentPosition(
    float* output, int entityAddress, int attachmentSelector)
{
    float z = g_ZeroVectorZ;
    float y = g_ZeroVectorY;
    float x = g_ZeroVectorX;

    if (!entityAddress)
    {
        output[0] = x;
        output[1] = y;
        output[2] = z;
        return output;
    }

    void* entity = reinterpret_cast<void*>(entityAddress);
    void* attachment = nullptr;
    bool hasAttachment = false;

    if ((CSF_AT(uint32_t, entity, 0x170) & 0x2000) != 0)
    {
        void* controller = CSF_AT(void*, entity, 0x44);
        void* handle =
            reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, controller, 0)[0xD8 / 4])(controller, nullptr);
        void* model =
            reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, handle, 0)[0xA8 / 4])(handle, nullptr);

        int index = -1;
        if (model)
        {
            index = reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, model, 0)[0x98 / 4])(model, nullptr, 100);
        }

        if (!model || index == -1)
        {
            Handle_GetValuePointer(handle, nullptr);
            output[0] = g_ZeroVectorX;
            output[1] = g_ZeroVectorY;
            output[2] = g_ZeroVectorZ;
            return output;
        }

        attachment =
            reinterpret_cast<void* (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, model, 0)[0x8C / 4])(model, nullptr, index);
        hasAttachment = true;
    }
    else if (static_cast<uint8_t>(
                 GameConfig_IsResolvedEntryActive(CSF_AT(int, entity, 0x50))) != 0)
    {
        void* controller = CSF_AT(void*, entity, 0x44);
        int kind = reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, controller, 0)[0xC0 / 4])(
                controller, nullptr, attachmentSelector);

        if (kind == 0 || kind == 1)
        {
            int attachmentId = kind == 0 ? 2100 : 2101;
            void* model =
                reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, entity, 0)[0x28 / 4])(entity, nullptr);
            int index =
                reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, model, 0)[0x98 / 4])(
                        model, nullptr, attachmentId);

            if (index != -1)
            {
                model = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, entity, 0)[0x28 / 4])(entity, nullptr);
                attachment =
                    reinterpret_cast<void* (__fastcall*)(void*, void*, int)>(
                        CSF_AT(void**, model, 0)[0x8C / 4])(model, nullptr, index);
                hasAttachment = true;
            }
        }
    }

    if (hasAttachment)
    {
        void* transform =
            reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, attachment, 0)[0x18 / 4])(attachment, nullptr);
        float* position =
            reinterpret_cast<float* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, transform, 0)[0x10 / 4])(transform, nullptr);
        y = position[1];
        x = position[0];
        z = position[2];
    }

    output[0] = x;
    output[1] = y;
    output[2] = z;
    return output;
}

HOOK(0x0051DBE0, FxParticleManager_GetAttachmentPosition, "auto");
