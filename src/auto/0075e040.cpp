#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_vector.h"

// Binary-derived vector helper; survey cluster 439, stride 72.
using Value = legacy::vc71::element<72>;
CSF_FUNC(void, __fastcall, Operation, 0x0075D430, (Value* self, void* edx));
struct Operations {
    using value_type = Value;
    static __forceinline void destroy(Value* dest) { Operation(dest, nullptr); }
};
static void __stdcall Replica_0075e040(Value* begin, Value* end) {
    legacy::vc71::vector_destroy_range<Operations>(begin, end);
}

HOOK(0x0075E040, Replica_0075e040, "stl");
