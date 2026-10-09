#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// CHandleTable::Resolve (__fastcall, ecx = this, edx unused, one stack arg)
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));

// Unnamed callee at 0x491FA0; returns a byte flag in al, takes object in ecx.
CSF_FUNC(char, __fastcall, sub_491FA0, 0x00491FA0, (void* self, void* edx));

// Clears the status byte of the record at a1, then resolves the handle entry
// referenced by a3. If the entry is live (non-null, has a sub-object at +0x34)
// and has type 0x1C, the byte is refreshed from sub_491FA0's result.
static int* __stdcall RefreshFlagFromHandleEntry(int a1, int /*a2*/, int a3)
{
    // a3 -> pointer to an object; that object's +4 field holds a base address; the entry lies at base+4.
    int* outerObject = *reinterpret_cast<int**>(a3);
    int* result = reinterpret_cast<int*>(outerObject[1] + 4);

    // Clear the status byte in the buffer referenced by a1+4.
    uint8_t* statusBuffer = *reinterpret_cast<uint8_t**>(reinterpret_cast<uint8_t*>(a1) + 4);
    statusBuffer[1] = 0;

    if (result[2] >= 0)
    {
        result = reinterpret_cast<int*>(CHandleTable_Resolve(
            g_pCurrentScene, nullptr, static_cast<int>(reinterpret_cast<intptr_t>(result))));

        if (result)
        {
            if (result[13])
            {
                if (result[20] == 28)
                {
                    // Original reloads a1+4 after the Resolve call.
                    uint8_t* statusBufferAfterCall =
                        *reinterpret_cast<uint8_t**>(reinterpret_cast<uint8_t*>(a1) + 4);
                    char refreshedFlag = sub_491FA0(result, nullptr);
                    result = reinterpret_cast<int*>(static_cast<intptr_t>(refreshedFlag));
                    statusBufferAfterCall[1] = static_cast<uint8_t>(refreshedFlag);
                }
            }
        }
    }
    return result;
}

HOOK(0x0045FF70, RefreshFlagFromHandleEntry, "auto");
