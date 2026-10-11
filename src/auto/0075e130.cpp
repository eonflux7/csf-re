#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(char*, __stdcall, ParseWaypointRecord_75DF90, 0x0075DF90,
         (char* a1, int a2));

CSF_FUNC(char, __fastcall, DestroyWaypointRecord_75D430, 0x0075D430,
         (void* self, void* edx));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Embedded EH handler stub and its FuncInfo; not standalone function starts.
CSF_GLOBAL(std::uint8_t, g_WaypointRecordExceptionHandlerCode, 0x00828300);
CSF_GLOBAL(std::uint8_t, g_WaypointRecordExceptionFuncInfo, 0x008935B0);

// Clang's x86 address space 257 addresses memory relative to FS.
typedef std::uint32_t
    __attribute__((address_space(257))) FsDword;

// Record stride of 0x48 bytes.
static __declspec(noinline) char* __cdecl ParseWaypointRecordRange(
    std::uint32_t base,
    std::uint32_t count,
    int parseArg,
    std::uint32_t receiver)
{
    const std::uint32_t baseBegin = base;
    char* result = nullptr;

    try
    {
        while (count != 0)
        {
            // The original sets ecx = receiver before the call; the
            // __stdcall binding above does not carry it.
            (void)receiver;
            result = ParseWaypointRecord_75DF90(
                reinterpret_cast<char*>(base), parseArg);

            --count;
            base += 0x48u;
        }
    }
    catch (...)
    {
        std::uint32_t completed = baseBegin;
        while (completed != base)
        {
            DestroyWaypointRecord_75D430(
                reinterpret_cast<void*>(completed), nullptr);
            completed += 0x48u;
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return result;
}

static std::uint32_t* __cdecl ParseWaypointRecordsWithCleanup(
    std::uint32_t base,
    std::uint32_t count,
    int parseArg,
    std::uint32_t receiver)
{
    if (count == 0)
    {
        // No loop runs: the original returns the FS:[0] head value it
        // saved in eax during the prologue.
        return reinterpret_cast<std::uint32_t*>(
            static_cast<std::uintptr_t>(*static_cast<FsDword*>(nullptr)));
    }

    return reinterpret_cast<std::uint32_t*>(
        ParseWaypointRecordRange(base, count, parseArg, receiver));
}

HOOK(0x0075E130, ParseWaypointRecordsWithCleanup, "auto");
