#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, CharacterDefs_DestroyEntry_511040, 0x00511040,
         (void* self, void* edx));

CSF_FUNC(char*, __stdcall, CharacterDefs_ProcessEntry_762CB0, 0x00762CB0,
         (char* a1, int a2));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Embedded EH handler stub and its FuncInfo; not standalone function starts.
CSF_GLOBAL(std::uint8_t, g_CharacterEntryRangeExceptionHandlerCode, 0x00828360);
CSF_GLOBAL(std::uint8_t, g_CharacterEntryRangeExceptionFuncInfo, 0x008937C4);

// Clang's x86 address space 257 addresses memory relative to FS.
typedef std::uint32_t
    __attribute__((address_space(257))) FsDword;

// Character entry stride of 0x22C bytes.
static __declspec(noinline) int __cdecl ProcessCharacterEntryRange(
    std::uint32_t base,
    std::uint32_t count,
    int argument)
{
    const std::uint32_t baseBegin = base;
    char* result = nullptr;

    try
    {
        while (count != 0)
        {
            result = CharacterDefs_ProcessEntry_762CB0(
                reinterpret_cast<char*>(base), argument);

            --count;
            base += 0x22Cu;
        }
    }
    catch (...)
    {
        std::uint32_t completed = baseBegin;
        while (completed != base)
        {
            CharacterDefs_DestroyEntry_511040(
                reinterpret_cast<void*>(completed), nullptr);
            completed += 0x22Cu;
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return static_cast<int>(reinterpret_cast<std::uintptr_t>(result));
}

static int __cdecl ProcessCharacterEntriesWithHandler(
    std::uint32_t base,
    std::uint32_t count,
    int argument,
    int context)
{
    if (count == 0)
    {
        // The original returns the FS:[0] head value it saved in eax
        // during the prologue when the loop never runs.
        return static_cast<int>(*static_cast<FsDword*>(nullptr));
    }

    // The context argument is loaded into ecx for the callee, which is
    // __stdcall and ignores it.
    (void)context;
    return ProcessCharacterEntryRange(base, count, argument);
}

HOOK(0x00762DC0, ProcessCharacterEntriesWithHandler, "auto");
