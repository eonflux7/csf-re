#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Processes one 8-byte record; the context pointer is passed as its second argument.
CSF_FUNC(void, __stdcall, GfxObject_ProcessRecord_6B33A0, 0x006B33A0,
         (int a1, void** a2));

// Destroys one already-processed 8-byte record during unwinding.
CSF_FUNC(int, __stdcall, GfxObject_DestroyRecord_6B3DF0, 0x006B3DF0,
         (int a1));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));
CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Unwind-info data block used by the compiler-generated exception frame.
[[maybe_unused]] static const void* const kGfxObjectUnwindInfo =
    reinterpret_cast<const void*>(CSF_ADDR_DATA(0x0088F870));

// Advances a pointer by one 8-byte record.
static int GfxObject_NextRecord(int record)
{
    return static_cast<int>(reinterpret_cast<std::uintptr_t>(
        reinterpret_cast<std::uint8_t*>(static_cast<std::uintptr_t>(record)) + 8));
}

// Calls GfxObject_ProcessRecord_6B33A0 on each of `recordCount` consecutive 8-byte
// records starting at `firstRecord`, passing `context` along. If a call throws, the
// records already processed are destroyed again and the exception is rethrown.
static void __cdecl GfxObject_ApplyToRecordRange(
    int firstRecord,
    std::uint32_t recordCount,
    const void** context,
    int /*unusedA4*/,
    int /*unusedA5*/)
{
    int currentRecord = firstRecord;

    try
    {
        while (recordCount != 0)
        {
            GfxObject_ProcessRecord_6B33A0(currentRecord, const_cast<void**>(context));
            --recordCount;
            currentRecord = GfxObject_NextRecord(currentRecord);
        }
    }
    catch (...)
    {
        int record = firstRecord;
        while (record != currentRecord)
        {
            GfxObject_DestroyRecord_6B3DF0(record);
            record = GfxObject_NextRecord(record);
        }

        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }
}

HOOK(0x006B3E10, GfxObject_ApplyToRecordRange, "auto");
