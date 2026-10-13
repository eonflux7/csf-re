#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Frees every non-null 16-byte element's first dword (last to first), then the
// count-prefixed header block, and resets the array pointer to null.
static int __fastcall AtomicEmitterArray_FreeElementsAndHeader(int* self, void* /*edx*/)
{
    int data = self[0];
    int result = 0;
    self[1] = 0;
    self[2] = 0;
    if (data)
    {
        int count = *reinterpret_cast<int*>(data - 4);
        int headerAddr = data - 4;
        int* element = reinterpret_cast<int*>(data + 16 * count);
        int remaining = count - 1;
        if (remaining >= 0)
        {
            int iterations = remaining + 1;
            do
            {
                int elementValue = *(element - 4);
                element -= 4;
                if (elementValue)
                {
                    CoreMemory_Free(reinterpret_cast<void*>(elementValue));
                    *element = 0;
                }
                --iterations;
            } while (iterations);
        }
        result = CoreMemory_Free(reinterpret_cast<void*>(headerAddr));
        self[0] = 0;
    }
    return result;
}

HOOK(0x00703600, AtomicEmitterArray_FreeElementsAndHeader, "auto");
