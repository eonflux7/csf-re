#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_vector.h"

using BoolVector = legacy::vc71::vector_bool;
using WordVector = legacy::vc71::vector<uint32_t>;
CSF_FUNC(void, __fastcall, LengthError, 0x0046FAC0, (BoolVector* self, void* edx));
CSF_FUNC(void, __fastcall, Resize, 0x0046D800, (WordVector* self, void* edx, uint32_t count, uint32_t fill));
CSF_FUNC(void, __cdecl, MoveBits, 0x0046F680, (void* result, uint32_t begin_offset, uint32_t* begin_word, uint32_t end_offset, uint32_t* end_word, uint32_t dest_offset, uint32_t* dest_word));
struct Operations {
    static __forceinline void length_error(BoolVector* self) { LengthError(self, nullptr); }
    static __forceinline void resize(WordVector* self, uint32_t count, uint32_t fill) { Resize(self, nullptr, count, fill); }
    static __forceinline void move(void* result, uint32_t begin_offset, uint32_t* begin_word, uint32_t end_offset, uint32_t* end_word, uint32_t dest_offset, uint32_t* dest_word) {
        MoveBits(result, begin_offset, begin_word, end_offset, end_word, dest_offset, dest_word);
    }
};
static uint32_t __fastcall Replica_004705c0(BoolVector* self, void*, uint32_t offset, uint32_t* word, uint32_t count) {
    return legacy::vc71::vector_bool_insert<Operations>(self, offset, word, count);
}

HOOK(0x004705C0, Replica_004705c0, "stl");
