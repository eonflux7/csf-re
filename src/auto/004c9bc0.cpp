#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ProcessFirstSubobject, 0x004C3600,
         (void* self, void* edx, void* a2));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ProcessSecondSubobject, 0x00449030,
         (void* self, void* edx, void* a2));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ProcessThirdSubobject, 0x004C3710,
         (void* self, void* edx, int a2));

static int __fastcall S_00523cf0_ProcessCoverAttackState(
    S_00523cf0* self, void* /*edx*/, S_00691860* context)
{
    auto* state = reinterpret_cast<std::uint8_t*>(self);
    const std::uint32_t savedContext = CSF_AT(std::uint32_t, context, 0x1C);

    bool stopped = false;
    if (CSF_AT(std::uint8_t, context, 0x00) == 0)
    {
        void* processor = reinterpret_cast<void*>(context->field_0004);
        stopped = !reinterpret_cast<bool(__fastcall*)(void*, void*, int, S_00691860*)>(
            CSF_AT(void**, processor, 0)[128 / 4])(processor, nullptr, 0, context);
    }

    CSF_AT(std::uint8_t, context, 0x00) = stopped;
    CSF_AT(std::uint32_t, context, 0x1C) = 0;
    if (!stopped)
    {
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        AiCoverAttackTasks_ProcessFirstSubobject(state + 0x28, nullptr, context);
    }

    bool proceed = CSF_AT(std::uint8_t, context, 0x00) == 0;
    CSF_AT(std::uint32_t, context, 0x1C) = 0;
    if (proceed)
    {
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        AiCoverAttackTasks_ProcessSecondSubobject(state + 0x4C, nullptr, context);
    }

    proceed = CSF_AT(std::uint8_t, context, 0x00) == 0;
    CSF_AT(std::uint32_t, context, 0x1C) = 0;
    if (proceed)
    {
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        AiCoverAttackTasks_ProcessThirdSubobject(
            state + 0x5C, nullptr, reinterpret_cast<int>(context));
    }

    proceed = CSF_AT(std::uint8_t, context, 0x00) == 0;
    CSF_AT(std::uint32_t, context, 0x1C) = 0;
    if (proceed)
    {
        void* processor = reinterpret_cast<void*>(context->field_0004);
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        CSF_AT(std::uint8_t, context, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, S_00691860*)>(
                CSF_AT(void**, processor, 0)[84 / 4])(
                    processor, nullptr, state + 0x98, context) == -1;
    }

    proceed = CSF_AT(std::uint8_t, context, 0x00) == 0;
    CSF_AT(std::uint32_t, context, 0x1C) = 0;
    if (proceed)
    {
        void* processor = reinterpret_cast<void*>(context->field_0004);
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        CSF_AT(std::uint8_t, context, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, S_00691860*)>(
                CSF_AT(void**, processor, 0)[84 / 4])(
                    processor, nullptr, state + 0x9C, context) == -1;
    }

    proceed = CSF_AT(std::uint8_t, context, 0x00) == 0;
    CSF_AT(std::uint32_t, context, 0x1C) = 0;
    if (proceed)
    {
        void* processor = reinterpret_cast<void*>(context->field_0004);
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        CSF_AT(std::uint8_t, context, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, S_00691860*)>(
                CSF_AT(void**, processor, 0)[68 / 4])(
                    processor, nullptr, state + 0xA8, context) == -1;
    }

    proceed = CSF_AT(std::uint8_t, context, 0x00) == 0;
    CSF_AT(std::uint32_t, context, 0x1C) = 0;
    if (proceed)
    {
        void* processor = reinterpret_cast<void*>(context->field_0004);
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        CSF_AT(std::uint8_t, context, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, S_00691860*)>(
                CSF_AT(void**, processor, 0)[32 / 4])(
                    processor, nullptr, state + 0xA4, context) == -1;
    }

    proceed = CSF_AT(std::uint8_t, context, 0x00) == 0;
    CSF_AT(std::uint32_t, context, 0x1C) = 0;
    if (proceed)
    {
        void* processor = reinterpret_cast<void*>(context->field_0004);
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        CSF_AT(std::uint8_t, context, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, S_00691860*)>(
                CSF_AT(void**, processor, 0)[68 / 4])(
                    processor, nullptr, state + 169, context) == -1;
    }

    proceed = CSF_AT(std::uint8_t, context, 0x00) == 0;
    CSF_AT(std::uint32_t, context, 0x1C) = 0;
    if (proceed)
    {
        void* processor = reinterpret_cast<void*>(context->field_0004);
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        CSF_AT(std::uint8_t, context, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, S_00691860*)>(
                CSF_AT(void**, processor, 0)[84 / 4])(
                    processor, nullptr, state + 0xA0, context) == -1;
    }

    proceed = CSF_AT(std::uint8_t, context, 0x00) == 0;
    CSF_AT(std::uint32_t, context, 0x1C) = 0;
    if (proceed)
    {
        void* processor = reinterpret_cast<void*>(context->field_0004);
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        CSF_AT(std::uint8_t, context, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, S_00691860*)>(
                CSF_AT(void**, processor, 0)[84 / 4])(
                    processor, nullptr, state + 0x94, context) == -1;
    }

    proceed = CSF_AT(std::uint8_t, context, 0x00) == 0;
    CSF_AT(std::uint32_t, context, 0x1C) = savedContext;
    if (proceed)
    {
        void* processor = reinterpret_cast<void*>(context->field_0004);
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, S_00691860*)>(
                CSF_AT(void**, processor, 0)[124 / 4])(processor, nullptr, context);
        if (static_cast<std::uint8_t>(result) != 0)
        {
            CSF_AT(std::uint8_t, context, 0x00) = 0;
            return result;
        }
    }

    CSF_AT(std::uint8_t, context, 0x00) = 1;
    return 1;
}

HOOK(0x004C9BC0, S_00523cf0_ProcessCoverAttackState, "auto");
