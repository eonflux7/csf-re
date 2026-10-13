#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_list.h"
using Node = legacy::vc71::list_node<uint32_t>;
static Node* __stdcall StlListBuy_00552e60(Node* next, Node* prev, const uint32_t* value) {
    return legacy::vc71::list_buy_node(next, prev, value);
}
HOOK(0x00552E60, StlListBuy_00552e60, "stl");
