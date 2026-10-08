#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(int, __fastcall, AdjustTextRangeLeft, 0x007215F0,
         (CGuiEdit* self, void* edx, char mode));
CSF_FUNC(int, __fastcall, AdjustTextRangeRight, 0x00721900,
         (CGuiEdit* self, void* edx, char mode));

// The original callees have an implicit EBX input. Keep this routine
// unoptimized and stack-based so that EBX remains untouched.
static __declspec(noinline) __attribute__((optnone))
int __fastcall CGuiEdit_MoveIndicesRight(CGuiEdit* self, void* /*edx*/)
{
    std::uint32_t length = CSF_AT(std::uint32_t, self, 0x164);
    if (length == 0)
        return 0;

    void* text = reinterpret_cast<void*>(self->m_pText);
    auto query = reinterpret_cast<int(__fastcall*)(void*, void*)>(
        CSF_AT(void**, text, 0)[0x88 / 4]);
    unsigned char textState = static_cast<unsigned char>(query(text, nullptr));

    std::uint32_t rangeState = self->field_01ac;
    length = CSF_AT(std::uint32_t, self, 0x164);

    if (rangeState == 0)
    {
        if (textState == 0)
        {
            std::uint32_t first = self->field_01a8;
            std::uint32_t last = self->field_01b0;

            if (first == last)
            {
                std::uint32_t nextFirst = first + 1u;
                if (nextFirst < length)
                    self->field_01a8 = nextFirst;

                std::uint32_t nextLast = last + 1u;
                if (nextLast < length)
                    self->field_01b0 = nextLast;

                AdjustTextRangeLeft(self, nullptr, 0);
                goto WrapAtEnd;
            }

            std::uint32_t nextFirst = first + 1u;
            if (nextFirst < length)
                self->field_01a8 = nextFirst;
        }

        return AdjustTextRangeLeft(self, nullptr, 0);
    }

    {
        std::uint32_t last = self->field_01b0;
        std::uint32_t first = self->field_01a8;
        std::uint32_t nextLast = last + 1u;

        if (nextLast == length)
        {
            if (first >= last)
                goto WrapAtEnd;
        }
        else if (first == last)
        {
            std::uint32_t nextFirst = first + 1u;
            if (nextFirst < length)
                self->field_01a8 = nextFirst;

            if (nextLast < length)
                self->field_01b0 = nextLast;

            return AdjustTextRangeLeft(self, nullptr, 0);
        }

        std::uint32_t nextFirst = first + 1u;
        if (nextFirst < length)
            self->field_01a8 = nextFirst;

        return AdjustTextRangeLeft(self, nullptr, 0);
    }

WrapAtEnd:
    {
        // Inline continuation at 0x00722530, not a separate function entry.
        std::uint32_t first = self->field_01a8;
        std::uint32_t currentLength = CSF_AT(std::uint32_t, self, 0x164);
        std::uint32_t nextFirst = first + 1u;

        if (nextFirst == currentLength && currentLength != 0)
        {
            self->field_01a8 = 0;
            self->field_01ac = 0;
            return AdjustTextRangeRight(self, nullptr, 1);
        }

        return static_cast<int>(currentLength);
    }
}

HOOK(0x007226E0, CGuiEdit_MoveIndicesRight, "auto");
