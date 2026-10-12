#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __stdcall, ApplyToRecord_43EE20, 0x0043EE20,
         (void* a1, int a2));

CSF_FUNC(unsigned int, __fastcall, DestroyRecord_43E300, 0x0043E300,
         (std::uint32_t* self, void* edx));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Embedded EH handler stub and its FuncInfo; not standalone function starts.
CSF_GLOBAL(std::uint8_t, g_RecordRangeExceptionHandlerCode, 0x008264D0);
CSF_GLOBAL(std::uint8_t, g_RecordRangeExceptionFuncInfo, 0x008871C8);

// Clang's x86 address space 257 addresses memory relative to FS.
typedef std::uint32_t
    __attribute__((address_space(257))) FsDword;

// Record stride of 0x38 bytes.
static __declspec(noinline) int __cdecl ApplyToRecordRange(
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
            result = ApplyToRecord_43EE20(
                reinterpret_cast<void*>(base), argument);

            --count;
            base += 0x38u;
        }
    }
    catch (...)
    {
        std::uint32_t completed = baseBegin;
        while (completed != base)
        {
            DestroyRecord_43E300(
                reinterpret_cast<std::uint32_t*>(completed), nullptr);
            completed += 0x38u;
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return result;
}

static int __cdecl ApplyToRecordsWithHandler(
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

    return ApplyToRecordRange(base, count, argument, context);
}

HOOK(0x0043F4A0, ApplyToRecordsWithHandler, "auto");
