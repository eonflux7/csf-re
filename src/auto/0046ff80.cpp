#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_vector.h"

using BoolVector = legacy::vc71::vector_bool;
CSF_FUNC(void, __fastcall, LengthError, 0x0046FAC0, (BoolVector* self, void* edx));
CSF_FUNC(uint32_t, __cdecl, CopyBytes, 0x007AEE60, (uint32_t dest, uint32_t source, uint32_t bytes));
struct Operations {
    static __forceinline void length_error(BoolVector* self) { LengthError(self, nullptr); }
    static __forceinline uint32_t copy(uint32_t dest, uint32_t source, uint32_t bytes) {
        return CopyBytes(dest, source, bytes);
    }
};
static void __fastcall Replica_0046ff80(legacy::vc71::vector_bool* self, void*, uint32_t count) {
    legacy::vc71::vector_bool_trim<Operations>(self, count);
}

HOOK(0x0046FF80, Replica_0046ff80, "stl");
