#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, C_00831638_ProcessCommonFields, 0x00474BD0,
         (void* self, void* edx, int a2));

static int __fastcall C_00831638_ProcessFields(
    C_00831638* self, void* /*edx*/, int contextAddress)
{
    void* context = reinterpret_cast<void*>(contextAddress);
    bool failed = false;

    // First step: if the context flag is clear, ask the processor (slot 0x80).
    if (CSF_AT(uint8_t, context, 0) == 0)
    {
        void* processor = CSF_AT(void*, context, 0x4);
        failed = !reinterpret_cast<bool (__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, processor, 0)[0x80 / 4])(
                processor, nullptr, 0, contextAddress);
    }

    const uint32_t savedState = CSF_AT(uint32_t, context, 0x1C);
    CSF_AT(uint8_t, context, 0) = failed;
    C_00831638_ProcessCommonFields(self, nullptr, contextAddress);

    bool processField = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (processField)
    {
        void* processor = CSF_AT(void*, context, 0x4);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        const int result =
            reinterpret_cast<int (__fastcall*)(void*, void*, void*, int)>(
                CSF_AT(void**, processor, 0)[0x38 / 4])(
                    processor, nullptr,
                    reinterpret_cast<void*>(&self->field_0034), contextAddress);
        CSF_AT(uint8_t, context, 0) = result == -1;
    }

    processField = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (processField)
    {
        void* processor = CSF_AT(void*, context, 0x4);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        const int result =
            reinterpret_cast<int (__fastcall*)(void*, void*, void*, int)>(
                CSF_AT(void**, processor, 0)[0x38 / 4])(
                    processor, nullptr,
                    reinterpret_cast<void*>(&self->field_0040), contextAddress);
        CSF_AT(uint8_t, context, 0) = result == -1;
    }

    processField = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (processField)
    {
        void* processor = CSF_AT(void*, context, 0x4);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        const int result =
            reinterpret_cast<int (__fastcall*)(void*, void*, void*, int)>(
                CSF_AT(void**, processor, 0)[0x58 / 4])(
                    processor, nullptr,
                    reinterpret_cast<void*>(&self->field_004c), contextAddress);
        CSF_AT(uint8_t, context, 0) = result == -1;
    }

    CSF_AT(uint32_t, context, 0x1C) = savedState;
    if (CSF_AT(uint8_t, context, 0) == 0)
    {
        void* processor = CSF_AT(void*, context, 0x4);
        // The original keeps the raw eax of this call as its return value
        // when the low byte is non-zero, so the full int is returned as-is.
        const int result =
            reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, processor, 0)[0x7C / 4])(
                    processor, nullptr, contextAddress);
        if ((result & 0xFF) != 0)
        {
            CSF_AT(uint8_t, context, 0) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, context, 0) = 1;
    return 1;
}

HOOK(0x004750F0, C_00831638_ProcessFields, "auto");
