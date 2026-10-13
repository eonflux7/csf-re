#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_vector.h"

// Binary-derived vector helper; survey cluster 439, stride 8.
using Value = legacy::vc71::element<8>;
CSF_FUNC(void, __fastcall, Operation, 0x0070CC60, (Value* self, void* edx));
struct Operations {
    using value_type = Value;
    static __forceinline void destroy(Value* dest) { Operation(dest, nullptr); }
};
static void __stdcall Replica_00709ab0(Value* begin, Value* end) {
    legacy::vc71::vector_destroy_range<Operations>(begin, end);
}

HOOK(0x00709AB0, Replica_00709ab0, "stl");
