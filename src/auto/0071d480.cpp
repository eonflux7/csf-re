#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, MenuManager_AppendPageName, 0x00470FD0,
         (void* self, void* edx, void* sourceString));

static int __fastcall C_00864508_AssignStringsForSelectedIds(
    C_00864508* self,
    void* /*edx*/,
    void* destinationString)
{
    std::uint32_t selectedCount = 0;

    const std::uint32_t selectedBegin = self->field_0048;
    if (selectedBegin != 0)
    {
        selectedCount = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(
                self->field_004c - selectedBegin) >> 2);
    }

    for (std::uint32_t selectedIndex = 0;
         selectedIndex < selectedCount;
         ++selectedIndex)
    {
        const std::uint32_t recordsBegin = self->field_0014;

        if (recordsBegin != 0)
        {
            std::uint32_t remaining = static_cast<std::uint32_t>(
                static_cast<std::int32_t>(
                    self->field_0018 - recordsBegin) >> 6);

            if (remaining != 0)
            {
                const std::uint32_t selectedOffset = selectedIndex * 4u;
                std::uint32_t recordOffset = 0;

                do
                {
                    const std::uint32_t recordAddress =
                        self->field_0014 + recordOffset;
                    void* record = reinterpret_cast<void*>(recordAddress);

                    const std::uint32_t recordId =
                        CSF_AT(std::uint32_t, record, 0x38);
                    const std::uint32_t selectedAddress =
                        self->field_0048 + selectedOffset;

                    if (recordId == CSF_AT(
                            std::uint32_t,
                            reinterpret_cast<void*>(selectedAddress), 0))
                    {
                        MenuManager_AppendPageName(
                            destinationString, nullptr,
                            reinterpret_cast<void*>(recordAddress + 0x1Cu));
                    }

                    recordOffset += 0x40u;
                    --remaining;
                }
                while (remaining != 0);
            }
        }
    }

    return static_cast<int>(selectedCount);
}

HOOK(0x0071D480, C_00864508_AssignStringsForSelectedIds, "auto");
