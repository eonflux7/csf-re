// Offline replication boundary, not a replica of NetZ's serializer/registry.
// Included by generated stubs.cpp after string_destroy and winmain_support.h.
namespace {
static uint32_t __cdecl mission_reference_construct(hook::Regs* r, const uint32_t* a) {
    // An actual online object needs genuine reference counting; never fake that.
    if (a[0]) std::abort();
    empty_link(pointer(r->ecx), 0); return r->ecx;
}
static uint32_t __cdecl mission_reference_copy(hook::Regs* r, const uint32_t* a) {
    if (field(pointer(a[0]), 0)) std::abort();
    empty_link(pointer(r->ecx), field(pointer(a[0]), 4)); return r->ecx;
}
static uint32_t __cdecl mission_entry_id(hook::Regs* r, const uint32_t*) {
    // 73d0f0 passes null when its empty reference cannot resolve. Zero = no ID.
    if (r->ecx) std::abort();
    return 0;
}
static uint32_t __cdecl mission_owner_slot(hook::Regs*, const uint32_t* a) {
    field(pointer(a[0]), 0) = 0; return a[0];
}
static uint32_t __cdecl mission_spawn_record(hook::Regs*, const uint32_t* a) {
    field(pointer(a[0]), 0) = field(pointer(a[0]), 4) = 0; return a[0];
}
// Game setters unconditionally pass this result to serialization writers, then
// commit it. It is an opaque sink, NOT a fake NetZ object or successful request.
// One allocation per transaction preserves the original consuming ownership.
struct MissionDiscardEntry { uint32_t magic; };
static constexpr uint32_t mission_discard_magic = 0x4e5a4f46;
static uint32_t __cdecl mission_discard_entry(hook::Regs*, const uint32_t*) {
    auto* p = static_cast<MissionDiscardEntry*>(game::Mem_Alloc(sizeof(MissionDiscardEntry)));
    if (!p) std::abort();
    p->magic = mission_discard_magic; return reinterpret_cast<uintptr_t>(p);
}
static void mission_check_discard(uint32_t address) {
    if (!address || reinterpret_cast<MissionDiscardEntry*>(address)->magic != mission_discard_magic)
        std::abort();
}
static uint32_t __cdecl mission_discard_write(hook::Regs* r, const uint32_t*) {
    mission_check_discard(r->ecx); return r->ecx;
}
static uint32_t __cdecl mission_discard_flag(hook::Regs* r, const uint32_t* a) { return mission_discard_write(r, a); }
static uint32_t __cdecl mission_discard_byte(hook::Regs* r, const uint32_t* a) { return mission_discard_write(r, a); }
static uint32_t __cdecl mission_discard_word(hook::Regs* r, const uint32_t* a) { return mission_discard_write(r, a); }
static uint32_t __cdecl mission_discard_dword(hook::Regs* r, const uint32_t* a) { return mission_discard_write(r, a); }
static uint32_t __cdecl mission_discard_pair(hook::Regs* r, const uint32_t* a) { return mission_discard_write(r, a); }
static uint32_t __cdecl mission_discard_commit(hook::Regs*, const uint32_t* a) {
    mission_check_discard(a[1]);
    game::Mem_Free(pointer(a[1])); return 0; // replication rejected, never success
}
static uint32_t __cdecl mission_discard_key(hook::Regs*, const uint32_t* a) {
    // Original takes the owned String BY VALUE and destroys it on both exits.
    uint32_t owned = a[0]; string_destroy(&owned); return 0; // invalid property ID
}
static uint32_t __cdecl mission_reject_position_state(hook::Regs*, const uint32_t*) {
    return 0; // no interpolation-state write / no remote update accepted
}
static uint32_t __cdecl mission_reject_rotation_state(hook::Regs*, const uint32_t*) { return 0; }
static uint32_t __cdecl mission_reference_get(hook::Regs* r, const uint32_t*) {
    // Invalid empty reference resolves to null; never manufacture a DDL object.
    if (field(pointer(r->ecx), 0)) std::abort();
    return 0;
}
static uint32_t __cdecl mission_station_name(hook::Regs*, const uint32_t*) {
    // Borrowed diagnostic text, not an allocated NetZ String.
    return reinterpret_cast<uintptr_t>("");
}
} // namespace
