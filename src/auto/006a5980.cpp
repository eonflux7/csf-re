#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Callee bound by address: 0x006A6110 (flushes one pending log record).
CSF_FUNC(int, __cdecl, LogSink_FlushRecord, 0x006A6110, (int a1, int a2));

// Callback stored at owner+0x24 (cdecl, two args); owner+0x28 is its context.
typedef void (__cdecl* SinkCallback)(uint32_t context, uint32_t value);
// Init routine stored at a1[14]: cdecl, three args (all passed as 0), returns a dword.
typedef uint32_t (__cdecl* SinkInitFn)(uint32_t, uint32_t, uint32_t);

// Invokes owner's callback (re-reads fn and context from the owner each time, as the original does).
static void InvokeSinkCallback(uint8_t* owner, uint32_t value)
{
    SinkCallback callback = reinterpret_cast<SinkCallback>(CSF_AT(uint32_t, owner, 0x24));
    callback(CSF_AT(uint32_t, owner, 0x28), value);
}

// Resets a log sink record and notifies its owner's callback.
// a1: record (type at [0], fields at [1], [3], [9], [10], [12]-[15]); a2: owner with callback table.
static int __cdecl LogSink_ResetAndNotify(uint32_t* a1, int a2)
{
    uint8_t* owner = reinterpret_cast<uint8_t*>(a2);

    if (a1[0] == 4 || a1[0] == 5)
        InvokeSinkCallback(owner, a1[3]);

    if (a1[0] == 6)
        LogSink_FlushRecord(static_cast<int>(a1[1]), a2);

    uint32_t initialValue = a1[10];
    a1[13] = initialValue;
    a1[12] = initialValue;

    SinkInitFn initializer = reinterpret_cast<SinkInitFn>(a1[14]);
    a1[0] = 0;
    a1[7] = 0;
    a1[8] = 0;

    if (initializer)
    {
        uint32_t result = initializer(0, 0, 0);
        a1[15] = result;
        CSF_AT(uint32_t, owner, 0x30) = result;
    }

    InvokeSinkCallback(owner, a1[10]);
    InvokeSinkCallback(owner, a1[9]);
    InvokeSinkCallback(owner, static_cast<uint32_t>(reinterpret_cast<uintptr_t>(a1)));

    return 0;
}

HOOK(0x006A5980, LogSink_ResetAndNotify, "auto");
