// VC7.1 basic_string layout and algorithms, extracted from the game binary.
#pragma once
#include <cstdint>
#include <cstddef>
#include "globals.h"
namespace legacy { namespace vc71 {
template<class C> struct string {
    uint32_t allocator;
    union { C buffer[16 / sizeof(C)]; C* heap; } storage;
    uint32_t length;
    uint32_t capacity;
    __forceinline C* data() { return capacity < 16 / sizeof(C) ? storage.buffer : storage.heap; }
};
static_assert(sizeof(string<char>) == 28 && sizeof(string<uint16_t>) == 28);
static_assert(offsetof(string<char>, storage) == 4);
static_assert(offsetof(string<char>, length) == 20);
static_assert(offsetof(string<char>, capacity) == 24);
CSF_FUNC(void, __fastcall, string_xran, 0x007AE981, (void* self, void* edx));
CSF_FUNC(void, __fastcall, string_xlen, 0x007AE9C1, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, string_move, 0x007AEE60, (void* dest, const void* source, size_t bytes));
// The original qmemcpy is inline REP MOVSD followed by REP MOVSB, not a CRT call.
__forceinline void string_copy(void* dest, const void* source, uint32_t bytes) {
    auto* d = static_cast<uint32_t*>(dest);
    auto* s = static_cast<const uint32_t*>(source);
    uint32_t words = bytes >> 2;
    for (uint32_t i = 0; i < words; ++i) d[i] = s[i];
    auto* db = reinterpret_cast<uint8_t*>(d + words);
    auto* sb = reinterpret_cast<const uint8_t*>(s + words);
    for (uint32_t i = 0; i < (bytes & 3); ++i) db[i] = sb[i];
}
template<class C> __forceinline string<C>* string_erase(string<C>* self, uint32_t pos, uint32_t count) {
    if (self->length < pos) string_xran(self, nullptr);
    uint32_t rest = self->length - pos;
    if (rest < count) count = rest;
    if (count) {
        string_move(self->data() + pos, self->data() + pos + count, sizeof(C) * (rest - count));
        uint32_t length = self->length - count;
        self->length = length;
        self->data()[length] = 0;
    }
    return self;
}
template<class Ops> __forceinline bool string_grow(string<typename Ops::char_type>* self, uint32_t size) {
    if (size == 0xFFFFFFFFu) string_xlen(self, nullptr);
    if (self->capacity < size) {
        Ops::grow(self, size, self->length);
    } else if (!size) {
        self->length = 0;
        self->data()[0] = 0;
        return false;
    }
    return size != 0;
}
template<class Ops> __forceinline string<typename Ops::char_type>* string_assign_range(
    string<typename Ops::char_type>* self, string<typename Ops::char_type>* source, uint32_t pos, uint32_t count) {
    using C = typename Ops::char_type;
    if (source->length < pos) string_xran(self, nullptr);
    uint32_t size = source->length - pos;
    if (count < size) size = count;
    if (self == source) {
        Ops::erase(self, pos + size, 0xFFFFFFFFu);
        Ops::erase(self, 0, pos);
        return self;
    }
    if (string_grow<Ops>(self, size)) {
        string_copy(self->data(), source->data() + pos, sizeof(C) * size);
        self->length = size;
        self->data()[size] = 0;
    }
    return self;
}
template<class C> __forceinline bool string_alias(string<C>* self, const C* source) {
    uint32_t p = reinterpret_cast<uintptr_t>(source);
    uint32_t begin = reinterpret_cast<uintptr_t>(self->data());
    return p >= begin && p < begin + sizeof(C) * self->length;
}
template<class C> __forceinline uint32_t string_offset(string<C>* self, const C* source) {
    int32_t delta = reinterpret_cast<uintptr_t>(source) - reinterpret_cast<uintptr_t>(self->data());
    if constexpr (sizeof(C) == 2) return static_cast<uint32_t>(delta >> 1);
    return static_cast<uint32_t>(delta);
}
template<class Ops> __forceinline string<typename Ops::char_type>* string_assign_ptr(
    string<typename Ops::char_type>* self, const typename Ops::char_type* source, uint32_t count) {
    using C = typename Ops::char_type;
    if (string_alias(self, source)) return Ops::assign(self, self, string_offset(self, source), count);
    if (string_grow<Ops>(self, count)) {
        string_copy(self->data(), source, sizeof(C) * count);
        self->length = count;
        self->data()[count] = 0;
    }
    return self;
}
template<class Ops> __forceinline string<typename Ops::char_type>* string_append_range(
    string<typename Ops::char_type>* self, string<typename Ops::char_type>* source, uint32_t pos, uint32_t count) {
    using C = typename Ops::char_type;
    if (source->length < pos) string_xran(self, nullptr);
    uint32_t size = source->length - pos;
    if (size < count) count = size;
    if (0xFFFFFFFFu - self->length <= count) string_xlen(self, nullptr);
    if (!count) return self;
    size = self->length + count;
    if (string_grow<Ops>(self, size)) {
        string_copy(self->data() + self->length, source->data() + pos, sizeof(C) * count);
        self->length = size;
        self->data()[size] = 0;
    }
    return self;
}
template<class Ops> __forceinline string<typename Ops::char_type>* string_append_ptr(
    string<typename Ops::char_type>* self, const typename Ops::char_type* source, uint32_t count) {
    using C = typename Ops::char_type;
    if (string_alias(self, source)) return Ops::append(self, self, string_offset(self, source), count);
    if (0xFFFFFFFFu - self->length <= count) string_xlen(self, nullptr);
    if (!count) return self;
    uint32_t size = self->length + count;
    if (string_grow<Ops>(self, size)) {
        string_copy(self->data() + self->length, source, sizeof(C) * count);
        self->length = size;
        self->data()[size] = 0;
    }
    return self;
}
template<class C> __forceinline void string_fill(C* dest, uint32_t count, C value) {
    uint32_t word = static_cast<uint16_t>(value);
    if constexpr (sizeof(C) == 1) word = static_cast<uint8_t>(value) * 0x01010101u;
    else word |= word << 16;
    uint32_t words = count / (4 / sizeof(C));
    auto* d = reinterpret_cast<uint32_t*>(dest);
    for (uint32_t i = 0; i < words; ++i) d[i] = word;
    dest += words * (4 / sizeof(C));
    for (uint32_t i = 0; i < count % (4 / sizeof(C)); ++i) dest[i] = value;
}
template<class Ops> __forceinline string<typename Ops::char_type>* string_append_fill(
    string<typename Ops::char_type>* self, uint32_t count, typename Ops::char_type value) {
    if (0xFFFFFFFFu - self->length <= count) string_xlen(self, nullptr);
    if (!count) return self;
    uint32_t size = self->length + count;
    if (string_grow<Ops>(self, size)) {
        string_fill(self->data() + self->length, count, value);
        self->length = size;
        self->data()[size] = 0;
    }
    return self;
}
template<class Ops> __forceinline int string_compare(string<typename Ops::char_type>* self,
    uint32_t pos, uint32_t count, const typename Ops::char_type* source, uint32_t source_count) {
    if (self->length < pos) string_xran(self, nullptr);
    uint32_t size = self->length - pos;
    if (size < count) count = size;
    if (count) {
        uint32_t n = count < source_count ? count : source_count;
        int result = Ops::compare(self->data() + pos, source, n);
        if (result) return result;
    }
    return count < source_count ? -1 : count != source_count;
}
// Survey calls this replace(pos,count,n,char); the binary has three arguments:
// it inserts count copies at pos, deleting nothing.
template<class Ops> __forceinline string<typename Ops::char_type>* string_insert_fill(
    string<typename Ops::char_type>* self, uint32_t pos, uint32_t count, typename Ops::char_type value) {
    using C = typename Ops::char_type;
    if (self->length < pos) string_xran(self, nullptr);
    if (0xFFFFFFFFu - self->length <= count) string_xlen(self, nullptr);
    if (!count) return self;
    uint32_t size = self->length + count;
    if (string_grow<Ops>(self, size)) {
        string_move(self->data() + pos + count, self->data() + pos, sizeof(C) * (self->length - pos));
        string_fill(self->data() + pos, count, value);
        self->length = size;
        self->data()[size] = 0;
    }
    return self;
}
template<class Ops> __forceinline string<typename Ops::char_type>* string_replace_range(
    string<typename Ops::char_type>* self, uint32_t pos, uint32_t count,
    string<typename Ops::char_type>* source, uint32_t source_pos, uint32_t source_count) {
    using C = typename Ops::char_type;
    if (self->length < pos || source->length < source_pos) string_xran(self, nullptr);
    if (self->length - pos < count) count = self->length - pos;
    if (source->length - source_pos < source_count) source_count = source->length - source_pos;
    if (0xFFFFFFFFu - source_count <= self->length - count) string_xlen(self, nullptr);
    uint32_t tail = self->length - pos - count;
    uint32_t size = source_count + self->length - count;
    if (self->length < size) string_grow<Ops>(self, size);
    if (self != source) {
        string_move(self->data() + pos + source_count, self->data() + pos + count, sizeof(C) * tail);
        string_copy(self->data() + pos, source->data() + source_pos, sizeof(C) * source_count);
    } else if (source_count <= count) {
        string_move(self->data() + pos, self->data() + source_pos, sizeof(C) * source_count);
        string_move(self->data() + pos + source_count, self->data() + pos + count, sizeof(C) * tail);
    } else if (source_pos <= pos) {
        string_move(self->data() + pos + source_count, self->data() + pos + count, sizeof(C) * tail);
        string_move(self->data() + pos, self->data() + source_pos, sizeof(C) * source_count);
    } else if (pos + count <= source_pos) {
        string_move(self->data() + pos + source_count, self->data() + pos + count, sizeof(C) * tail);
        string_move(self->data() + pos, self->data() + source_count + source_pos - count, sizeof(C) * source_count);
    } else {
        string_move(self->data() + pos, self->data() + source_pos, sizeof(C) * count);
        string_move(self->data() + pos + source_count, self->data() + pos + count, sizeof(C) * tail);
        string_move(self->data() + pos + count, self->data() + source_count + source_pos, sizeof(C) * (source_count - count));
    }
    self->length = size;
    self->data()[size] = 0;
    return self;
}
}}
