#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __stdcall, ApplyToSceneObject_6CE2E0, 0x006CE2E0,
         (int a1, int a2));

CSF_FUNC(unsigned int, __fastcall, DestroySceneObjectOptionsStorage, 0x004018E0,
         (void* self, void* edx));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// FuncInfo of the embedded EH frame; the frame's handler stub is not a function start.
CSF_GLOBAL(std::uint8_t, g_SceneObjectRangeExceptionFuncInfo, 0x00890640);

// Clang's x86 address space 257 addresses memory relative to FS.
typedef std::uint32_t
    __attribute__((address_space(257))) FsDword;

// Scene object stride of 0x64 bytes (25 dwords).
static __declspec(noinline) int __cdecl ApplyToSceneObjectRange(
    std::uint32_t base,
    std::uint32_t count,
    int argument,
    int context)
{
    const std::uint32_t baseBegin = base;
    int result = 0;

    try
    {
        while (count != 0)
        {
            // Callee is __stdcall; ecx carries the context value and is ignored.
            (void)context;
            result = ApplyToSceneObject_6CE2E0(
                static_cast<int>(base), argument);

            --count;
            base += 0x64u;
        }
    }
    catch (...)
    {
        std::uint32_t completed = baseBegin;
        while (completed != base)
        {
            DestroySceneObjectOptionsStorage(
                reinterpret_cast<void*>(completed), nullptr);
            completed += 0x64u;
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return result;
}

static int __cdecl ApplyToSceneObjectsWithHandler(
    std::uint32_t base,
    std::uint32_t count,
    int argument,
    int context,
    int unusedArgument)
{
    (void)unusedArgument;

    if (count == 0)
    {
        // No EH frame in this wrapper: the original returns the FS:[0]
        // head value it saved in eax during the prologue.
        return static_cast<int>(*static_cast<FsDword*>(nullptr));
    }

    return ApplyToSceneObjectRange(base, count, argument, context);
}

HOOK(0x006CE4C0, ApplyToSceneObjectsWithHandler, "auto");
