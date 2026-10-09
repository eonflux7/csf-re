#include <cstddef>
#include <cstdint>

#include "globals.h"
#include "hook.h"

// Per-record callback. Called with ecx = owner (this), two stack args.
CSF_FUNC(int, __fastcall, ProcessRecordCallback_778120, 0x00778120,
         (void* self, void* edx, void* record, int argument));

// Per-record destructor, called with ecx = record.
CSF_FUNC(char, __fastcall, DestroyRecord_778140, 0x00778140,
         (void* self, void* edx));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));

// Embedded EH entry and its funcinfo data; not standalone function starts.
CSF_GLOBAL(std::uint8_t, g_RecordRangeExceptionHandlerCode, 0x00828660);
CSF_GLOBAL(std::uint8_t, g_RecordRangeExceptionMetadata, 0x00894C38);

// Clang's x86 address space 257 addresses memory relative to FS.
typedef volatile std::uint32_t
    __attribute__((address_space(257))) FsDword;

static int __cdecl ProcessRecordRange(
    std::uint8_t* firstRecord,
    std::uint32_t recordCount,
    int argument,
    void* owner,
    [[maybe_unused]] int unusedArg)
{
    // eax keeps the previous FS:[0] value when the loop never runs.
    const int previousHandler =
        static_cast<int>(*static_cast<FsDword*>(nullptr));

    std::uint8_t* currentRecord = firstRecord;
    int result = previousHandler;

    try
    {
        while (recordCount != 0)
        {
            result = ProcessRecordCallback_778120(
                owner, nullptr, currentRecord, argument);

            --recordCount;
            currentRecord += 0x48;
        }
    }
    catch (...)
    {
        // Destroy every record that was fully processed before the throw.
        std::uint8_t* completedRecord = firstRecord;
        while (completedRecord != currentRecord)
        {
            DestroyRecord_778140(completedRecord, nullptr);
            completedRecord += 0x48;
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return result;
}

HOOK(0x00778B20, ProcessRecordRange, "auto");
