#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_list.h"
using List = legacy::vc71::list<uint32_t>;
using Node = legacy::vc71::list_node<uint32_t>;
CSF_FUNC(Node*, __stdcall, Buy, 0x007412D0, (Node* next, Node* prev, const uint32_t* value));
CSF_FUNC(uint32_t, __fastcall, Inc, 0x0073FE80, (List* self, void* edx, uint32_t count));
struct Operations {
    using value_type = uint32_t;
    static __forceinline Node* buy(Node* next, Node* prev, const uint32_t* value) { return Buy(next, prev, value); }
    static __forceinline uint32_t incsize(List* self, uint32_t count) { return Inc(self, nullptr, count); }
};
static uint32_t __fastcall StlListPush_00741de0(List* self, void*, const uint32_t* value) {
    return legacy::vc71::list_push_back<Operations>(self, value);
}
HOOK(0x00741DE0, StlListPush_00741de0, "stl");
