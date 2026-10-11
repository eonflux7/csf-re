#include <cstdint>
#include "globals.h"
#include <cstring>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, sub_7340D0, 0x007340D0, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, sub_734180, 0x00734180, (void* self, void* edx, int a2));

static void __fastcall C_008670f0_AppendCompactRecords(
    C_008670f0* self, void* /*edx*/)
{
    uint32_t remaining = self->field_0060;
    if (remaining)
    {
        void* records = reinterpret_cast<uint8_t*>(self) + 0x78;
        if (remaining > self->field_007c)
            sub_7340D0(records, nullptr,
                       static_cast<int>(remaining - self->field_007c));

        uint32_t sourceOffset = 0;
        do
        {
            const void* source = reinterpret_cast<const void*>(
                self->field_0058 + sourceOffset + 16);

            if (CSF_AT(uint32_t, self, 0x80) >= self->field_007c)
                sub_7340D0(records, nullptr,
                           static_cast<int>(CSF_AT(uint32_t, self, 0x84)));

            const uint32_t index = CSF_AT(uint32_t, self, 0x80);
            void* destination = reinterpret_cast<void*>(
                self->field_0078 + 28 * index);
            CSF_AT(uint32_t, self, 0x80) = index + 1;
            std::memcpy(destination, source, 0x1C);

            sourceOffset += 48;
        } while (--remaining);
    }

    remaining = self->field_0070;
    if (remaining)
    {
        void* records = reinterpret_cast<uint8_t*>(self) + 0x88;
        if (remaining > self->field_008c)
            sub_734180(records, nullptr,
                       static_cast<int>(remaining - self->field_008c));

        uint32_t sourceOffset = 0;
        do
        {
            const void* source = reinterpret_cast<const void*>(
                self->field_0068 + sourceOffset + 4);

            if (CSF_AT(uint32_t, self, 0x90) >= self->field_008c)
                sub_734180(records, nullptr,
                           static_cast<int>(CSF_AT(uint32_t, self, 0x94)));

            const uint32_t index = CSF_AT(uint32_t, self, 0x90);
            const uint32_t data = CSF_AT(uint32_t, self, 0x88);
            CSF_AT(uint32_t, self, 0x90) = index + 1;
            void* destination = reinterpret_cast<void*>(data + 12 * index);

            CSF_AT(uint32_t, destination, 0x0) = CSF_AT(uint32_t, source, 0x0);
            CSF_AT(uint32_t, destination, 0x4) = CSF_AT(uint32_t, source, 0x4);
            CSF_AT(uint32_t, destination, 0x8) = CSF_AT(uint32_t, source, 0x8);

            sourceOffset += 48;
        } while (--remaining);
    }
}

HOOK(0x00734770, C_008670f0_AppendCompactRecords, "auto");
