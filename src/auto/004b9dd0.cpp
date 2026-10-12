#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetPathStatus, 0x004AEB90, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiDistracted_DispatchEvent, 0x004B9C30,
         (std::uint32_t* self, void* edx, int context, int event));

static int __fastcall S_004aa4d0_HandlePathEvent(
    S_004aa4d0* self, void* /*edx*/, int context, int /*unused*/, int event)
{
    auto* owner = reinterpret_cast<std::uint32_t*>(
        reinterpret_cast<std::uint8_t*>(self) - 4);

    if (event == 1)
    {
        switch (GetPathStatus(owner, nullptr))
        {
        case 0:
            event = 2;
            break;
        case 1:
            event = 3;
            break;
        case 2:
            event = 4;
            break;
        case 3:
            event = 5;
            break;
        case -1:
            event = 0;
            break;
        default:
            break;
        }
    }

    return AiDistracted_DispatchEvent(owner, nullptr, context, event);
}

HOOK(0x004B9DD0, S_004aa4d0_HandlePathEvent, "auto");
