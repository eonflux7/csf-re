#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(char, __fastcall, BlockList_TryHandle, 0x0069B590, (void* self, void* edx, unsigned int a2));

CSF_GLOBAL(S_0069b1c0, g_BlockList0, 0x008B032C);
CSF_GLOBAL(S_0069b1c0, g_BlockList1, 0x008B0370);
CSF_GLOBAL(S_0069b1c0, g_BlockList2, 0x008B03B4);
CSF_GLOBAL(S_0069b1c0, g_BlockList3, 0x008B03F8);
CSF_GLOBAL(S_0069b1c0, g_BlockList4, 0x008B02E8);

static void __fastcall S_0069b1c0_PrepareStateRecord(
    S_0069b1c0* self, void* /*edx*/)
{
    self->field_0008 = 0;

    const uint32_t begin = self->field_0010;
    const uint32_t count = begin
        ? static_cast<uint32_t>(
              static_cast<int32_t>(CSF_AT(uint32_t, self, 0x14) - begin) >> 2)
        : 0;

    for (uint32_t index = 0; index < count; ++index)
    {
        void* object = CSF_AT(
            void*, reinterpret_cast<void*>(self->field_0010), 4 * index);

        if (object)
        {
            ((void(__fastcall*)(void*, void*, uint32_t))
                CSF_AT(void**, object, 0)[0])(object, nullptr, 0);

            const unsigned int block = reinterpret_cast<unsigned int>(object);
            if (!BlockList_TryHandle(&g_BlockList0, nullptr, block)
                && !BlockList_TryHandle(&g_BlockList1, nullptr, block)
                && !BlockList_TryHandle(&g_BlockList2, nullptr, block)
                && !BlockList_TryHandle(&g_BlockList3, nullptr, block)
                && !BlockList_TryHandle(&g_BlockList4, nullptr, block))
            {
                Mem_FreeDebug(object);
            }
        }

        CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0010),
               4 * index) = 0;
    }
}

HOOK(0x00471E00, S_0069b1c0_PrepareStateRecord, "auto");
