#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __stdcall, ProcessSaveRecord_512A80, 0x00512A80,
         (void* a1, void* a2));

CSF_FUNC(unsigned int, __fastcall, SaveGame_DestroyRecord, 0x00500A80,
         (std::uint32_t* self, void*));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

// FuncInfo for the EH frame of the original; not a standalone function start.
CSF_GLOBAL(std::uint8_t, g_SaveRecordRangeFuncInfo, 0x0088AD78);

// Clang's x86 address space 257 addresses memory relative to FS.
typedef std::uint32_t
    __attribute__((address_space(257))) FsDword;

// Record stride of 0xC8 bytes.
static __declspec(noinline) int __cdecl ProcessSaveRecordRange(
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
            result = ProcessSaveRecord_512A80(
                reinterpret_cast<void*>(base),
                reinterpret_cast<void*>(static_cast<std::uintptr_t>(argument)));

            --count;
            base += 0xC8u;
        }
    }
    catch (...)
    {
        std::uint32_t completed = baseBegin;
        while (completed != base)
        {
            SaveGame_DestroyRecord(reinterpret_cast<std::uint32_t*>(completed), nullptr);
            completed += 0xC8u;
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return result;
}

static int __cdecl ProcessSaveRecordsWithHandler(
    std::uint32_t base,
    std::uint32_t count,
    int argument,
    int context)
{
    if (count == 0)
    {
        // No loop iterations: the original returns the FS:[0] head value
        // it saved in eax during the prologue.
        return static_cast<int>(*static_cast<FsDword*>(nullptr));
    }

    // The context argument is loaded into ecx for the callee, which is
    // __stdcall and ignores it.
    (void)context;
    return ProcessSaveRecordRange(base, count, argument);
}

HOOK(0x00512E90, ProcessSaveRecordsWithHandler, "auto");
