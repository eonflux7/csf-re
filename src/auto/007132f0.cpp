#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

using GetWindowLongAProc = int32_t (__stdcall*)(void*, int);
CSF_GLOBAL(GetWindowLongAProc, g_GetWindowLongA, 0x0082D31C);

CSF_FUNC(int32_t, __fastcall, InputDInput_CreateDirectInput8, 0x007DB650,
         (void* self, void* edx, void* instance, uint32_t version,
          const void* interfaceId, uint32_t* output, void* outer));

static char __fastcall CInputDirectInput_InitializeDevices(
    CInputDirectInput* self, void* /*edx*/, void* window, char exclusiveMouse)
{
    void* instance = reinterpret_cast<void*>(g_GetWindowLongA(window, -6));
    if (InputDInput_CreateDirectInput8(
            nullptr, nullptr, instance, 0x800u,
            reinterpret_cast<const void*>(CSF_ADDR_DATA(0x00872C94)),
            &self->field_001c, nullptr))
        return 0;

    void* input = reinterpret_cast<void*>(self->field_001c);
    if (reinterpret_cast<int32_t (__stdcall*)(
            void*, const void*, uint32_t*, void*)>(
            CSF_AT(void**, input, 0)[12 / 4])(
                input,
                reinterpret_cast<const void*>(CSF_ADDR_DATA(0x00872C34)),
                &self->field_0020, nullptr))
        return 0;

    void* keyboard = reinterpret_cast<void*>(self->field_0020);
    reinterpret_cast<int32_t (__stdcall*)(void*, const void*)>(
        CSF_AT(void**, keyboard, 0)[44 / 4])(
            keyboard,
            reinterpret_cast<const void*>(CSF_ADDR_DATA(0x00883BD4)));

    keyboard = reinterpret_cast<void*>(self->field_0020);
    reinterpret_cast<int32_t (__stdcall*)(void*, void*, int)>(
        CSF_AT(void**, keyboard, 0)[52 / 4])(keyboard, window, 6);

    alignas(8) uint8_t propertyBytes[0x14];
    uint32_t* property = reinterpret_cast<uint32_t*>(propertyBytes);
    property[0] = 20;
    property[1] = 16;
    property[2] = 0;
    property[3] = 0;
    property[4] = 64;

    keyboard = reinterpret_cast<void*>(self->field_0020);
    reinterpret_cast<int32_t (__stdcall*)(void*, uint32_t, void*)>(
        CSF_AT(void**, keyboard, 0)[24 / 4])(keyboard, 1, propertyBytes);

    keyboard = reinterpret_cast<void*>(self->field_0020);
    reinterpret_cast<int32_t (__stdcall*)(void*)>(
        CSF_AT(void**, keyboard, 0)[28 / 4])(keyboard);

    input = reinterpret_cast<void*>(self->field_001c);
    if (reinterpret_cast<int32_t (__stdcall*)(
            void*, const void*, uint32_t*, void*)>(
            CSF_AT(void**, input, 0)[12 / 4])(
                input,
                reinterpret_cast<const void*>(CSF_ADDR_DATA(0x00872C44)),
                &self->field_0024, nullptr))
        return 0;

    void* mouse = reinterpret_cast<void*>(self->field_0024);
    reinterpret_cast<int32_t (__stdcall*)(void*, const void*)>(
        CSF_AT(void**, mouse, 0)[44 / 4])(
            mouse,
            reinterpret_cast<const void*>(CSF_ADDR_DATA(0x008839CC)));

    mouse = reinterpret_cast<void*>(self->field_0024);
    reinterpret_cast<int32_t (__stdcall*)(void*, void*, int)>(
        CSF_AT(void**, mouse, 0)[52 / 4])(
            mouse, window, 6 - (exclusiveMouse != 0));

    hook::Regs registers{};
    registers.eax = self->field_0024;
    registers.ebx = reinterpret_cast<uint32_t>(&self->field_001c);
    registers.esi = reinterpret_cast<uint32_t>(&self->field_0024);
    registers.edi = reinterpret_cast<uint32_t>(self);
    uint32_t arguments[] = {64};
    hook::call_regs(CSF_ADDR_FN(0x00712F80), &registers, arguments, 1);

    mouse = reinterpret_cast<void*>(self->field_0024);
    reinterpret_cast<int32_t (__stdcall*)(void*)>(
        CSF_AT(void**, mouse, 0)[28 / 4])(mouse);

    self->field_0c91 = 1;
    return 1;
}

HOOK(0x007132F0, CInputDirectInput_InitializeDevices, "auto");
