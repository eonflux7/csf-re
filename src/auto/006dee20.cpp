#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, sub_6DECC0, 0x006DECC0, (void* a1, int a2));
CSF_FUNC(int, __stdcall, DestroyElementThroughContext, 0x006B5220, (int* record));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));
CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

static int __cdecl GfxTextureLoader_ProcessRecords(
    S_0046bea0* first,
    std::uint32_t count,
    int argument,
    void* /*context*/)
{
    auto* current = reinterpret_cast<std::uint8_t*>(first);
    int result = 0;

    try
    {
        while (count != 0)
        {
            result = sub_6DECC0(current, argument);
            --count;
            current += 0x10;
        }
    }
    catch (...)
    {
        auto* record = reinterpret_cast<std::uint8_t*>(first);
        while (record != current)
        {
            DestroyElementThroughContext(reinterpret_cast<int*>(record));
            record += 0x10;
        }

        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x006DEE20, GfxTextureLoader_ProcessRecords, "auto");
