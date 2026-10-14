// Offline process-object lifetime boundary. Included only by generated stubs.cpp.
// Original vtable identities are retained: unprovided operations still trap.
#include <windows.h>
#include <cstdlib>
namespace {
static uint32_t& field(void* p, size_t n) { return CSF_AT(uint32_t, p, n); }
static void* pointer(uint32_t p) { return reinterpret_cast<void*>(p); }
static uint32_t address(void* p) { return reinterpret_cast<uintptr_t>(p); }
static void* owned(size_t n) {
    void* p = game::Mem_Alloc(n);
    if (!p) std::abort(); // callers immediately dereference; never publish partial state
    std::memset(p, 0, n); return p;
}
static void critical_init(void* p) {
    auto* cs = static_cast<CRITICAL_SECTION*>(owned(sizeof(CRITICAL_SECTION)));
    static_assert(sizeof(CRITICAL_SECTION) == 24);
    InitializeCriticalSection(cs); field(p, 0) = address(cs);
}
static void critical_destroy(void* p) {
    auto* cs = static_cast<CRITICAL_SECTION*>(pointer(field(p, 0)));
    if (cs) { DeleteCriticalSection(cs); game::Mem_Free(cs); }
    field(p, 0) = 0;
}
static uint32_t __cdecl critical_construct(hook::Regs* r, const uint32_t*) {
    critical_init(pointer(r->ecx)); return r->ecx;
}
static uint32_t __cdecl critical_destruct(hook::Regs* r, const uint32_t*) {
    critical_destroy(pointer(r->ecx)); return 0;
}
static void* circular_head() {
    void* p = owned(12); field(p, 0) = field(p, 4) = address(p); return p;
}
static void* tree_head() {
    void* p = owned(24);
    field(p, 0) = field(p, 4) = field(p, 8) = address(p);
    CSF_AT(uint8_t, p, 0x14) = 1; CSF_AT(uint8_t, p, 0x15) = 1;
    return p;
}
// These factories return unlinked nodes; the initializer sets sentinel marker/links.
static uint32_t tree_node() {
    void* p = owned(24); CSF_AT(uint8_t, p, 0x14) = 1; return address(p);
}
static uint32_t __cdecl tree_factory_66b390(hook::Regs*, const uint32_t*) { return tree_node(); }
static uint32_t __cdecl tree_factory_7aa3c0(hook::Regs*, const uint32_t*) { return tree_node(); }
static uint32_t __cdecl list_factory_668450(hook::Regs*, const uint32_t*) { return address(circular_head()); }
static uint32_t __cdecl list_factory_66d400(hook::Regs*, const uint32_t*) { return address(circular_head()); }
static uint32_t __cdecl list_factory_6882d0(hook::Regs*, const uint32_t*) { return address(circular_head()); }
static uint32_t __cdecl list_factory_796490(hook::Regs*, const uint32_t*) { return address(circular_head()); }
static uint32_t __cdecl list_factory_79c090(hook::Regs*, const uint32_t*) { return address(circular_head()); }
static uint32_t empty_tree_range(hook::Regs* r, const uint32_t* a) {
    void* p = pointer(r->ecx); uint32_t head = field(p, 4);
    // Only empty offline containers are owned here; a populated online tree is not ours.
    if (field(p, 8) || a[1] != head || a[2] != head) std::abort();
    field(pointer(a[0]), 0) = head; return a[0];
}
static uint32_t __cdecl tree_range_66bbe0(hook::Regs* r, const uint32_t* a) { return empty_tree_range(r, a); }
static uint32_t __cdecl tree_range_7aa730(hook::Regs* r, const uint32_t* a) { return empty_tree_range(r, a); }
static uint32_t __cdecl registry_base_construct(hook::Regs* r, const uint32_t*) {
    void* p = pointer(r->ecx); field(p, 0) = CSF_ADDR_VT(0x00858CA0); field(p, 4) = 0; return r->ecx;
}
static uint32_t __cdecl registry_base_destruct(hook::Regs* r, const uint32_t*) {
    field(pointer(r->ecx), 0) = CSF_ADDR_VT(0x00858CA0); return 0;
}
static uint32_t __cdecl registry_construct(hook::Regs* r, const uint32_t*) {
    field(pointer(r->ecx), 0) = CSF_ADDR_VT(0x00858DB8); return r->ecx;
}
static uint32_t __cdecl registry_destruct(hook::Regs* r, const uint32_t*) {
    field(pointer(r->ecx), 0) = CSF_ADDR_VT(0x00858DB8); return 0;
}
static uint32_t& registry_first() { return CSF_AT(uint32_t, CSF_ADDR_DATA(0x008B9A88), 0); }
static uint32_t& registry_size() { return CSF_AT(uint32_t, CSF_ADDR_DATA(0x008B9A8C), 0); }
static uint32_t context_count() { return CSF_AT(uint32_t, CSF_ADDR_DATA(0x008A6544), 0); }
static void registry_add(void* p) {
    // All initialiser-reached registry tables' slots 4/5 are +4 get/set (6683f0/67aca0).
    uint32_t* link = &registry_first();
    while (*link) link = &field(pointer(*link), 4);
    *link = address(p); ++registry_size();
}
static void registry_remove(void* p) {
    uint32_t* link = &registry_first();
    while (*link && *link != address(p)) link = &field(pointer(*link), 4);
    if (*link) { *link = field(p, 4); field(p, 4) = 0; --registry_size(); }
}
static uint32_t __cdecl registry_append(hook::Regs*, const uint32_t* a) { registry_add(pointer(a[0])); return 0; }
static uint32_t __cdecl registry_unlink(hook::Regs*, const uint32_t* a) { registry_remove(pointer(a[0])); return 0; }
static void scalar_contexts(void* p, unsigned width, size_t source) {
    // The original process starts with one context; never treat -1 as an allocation size.
    if (!context_count() || context_count() > 1024) std::abort();
    void* values = owned(context_count() * width);
    for (uint32_t i = 0; i < context_count(); ++i)
        std::memcpy(static_cast<uint8_t*>(values) + i * width, static_cast<uint8_t*>(p) + source, width);
    field(p, 8) = address(values);
}
static uint32_t __cdecl context_dword_647840(hook::Regs* r, const uint32_t*) { scalar_contexts(pointer(r->ecx), 4, 0x10); return r->ecx; }
static uint32_t __cdecl context_dword_6523a0(hook::Regs* r, const uint32_t*) { scalar_contexts(pointer(r->ecx), 4, 0x10); return r->ecx; }
static uint32_t __cdecl context_dword_67ac40(hook::Regs* r, const uint32_t*) { scalar_contexts(pointer(r->ecx), 4, 0x10); return r->ecx; }
static uint32_t __cdecl context_word_657810(hook::Regs* r, const uint32_t*) { scalar_contexts(pointer(r->ecx), 2, 0xe); return r->ecx; }
static uint32_t __cdecl context_pair_construct(hook::Regs* r, const uint32_t* a) {
    void* p = pointer(r->ecx); field(p, 0) = CSF_ADDR_VT(0x00858DA0); field(p, 4) = 0;
    std::memcpy(static_cast<uint8_t*>(p) + 0x10, pointer(a[0]), 8);
    std::memcpy(static_cast<uint8_t*>(p) + 0x18, pointer(a[0]), 8);
    field(p, 8) = 0; registry_add(p);
    if (context_count() > 1) scalar_contexts(p, 8, 0x18);
    return r->ecx;
}
static void value_init(void* p) {
    CSF_AT(uint8_t, p, 0) = 0; field(p, 4) = 0;
    field(p, 8) = CSF_ADDR_VT(0x008583FC); CSF_AT(uint8_t, p, 0x10) = 0;
    field(p, 0x14) = field(p, 0x18) = field(p, 0x1c) = 0;
}
static void value_copy(void* out, void* in) {
    CSF_AT(uint8_t, out, 0) = CSF_AT(uint8_t, in, 0);
    field(out, 4) = field(in, 4); CSF_AT(uint8_t, out, 0x10) = CSF_AT(uint8_t, in, 0x10);
    field(out, 0x14) = field(in, 0x14); field(out, 0x18) = field(in, 0x18); field(out, 0x1c) = field(in, 0x1c);
}
static uint32_t __cdecl value_construct(hook::Regs* r, const uint32_t*) { value_init(pointer(r->ecx)); return r->ecx; }
static uint32_t __cdecl value_destruct(hook::Regs* r, const uint32_t*) { field(pointer(r->ecx), 8) = CSF_ADDR_VT(0x008583FC); return 0; }
static uint32_t __cdecl context_value_construct(hook::Regs* r, const uint32_t* a) {
    void* p = pointer(r->ecx); field(p, 0) = CSF_ADDR_VT(0x008590C4); field(p, 4) = field(p, 8) = 0;
    value_init(static_cast<uint8_t*>(p) + 0x10); value_init(static_cast<uint8_t*>(p) + 0x30);
    value_copy(static_cast<uint8_t*>(p) + 0x30, pointer(a[0])); value_copy(static_cast<uint8_t*>(p) + 0x10, pointer(a[0]));
    registry_add(p);
    if (context_count() > 1) {
        void* v = owned(context_count() * 0x20);
        for (uint32_t i = 0; i < context_count(); ++i) { void* e = static_cast<uint8_t*>(v) + i * 0x20; value_init(e); value_copy(e, static_cast<uint8_t*>(p) + 0x30); }
        field(p, 8) = address(v);
    }
    return r->ecx;
}
static uint32_t __cdecl context_value_destruct(hook::Regs* r, const uint32_t*) {
    void* p = pointer(r->ecx); game::Mem_Free(pointer(field(p, 8))); field(p, 8) = 0; return 0;
}
static void plain_list_init(void* p) { field(p, 4) = address(circular_head()); field(p, 8) = 0; }
static void plain_list_copy(void* out, void* in) {
    uint32_t head = field(in, 4), at = field(pointer(head), 0);
    void* target = pointer(field(out, 4));
    while (at != head) {
        void* n = owned(12); field(n, 8) = field(pointer(at), 8);
        uint32_t prev = field(target, 4); field(n, 0) = address(target); field(n, 4) = prev;
        field(pointer(prev), 0) = address(n); field(target, 4) = address(n); ++field(out, 8);
        at = field(pointer(at), 0);
    }
}
static void plain_list_destroy(void* p) {
    void* h = pointer(field(p, 4)); if (!h) return;
    uint32_t at = field(h, 0);
    while (at != address(h)) { void* n = pointer(at); at = field(n, 0); game::Mem_Free(n); }
    game::Mem_Free(h); field(p, 4) = field(p, 8) = 0;
}
static uint32_t __cdecl context_list_construct(hook::Regs* r, const uint32_t* a) {
    void* p = pointer(r->ecx); field(p, 0) = CSF_ADDR_VT(0x0085AFC8); field(p, 4) = field(p, 8) = 0;
    void* current = static_cast<uint8_t*>(p) + 0xc; void* initial = static_cast<uint8_t*>(p) + 0x1c;
    plain_list_init(current); plain_list_init(initial); plain_list_copy(initial, pointer(a[0])); plain_list_copy(current, initial);
    registry_add(p);
    // More contexts need lists with independent ownership, not shallow copies.
    if (context_count() > 1) {
        void* v = owned(context_count() * 16);
        for (uint32_t i = 0; i < context_count(); ++i) { void* e = static_cast<uint8_t*>(v) + i * 16; plain_list_init(e); plain_list_copy(e, initial); }
        field(p, 8) = address(v);
    }
    return r->ecx;
}
static uint32_t __cdecl context_list_destruct(hook::Regs* r, const uint32_t*) {
    void* p = pointer(r->ecx); registry_remove(p);
    void* v = pointer(field(p, 8)); if (v) {
        for (uint32_t i = 0; i < context_count(); ++i) plain_list_destroy(static_cast<uint8_t*>(v) + i * 16);
        game::Mem_Free(v); field(p, 8) = 0;
    }
    plain_list_destroy(static_cast<uint8_t*>(p) + 0x1c); plain_list_destroy(static_cast<uint8_t*>(p) + 0xc);
    field(p, 0) = CSF_ADDR_VT(0x00858CA0); return 0;
}
static uint32_t __cdecl counter_sample(hook::Regs*, const uint32_t* a) {
    void* provider = pointer(CSF_AT(uint32_t, CSF_ADDR_DATA(0x008B99B0), 0));
    if (provider) {
        auto table = *static_cast<void***>(provider);
        reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(table[1])(provider, nullptr, pointer(a[0]));
        return a[0];
    }
    LARGE_INTEGER count{}, frequency{};
    if (!QueryPerformanceCounter(&count) || !QueryPerformanceFrequency(&frequency) || !frequency.QuadPart) std::abort();
    *reinterpret_cast<uint64_t*>(a[0]) = static_cast<uint64_t>(count.QuadPart) * 1000 / static_cast<uint64_t>(frequency.QuadPart);
    return a[0];
}
static uint32_t __cdecl stopwatch_construct(hook::Regs* r, const uint32_t* a) {
    void* p = pointer(r->ecx); std::memset(p, 0, 0x24);
    field(p, 0xc) = a[0]; field(p, 0x14) = a[1]; field(p, 0x20) = 0x3f800000;
    uint64_t now; uint32_t arg[] = {address(&now)}; counter_sample(nullptr, arg); field(p, 0x10) = static_cast<uint32_t>(now);
    // A local stopwatch is registered in the real 12-byte-node process list.
    auto* list = pointer(CSF_AT(uint32_t, CSF_ADDR_DATA(0x008B9A24), 0));
    if (!list) { list = owned(16); plain_list_init(list); CSF_AT(uint32_t, CSF_ADDR_DATA(0x008B9A24), 0) = address(list); }
    void* head = pointer(field(list, 4)); void* n = owned(12); uint32_t prev = field(head, 4);
    field(n, 0) = address(head); field(n, 4) = prev; field(n, 8) = r->ecx;
    field(pointer(prev), 0) = address(n); field(head, 4) = address(n); ++field(list, 8); return r->ecx;
}
static uint32_t __cdecl stopwatch_destruct(hook::Regs* r, const uint32_t*) {
    auto& slot = CSF_AT(uint32_t, CSF_ADDR_DATA(0x008B9A24), 0); void* list = pointer(slot);
    if (!list) std::abort();
    uint32_t head = field(list, 4), at = field(pointer(head), 0);
    while (at != head && field(pointer(at), 8) != r->ecx) at = field(pointer(at), 0);
    if (at == head) std::abort();
    void* n = pointer(at); field(pointer(field(n, 4)), 0) = field(n, 0); field(pointer(field(n, 0)), 4) = field(n, 4);
    game::Mem_Free(n); --field(list, 8);
    if (!field(list, 8)) { plain_list_destroy(list); game::Mem_Free(list); slot = 0; }
    return 0;
}
static uint32_t __cdecl settings_construct(hook::Regs* r, const uint32_t*) {
    void* p = pointer(r->ecx); std::memset(p, 0, 0x20); CSF_AT(uint8_t, p, 0x20) = 1; field(p, 0x24) = 0; return r->ecx;
}
static uint32_t __cdecl settings_destruct(hook::Regs* r, const uint32_t*) { std::memset(pointer(r->ecx), 0, 0x20); return 0; }
static uint32_t __cdecl random_construct(hook::Regs* r, const uint32_t*) {
    void* p = pointer(r->ecx); field(p, 0) = 1;
    for (unsigned i = 1; i < 624; ++i) field(p, i * 4) = field(p, (i - 1) * 4) * 69069u;
    field(p, 0x9c0) = 624; field(p, 0x9c4) = 0; field(p, 0x9c8) = 0x9908b0df; return r->ecx;
}
static uint32_t __cdecl root_construct(hook::Regs* r, const uint32_t* a) {
    void* p = pointer(r->ecx); field(p, 0) = CSF_ADDR_VT(0x0085CAF4); field(p, 4) = field(p, 0xc) = 0;
    CSF_AT(uint8_t, p, 8) = 0; CSF_AT(uint8_t, p, 0x10) = static_cast<uint8_t>(a[0]); return r->ecx;
}
static uint32_t __cdecl root_destruct(hook::Regs* r, const uint32_t*) { field(pointer(r->ecx), 0) = CSF_ADDR_VT(0x0085CAF4); return 0; }
static uint32_t __cdecl reference_construct(hook::Regs* r, const uint32_t*) {
    void* p = pointer(r->ecx); field(p, 0) = 0; CSF_AT(uint8_t, p, 4) = CSF_AT(uint8_t, p, 5) = 0; return r->ecx;
}
static uint32_t __cdecl timeout_construct(hook::Regs* r, const uint32_t*) {
    void* p = pointer(r->ecx); field(p, 0) = 0x40800000; field(p, 4) = 0xbf800000;
    field(p, 8) = 1000; field(p, 0xc) = 250; CSF_AT(uint8_t, p, 0x10) = 1; field(p, 0x14) = 3500; return r->ecx;
}
static uint32_t __cdecl interpolation_construct(hook::Regs* r, const uint32_t*) {
    void* p = pointer(r->ecx); std::memset(p, 0, 0x1c); field(p, 0x1c) = 0xbf800000; return r->ecx;
}
static uint32_t __cdecl provider_get(hook::Regs* r, const uint32_t*) { return field(pointer(r->ecx), 0xc); }
static uint32_t __cdecl factory_construct(hook::Regs* r, const uint32_t*) {
    void* p = pointer(r->ecx); field(p, 0) = CSF_ADDR_VT(0x0085B8D4);
    void* generator = owned(4); field(generator, 0) = CSF_ADDR_VT(0x0085B8E0);
    void* model = owned(32); field(model, 0) = CSF_ADDR_VT(0x0085D93C);
    uint64_t now; uint32_t arg[] = {address(&now)}; counter_sample(nullptr, arg); field(model, 0x1c) = static_cast<uint32_t>(now);
    field(p, 4) = address(generator); field(p, 8) = address(model); field(p, 0xc) = 0; return r->ecx;
}
static uint32_t __cdecl factory_destruct(hook::Regs* r, const uint32_t*) {
    void* p = pointer(r->ecx);
    if (field(p, 0xc)) std::abort(); // online extension is not an owned offline child
    void* generator = pointer(field(p, 4)); void* model = pointer(field(p, 8));
    if ((generator && field(generator, 0) != CSF_ADDR_VT(0x0085B8E0)) ||
        (model && field(model, 0) != CSF_ADDR_VT(0x0085D93C))) std::abort();
    field(p, 0) = CSF_ADDR_VT(0x0085B8D4);
    game::Mem_Free(model); game::Mem_Free(generator); field(p, 4) = field(p, 8) = 0; return 0;
}
static void thread_map_register(void* p) {
    void* list = pointer(CSF_ADDR_DATA(0x008B9A0C));
    if (!field(list, 4)) {
        plain_list_init(list); critical_init(static_cast<uint8_t*>(list) + 0x10);
        CSF_AT(uint32_t, CSF_ADDR_DATA(0x008B9A20), 0) |= 1;
        // Offline lifetime ends with the last map, not a hidden NetZ atexit callback.
    }
    void* head = pointer(field(list, 4)); void* n = owned(12); uint32_t prev = field(head, 4);
    field(n, 0) = address(head); field(n, 4) = prev; field(n, 8) = address(p);
    field(pointer(prev), 0) = address(n); field(head, 4) = address(n); ++field(list, 8);
}
static void thread_map_unregister(void* p) {
    void* list = pointer(CSF_ADDR_DATA(0x008B9A0C)); uint32_t head = field(list, 4);
    if (!head) std::abort();
    uint32_t at = field(pointer(head), 0);
    while (at != head && field(pointer(at), 8) != address(p)) at = field(pointer(at), 0);
    if (at == head) std::abort();
    void* n = pointer(at); field(pointer(field(n, 4)), 0) = field(n, 0); field(pointer(field(n, 0)), 4) = field(n, 4);
    game::Mem_Free(n); --field(list, 8);
    if (!field(list, 8)) {
        plain_list_destroy(list); critical_destroy(static_cast<uint8_t*>(list) + 0x10);
        CSF_AT(uint32_t, CSF_ADDR_DATA(0x008B9A20), 0) &= ~1u;
    }
}
static uint32_t thread_map_init(hook::Regs* r, const uint32_t* a, uint32_t table) {
    void* p = pointer(r->ecx); field(p, 0) = table; field(p, 4) = a[0];
    critical_init(static_cast<uint8_t*>(p) + 8);
    void* head = tree_head(); void* node = owned(24);
    field(node, 0) = field(node, 8) = address(head); field(node, 4) = address(head);
    field(node, 0xc) = GetCurrentThreadId(); field(node, 0x10) = a[0]; CSF_AT(uint8_t, node, 0x14) = 1;
    field(head, 0) = field(head, 4) = field(head, 8) = address(node);
    field(p, 0x10) = address(head); field(p, 0x14) = 1; thread_map_register(p); return r->ecx;
}
static uint32_t thread_map_destroy(hook::Regs* r, uint32_t table) {
    void* p = pointer(r->ecx); field(p, 0) = table;
    void* head = pointer(field(p, 0x10));
    if (field(p, 0x14) != 1 || !head) std::abort();
    void* node = pointer(field(head, 4));
    if (field(node, 0) != address(head) || field(node, 8) != address(head)) std::abort();
    game::Mem_Free(node); game::Mem_Free(head); field(p, 0x10) = field(p, 0x14) = 0;
    critical_destroy(static_cast<uint8_t*>(p) + 8); thread_map_unregister(p);
    field(p, 0) = CSF_ADDR_VT(0x0085839C); return 0;
}
static uint32_t __cdecl thread_map_construct(hook::Regs* r, const uint32_t* a) { return thread_map_init(r, a, CSF_ADDR_VT(0x00858034)); }
static uint32_t __cdecl thread_map_destruct(hook::Regs* r, const uint32_t*) { return thread_map_destroy(r, CSF_ADDR_VT(0x00858034)); }
static uint32_t __cdecl thread_value_construct(hook::Regs* r, const uint32_t* a) { return thread_map_init(r, a, CSF_ADDR_VT(0x00858390)); }
static uint32_t __cdecl thread_value_destruct(hook::Regs* r, const uint32_t*) { return thread_map_destroy(r, CSF_ADDR_VT(0x00858390)); }
} // namespace
