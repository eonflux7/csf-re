#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_list.h"
using List = legacy::vc71::list<uint32_t>;
using Node = legacy::vc71::list_node<uint32_t>;
CSF_GLOBAL(uint32_t, Selection, 0x008B0694);
CSF_GLOBAL(float, Zero, 0x0082D4CC);
CSF_FUNC(Node*, __cdecl, Sentinel, 0x00552DD0, ());
CSF_FUNC(uint32_t, __cdecl, Global, 0x0073C020, ());
CSF_FUNC(void, __stdcall, Enumerate, 0x0073F3B0, (List* list));
CSF_FUNC(int32_t, __cdecl, Compare, 0x00695760, (const void* a, const void* b));
CSF_FUNC(Node*, __stdcall, Buy, 0x00552E60, (Node* next, Node* prev, const uint32_t* value));
CSF_FUNC(uint32_t, __fastcall, Inc, 0x0073FE80, (List* self, void* edx, uint32_t count));
CSF_FUNC(uint32_t, __cdecl, Free, 0x00690E60, (void* block));
CSF_FUNC(uint32_t, __fastcall, SetChanged, 0x00740FF0, (void* self, void* edx, uint32_t index));
CSF_FUNC(uint32_t, __fastcall, SetIndex, 0x007450D0, (void* self, void* edx, uint32_t index));
static void __fastcall UpdateSpawnState(uint8_t* self, void*) {
    uint32_t owner = 0;
    if (Selection) {
        uint32_t member = *reinterpret_cast<uint32_t*>(Selection + 4);
        if (member) owner = member - 776;
    }
    void* interface = reinterpret_cast<void*>(owner + 776);
    auto* vtable = *reinterpret_cast<uint32_t**>(interface);
    auto test = reinterpret_cast<uint8_t (__fastcall*)(void*, void*)>(vtable[2]);
    if (!test(interface, nullptr)) return;
    if (!self[5084]) {
        if (!(*reinterpret_cast<float*>(self + 5080) < Zero)) return;
        uint32_t state = *reinterpret_cast<uint32_t*>(Global() + 200);
        uint8_t flag = *reinterpret_cast<uint8_t*>(state + 1452);
        *reinterpret_cast<uint8_t*>(state + 1452) = 0;
        if (!flag) return;
    }
    *reinterpret_cast<uint32_t*>(self + 5080) = 0x3F800000u;
    self[5084] = 0;
    List players;
    players.head = Sentinel(); players.size = 0;
    Global();
    Enumerate(&players);
    Node* first = players.head->next;
    while (first != players.head) {
        List pending;
        pending.head = Sentinel(); pending.size = 0;
        uint32_t player = first->value;
        int32_t index = *reinterpret_cast<int8_t*>(player + 1100);
        Node* node = first->next;
        while (node != players.head) {
            uint32_t value = node->value;
            if (!Compare(reinterpret_cast<void*>(player + 1380), reinterpret_cast<void*>(value + 1380))) {
                int32_t candidate = *reinterpret_cast<int8_t*>(value + 1100);
                if (candidate > index) index = candidate;
                if (candidate == -1) {
                    Node* added = Buy(pending.head, pending.head->prev, &value);
                    Inc(&pending, nullptr, 1);
                    pending.head->prev = added;
                    added->prev->next = added;
                    Node* erased = node;
                    node = node->prev;
                    if (erased != players.head) {
                        node->next = erased->next;
                        erased->next->prev = erased->prev;
                        Free(erased);
                        --players.size;
                    }
                }
            }
            node = node->next;
        }
        node = pending.head->next;
        while (node != pending.head) {
            ++index;
            uint32_t value = node->value;
            if (!index) index = 1;
            interface = reinterpret_cast<void*>(value + 1712);
            vtable = *reinterpret_cast<uint32_t**>(interface);
            test = reinterpret_cast<uint8_t (__fastcall*)(void*, void*)>(vtable[2]);
            if (test(interface, nullptr)) SetChanged(reinterpret_cast<void*>(value), nullptr, index);
            else SetIndex(reinterpret_cast<void*>(value), nullptr, index);
            node = node->next;
        }
        node = pending.head->next;
        pending.head->next = pending.head; pending.head->prev = pending.head;
        pending.size = 0;
        while (node != pending.head) {
            Node* next = node->next;
            Free(node); node = next;
        }
        Free(pending.head);
        first = first->next;
    }
    Node* node = players.head->next;
    players.head->next = players.head; players.head->prev = players.head;
    players.size = 0;
    while (node != players.head) {
        Node* next = node->next;
        Free(node); node = next;
    }
    if (players.head) Free(players.head);
}
HOOK(0x00556E80, UpdateSpawnState, "stl");
