#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AiCoverAttackTasks_GrowSubobjectArray, 0x00448600, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ProcessSecondSubobject, 0x00449030, (void* self, void* edx, void* a2));

static int __stdcall AiCoverAttackTasks_ProcessSubobjectArray(
    uint32_t* array, S_00691860* context)
{
    const uint8_t existingMode = static_cast<uint8_t>(context->field_0014);
    const uint8_t secondaryMode = CSF_AT(uint8_t, context, 0x15);

    alignas(8) uint8_t countBytes[0x4];
    uint32_t& count = *reinterpret_cast<uint32_t*>(countBytes);
    count = existingMode ? CSF_AT(uint32_t, array, 0x08) : 0;

    bool failed = false;
    if (CSF_AT(uint8_t, context, 0x00) == 0)
    {
        void* handler = reinterpret_cast<void*>(context->field_0004);
        failed = !((bool(__fastcall*)(void*, void*, uint32_t*, S_00691860*))
                       CSF_AT(void**, handler, 0x00)[0x78 / 4])(
            handler, nullptr, &count, context);
    }
    CSF_AT(uint8_t, context, 0x00) = failed;
    if (failed)
    {
        CSF_AT(uint8_t, context, 0x00) = 1;
        return 1;
    }

    if (existingMode || secondaryMode)
    {
        uint32_t offset = 0;
        for (uint32_t index = 0; index < count; ++index, offset += 16)
        {
            void* element = CSF_AT(uint8_t*, array, 0x00) + offset;
            const bool process = CSF_AT(uint8_t, context, 0x00) == 0;
            CSF_AT(uint32_t, context, 0x1C) = 0;
            if (process)
            {
                CSF_AT(uint8_t, context, 0x15) = secondaryMode;
                CSF_AT(uint8_t, context, 0x16) = 0;
                AiCoverAttackTasks_ProcessSecondSubobject(element, nullptr, context);
            }
        }
    }
    else
    {
        const uint32_t capacity = CSF_AT(uint32_t, array, 0x04);
        CSF_AT(uint32_t, array, 0x08) = 0;
        if (count > capacity)
        {
            AiCoverAttackTasks_GrowSubobjectArray(
                array, nullptr, static_cast<int>(count - capacity));
        }

        uint32_t offset = 0;
        for (uint32_t index = 0; index < count; ++index, offset += 16)
        {
            if (CSF_AT(uint32_t, array, 0x08) >= CSF_AT(uint32_t, array, 0x04))
            {
                AiCoverAttackTasks_GrowSubobjectArray(
                    array, nullptr, static_cast<int>(CSF_AT(uint32_t, array, 0x0C)));
            }

            const uint32_t insertionIndex = CSF_AT(uint32_t, array, 0x08);
            uint8_t* inserted = CSF_AT(uint8_t*, array, 0x00) + 16 * insertionIndex;
            CSF_AT(uint32_t, array, 0x08) = insertionIndex + 1;
            CSF_AT(int32_t, inserted, 0x04) = -1;
            CSF_AT(int32_t, inserted, 0x08) = -1;
            // The original copies an uninitialized stack value here.
            CSF_AT(uint32_t, inserted, 0x0C) = 0;

            void* element = CSF_AT(uint8_t*, array, 0x00) + offset;
            const bool process = CSF_AT(uint8_t, context, 0x00) == 0;
            CSF_AT(uint32_t, context, 0x1C) = 0;
            if (process)
            {
                CSF_AT(uint8_t, context, 0x15) = 0;
                CSF_AT(uint8_t, context, 0x16) = 0;
                AiCoverAttackTasks_ProcessSecondSubobject(element, nullptr, context);
            }
        }
    }

    if (CSF_AT(uint8_t, context, 0x00) == 0)
    {
        void* handler = reinterpret_cast<void*>(context->field_0004);
        if (((bool(__fastcall*)(void*, void*, S_00691860*))
                 CSF_AT(void**, handler, 0x00)[0x7C / 4])(handler, nullptr, context))
        {
            CSF_AT(uint8_t, context, 0x00) = 0;
            return 0;
        }
    }

    CSF_AT(uint8_t, context, 0x00) = 1;
    return 1;
}

HOOK(0x00449C60, AiCoverAttackTasks_ProcessSubobjectArray, "auto");
