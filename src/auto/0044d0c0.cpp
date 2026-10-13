#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, FindDestinationById, 0x0044BC20,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, FindDestinationByPosition, 0x0044BCF0,
         (void* self, void* edx, float* a2));
CSF_FUNC(int, __fastcall, InitializeDestinationPayload, 0x0044C3D0,
         (int a1, int a2));
CSF_FUNC(uint32_t*, __fastcall, AppendDestinationPayload, 0x0044CDB0,
         (void* self, void* edx, int a3));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

CSF_GLOBAL(int, g_CurrentSceneIndex, 0x0089972C);

static int __fastcall S_0044bc20_PostPayloadMessage(
    S_0044bc20* self, void* /*edx*/, int destinationId,
    int positionAddress, int* payloadValues)
{
    alignas(8) uint8_t payload[0x70];

    CSF_AT(void**, payload, 0x64) = CSF_VTBL(0x0082D448);
    CSF_AT(int, payload, 0x68) = -1;
    CSF_AT(int, payload, 0x6C) = -1;

    // The packet also identifies this callable address as a vtable reference.
    (void)CSF_VTBL(0x0044BC20);

    if (destinationId >= 0)
    {
        const int index = FindDestinationById(self, nullptr, destinationId);
        if (index != -1)
        {
            const uint32_t offset = 100 * static_cast<uint32_t>(index);
            CSF_AT(int, reinterpret_cast<uint8_t*>(self->field_0010) + offset,
                   0x38) = -1;
            CSF_AT(uint8_t, reinterpret_cast<uint8_t*>(self->field_0010) + offset,
                   0x28) = 0;
        }
    }

    auto* position = reinterpret_cast<float*>(positionAddress);
    const int index = FindDestinationByPosition(self, nullptr, position);
    if (index != -1)
    {
        const uint32_t offset = 100 * static_cast<uint32_t>(index);
        CSF_AT(int, reinterpret_cast<uint8_t*>(self->field_0010) + offset,
               0x38) = destinationId;
        return static_cast<int>(offset);
    }

    InitializeDestinationPayload(
        static_cast<int>(reinterpret_cast<uintptr_t>(payload)), 0);

    const int firstValue = payloadValues[0];
    const int positionX = CSF_AT(int, position, 0x00);
    const int secondValue = payloadValues[1];
    CSF_AT(int, payload, 0x5C) = firstValue;
    const int positionY = CSF_AT(int, position, 0x04);
    CSF_AT(int, payload, 0x04) = positionX;
    const int sceneIndex = g_CurrentSceneIndex;
    CSF_AT(int, payload, 0x08) = positionY;
    CSF_AT(int, payload, 0x60) = secondValue;
    const int positionZ = CSF_AT(int, position, 0x08);
    CSF_AT(int, payload, 0x1C) = sceneIndex;
    CSF_AT(int, payload, 0x00) = 1;
    CSF_AT(int, payload, 0x38) = destinationId;
    CSF_AT(int, payload, 0x0C) = positionZ;

    int result = static_cast<int>(reinterpret_cast<uintptr_t>(
        AppendDestinationPayload(
            reinterpret_cast<uint8_t*>(self) + 0x0C, nullptr,
            static_cast<int>(reinterpret_cast<uintptr_t>(payload)))));

    if (CSF_AT(uint32_t, payload, 0x58) >= 0x10)
    {
        void* block = CSF_AT(void*, payload, 0x44);
        result = static_cast<int>(reinterpret_cast<uintptr_t>(block));
        if (block)
            return Mem_FreeDebug(block);
    }
    return result;
}

HOOK(0x0044D0C0, S_0044bc20_PostPayloadMessage, "auto");
