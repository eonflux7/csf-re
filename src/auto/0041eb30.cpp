#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ButtonList_InsertAt, 0x0041E510, (void* self, void* edx, int a2, uint32_t* a3));
CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(int, __fastcall, ButtonList_FindInsertIndex, 0x0041E660, (void* self, void* edx, unsigned int a2));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(unsigned int*, __fastcall, ButtonItem_InitBase, 0x006AF390, (void* self, void* edx));

static unsigned int __fastcall C_0082e6a0_EnsureButtonCount(C_0082e6a0* self, void* /*edx*/, unsigned int count)
{
    unsigned int result = self->field_000c;
    unsigned int oldCount = result;

    if (count > result) {
        uint8_t* arrA = reinterpret_cast<uint8_t*>(self) + 4;
        if (count > self->field_0008) {
            GrowButtonArray(arrA, nullptr, (int)(count - self->field_0008));
        }
        uint8_t* arrB = reinterpret_cast<uint8_t*>(self) + 0x14;
        unsigned int capB = CSF_AT(unsigned int, self, 0x18);
        if (count > capB) {
            GrowButtonArray(arrB, nullptr, (int)(count - capB));
        }
        result = oldCount;
        if (oldCount < count) {
            unsigned int remaining = count - oldCount;
            do {
                void* obj = Mem_Alloc(0x68);
                if (obj) {
                    *reinterpret_cast<void***>(obj) = CSF_VTBL(0x0082E684);
                    ButtonItem_InitBase(reinterpret_cast<uint8_t*>(obj) + 4, nullptr);
                } else {
                    obj = nullptr;
                }
                uint32_t item = (uint32_t)(uintptr_t)obj;
                // stack block: [ebp-8] holds the item pointer, [ebp-4] holds this
                uint32_t slotBlock[2];
                slotBlock[0] = item;
                slotBlock[1] = (uint32_t)(uintptr_t)self;
                int index = ButtonList_FindInsertIndex(self, nullptr, item);
                if ((unsigned int)index >= self->field_000c) {
                    if (CSF_AT(unsigned int, arrA, 8) >= CSF_AT(unsigned int, arrA, 4)) {
                        GrowButtonArray(arrA, nullptr, CSF_AT(int, arrA, 0xC));
                    }
                    unsigned int n = CSF_AT(unsigned int, arrA, 8);
                    uint32_t* dst = reinterpret_cast<uint32_t*>(CSF_AT(uint8_t*, arrA, 0)) + n;
                    CSF_AT(unsigned int, arrA, 8) = n + 1;
                    *dst = item;
                } else {
                    ButtonList_InsertAt(arrA, nullptr, index, slotBlock);
                }
                if (CSF_AT(unsigned int, arrB, 8) >= CSF_AT(unsigned int, arrB, 4)) {
                    GrowButtonArray(arrB, nullptr, CSF_AT(int, arrB, 0xC));
                }
                unsigned int m = CSF_AT(unsigned int, arrB, 8);
                uint32_t* dstB = reinterpret_cast<uint32_t*>(CSF_AT(uint8_t*, arrB, 0)) + m;
                CSF_AT(unsigned int, arrB, 8) = m + 1;
                result = remaining - 1;
                *dstB = item;
                --remaining;
            } while (remaining != 0);
        }
    }
    return result;
}

HOOK(0x0041EB30, C_0082e6a0_EnsureButtonCount, "auto");
