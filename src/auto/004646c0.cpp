#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, sub_471540, 0x00471540, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, sub_473490, 0x00473490, (void* self, void* edx, unsigned int a2, int a3));
CSF_FUNC(int, __fastcall, sub_672AC0, 0x00672AC0, (void* self, void* edx));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static unsigned int __fastcall ClampIndexedEntryValue(void* self, void* /*edx*/, int a2, int a3, uint32_t* a4)
{
    (void)a2;
    (void)a3;

    // Owner table selected by self->field_0020 unless the flag bit 0x800000 is set.
    void* entryOwner = CSF_AT(void*, self, 0x20);
    uint32_t entryIndex = CSF_AT(uint32_t, reinterpret_cast<void*>(*a4), 0x08);

    if ((entryIndex & 0x800000u) != 0)
    {
        void* sceneSub = CSF_AT(void*, g_pCurrentScene, 0xBC);
        entryOwner = CSF_AT(void*, sceneSub, 0x38);
        entryIndex &= ~0x800000u;
    }

    // Each table entry is 12 bytes; the object pointer lives at entry+4.
    uint8_t* entry = static_cast<uint8_t*>(CSF_AT(void*, entryOwner, 0x08)) + entryIndex * 12u;

    unsigned int firstValue = static_cast<unsigned int>(sub_471540(CSF_AT(void*, entry, 0x04), nullptr));
    unsigned int result = static_cast<unsigned int>(sub_672AC0(CSF_AT(void*, entry, 0x04), nullptr));

    if (firstValue < result)
    {
        return sub_473490(CSF_AT(void*, entry, 0x04), nullptr, firstValue, static_cast<int>(a4[1]));
    }
    return result;
}

HOOK(0x4646C0, ClampIndexedEntryValue, "auto");
