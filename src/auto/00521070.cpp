#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, FxParticleManager_GrowSimpleQueue, 0x0051FCD0,
         (void* self, void* edx, int a2));
CSF_FUNC(unsigned int, __fastcall, FxParticleManager_GrowDetailedQueue, 0x00520AE0,
         (void* self, void* edx, int a2));

static int __fastcall S_0051f950_CollectParticleRequests(
    S_0051f950* self, void* /*edx*/, int context)
{
    (void)CSF_VTBL(0x00520AE0);

    const uint32_t emitterCount = self->field_0014;
    for (uint32_t emitterIndex = 0; emitterIndex < emitterCount; ++emitterIndex)
    {
        void* emitters = reinterpret_cast<void*>(self->field_000c);
        self->field_008c = 1;
        self->field_0084 = 0;

        void* emitter = CSF_AT(void*, emitters, emitterIndex * 4);
        const bool producedRequests =
            reinterpret_cast<bool (__fastcall*)(void*, void*, int, uint32_t*)>(
                CSF_AT(void**, emitter, 0)[2])(
                    emitter, nullptr, context, &self->field_007c);

        if (producedRequests)
        {
            uint32_t remaining = self->field_0084;
            if (remaining != 0)
            {
                uint32_t requestOffset = 0;
                do
                {
                    auto* request =
                        reinterpret_cast<uint8_t*>(self->field_007c) + requestOffset;

                    if (CSF_AT(int, request, 0x0C) < 0)
                    {
                        const int particle = CSF_AT(int, request, 0x00);
                        const int parameter = CSF_AT(int, request, 0x10);
                        const int extra = CSF_AT(int, request, 0x14);
                        const int contextValue =
                            CSF_AT(int, reinterpret_cast<void*>(context), 0x28);

                        void* queue = reinterpret_cast<uint8_t*>(self) + 0x4C;
                        if (self->field_0054 >= CSF_AT(uint32_t, queue, 0x04))
                        {
                            FxParticleManager_GrowSimpleQueue(
                                queue, nullptr, CSF_AT(int, queue, 0x0C));
                        }

                        const uint32_t index = self->field_0054;
                        auto* destination =
                            reinterpret_cast<uint8_t*>(
                                CSF_AT(uint32_t, queue, 0x00)) + 16 * index;
                        self->field_0054 = index + 1;

                        CSF_AT(int, destination, 0x00) = particle;
                        CSF_AT(int, destination, 0x04) = contextValue;
                        CSF_AT(int, destination, 0x08) = extra;
                        CSF_AT(int, destination, 0x0C) = parameter;
                    }
                    else
                    {
                        const int particle = CSF_AT(int, request, 0x00);
                        const int detail = CSF_AT(int, request, 0x08);
                        const int kind = CSF_AT(int, request, 0x0C);
                        const int contextValue =
                            CSF_AT(int, reinterpret_cast<void*>(context), 0x28);
                        const int extra = CSF_AT(int, request, 0x14);
                        const int parameter = CSF_AT(int, request, 0x10);

                        auto* queue = reinterpret_cast<S_00520ae0*>(
                            reinterpret_cast<uint8_t*>(self) + 0x5C);
                        if (queue->field_0008 >= queue->field_0004)
                        {
                            FxParticleManager_GrowDetailedQueue(
                                queue, nullptr, static_cast<int>(queue->field_000c));
                        }

                        const uint32_t index = queue->field_0008;
                        auto* destination =
                            reinterpret_cast<uint8_t*>(queue->field_0000) + 36 * index;
                        queue->field_0008 = index + 1;

                        CSF_AT(int, destination, 0x00) = 0;
                        CSF_AT(int, destination, 0x08) = detail;
                        CSF_AT(int, destination, 0x0C) = kind;
                        CSF_AT(int, destination, 0x14) = particle;
                        CSF_AT(int, destination, 0x18) = contextValue;
                        CSF_AT(int, destination, 0x10) = -1;
                        CSF_AT(int, destination, 0x1C) = extra;
                        CSF_AT(int, destination, 0x20) = parameter;
                    }

                    requestOffset += 24;
                    --remaining;
                } while (remaining != 0);
            }
        }

        self->field_008c = 0;
    }

    return static_cast<int>(emitterCount);
}

HOOK(0x00521070, S_0051f950_CollectParticleRequests, "auto");
