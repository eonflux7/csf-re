#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __stdcall, HudStringShow_InitializeRecord, 0x004F24C0,
         (void* a1, int a2));
CSF_FUNC(unsigned int, __fastcall, HudStringShow_DestroyRecord, 0x004F2240,
         (void* self, void* edx));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));
CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

CSF_GLOBAL(std::uint8_t, g_HudStringShowRangeExceptionHandlerCode, 0x00826C80);
CSF_GLOBAL(std::uint8_t, g_HudStringShowRangeExceptionFuncInfo, 0x0088A6EC);

typedef std::uint32_t __attribute__((address_space(257))) FsDword;

static __declspec(noinline) int __cdecl HudStringShow_InitializeRange(
    std::uint32_t base, std::uint32_t count, int argument,
    int context, int unused)
{
    const std::uint32_t begin = base;
    int result = 0;
    (void)context;
    (void)unused;

    try
    {
        while (count != 0)
        {
            result = HudStringShow_InitializeRecord(
                reinterpret_cast<void*>(base), argument);
            --count;
            base += 124;
        }
    }
    catch (...)
    {
        std::uint32_t completed = begin;
        while (completed != base)
        {
            HudStringShow_DestroyRecord(
                reinterpret_cast<void*>(completed), nullptr);
            completed += 124;
        }
        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }
    return result;
}

static int __cdecl HudStringShow_InitializeRecords(
    std::uint32_t base, std::uint32_t count, int argument,
    int context, int unused)
{
    if (count == 0)
        return static_cast<int>(*static_cast<FsDword*>(nullptr));

    return HudStringShow_InitializeRange(base, count, argument, context, unused);
}

HOOK(0x004F2C00, HudStringShow_InitializeRecords, "auto");
