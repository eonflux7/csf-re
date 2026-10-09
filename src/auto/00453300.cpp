#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(uint32_t*, __stdcall, sub_4526B0, 0x004526B0, (uint32_t* a1, uint32_t* a2));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666, (void* exceptionObject, void* throwInformation));

// Unwind cleanup: stamps the C_0082d410 vtable into the 24-byte elements
// that were already processed when the loop was interrupted by an exception.
static void ResetSpanVtables(uint32_t* begin, uint32_t* end)
{
    uint32_t vtbl = (uint32_t)(uintptr_t)CSF_VTBL(0x0082D410);
    for (uint32_t* element = begin; element != end; element += 6)
    {
        element[3] = vtbl;
        element[0] = vtbl;
    }
}

static void RunStepOverElements(uint32_t* elements, int count, uint32_t* argument, void* context)
{
    (void)context;
    uint32_t* cursor = elements;
    uint32_t* first = elements;
    try
    {
        while (count != 0)
        {
            sub_4526B0(cursor, argument);
            --count;
            cursor += 6;
        }
    }
    catch (...)
    {
        ResetSpanVtables(first, cursor);
        RethrowCppException(nullptr, nullptr);
    }
}

HOOK(0x00453300, RunStepOverElements, "auto");
