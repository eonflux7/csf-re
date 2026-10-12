#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void**, __stdcall, ConstructSubobject_775520, 0x00775520,
         (void* a1, int a2));

CSF_FUNC(char, __fastcall, DestroyElement168_774DC0, 0x00774DC0,
         (void* self, void* edx));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Embedded EH handler stub and its FuncInfo; not standalone function starts.
CSF_GLOBAL(std::uint8_t, g_ElementRangeExceptionHandlerCode, 0x008285B0);
CSF_GLOBAL(std::uint8_t, g_ElementRangeExceptionFuncInfo, 0x008947F8);

// Clang's x86 address space 257 addresses memory relative to FS.
typedef std::uint32_t
    __attribute__((address_space(257))) FsDword;

// Element stride of 0xA8 (168) bytes.
static __declspec(noinline) void** __cdecl ConstructElementRange168(
    std::uint32_t base,
    std::uint32_t count,
    int argument,
    int context)
{
    const std::uint32_t baseBegin = base;

    // With a zero count the original returns the FS:[0] head value it saved in eax.
    void** result = reinterpret_cast<void**>(*static_cast<FsDword*>(nullptr));

    try
    {
        while (count != 0)
        {
            // Callee is __stdcall; ecx carries the context value and is ignored.
            (void)context;
            result = ConstructSubobject_775520(
                reinterpret_cast<void*>(base), argument);

            --count;
            base += 0xA8u;
        }
    }
    catch (...)
    {
        std::uint32_t completed = baseBegin;
        while (completed != base)
        {
            DestroyElement168_774DC0(
                reinterpret_cast<void*>(completed), nullptr);
            completed += 0xA8u;
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return result;
}

HOOK(0x00775830, ConstructElementRange168, "auto");
