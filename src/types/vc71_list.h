// Binary-derived VC7.1 list helpers. No STL implementation source used.
#pragma once
#include <cstdint>
#include <cstddef>
#include "globals.h"
namespace legacy { namespace vc71 {
template<class T> struct list_node { list_node* next; list_node* prev; T value; };
template<class T> struct list { uint32_t allocator; list_node<T>* head; uint32_t size; };
static_assert(sizeof(list<uint32_t>) == 12);
static_assert(offsetof(list<uint32_t>, head) == 4);
static_assert(offsetof(list<uint32_t>, size) == 8);
static_assert(sizeof(list_node<uint32_t>) == 12);
CSF_FUNC(void*, __cdecl, list_allocate, 0x00690DC0, (size_t bytes, int file, int line));
CSF_FUNC(uint32_t, __cdecl, list_free, 0x00690E60, (void* block));
CSF_FUNC(void, __stdcall, list_rethrow, 0x007AF666, (void* object, void* info));
template<class T> static void list_construct(list_node<T>* node, list_node<T>* next, list_node<T>* prev, const T* value) {
    node->next = next; node->prev = prev; node->value = *value;
}
template<class T> __forceinline list_node<T>* list_buy_node(list_node<T>* next, list_node<T>* prev, const T* value) {
    auto* node = static_cast<list_node<T>*>(list_allocate(sizeof(list_node<T>), 0, 0));
    try {
        // Keep the original cleanup funclet even for these trivial values. A volatile
        // local call boundary prevents the newer compiler deleting the catch as dead.
        void (*volatile construct)(list_node<T>*, list_node<T>*, list_node<T>*, const T*) = list_construct<T>;
        if (node) construct(node, next, prev, value);
    } catch (...) {
        if (node) list_free(node);
        list_rethrow(nullptr, nullptr);
    }
    return node;
}
template<class Ops> __forceinline uint32_t list_push_back(list<typename Ops::value_type>* self, const typename Ops::value_type* value) {
    auto* head = self->head;
    auto* node = Ops::buy(head, head->prev, value);
    uint32_t result = Ops::incsize(self, 1);
    head->prev = node;
    node->prev->next = node;
    return result;
}
template<class Ops> __forceinline uint32_t list_incsize(list<typename Ops::value_type>* self, uint32_t count) {
    uint32_t size = self->size;
    if (Ops::max_size - size < count) Ops::length_error(count);
    uint32_t result = size + count;
    self->size = result;
    return result;
}
}}
