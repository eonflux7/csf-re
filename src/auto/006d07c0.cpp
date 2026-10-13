#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_list.h"
using Node = legacy::vc71::list_node<uint32_t>;
static Node* __stdcall StlListBuy_006d07c0(Node* next, Node* prev, const uint32_t* value) {
    return legacy::vc71::list_buy_node(next, prev, value);
}
HOOK(0x006D07C0, StlListBuy_006d07c0, "stl");
