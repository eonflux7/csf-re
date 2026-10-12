#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(char*, __stdcall, ApplyToElement_43EE40, 0x0043EE40,
         (char* a1, int a2));

CSF_FUNC(unsigned int, __fastcall, DestroyElement_43E360, 0x0043E360,
         (void* self, void* edx));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Embedded EH handler stub and its FuncInfo; not standalone function starts.
CSF_GLOBAL(std::uint8_t, g_ElementRangeExceptionHandlerCode, 0x008264E0);
CSF_GLOBAL(std::uint8_t, g_ElementRangeExceptionFuncInfo, 0x00887218);

// Clang's x86 address space 257 addresses memory relative to FS.
typedef std::uint32_t
    __attribute__((address_space(257))) FsDword;

// Element stride of 0xC4 (196) bytes.
static __declspec(noinline) int __cdecl ApplyToElementRange196(
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
            result = reinterpret_cast<int>(ApplyToElement_43EE40(
                reinterpret_cast<char*>(base), argument));

            --count;
            base += 0xC4u;
        }
    }
    catch (...)
    {
        std::uint32_t completed = baseBegin;
        while (completed != base)
        {
            DestroyElement_43E360(reinterpret_cast<void*>(completed), nullptr);
            completed += 0xC4u;
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return result;
}

static int __cdecl ApplyToElementsStride196(
    std::uint32_t base,
    std::uint32_t count,
    int argument,
    int context)
{
    if (count == 0)
    {
        // The original returns the FS:[0] head value it saved in eax during
        // the prologue when the loop never runs.
        return static_cast<int>(*static_cast<FsDword*>(nullptr));
    }

    // The context argument is loaded into ecx for the callee, which is
    // __stdcall and ignores it.
    (void)context;
    return ApplyToElementRange196(base, count, argument);
}

HOOK(0x0043F530, ApplyToElementsStride196, "auto");
