#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, DynamicObjects_SerializeReference, 0x00442E70, (S_00691860* context, bool* state));
CSF_FUNC(int, __fastcall, ProcessContextStageHandlers, 0x006CA6F0, (void* self, void* , int contextAddress));

static int __fastcall S_006ca6f0_SerializeContext(
    S_006ca6f0* self, void* /*edx*/, int contextAddress)
{
    // The packet also identifies the stage-handler address as a vtable reference.
    (void)CSF_VTBL(0x006CA6F0);

    auto* context = reinterpret_cast<std::uint8_t*>(contextAddress);
    bool failed = false;
    if (CSF_AT(std::uint8_t, context, 0x00) == 0)
    {
        void* handler = CSF_AT(void*, context, 0x04);
        failed = !((bool(__fastcall*)(void*, void*, int, int))
            CSF_AT(void**, handler, 0)[0x80 / 4])(
                handler, nullptr, 0, contextAddress);
    }

    const int savedStage = CSF_AT(int, context, 0x1C);
    CSF_AT(std::uint8_t, context, 0x00) = failed;
    ProcessContextStageHandlers(self, nullptr, contextAddress);

    bool active = CSF_AT(std::uint8_t, context, 0x00) == 0;
    CSF_AT(int, context, 0x1C) = 0;
    if (active)
    {
        void* handler = CSF_AT(void*, context, 0x04);
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        const int result = ((int(__fastcall*)(void*, void*, std::uint8_t*, int))
            CSF_AT(void**, handler, 0)[0x44 / 4])(
                handler, nullptr,
                reinterpret_cast<std::uint8_t*>(self) + 0x20,
                contextAddress);
        CSF_AT(std::uint8_t, context, 0x00) = result == -1;
    }

    active = CSF_AT(std::uint8_t, context, 0x00) == 0;
    CSF_AT(int, context, 0x1C) = 0;
    if (active)
    {
        CSF_AT(std::uint8_t, context, 0x15) = 1;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        DynamicObjects_SerializeReference(
            reinterpret_cast<S_00691860*>(
                reinterpret_cast<std::uint8_t*>(self) + 0x38),
            reinterpret_cast<bool*>(context));
    }

    active = CSF_AT(std::uint8_t, context, 0x00) == 0;
    CSF_AT(int, context, 0x1C) = savedStage;
    if (active)
    {
        void* handler = CSF_AT(void*, context, 0x04);
        const int result = ((int(__fastcall*)(void*, void*, int))
            CSF_AT(void**, handler, 0)[0x7C / 4])(
                handler, nullptr, contextAddress);
        if (static_cast<std::uint8_t>(result) != 0)
        {
            CSF_AT(std::uint8_t, context, 0x00) = 0;
            return result;
        }
    }

    CSF_AT(std::uint8_t, context, 0x00) = 1;
    return 1;
}

HOOK(0x006F2440, S_006ca6f0_SerializeContext, "auto");
