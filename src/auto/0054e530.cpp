#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Element helper called once per dword in the range loop (both stdcall, first stack arg first).
// The original also loads its 5th argument into ecx before the call; this binding does not take ecx.
CSF_FUNC(std::uint32_t*, __stdcall, CopyDwordIfNotNull,
         0x00730830, (std::uint32_t* dest, const std::uint32_t* src));

// __CxxThrowException@8 - used to rethrow out of the catch(...) block.
CSF_FUNC(void, __stdcall, RethrowCppException,
         0x007AF666, (void* exceptionObject, void* throwInformation));

// Referenced by the SEH frame emitted for this function.
CSF_FUNC(int, __cdecl, ___CxxFrameHandler,
         0x007AF226, (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Exception information referenced by the frame's handler stub (loaded into eax at 0x827080).
CSF_GLOBAL(std::uint8_t, g_DwordRangeExceptionInformation, 0x0088BF04);

// Walks the range [firstBegin, firstEnd) one dword at a time. For each element it calls
// CopyDwordIfNotNull with the current first-range pointer as the first stack argument and the
// current second-range pointer as the second, then advances both pointers by 4.
// The fifth argument is never read by the original. Returns the advanced second pointer.
static int __cdecl CopyDwordRangeAdvancingSecond(
    std::uint32_t firstBegin,
    std::uint32_t firstEnd,
    std::uint32_t secondBegin,
    std::uint32_t /*copyContext*/,
    std::uint32_t /*unused*/)
{
    try
    {
        while (firstBegin != firstEnd)
        {
            CopyDwordIfNotNull(
                reinterpret_cast<std::uint32_t*>(firstBegin),
                reinterpret_cast<const std::uint32_t*>(secondBegin));
            secondBegin += 4u;
            firstBegin += 4u;
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
    }

    return static_cast<int>(secondBegin);
}

HOOK(0x0054E530, CopyDwordRangeAdvancingSecond, "auto");
