#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, UpdateChild, 0x0040AFA0,
         (void* self, void* edx, float a2));

static uint32_t __fastcall C_0082d5e0_UpdateRelativePositions(
    C_0082d5e0* self, void* /*edx*/, float updateValue)
{
    if (self->field_0014 != 0)
    {
        uint32_t index = 0;
        uint32_t recordOffset = 0;
        do
        {
            auto* records = reinterpret_cast<uint8_t*>(self->field_000c);
            const int objectId = CSF_AT(int, records + recordOffset, 0x04);
            if (objectId != -1)
            {
                void* owner = reinterpret_cast<void*>(self->field_0008);
                void* context =
                    reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                        CSF_AT(void**, owner, 0)[0x28 / 4])(owner, nullptr);

                void* registry =
                    reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                        CSF_AT(void**, context, 0)[0x68 / 4])(context, nullptr);

                if (registry)
                {
                    void* object =
                        reinterpret_cast<void* (__fastcall*)(void*, void*, int)>(
                            CSF_AT(void**, registry, 0)[0x6C / 4])(
                                registry, nullptr, objectId);

                    if (object)
                    {
                        auto* record =
                            reinterpret_cast<uint8_t*>(self->field_000c) +
                            recordOffset;
                        float positionBuffer[3] = {};
                        float displacement[5] = {};

                        float* position =
                            reinterpret_cast<float* (__fastcall*)(
                                void*, void*, float*)>(
                                    CSF_AT(void**, object, 0)[0x20 / 4])(
                                        object, nullptr, positionBuffer);

                        displacement[2] = CSF_AT(float, record, 0x18) - position[2];
                        displacement[1] = CSF_AT(float, record, 0x14) - position[1];
                        displacement[0] = CSF_AT(float, record, 0x10) - position[0];

                        reinterpret_cast<void (__fastcall*)(
                            void*, void*, float*)>(
                                CSF_AT(void**, object, 0)[0x28 / 4])(
                                    object, nullptr, displacement);
                    }
                }
            }

            ++index;
            recordOffset += 28;
        } while (index < self->field_0014);
    }

    uint32_t count = self->field_0024;
    uint32_t index = 0;
    while (index < count)
    {
        auto* children = reinterpret_cast<void**>(self->field_001c);
        UpdateChild(children[index], nullptr, updateValue);
        count = self->field_0024;
        ++index;
    }
    return count;
}

HOOK(0x00408700, C_0082d5e0_UpdateRelativePositions, "auto");
