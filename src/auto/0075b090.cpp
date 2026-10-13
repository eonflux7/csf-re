#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __stdcall, SoundDefs_ApplyToEntry_75AEF0, 0x0075AEF0,
         (int a1, int a2));

CSF_FUNC(int, __fastcall, BddTables_ResetElementBuffer, 0x0051BEB0,
         (void* self, void* edx));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Embedded EH handler stub and its FuncInfo; not standalone function starts.
CSF_GLOBAL(std::uint8_t, g_SoundEntryRangeExceptionHandlerCode, 0x00828270);
CSF_GLOBAL(std::uint8_t, g_SoundEntryRangeExceptionFuncInfo, 0x00893188);

// Clang's x86 address space 257 addresses memory relative to FS.
typedef std::uint32_t
    __attribute__((address_space(257))) FsDword;

// Sound entry stride of 0x44 bytes.
static __declspec(noinline) void* __cdecl SoundDefs_ApplyToEntryRangeInner(
    std::uint32_t base,
    std::uint32_t count,
    int argument,
    int context)
{
    const std::uint32_t baseBegin = base;
    void* result = nullptr;

    try
    {
        while (count != 0)
        {
            // Callee is __stdcall; ecx carries the context value and is ignored.
            (void)context;
            result = SoundDefs_ApplyToEntry_75AEF0(
                static_cast<int>(base), argument);

            --count;
            base += 0x44u;
        }
    }
    catch (...)
    {
        std::uint32_t completed = baseBegin;
        while (completed != base)
        {
            BddTables_ResetElementBuffer(
                reinterpret_cast<void*>(completed), nullptr);
            completed += 0x44u;
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return result;
}

static void* __cdecl SoundDefs_ApplyToEntryRange(
    std::uint32_t base,
    std::uint32_t count,
    int argument,
    int context,
    int unused)
{
    (void)unused;

    if (count == 0)
    {
        // No EH frame in this wrapper path: the original returns the FS:[0]
        // head value it saved in eax during the prologue.
        return reinterpret_cast<void*>(
            static_cast<std::uintptr_t>(*static_cast<FsDword*>(nullptr)));
    }

    return SoundDefs_ApplyToEntryRangeInner(base, count, argument, context);
}

HOOK(0x0075B090, SoundDefs_ApplyToEntryRange, "auto");
