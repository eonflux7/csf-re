#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ScriptRecords_InsertIntoList, 0x0046A9F0, (void* self, void* edx, int a1, unsigned int a3, int arg8));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int*, __fastcall, ScriptRecords_EraseRange, 0x0076A4F0, (void* self, void* edx, int* a2, int a3, int a4));

static char __fastcall ScriptRecords_Resize(S_0076a4f0* self, void* /*edx*/, unsigned int newCount, char fillValue,
                                            int a4, int a5, int a6, int a7, int a8, int a9,
                                            void* block, int a11, int a12, int a13, int a14,
                                            unsigned int capacity, int a16, int a17, int a18, int a19)
{
    uint32_t begin = self->field_0004;
    unsigned int count;
    if (begin)
        count = (int)(self->field_0008 - begin) / 68;
    else
        count = 0;

    if (count >= newCount)
    {
        if (begin)
        {
            uint32_t end = self->field_0008;
            count = (int)(end - begin) / 68;
            if (newCount < count)
            {
                count = (unsigned int)(uintptr_t)ScriptRecords_EraseRange(
                    self, nullptr, (int*)&newCount, (int)(begin + 68 * newCount), (int)self->field_0008);
            }
        }
    }
    else
    {
        int cur;
        if (begin)
            cur = (int)(self->field_0008 - begin) / 68;
        else
            cur = 0;
        count = (unsigned int)ScriptRecords_InsertIntoList(self, nullptr, (int)self->field_0008,
                                                           newCount - cur, (int)&fillValue);
    }

    if (capacity >= 0x10)
    {
        if (block)
            count = (unsigned int)Mem_FreeDebug(block);
    }
    return (char)count;
}

HOOK(0x007702E0, ScriptRecords_Resize, "auto");
