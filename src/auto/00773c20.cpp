#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(char*, __stdcall, ApplyToAnimRecord_773300, 0x00773300,
         (char* a1, int a2));

CSF_FUNC(int, __fastcall, Tables_ClearRecordBuffers, 0x0051ABD0,
         (void* self, void*));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Embedded EH handler stub and its FuncInfo; not standalone function starts.
CSF_GLOBAL(std::uint8_t, g_AnimRangeExceptionHandlerCode, 0x00828540);
CSF_GLOBAL(std::uint8_t, g_AnimRangeExceptionFuncInfo, 0x008944A4);

// Clang's x86 address space 257 addresses memory relative to FS.
typedef std::uint32_t
    __attribute__((address_space(257))) FsDword;

// Animation record stride of 0x60 bytes.
static __declspec(noinline) char* __cdecl ApplyToAnimRecordRange(
    std::uint32_t base,
    std::uint32_t count,
    int argument,
    int context)
{
    const std::uint32_t baseBegin = base;
    char* result = nullptr;

    try
    {
        while (count != 0)
        {
            // Callee is __stdcall; the original loads the context into ecx
            // before the call, which this binding does not carry.
            (void)context;
            result = ApplyToAnimRecord_773300(
                reinterpret_cast<char*>(base), argument);

            --count;
            base += 0x60u;
        }
    }
    catch (...)
    {
        std::uint32_t completed = baseBegin;
        while (completed != base)
        {
            Tables_ClearRecordBuffers(
                reinterpret_cast<void*>(completed), nullptr);
            completed += 0x60u;
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return result;
}

static char* __cdecl ApplyToAnimRecords(char* a1, int a2, int a3, int a4)
{
    if (a2 == 0)
    {
        // No EH frame in this wrapper: the original returns the FS:[0]
        // head value it saved in eax during the prologue.
        return reinterpret_cast<char*>(
            static_cast<std::uint32_t>(*static_cast<FsDword*>(nullptr)));
    }

    return ApplyToAnimRecordRange(
        reinterpret_cast<std::uint32_t>(a1),
        static_cast<std::uint32_t>(a2),
        a3,
        a4);
}

HOOK(0x00773C20, ApplyToAnimRecords, "auto");
