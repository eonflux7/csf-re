#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __stdcall, sub_6B4DC0, 0x006B4DC0, (int a1, int a2));

static void*** __fastcall GfxSkin_CopyMatrixBlocksToList(
    S_006c6dc0* self, void* /*edx*/,
    void*** result, void** destination, const void** source)
{
    if (static_cast<const void*>(destination) != static_cast<const void*>(source))
    {
        const void** sourceCursor = source;
        const void** sourceEnd =
            reinterpret_cast<const void**>(self->field_0008);
        void** destinationCursor = destination;

        if (sourceCursor != sourceEnd)
        {
            do
            {
                const void* sourceBlock = *sourceCursor;
                void* destinationBlock = *destinationCursor;
                sourceCursor += 2;
                destinationCursor += 2;
                std::memcpy(destinationBlock, sourceBlock, 0x40);
            } while (sourceCursor != sourceEnd);
        }

        sub_6B4DC0(
            static_cast<int>(reinterpret_cast<uintptr_t>(destinationCursor)),
            static_cast<int>(self->field_0008));
        self->field_0008 =
            static_cast<uint32_t>(reinterpret_cast<uintptr_t>(destinationCursor));
    }

    *result = destination;
    return result;
}

HOOK(0x006C6DC0, GfxSkin_CopyMatrixBlocksToList, "auto");
