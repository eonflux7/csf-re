#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __stdcall, ApplyToElement_500DE0, 0x00500DE0,
         (void* a1, int a2));

CSF_FUNC(void, __fastcall, DestroyElement_4FF490, 0x004FF490,
         (std::uint32_t self, void* edx));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Embedded EH handler stub and its FuncInfo; not standalone function starts.
CSF_GLOBAL(std::uint8_t, g_ElementRangeExceptionHandlerCode, 0x00826CE0);
CSF_GLOBAL(std::uint8_t, g_ElementRangeExceptionFuncInfo, 0x0088A950);

// Clang's x86 address space 257 addresses memory relative to FS.
typedef std::uint32_t
    __attribute__((address_space(257))) FsDword;

// Element stride of 0x58 bytes.
static __declspec(noinline) int __cdecl ApplyToElementRange(
    std::uint32_t base,
    std::uint32_t count,
    int argument)
{
    const std::uint32_t baseBegin = base;
    int result = 0;

    try
    {
        while (count != 0)
        {
            result = ApplyToElement_500DE0(
                reinterpret_cast<void*>(base), argument);

            --count;
            base += 0x58u;
        }
    }
    catch (...)
    {
        std::uint32_t completed = baseBegin;
        while (completed != base)
        {
            DestroyElement_4FF490(completed, nullptr);
            completed += 0x58u;
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return result;
}

static int __cdecl ApplyToElementsWithHandler(
    std::uint32_t base,
    std::uint32_t count,
    int argument,
    int context)
{
    if (count == 0)
    {
        // No EH frame in this wrapper: the original returns the FS:[0]
        // head value it saved in eax during the prologue.
        return static_cast<int>(*static_cast<FsDword*>(nullptr));
    }

    // The context argument is loaded into ecx for the callee, which is
    // __stdcall and ignores it.
    (void)context;
    return ApplyToElementRange(base, count, argument);
}

HOOK(0x00501050, ApplyToElementsWithHandler, "auto");
