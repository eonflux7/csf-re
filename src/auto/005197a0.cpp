#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(unsigned int, __fastcall, SceneEntryList_RemoveValueAndNotify, 0x006B1840, (void* self, void* /*edx*/, int item));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

// Finds `value` in the object's int array (field_0008, count field_0010), removes it by
// shifting the tail down, and notifies the scene entry list. Returns the new count (as a
// pointer-sized value) when nothing was shifted, otherwise the memcpy destination.
static void* __fastcall C_00840d64_RemoveValueFromList(C_00840d64* self, void* /*edx*/, int value)
{
    uint32_t count = self->field_0010;
    uint32_t searchIndex = 0;
    int foundIndex;

    if (count != 0) {
        uint32_t* cursor = reinterpret_cast<uint32_t*>(self->field_0008);
        while (static_cast<int>(*cursor) != value) {
            ++searchIndex;
            ++cursor;
            if (searchIndex >= count) {
                foundIndex = -1;
                goto notFound;
            }
        }
        foundIndex = static_cast<int>(searchIndex);
    } else {
notFound:
        foundIndex = -1;
    }

    // The original pushes `value` before calling GetGlobalInt; that stack slot is consumed
    // by SceneEntryList_RemoveValueAndNotify, so GetGlobalInt is called once and its
    // result becomes the `this` for the notify call.
    void* notifyTarget = reinterpret_cast<void*>(static_cast<std::uintptr_t>(GetGlobalInt()));
    SceneEntryList_RemoveValueAndNotify(notifyTarget, nullptr, value);

    uint32_t newCount = self->field_0010 - 1;
    void* result = reinterpret_cast<void*>(static_cast<std::uintptr_t>(newCount));
    self->field_0010 = newCount;

    if (foundIndex != static_cast<int>(newCount)) {
        uint8_t* base = reinterpret_cast<uint8_t*>(self->field_0008);
        int shiftBytes = 4 * (static_cast<int>(newCount) - foundIndex);
        return _memcpy(base + 4 * foundIndex,
                       base + 4 * foundIndex + 4,
                       static_cast<std::size_t>(shiftBytes));
    }
    return result;
}

HOOK(0x5197A0, C_00840d64_RemoveValueFromList, "auto");
