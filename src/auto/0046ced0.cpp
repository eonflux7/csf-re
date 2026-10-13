#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_vector.h"
// Binary-derived _Insert_n, cluster None; 8-byte elements.
using Value = legacy::vc71::element<8>;
using Vector = legacy::vc71::vector<Value>;
CSF_FUNC(uint32_t, __fastcall, CopyNew, 0x0046C440, (Vector* self, void* edx, uint32_t begin, uint32_t end, uint32_t dest));
CSF_FUNC(uint32_t, __fastcall, Fill, 0x0046C5F0, (Vector* self, void* edx, uint32_t dest, uint32_t count, const Value* value));
CSF_FUNC(uint32_t, __cdecl, CopySpare, 0x0046C0C0, (uint32_t begin, uint32_t end, uint32_t dest, Vector* context, uint32_t tag));
CSF_FUNC(uint32_t, __cdecl, CopyBackward, 0x004DC700, (uint32_t begin, uint32_t end, uint32_t dest));
CSF_FUNC(uint32_t, __cdecl, Assign, 0x0070CE20, (uint32_t begin, uint32_t end, const Value* value));
CSF_FUNC(uint32_t, __cdecl, ReleaseObject, 0x004714E0, (void* object));
CSF_FUNC(uint32_t, __cdecl, DestroyOld, 0x0046C190, (uint32_t begin, uint32_t end, Vector* context, uint32_t tag));
CSF_FUNC(uint32_t, __fastcall, DestroyFailed, 0x0046C5D0, (Vector* self, void* edx, uint32_t begin, uint32_t end));
struct Operations {
    using value_type = Value;
    static constexpr bool owns_value = false;
    static __forceinline void construct(Value* dest, const Value* source) { *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(dest) + 0) = *reinterpret_cast<const uint32_t*>(reinterpret_cast<const uint8_t*>(source) + 0);
        *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(dest) + 4) = *reinterpret_cast<const uint32_t*>(reinterpret_cast<const uint8_t*>(source) + 4); }
    static __forceinline uint32_t release_value(Value*) { return 0; }
    static __forceinline uint32_t copy_new(Vector* self, uint32_t begin, uint32_t end, uint32_t dest) { return CopyNew(self, nullptr, begin, end, dest); }
    static __forceinline uint32_t copy_spare(Vector* self, uint32_t begin, uint32_t end, uint32_t dest) { return CopySpare(begin, end, dest, self, 0); }
    static __forceinline uint32_t fill(Vector* self, uint32_t dest, uint32_t count, const Value* value) { return Fill(self, nullptr, dest, count, value); }
    static __forceinline uint32_t copy_backward(uint32_t begin, uint32_t end, uint32_t dest) { return CopyBackward(begin, end, dest); }
    static __forceinline uint32_t assign(uint32_t begin, uint32_t end, const Value* value) { return Assign(begin, end, value); }
    static __forceinline void destroy_old(Vector* self, uint32_t begin, uint32_t end) { DestroyOld(begin, end, self, 0); }
    static __forceinline void destroy_failed(Vector* self, uint32_t begin, uint32_t end) { DestroyFailed(self, nullptr, begin, end); }
    static __forceinline uint32_t finish_value(Value* value, uint32_t result) { uint32_t eax = result;
        auto* words = reinterpret_cast<uint32_t*>(value);
        auto* owner = legacy::vc71::pointer<uint32_t>(words[1]);
        if (!owner) return eax;
        using Destructor = uint32_t (__fastcall*)(void*, void*, uint32_t);
auto* table = *reinterpret_cast<void***>(owner);
        reinterpret_cast<Destructor>(table[0])(owner, nullptr, 0);
        return ReleaseObject(owner);
    }
};
static uint32_t __fastcall StlInsert_0046ced0(Vector* self, void*, Value* pos, uint32_t count, const Value* value) {
    return legacy::vc71::vector_insert_n<Operations>(self, pos, count, value);
}
HOOK(0x0046CED0, StlInsert_0046ced0, "stl");
