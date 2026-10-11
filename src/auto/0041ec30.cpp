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

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(unsigned int*, __fastcall, sub_6AF390, 0x006AF390, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, C_0082e664_StoreAllocatedSlot, 0x0044DCE0, (void* self, void* edx, unsigned int a2));

static uint32_t* __fastcall C_0082e664_TakeFromFreeListOrAllocate(C_0082e664* self, void* /*edx*/)
{
    uint32_t freeCount = self->field_001c;
    uint32_t* result = nullptr;

    if (freeCount != 0)
    {
        result = reinterpret_cast<uint32_t*>(
            reinterpret_cast<uint32_t*>(self->field_0014)[freeCount - 1]);
        --self->field_001c;
    }

    if (result == nullptr)
    {
        void* raw = Mem_Alloc(104);
        if (raw != nullptr)
        {
            uint32_t* newObject = static_cast<uint32_t*>(raw);
            *reinterpret_cast<void**>(newObject) = CSF_VTBL(0x0082E684);
            sub_6AF390(reinterpret_cast<uint8_t*>(newObject) + 4, nullptr);
            C_0082e664_StoreAllocatedSlot(self, nullptr, reinterpret_cast<unsigned int>(newObject));
            return newObject;
        }
        C_0082e664_StoreAllocatedSlot(self, nullptr, 0);
        return nullptr;
    }

    return result;
}

HOOK(0x41EC30, C_0082e664_TakeFromFreeListOrAllocate, "auto");
