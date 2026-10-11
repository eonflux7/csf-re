#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);

CSF_FUNC(unsigned int, __fastcall, C_0083b4a0_ForwardPrimaryFlag, 0x004CF810,
         (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, C_0083b4a0_ForwardSecondaryFlag, 0x004CF920,
         (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, C_0083b4a0_ForwardEmbeddedPayload, 0x004CFA70,
         (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, C_0083b4a0_ForwardPayload, 0x004CFAD0,
         (void* self, void* edx, int a2));

static void __fastcall C_0083b4a0_HandleCommand(
    C_0083b4a0* self, void* /*edx*/, uint32_t* command)
{
    switch (command[0])
    {
    case 1:
        C_0083b4a0_ForwardPrimaryFlag(
            self, nullptr,
            static_cast<double>(command[1]) == g_fZero ? 0 : 1);
        break;

    case 2:
        C_0083b4a0_ForwardPayload(
            self, nullptr, static_cast<int>(command[1]));
        break;

    case 3:
    {
        const uint32_t payload = command[1];
        if (payload)
        {
            C_0083b4a0_ForwardEmbeddedPayload(
                self, nullptr, static_cast<int>(payload + 28));
        }
        break;
    }

    case 4:
        self->field_004c = static_cast<uint32_t>(-1);
        self->field_0050 = static_cast<uint32_t>(-1);
        break;

    case 7:
        C_0083b4a0_ForwardSecondaryFlag(
            self, nullptr,
            static_cast<double>(command[1]) == g_fZero ? 0 : 1);
        break;

    default:
        return;
    }
}

HOOK(0x004CFB50, C_0083b4a0_HandleCommand, "auto");
