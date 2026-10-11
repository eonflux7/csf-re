#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, S_005080e0_BuildProgressInput, 0x005080E0,
         (void* self, void* edx, uint32_t* a2, uint16_t* a3));
CSF_FUNC(uint32_t*, __fastcall, S_00508240_SubmitProgressInput, 0x00508790,
         (void* self, void* edx, uint32_t* a2, int a3));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(std::uint32_t*, __fastcall, MenuProgressInput_AssignText, 0x00696F90,
         (std::uint32_t* self, void* edx, std::int16_t* text));
CSF_FUNC(int, __cdecl, GetValue_8BAF98, 0x0069C370, ());

static char __fastcall S_005080e0_HandleProgressInput(
    S_005080e0* self, void* /*edx*/, S_004f21d0* text)
{
    // The packet labels this direct-call address as a vtable reference.
    (void)CSF_VTBL(0x005080E0);

    alignas(8) std::uint8_t inputBytes[0x38] = {};
    CSF_AT(std::uint32_t, inputBytes, 0x00) = 1;
    CSF_AT(float, inputBytes, 0x14) = 1.0f;
    CSF_AT(std::uint32_t, inputBytes, 0x34) = 7;
    CSF_AT(std::uint32_t, inputBytes, 0x30) = 0;
    CSF_AT(std::uint16_t, inputBytes, 0x20) = 0;

    MenuProgressInput_AssignText(
        reinterpret_cast<std::uint32_t*>(inputBytes + 0x1C),
        nullptr, reinterpret_cast<std::int16_t*>(text));

    char result = S_005080e0_BuildProgressInput(
        self, nullptr, reinterpret_cast<std::uint32_t*>(inputBytes),
        reinterpret_cast<std::uint16_t*>(text));

    if (result)
    {
        bool submit;
        if (!self->field_0064)
        {
            submit = true;
        }
        else
        {
            int currentValue = GetValue_8BAF98();
            submit = self->field_0064 == static_cast<std::uint32_t>(currentValue);
        }

        if (submit)
        {
            S_00508240_SubmitProgressInput(
                reinterpret_cast<std::uint8_t*>(self) + 0x54,
                nullptr, reinterpret_cast<std::uint32_t*>(inputBytes), 0);
        }
    }

    if (CSF_AT(std::uint32_t, inputBytes, 0x34) >= 8)
    {
        void* block = CSF_AT(void*, inputBytes, 0x20);
        if (block)
            Mem_FreeDebug(block);
    }

    return result;
}

HOOK(0x00508810, S_005080e0_HandleProgressInput, "auto");
