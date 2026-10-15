#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/vc71_string.h"
using String = legacy::vc71::string<uint16_t>;
CSF_FUNC(void, __fastcall, Xran, 0x007AE981, (void* self, void* edx));
CSF_FUNC(void, __fastcall, Xlen, 0x007AE9C1, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, Grow, 0x00486550, (String* self, void* edx, uint32_t size, uint32_t length));
CSF_FUNC(void*, __cdecl, Move, 0x007AEE60, (void* dest, const void* source, uint32_t bytes));
static __forceinline uint16_t* data(String* s) {
    return s->capacity < 8 ? s->storage.buffer : s->storage.heap;
}
static String* __fastcall ReplaceTextRange(String* self, void*, uint32_t pos, uint32_t remove, String* source, uint32_t start, uint32_t count) {
    if (self->length < pos || source->length < start) Xran(self, nullptr);
    if (self->length - pos < remove) remove = self->length - pos;
    if (source->length - start < count) count = source->length - start;
    if (0xFFFFFFFFu - count <= self->length - remove) Xlen(self, nullptr);
    uint32_t tail = self->length - pos - remove;
    uint32_t size = count + self->length - remove;
    if (self->length < size) {
        if (size == 0xFFFFFFFFu) Xlen(self, nullptr);
        if (self->capacity < size) Grow(self, nullptr, size, self->length);
        else if (!size) {
            self->length = 0;
            *data(self) = 0;
        }
    }
    if (self != source) {
        Move(data(self) + pos + count, data(self) + pos + remove, 2 * tail);
        // The non-aliasing source copy is an inline rep movs in the original.
        legacy::vc71::string_copy(data(self) + pos, data(source) + start, 2 * count);
    } else if (count <= remove) {
        Move(data(self) + pos, data(self) + start, 2 * count);
        Move(data(self) + pos + count, data(self) + pos + remove, 2 * tail);
    } else if (start <= pos) {
        Move(data(self) + pos + count, data(self) + pos + remove, 2 * tail);
        Move(data(self) + pos, data(self) + start, 2 * count);
    } else if (pos + remove <= start) {
        Move(data(self) + pos + count, data(self) + pos + remove, 2 * tail);
        Move(data(self) + pos, data(self) + count + start - remove, 2 * count);
    } else {
        Move(data(self) + pos, data(self) + start, 2 * remove);
        Move(data(self) + pos + count, data(self) + pos + remove, 2 * tail);
        Move(data(self) + pos + remove, data(self) + count + start, 2 * (count - remove));
    }
    self->length = size;
    data(self)[size] = 0;
    return self;
}
HOOK(0x00533040, ReplaceTextRange, "stl");
