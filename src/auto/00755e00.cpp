#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __stdcall, GameConfig_ProcessOneElement, 0x00755CC0,
         (void* a1, int a2));

CSF_FUNC(unsigned int, __fastcall, SceneLoader_ReleaseAndResetBuffers, 0x00500210,
         (void* self, void* edx));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Embedded EH handler stub and its FuncInfo; not standalone function starts.
CSF_GLOBAL(std::uint8_t, g_ProcessElementsExceptionHandlerCode, 0x00828240);
CSF_GLOBAL(std::uint8_t, g_ProcessElementsExceptionFuncInfo, 0x00893014);

// Clang's x86 address space 257 addresses memory relative to FS.
typedef std::uint32_t
    __attribute__((address_space(257))) FsDword;

// Element stride of 0x50 bytes.
static __declspec(noinline) void* __cdecl GameConfig_ProcessElementRange(
    std::uint32_t base,
    std::uint32_t count,
    int argument,
    int context)
{
    const std::uint32_t baseBegin = base;
    void* result = nullptr;

    // The context argument is loaded into ecx for the callee, which is
    // __stdcall and ignores it.
    (void)context;

    try
    {
        while (count != 0)
        {
            result = GameConfig_ProcessOneElement(
                reinterpret_cast<void*>(base), argument);

            --count;
            base += 0x50u;
        }
    }
    catch (...)
    {
        std::uint32_t completed = baseBegin;
        while (completed != base)
        {
            SceneLoader_ReleaseAndResetBuffers(
                reinterpret_cast<void*>(completed), nullptr);
            completed += 0x50u;
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return result;
}

static void* __cdecl GameConfig_ProcessElements(
    void* a1,
    int a2,
    void* a3,
    int a4,
    int a5)
{
    (void)a5;

    const std::uint32_t base = static_cast<std::uint32_t>(
        reinterpret_cast<std::uintptr_t>(a1));
    const std::uint32_t count = static_cast<std::uint32_t>(a2);
    const int argument = static_cast<int>(
        reinterpret_cast<std::uintptr_t>(a3));

    if (count == 0)
    {
        // The whole function has an EH frame, but the loop never runs:
        // the original returns the FS:[0] head value it saved in eax
        // during the prologue.
        return reinterpret_cast<void*>(static_cast<std::uintptr_t>(
            *static_cast<FsDword*>(nullptr)));
    }

    return GameConfig_ProcessElementRange(base, count, argument, a4);
}

HOOK(0x00755E00, GameConfig_ProcessElements, "auto");
