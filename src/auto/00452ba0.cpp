#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(int, __fastcall, GuiScene_FindControlById, 0x00452A30, (void* self, void* edx, unsigned int a2));
CSF_FUNC(int, __fastcall, GuiControl_SetChildEnabled, 0x0048EF60, (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, GuiControl_GetChildByIndex, 0x004A0F00, (void* self, void* edx, unsigned int a2));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

// Packet-required binding; the disassembly contains no vtable store.
static const auto g_ButtonArrayVtable = CSF_VTBL(0x0041E5F0);

static char __fastcall CGuiControl_SetControlIdInactive(
    CGuiControl* self, void* /*edx*/, unsigned int controlId, char inactive)
{
    const uint32_t count = self->m_inputState;
    unsigned int index = 0;
    bool found = false;

    if (count != 0)
    {
        const auto* ids = reinterpret_cast<const unsigned int*>(self->field_00d8);
        while (index < count)
        {
            if (ids[index] == controlId)
            {
                found = true;
                break;
            }
            ++index;
        }
    }

    if (!found || index == static_cast<unsigned int>(-1))
    {
        if (inactive)
            return inactive;

        if (self->m_inputState >= CSF_AT(uint32_t, self, 0xDC))
        {
            GrowButtonArray(
                reinterpret_cast<uint8_t*>(self) + 0xD8,
                nullptr,
                CSF_AT(int, self, 0xE4));
        }

        const uint32_t insertionIndex = self->m_inputState;
        auto* destination =
            reinterpret_cast<unsigned int*>(self->field_00d8) + insertionIndex;
        self->m_inputState = insertionIndex + 1;
        *destination = controlId;
    }
    else
    {
        if (!inactive)
            return static_cast<char>(index);

        const uint32_t remainingCount = self->m_inputState - 1;
        self->m_inputState = remainingCount;
        if (index != remainingCount)
        {
            auto* destination =
                reinterpret_cast<unsigned int*>(self->field_00d8) + index;
            _memcpy(destination, destination + 1, 4 * (remainingCount - index));
        }
    }

    int result = GuiScene_FindControlById(self, nullptr, controlId);
    if (result != 0)
    {
        void* control = reinterpret_cast<void*>(result);
        const unsigned int childCount = CSF_AT(unsigned int, control, 0x0C);
        for (unsigned int childIndex = 0; childIndex < childCount; ++childIndex)
        {
            result = GuiControl_GetChildByIndex(control, nullptr, childIndex);
            if (result != 0)
            {
                result = GuiControl_SetChildEnabled(
                    reinterpret_cast<void*>(result),
                    nullptr,
                    static_cast<char>(inactive == 0));
            }
        }
    }

    return static_cast<char>(result);
}

HOOK(0x00452BA0, CGuiControl_SetControlIdInactive, "auto");
