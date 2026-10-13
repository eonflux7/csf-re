#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_vector.h"

// Binary-derived vector helper; survey cluster 439, stride 68.
using Value = legacy::vc71::element<68>;
CSF_FUNC(void, __fastcall, Operation, 0x0051BEB0, (Value* self, void* edx));
struct Operations {
    using value_type = Value;
    static __forceinline void destroy(Value* dest) { Operation(dest, nullptr); }
};
static void __stdcall Replica_0051bf40(Value* begin, Value* end) {
    legacy::vc71::vector_destroy_range<Operations>(begin, end);
}

HOOK(0x0051BF40, Replica_0051bf40, "stl");
