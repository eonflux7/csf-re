// Offline session ownership and shared game getters; not an online NetZ implementation.
namespace {
static uint32_t __cdecl session_container_head(hook::Regs*, const uint32_t*) {
    // Original allocates 0x40, not a 12-byte generic list node. Payload is unconstructed.
    void* p = owned(0x40); field(p, 0) = field(p, 4) = address(p); return address(p);
}
static uint32_t __cdecl session_container_clear(hook::Regs* r, const uint32_t*) {
    void* p = pointer(r->ecx); void* head = pointer(field(p, 4));
    if (!head || field(p, 8) || field(head, 0) != address(head) || field(head, 4) != address(head))
        std::abort(); // populated transport payloads are not offline-owned
    field(head, 0) = field(head, 4) = address(head); field(p, 8) = 0; return 0;
}
static uint32_t __cdecl session_embedded_string(hook::Regs* r, const uint32_t*) {
    string_init(pointer(r->ecx), CSF_STR(0x0085B4E4, "prudp:/")); return r->ecx;
}
static uint32_t __cdecl runtime_state(hook::Regs*, const uint32_t*) { return CSF_ADDR_DATA(0x008BAA9C); }
static uint32_t __cdecl clump_manager_get(hook::Regs* r, const uint32_t*) { return field(pointer(r->ecx), 0x64); }
static uint32_t __cdecl object_manager_get(hook::Regs* r, const uint32_t*) { return field(pointer(r->ecx), 0x70); }
static uint32_t __cdecl constant_enabled(hook::Regs*, const uint32_t*) { return 1; }
static void set_thread_notice(uint32_t original, uint32_t value) {
    void* p = pointer(original); void* head = pointer(field(p, 0x10));
    if (!head || field(p, 0x14) != 1) std::abort();
    void* node = pointer(field(head, 4));
    if (field(node, 0xc) != GetCurrentThreadId()) std::abort();
    field(node, 0x10) = value;
}
static uint32_t __cdecl status_notice(hook::Regs*, const uint32_t* a) {
    // Original stores status arg2 and detail arg3 in the two current-thread maps.
    set_thread_notice(CSF_ADDR_DATA(0x008B9980), a[2]);
    set_thread_notice(CSF_ADDR_DATA(0x008B9968), a[3]); return 0;
}
static uint32_t __cdecl class_index_lookup(hook::Regs*, const uint32_t* a) {
    void* registry = pointer(CSF_AT(uint32_t, CSF_ADDR_DATA(0x008BA9C4), 0));
    // No online classes are fabricated. Preserve genuine registered object identities.
    if (!registry || a[0] >= field(registry, 0)) return 0;
    return field(pointer(field(registry, 4)), a[0] * 4);
}
static uint32_t __cdecl selected_object_test(hook::Regs* r, const uint32_t*) {
    // Current online selection context is absent: original accepts any nonnull owner.
    return field(pointer(r->ecx), 0xc) != 0;
}
static uint32_t __cdecl selection_context(hook::Regs*, const uint32_t*) { return 0; }
static uint32_t __cdecl singleton_lookup(hook::Regs*, const uint32_t*) { return 0; }
static uint32_t __cdecl callback_register_reject(hook::Regs*, const uint32_t*) { return 0; }
static uint32_t __cdecl callback_unregister(hook::Regs*, const uint32_t*) {
    // Offline registration is rejected (669B40); never dispatch callbacks.
    return 0;
}
static void empty_link(void* p, uint32_t id) {
    field(p, 0) = 0; field(p, 4) = id; CSF_AT(uint8_t, p, 8) = 1;
}
static uint32_t __cdecl selection_link_reset(hook::Regs* r, const uint32_t* a) {
    void* p = pointer(r->ecx);
    if (field(p, 0)) std::abort(); // an online reference is never silently leaked
    field(p, 4) = field(pointer(a[0]), 0); return r->ecx;
}
static uint32_t __cdecl selection_link_id(hook::Regs* r, const uint32_t* a) {
    empty_link(pointer(r->ecx), a[0]); return r->ecx;
}
static uint32_t __cdecl selection_link_release(hook::Regs* r, const uint32_t*) {
    if (field(pointer(r->ecx), 0)) std::abort(); return 0;
}
static void sentinel_check(void* p) {
    if (field(p, 0) != CSF_ADDR_VT(0x00859FA0)) std::abort();
}
static uint32_t __cdecl selection_delete(hook::Regs* r, const uint32_t* a) {
    void* p = pointer(r->ecx); sentinel_check(p); CSF_AT(uint16_t, p, 4) = 0;
    if (a[0] & 1) game::Mem_Free(p); return r->ecx;
}
static void sentinel_add(void* p) {
    sentinel_check(p); auto& n = CSF_AT(uint16_t, p, 4);
    if (!n || n == 0xffff) std::abort(); ++n;
}
static void sentinel_release(void* p) {
    sentinel_check(p); auto& n = CSF_AT(uint16_t, p, 4);
    if (!n) std::abort(); if (--n == 0) game::Mem_Free(p);
}
static uint32_t __cdecl selection_addref(hook::Regs* r, const uint32_t*) { sentinel_add(pointer(r->ecx)); return r->ecx; }
static uint32_t __cdecl selection_release(hook::Regs* r, const uint32_t*) { sentinel_release(pointer(r->ecx)); return 0; }
static uint32_t __cdecl selection_reject_id(hook::Regs*, const uint32_t*) { return 0; }
static uint32_t __cdecl selection_reject_object(hook::Regs*, const uint32_t*) { return 0; }
static uint32_t __cdecl selection_first(hook::Regs*, const uint32_t* a) { field(pointer(a[0]), 0) = 0xffffffffu; return a[0]; }
static uint32_t __cdecl selection_last(hook::Regs*, const uint32_t* a) { field(pointer(a[0]), 0) = 0; return a[0]; }
static uint32_t __cdecl selection_sentinel(hook::Regs* r, const uint32_t* a) {
    void* p = pointer(r->ecx); field(p, 0) = CSF_ADDR_VT(0x00859FA0);
    CSF_AT(uint16_t, p, 4) = 1; field(p, 8) = 0; field(p, 0x10) = a[0]; return r->ecx;
}
static void assign_sentinel(void* p, uint32_t next) {
    uint32_t old = field(p, 0x10);
    if (old != next) {
        if (old) sentinel_release(pointer(old));
        field(p, 0x10) = next;
        if (next) sentinel_add(pointer(next));
    }
    field(p, 0x14) = next ? 0xffffffffu : 0;
    field(p, 0x18) = next ? 0 : 0xffffffffu;
}
static uint32_t __cdecl selection_assign(hook::Regs* r, const uint32_t* a) { assign_sentinel(pointer(r->ecx), a[0]); return 0; }
static uint32_t __cdecl selection_construct(hook::Regs* r, const uint32_t* a) {
    void* p = pointer(r->ecx);
    if (a[0]) std::abort(); // cannot construct a populated online selection
    field(p, 0) = 0; empty_link(static_cast<uint8_t*>(p) + 4, 0);
    field(p, 0x10) = field(p, 0x14) = field(p, 0x18) = field(p, 0x20) = 0;
    CSF_AT(uint8_t, p, 0x1c) = 1; assign_sentinel(p, a[1]); return r->ecx;
}
}
