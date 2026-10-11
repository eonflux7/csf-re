#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Chain: if enabled flag at +0x0C is set, take the object at +0x04, call its
// slot 10 getter, then slot 28 on the result, then slot 10 on that result.
// Returns the low byte of the final value (or of the flag when it is clear).
static char __fastcall C_0082e520_UpdateFromEvent(C_0082e520* self, void* /*edx*/, int /*a2*/)
{
    char result = static_cast<char>(self->field_000c);
    if (result != 0) {
        void* obj = reinterpret_cast<void*>(self->field_0004);
        void* first = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, obj, 0)[0x28 / 4])(obj, nullptr);
        if (first != nullptr) {
            void* second = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, first, 0)[0x70 / 4])(first, nullptr);
            if (second != nullptr) {
                result = static_cast<char>(
                    reinterpret_cast<uintptr_t>(
                        ((void* (__fastcall*)(void*, void*))CSF_AT(void**, second, 0)[0x28 / 4])(second, nullptr)));
            }
        }
    }
    return result;
}

HOOK(0x0041E090, C_0082e520_UpdateFromEvent, "auto");
