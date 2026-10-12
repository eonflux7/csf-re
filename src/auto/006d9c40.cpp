#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static CSF_FUNC(int, __fastcall, PTank_AssignElement, 0x006D9890,
               (void* self, void* edx, int a4a));

static void* __fastcall S_0046bea0_AssignNineElements(
    S_0046bea0* self, void* /*edx*/, int source)
{
    auto* destination = reinterpret_cast<uint8_t*>(self);
    const uint32_t sourceAddress = static_cast<uint32_t>(source);

    PTank_AssignElement(destination, nullptr, source);
    PTank_AssignElement(destination + 16, nullptr, static_cast<int>(sourceAddress + 16));
    PTank_AssignElement(destination + 32, nullptr, static_cast<int>(sourceAddress + 32));
    PTank_AssignElement(destination + 48, nullptr, static_cast<int>(sourceAddress + 48));
    PTank_AssignElement(destination + 64, nullptr, static_cast<int>(sourceAddress + 64));
    PTank_AssignElement(destination + 80, nullptr, static_cast<int>(sourceAddress + 80));
    PTank_AssignElement(destination + 96, nullptr, static_cast<int>(sourceAddress + 96));
    PTank_AssignElement(destination + 112, nullptr, static_cast<int>(sourceAddress + 112));
    PTank_AssignElement(destination + 128, nullptr, static_cast<int>(sourceAddress + 128));

    return self;
}

HOOK(0x006D9C40, S_0046bea0_AssignNineElements, "auto");
