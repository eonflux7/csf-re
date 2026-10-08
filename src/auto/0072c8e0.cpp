#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(unsigned int, __fastcall, PointerVector_Append, 0x0072C780,
         (void* self, void* edx, uint32_t* value));
CSF_FUNC(unsigned int, __fastcall, PointerVector_InsertCopies, 0x0072C170,
         (void* self, void* edx, uint32_t* position,
          unsigned int count, uint32_t* value));
CSF_FUNC(int, __fastcall, PointerVector_Assign, 0x0072C3D0,
         (void* self, void* edx, void* source));
CSF_FUNC(unsigned int, __fastcall, PointerVector_InsertRange, 0x0072BEE0,
         (void* self, void* edx, uint32_t* position,
          uint32_t* first, uint32_t* last, CGuiControl* preferredControl));

static void* __fastcall CGuiPanel_ReorderControls(
    CGuiPanel* self, void* /*edx*/, CGuiControl* preferredControl)
{
    // Each temporary includes an untouched leading allocator word.
    uint32_t inactive[4];
    uint32_t active[4];
    inactive[1] = inactive[2] = inactive[3] = 0;
    active[1] = active[2] = active[3] = 0;

    uint32_t cachedActiveEnd = 0;
    uint32_t* inactiveBegin = nullptr;
    const uint32_t originalBegin = self->field_0138;
    const uint32_t count = originalBegin
        ? static_cast<uint32_t>(
              static_cast<int32_t>(self->field_013c - originalBegin) >> 2)
        : 0;

    for (uint32_t index = 0; index < count; ++index)
    {
        void* child = reinterpret_cast<void*>(
            reinterpret_cast<uint32_t*>(self->field_0138)[index]);

        const uint8_t isActive =
            reinterpret_cast<uint8_t(__fastcall*)(void*, void*)>(
                CSF_AT(void**, child, 0)[0xCC / 4])(child, nullptr);

        // The virtual call can change the panel's storage.
        uint32_t* entry =
            reinterpret_cast<uint32_t*>(self->field_0138 + index * 4u);

        if (!isActive)
        {
            PointerVector_Append(inactive, nullptr, entry);
        }
        else
        {
            bool insertAtBeginning = false;
            uint32_t activeBegin = 0;

            if (*entry == reinterpret_cast<uintptr_t>(preferredControl))
            {
                activeBegin = active[1];
                if (activeBegin)
                {
                    insertAtBeginning =
                        (static_cast<int32_t>(
                             cachedActiveEnd - activeBegin) >> 2) != 0;
                }
            }

            if (insertAtBeginning)
            {
                PointerVector_InsertCopies(
                    active, nullptr,
                    reinterpret_cast<uint32_t*>(activeBegin), 1u, entry);
            }
            else
            {
                PointerVector_Append(active, nullptr, entry);
            }
            cachedActiveEnd = active[2];
        }
    }

    if (count)
        inactiveBegin = reinterpret_cast<uint32_t*>(inactive[1]);

    const uint32_t oldStorage = self->field_0138;
    void* controlVector = reinterpret_cast<uint8_t*>(self) + 0x134;
    if (oldStorage)
        Mem_FreeDebug(reinterpret_cast<void*>(oldStorage));

    self->field_0138 = 0;
    self->field_013c = 0;
    CSF_AT(uint32_t, self, 0x140) = 0;

    PointerVector_Assign(controlVector, nullptr, active);

    uint32_t* inactiveEnd = reinterpret_cast<uint32_t*>(inactive[2]);
    uint32_t* destinationEnd =
        reinterpret_cast<uint32_t*>(self->field_013c);
    PointerVector_InsertRange(
        controlVector, nullptr, destinationEnd,
        inactiveBegin, inactiveEnd, preferredControl);

    if (inactiveBegin)
        Mem_FreeDebug(inactiveBegin);

    void* activeStorage = reinterpret_cast<void*>(active[1]);
    if (activeStorage)
    {
        return reinterpret_cast<void*>(
            static_cast<uintptr_t>(Mem_FreeDebug(activeStorage)));
    }
    return activeStorage;
}

HOOK(0x0072C8E0, CGuiPanel_ReorderControls, "auto");
