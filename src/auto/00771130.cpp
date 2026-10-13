#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(char*, __stdcall, SoundArea_ConfigureAreaRecord, 0x00770F80,
         (char* a1, int a2));

CSF_FUNC(int, __fastcall, TextDatabase_ResetBuffers, 0x004EFD10,
         (std::uint8_t* self, void* edx));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Embedded EH handler stub and its FuncInfo; not standalone function starts.
CSF_GLOBAL(std::uint8_t, g_AreaRangeExceptionHandlerCode, 0x00828460);
CSF_GLOBAL(std::uint8_t, g_AreaRangeExceptionFuncInfo, 0x00893ED4);

// Clang's x86 address space 257 addresses memory relative to FS.
typedef std::uint32_t
    __attribute__((address_space(257))) FsDword;

// Area record stride of 0x68 bytes.
static __declspec(noinline) std::uint32_t* __cdecl ProcessAreaRange(
    std::uint8_t* a1,
    std::uint32_t a2,
    int a3,
    int a4)
{
    std::uint8_t* current = a1;
    std::uint32_t remaining = a2;
    char* result = nullptr;

    try
    {
        while (remaining != 0)
        {
            // Callee is __stdcall; ecx carries the context value and is ignored.
            (void)a4;
            result = SoundArea_ConfigureAreaRecord(
                reinterpret_cast<char*>(current), a3);

            --remaining;
            current += 0x68u;
        }
    }
    catch (...)
    {
        std::uint8_t* completed = a1;
        while (completed != current)
        {
            TextDatabase_ResetBuffers(completed, nullptr);
            completed += 0x68u;
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return reinterpret_cast<std::uint32_t*>(result);
}

static std::uint32_t* __cdecl SoundArea_ProcessAreaRange(
    std::uint8_t* a1,
    std::uint32_t a2,
    int a3,
    int a4)
{
    if (a2 == 0)
    {
        // No calls on this path: the original returns the FS:[0] head
        // value it saved in eax during the prologue.
        return reinterpret_cast<std::uint32_t*>(
            static_cast<std::uintptr_t>(*static_cast<FsDword*>(nullptr)));
    }

    return ProcessAreaRange(a1, a2, a3, a4);
}

HOOK(0x00771130, SoundArea_ProcessAreaRange, "auto");
