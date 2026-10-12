#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __stdcall, Records_ProcessEntry, 0x00449170,
         (uint32_t* a1, uint32_t* a2));
CSF_FUNC(int, __stdcall, Records_DestroyEntry, 0x00448D50,
         (int a1));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));
CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

static uint32_t* __cdecl Records_ProcessEntries(
    uint32_t* first, uint32_t count, uint32_t* argument,
    int /*context*/, int /*unused*/)
{
    uint32_t* current = first;
    uint32_t* result = nullptr;

    try
    {
        while (count != 0)
        {
            result = Records_ProcessEntry(current, argument);
            --count;
            current = reinterpret_cast<uint32_t*>(
                reinterpret_cast<uint8_t*>(current) + 0x14);
        }
    }
    catch (...)
    {
        uint32_t* entry = first;
        while (entry != current)
        {
            Records_DestroyEntry(
                static_cast<int>(reinterpret_cast<uintptr_t>(entry)));
            entry = reinterpret_cast<uint32_t*>(
                reinterpret_cast<uint8_t*>(entry) + 0x14);
        }
        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x004495B0, Records_ProcessEntries, "auto");
