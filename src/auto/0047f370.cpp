#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);

CSF_FUNC(uint32_t*, __fastcall, AiAttackTasks_AppendTangent, 0x0044BA20,
         (void* self, void* edx, uint32_t* a3));

static char __fastcall C_00832f18_BuildPointTangents(
    C_00832f18* self, void* /*edx*/)
{
    char result = static_cast<char>(self->field_002c);
    if (result)
        return result;

    const uint32_t pointCount =
        ((uint32_t(__fastcall*)(void*, void*))self->vtbl[0x20 / 4])(
            self, nullptr);
    result = static_cast<char>(pointCount);

    alignas(8) uint8_t tangentBytes[0x4C] = {};

    for (uint32_t index = 0; index < pointCount; ++index)
    {
        float incomingX = 0.0f;
        float incomingY = 0.0f;
        float incomingZ = 0.0f;
        float outgoingX = 0.0f;
        float outgoingY = 0.0f;
        float outgoingZ = 0.0f;

        if (index)
        {
            float* previous =
                ((float*(__fastcall*)(void*, void*, uint32_t))
                     self->vtbl[0x24 / 4])(self, nullptr, index - 1);
            float* current =
                ((float*(__fastcall*)(void*, void*, uint32_t))
                     self->vtbl[0x24 / 4])(self, nullptr, index);

            incomingX = current[0] - previous[0];
            incomingY = current[1] - previous[1];
            incomingZ = current[2] - previous[2];
        }

        if (index != pointCount - 1)
        {
            float* current =
                ((float*(__fastcall*)(void*, void*, uint32_t))
                     self->vtbl[0x24 / 4])(self, nullptr, index);
            float* next =
                ((float*(__fastcall*)(void*, void*, uint32_t))
                     self->vtbl[0x24 / 4])(self, nullptr, index + 1);

            outgoingX = next[0] - current[0];
            outgoingY = next[1] - current[1];
            outgoingZ = next[2] - current[2];
        }

        CSF_AT(float, tangentBytes, 0x00) =
            incomingX * g_HalfDurationScale + outgoingX * g_HalfDurationScale;
        CSF_AT(float, tangentBytes, 0x04) =
            incomingY * g_HalfDurationScale + outgoingY * g_HalfDurationScale;
        CSF_AT(float, tangentBytes, 0x08) =
            incomingZ * g_HalfDurationScale + outgoingZ * g_HalfDurationScale;

        result = static_cast<char>(reinterpret_cast<uintptr_t>(
            AiAttackTasks_AppendTangent(
                reinterpret_cast<uint8_t*>(self) + 0x30,
                nullptr, reinterpret_cast<uint32_t*>(tangentBytes))));
    }

    self->field_002c = 1;
    return result;
}

HOOK(0x0047F370, C_00832f18_BuildPointTangents, "auto");
