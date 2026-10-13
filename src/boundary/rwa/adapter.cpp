// Address ABI adapter, not a port of RenderWare Audio.
#ifdef CSF_STANDALONE
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <objbase.h>
#include <vector>
#include <memory>
#include <mutex>
#include <cstring>
#include <cstdlib>
#include <cstdarg>
#include <string>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "backend.h"
#include "bank.h"

CSF_FUNC(int, __cdecl, boundary_stream_read, 0x006293E0, (void* stream,void* data,unsigned bytes));

namespace {
using Word = uint32_t;
static_assert(sizeof(void*)==4, "RWA address ABI requires x86");
template<class T> T& field(void* p, size_t offset) { return *reinterpret_cast<T*>(static_cast<char*>(p)+offset); }
std::recursive_mutex mutex;
using Guard = std::lock_guard<std::recursive_mutex>;
struct Object {
    alignas(8) unsigned char abi[320]{};
    enum Kind { device, listener, voice, parameter, sound } kind;
    Object* owner = nullptr;
    Object* control = nullptr;
    audio::Timeline time;
    std::vector<unsigned char> pcm;
    std::vector<unsigned char> metadata;
    std::string name;
    Word guid[4]{};
    void* native = nullptr;
    explicit Object(Kind k):kind(k) {}
    ~Object() { audio::selected_backend().destroy(native); }
};
std::vector<std::unique_ptr<Object>> objects;
bool engineOpen = false;
float masterGain = 1;
Object* find(void* p) { for(auto& o:objects) if(o->abi==p) return o.get(); return nullptr; }
Object* make(Object::Kind k) {
    auto o=std::make_unique<Object>(k); auto p=o.get(); objects.push_back(std::move(o));
    field<float>(p->abi,0x38)=1; field<float>(p->abi,0x3c)=1; field<float>(p->abi,0x40)=1;
    return p;
}
void refresh(Object* o) {
    if(!o) return;
    Word pos=o->time.cursor(GetTickCount());
    field<Word>(o->abi,0x48)=pos;
    if(!o->time.active) {
        field<Word>(o->abi,0x34)&=~3u;
        audio::selected_backend().stop(o->native);
    }
}
Word frequency(Object* o) {
    float hz=field<float>(o->abi,0x3c);
    return std::isfinite(hz) && hz>0 && double(hz)<UINT32_MAX?Word(hz):0;
}
Word byte_rate(Object* o) {
    if(!o->owner) return 0;
    uint64_t n=uint64_t(frequency(o))*field<uint8_t>(o->owner->abi,0x39)*field<uint8_t>(o->owner->abi,0x38)/8;
    return n<=UINT32_MAX?Word(n):0;
}
void rebuild(Object* o) {
    auto& b=audio::selected_backend(); b.destroy(o->native); o->native=nullptr;
    if(o->owner && !o->owner->pcm.empty()) {
        void* s=o->owner->abi;
        o->native=b.create({field<Word>(s,0x2c),field<uint8_t>(s,0x39),field<uint8_t>(s,0x38),o->owner->pcm.data(),o->owner->pcm.size()});
    }
}
void stop(Object* o) { o->time.stop(GetTickCount()); audio::selected_backend().stop(o->native); field<Word>(o->abi,0x34)&=~3u; }
void start(Object* o) {
    if(!o->owner) return;
    o->time.bytes=field<Word>(o->owner->abi,0x34);
    o->time.rate=byte_rate(o);
    o->time.offset=field<Word>(o->abi,0x48);
    o->time.loop=(field<Word>(o->abi,0x34)&4)!=0;
    o->time.play(GetTickCount());
    field<Word>(o->abi,0x34)=(field<Word>(o->abi,0x34)&~3u)|(o->time.active?3u:0u);
    if(!o->time.active) { audio::selected_backend().stop(o->native); return; }
    if(!o->native) rebuild(o);
    audio::selected_backend().gain(o->native,masterGain*field<float>(o->abi,0x40));
    audio::selected_backend().frequency(o->native,frequency(o));
    audio::selected_backend().play(o->native,o->time.loop,o->time.offset);
}
struct Dictionary { unsigned char abi[36]{}; std::vector<void*> entries; };
std::vector<std::unique_ptr<Dictionary>> dictionaries;
CSF_GLOBAL(void*, currentDictionary, 0x008BC990);
CSF_GLOBAL(void*, defaultDictionary, 0x008BC98C);
Dictionary* dict(void* p) { for(auto& d:dictionaries) if(d->abi==p) return d.get(); return nullptr; }
static void* __cdecl create_dict(void* supplied) {
    Guard g(mutex);
    // All observed game calls supply null. Reject unsupported caller-owned storage.
    if(supplied) return nullptr;
    auto d=std::make_unique<Dictionary>(); auto p=d->abi;
    field<void*>(p,12)=static_cast<char*>(static_cast<void*>(p))+12; field<void*>(p,16)=field<void*>(p,12);
    dictionaries.push_back(std::move(d)); if(!defaultDictionary) defaultDictionary=p; return p;
}
static int __cdecl destroy_dict(void* p) {
    Guard g(mutex); auto d=dict(p); if(!d) return 0;
    if(currentDictionary==p) currentDictionary=defaultDictionary==p?nullptr:defaultDictionary;
    if(defaultDictionary==p) defaultDictionary=nullptr;
    for(auto i=dictionaries.begin();i!=dictionaries.end();++i) if(i->get()==d) { dictionaries.erase(i); break; }
    // Shared sound entries survive until engine shutdown; no dangling active voices.
    return 1;
}
static void* __cdecl insert(void* p,void* entry) { Guard g(mutex); auto d=dict(p); if(!d || !entry) return nullptr; for(auto e:d->entries) if(e==entry) return p; d->entries.push_back(entry); return p; }
static void* __cdecl remove_all(void* entry) { Guard g(mutex); for(auto& d:dictionaries) for(auto i=d->entries.begin();i!=d->entries.end();) if(*i==entry) i=d->entries.erase(i); else ++i; return entry; }
static void* __cdecl get_current() { Guard g(mutex); return currentDictionary; }
static void* __cdecl set_current(void* p) { Guard g(mutex); auto old=currentDictionary; currentDictionary=p; return old; }
static void* __cdecl get_default() { Guard g(mutex); return defaultDictionary; }
static int __cdecl count(void* p) { Guard g(mutex); auto d=dict(p); return d?int(d->entries.size()):0; }
static void* __cdecl entry(void* p,int n) { Guard g(mutex); auto d=dict(p); return d && n>=0 && size_t(n)<d->entries.size()?d->entries[n]:nullptr; }
static void* __cdecl named(void* p,const char* name) { Guard g(mutex); auto d=dict(p); if(d && name) for(auto e:d->entries) { auto s=field<const char*>(e,4); if(s && !_stricmp(s,name)) return e; } return nullptr; }
static int __cdecl open_engine(int,int) { Guard g(mutex); engineOpen=true; return 1; }
static int __cdecl is_open() { Guard g(mutex); return engineOpen; }
static int __cdecl close_engine() { Guard g(mutex); objects.clear(); dictionaries.clear(); currentDictionary=defaultDictionary=nullptr; engineOpen=false; masterGain=1; return 0; }
static int __cdecl update() { Guard g(mutex); for(auto& o:objects) if(o->kind==Object::voice) refresh(o.get()); return 1; }
using Visitor = int(__cdecl*)(void*,int);
static void visit(Visitor cb,int context,bool activeOnly) {
    Guard g(mutex); if(!cb) return;
    std::vector<void*> snapshot;
    for(auto& o:objects) if(o->kind==Object::voice) { refresh(o.get()); if(!activeOnly || o->time.active) snapshot.push_back(o->abi); }
    // Game callbacks may detach or destroy objects. Never iterate a mutated vector.
    for(auto p:snapshot) if(find(p) && !cb(p,context)) break;
}
static void __cdecl visit_active(Visitor cb,int context) { visit(cb,context,true); }
static void __cdecl visit_all(Visitor cb,int context) { visit(cb,context,false); }
static int __cdecl initialize() { return 1; }
using Factory = int(__cdecl*)(int,int,char*,int);
static void* construct(Object::Kind k, void* parent, char*, int, Factory callback, int) {
    // No observed caller supplies factory. Never fabricate a factory-owned object.
    if(callback) return nullptr;
    auto o=make(k); o->owner=find(parent);
    if(k==Object::voice) { o->control=make(Object::parameter); o->control->owner=o; field<void*>(o->abi,0x30)=o->control->abi; field<float>(o->abi,0x70)=200; field<float>(o->abi,0x74)=8000; }
    return o->abi;
}
static void* __cdecl create_voice(void* parent,char* config,int flags,Factory cb,int context) { Guard g(mutex); return construct(Object::voice,parent,config,flags,cb,context); }
static void* __cdecl create_device(void* parent,char* config,int flags,Factory cb,int context) {
    Guard g(mutex); auto p=construct(Object::device,parent,config,flags,cb,context);
    if(p) field<void*>(p,0x7c)=audio::selected_backend().device_handle();
    return p;
}
static void* __cdecl create_channel(void* parent,char* config,int flags,Factory cb,int context) { Guard g(mutex); return construct(Object::device,parent,config,flags,cb,context); }
static void* __cdecl create_parameter(void* parent,char* config,int flags,Factory cb,int context) { Guard g(mutex); return construct(Object::parameter,parent,config,flags,cb,context); }
static void* __cdecl create_listener(int,int,int) { Guard g(mutex); return make(Object::listener)->abi; }
static int __cdecl release(void* p,void*,int) {
    Guard g(mutex); auto o=find(p); if(!o) return 0;
    if(o->kind==Object::sound) remove_all(p);
    auto control=o->control;
    for(auto& v:objects) if(v->owner==o) {
        if(v->kind==Object::voice) { stop(v.get()); field<void*>(v->abi,0x50)=nullptr; }
        v->owner=nullptr;
    }
    // The control is exclusively owned by this voice, not a separately live
    // parameter whose sidecar may retain a dangling pointer after voice release.
    for(auto i=objects.begin();i!=objects.end();) {
        if(i->get()==o || i->get()==control) i=objects.erase(i); else ++i;
    }
    return 1;
}
static void __cdecl activate(void* p,int on) { Guard g(mutex); auto o=find(p); if(o) { if(on) start(o); else stop(o); } }
static void* __cdecl attach(void* p,void* sound) {
    Guard g(mutex); auto o=find(p); if(!o) return nullptr; stop(o); o->owner=find(sound);
    field<void*>(p,0x50)=sound;
    if(sound && !field<Word>(p,0x84)) field<float>(p,0x3c)=float(field<Word>(sound,0x2c));
    o->time.offset=0; field<Word>(p,0x48)=0; rebuild(o); return p;
}
static int __cdecl prepare(void*,void*,unsigned) { return 1; }
static void* __cdecl detach_device(void* p,void* sound) { Guard g(mutex); if(p) { field<void*>(p,0x84)=sound; field<float>(p,0x38)=field<float>(p,0x3c)=1; } return p; }
static void* __cdecl register_listener(void* p,void* value) { Guard g(mutex); if(p) field<void*>(p,0x3c)=value; return p; }
static void* __cdecl device_gain(void* p,float value) {
    Guard g(mutex); if(!find(p)) return nullptr;
    field<float>(p,0x38)=value; masterGain=value;
    for(auto& o:objects) if(o->kind==Object::voice)
        audio::selected_backend().gain(o->native,masterGain*field<float>(o->abi,0x40));
    return p;
}
static void* __cdecl device_mode(void* p,int value) { Guard g(mutex); if(p) { field<int>(p,0x74)=value; field<Word>(p,0x78)|=0x100; } return p; }
static int __cdecl speakers(void*) { return 1; } // RWA stereo code (DS mono maps to 2)
static void* __cdecl lookup_class(void*,const void*,void** out) { Guard g(mutex); auto p=make(Object::device)->abi; if(out) *out=nullptr; return p; }
static void* __cdecl defaults(void* p,void* out) {
    if(!p || !out) return nullptr; std::memset(out,0,40);
    field<int>(out,12)=1; field<Word>(out,20)=48000;
    field<uint16_t>(out,24)=16; field<uint16_t>(out,26)=32; field<int>(out,36)=1;
    return p;
}
static void* __cdecl create_sound(void*,void* source,void*,char,void* storage) {
    Guard g(mutex); if(!source || storage) return nullptr; auto o=make(Object::sound);
    // Preserve source PCM rather than pretending a resampling/ADPCM conversion occurred.
    std::memcpy(o->abi+0x10,source,28); std::memcpy(o->abi+0x2c,source,28);
    insert(currentDictionary,o->abi); return o->abi;
}
static void* __cdecl identity(void* p,const void* guid,const char* name) {
    Guard g(mutex); auto o=find(p); if(!o) return nullptr;
    if(guid) { std::memcpy(o->guid,guid,16); field<void*>(p,0)=o->guid; field<Word>(p,8)|=1; }
    if(name) { o->name=name; field<Word>(p,8)|=2; }
    field<const char*>(p,4)=o->name.c_str(); return p;
}
static void* __cdecl upload(void* p,unsigned offset,const void* data,unsigned bytes) {
    Guard g(mutex); auto o=find(p); if(!o || !data || offset>64*1024*1024 || bytes>64*1024*1024-offset) return nullptr;
    if(o->pcm.size()<offset+bytes) o->pcm.resize(offset+bytes); std::memcpy(o->pcm.data()+offset,data,bytes); return p;
}
static int __cdecl loaded(void*) { return 0; } // synchronous copy; caller frees its source after this
static Word __cdecl guid(Word* out) { GUID id{}; if(FAILED(CoCreateGuid(&id))) return 0; std::memcpy(out,&id,16); return out[3]; }
static void* dispatch(void* p,int parameter,const Word* value) {
    auto c=find(p); if(!c) return nullptr; auto o=c->kind==Object::parameter?c->owner:c; if(!o || !value) return nullptr;
    switch(parameter) {
    case 0: std::memcpy(o->abi+0x58,value,12); break;
    case 2: field<Word>(o->abi,0x70)=*value; break;
    case 3: field<Word>(o->abi,0x74)=*value; break;
    case 4:
        field<Word>(o->abi,0x3c)=*value;
        o->time.set_rate(GetTickCount(),byte_rate(o));
        audio::selected_backend().frequency(o->native,frequency(o)); refresh(o); break;
    case 5: field<Word>(o->abi,0x40)=*value; audio::selected_backend().gain(o->native,masterGain*field<float>(o->abi,0x40)); break;
    case 6: if(*value) start(o); else stop(o); break;
    case 7: o->time.offset=*value; field<Word>(o->abi,0x48)=*value; if(o->time.active) start(o); break;
    case 8:
        o->time.set_loop(GetTickCount(),*value!=0);
        if(*value) field<Word>(o->abi,0x34)|=4; else field<Word>(o->abi,0x34)&=~4u;
        if(o->time.active) {
            audio::selected_backend().play(o->native,o->time.loop,o->time.offset);
        }
        break;
    case 11: field<Word>(o->abi,0x88)=*value; break;
    default: return nullptr;
    } return p;
}
static void* __cdecl command(void* p,int id,int mode,const Word* args) { Guard g(mutex); return mode==1?dispatch(p,id,args):nullptr; }
static void* __cdecl parameter(void* p,int id,int mode,...) { Guard g(mutex); va_list ap; va_start(ap,mode); Word word=va_arg(ap,Word); va_end(ap); return mode==1?dispatch(p,id,&word):nullptr; }
static int __cdecl query(void* p,int id,int fallback,Word* valid) { Guard g(mutex); auto c=find(p); auto o=c && c->kind==Object::parameter?c->owner:c; if(valid) *valid=o!=nullptr; if(!o) return fallback; refresh(o); if(id==6) return o->time.active; if(id==7) return field<Word>(o->abi,0x48); if(valid) *valid=0; return fallback; }
// Encoding/conversion is deliberately an honest failure until a bank codec is available.
static void* __cdecl write_entry(void*,void*,void*,void*,int) { return nullptr; }
CSF_GLOBAL(Word, bankAlignment, 0x008BF5E0);
static void* __cdecl read_entry(void* stream,void*,void*,void*,unsigned flags) {
    Guard g(mutex);
    // Only the observed game path (flags=0, formats selected from bank) is supported.
    if(!stream || flags) return nullptr;
    Word header[3];
    if(boundary_stream_read(stream,header,12)!=12 || header[0]!=2051 || header[1]>64*1024*1024) return nullptr;
    std::vector<unsigned char> meta(header[1]);
    if(boundary_stream_read(stream,meta.data(),header[1])!=int(header[1])) return nullptr;
    audio::BankEntry e;
    if(!audio::parse_bank_metadata(meta.data(),meta.size(),bankAlignment,e)) return nullptr;
    if(boundary_stream_read(stream,header,12)!=12 || header[0]!=2052 || header[1]>64*1024*1024) return nullptr;
    // WAV loader 0070F630 identifies this registered codec as 16-bit PCM.
    // GUID values in bank metadata are identities, not original class pointers.
    auto pcm_codec=[&](const audio::BankFormat& f) {
        return f.codec && !std::memcmp(meta.data()+f.codec,reinterpret_cast<const void*>(CSF_ADDR_DATA(0x00883C0C)),16);
    };
    bool pcm=pcm_codec(e.source) && pcm_codec(e.target) &&
        e.source.word(0)==e.target.word(0) && e.source.raw[12]==e.target.raw[12] &&
        e.source.raw[13]==e.target.raw[13] && e.target.word(8)==header[1] &&
        (e.target.raw[12]==8 || e.target.raw[12]==16);
    std::vector<unsigned char> data;
    if(pcm) data.resize(header[1]);
    // Consume the entire payload even without a decoder. Don't expose compressed
    // bytes as PCM, and don't leave the stream at the next entry's data chunk.
    unsigned char discard[4096];
    for(Word remaining=header[1],pos=0;remaining;) {
        Word n=remaining>sizeof(discard)?sizeof(discard):remaining;
        if(boundary_stream_read(stream,pcm?data.data()+pos:discard,n)!=int(n)) return nullptr;
        remaining-=n; pos+=n;
    }
    auto o=make(Object::sound); o->metadata=std::move(meta); o->pcm=std::move(data);
    auto store=[&](const audio::BankFormat& f,unsigned offset) {
        std::memcpy(o->abi+offset,f.raw,28);
        field<void*>(o->abi,offset+4)=f.codec?o->metadata.data()+f.codec:nullptr;
        field<void*>(o->abi,offset+16)=f.extra?o->metadata.data()+f.extra:nullptr;
    };
    store(e.source,0x10); store(e.target,0x2c);
    identity(o->abi,e.guid?o->metadata.data()+e.guid:nullptr,e.name?reinterpret_cast<const char*>(o->metadata.data()+e.name):"");
    field<Word>(o->abi,0x54)=e.voiceFlags;
    insert(currentDictionary,o->abi); return o->abi;
}
}

HOOK(0x007DB830,close_engine,"boundary-rwa");
HOOK(0x007DB8B0,update,"boundary-rwa");
HOOK(0x007DBBA0,create_dict,"boundary-rwa");
HOOK(0x007DBC20,destroy_dict,"boundary-rwa");
HOOK(0x007DBD90,insert,"boundary-rwa");
HOOK(0x007DBE40,remove_all,"boundary-rwa");
HOOK(0x007DBE90,get_current,"boundary-rwa");
HOOK(0x007DC410,defaults,"boundary-rwa");
HOOK(0x007DC630,lookup_class,"boundary-rwa");
HOOK(0x007DD4C0,create_listener,"boundary-rwa");
HOOK(0x007DE320,prepare,"boundary-rwa");
HOOK(0x007DE4E0,initialize,"boundary-rwa");
HOOK(0x007DEF60,release,"boundary-rwa");
HOOK(0x007DF2B0,create_voice,"boundary-rwa");
HOOK(0x007DF700,activate,"boundary-rwa");
HOOK(0x007DF740,attach,"boundary-rwa");
HOOK(0x007DFBD0,identity,"boundary-rwa");
HOOK(0x007DFF20,upload,"boundary-rwa");
HOOK(0x007E0170,loaded,"boundary-rwa");
HOOK(0x007E0200,write_entry,"boundary-rwa");
HOOK(0x007E0540,read_entry,"boundary-rwa");
HOOK(0x007E0D10,guid,"boundary-rwa");
HOOK(0x007E1320,command,"boundary-rwa");
HOOK(0x007E1380,parameter,"boundary-rwa");
HOOK(0x007E13D0,query,"boundary-rwa");
HOOK(0x007E1420,create_parameter,"boundary-rwa");
HOOK(0x007EE4C0,create_channel,"boundary-rwa");
HOOK(0x007EF3C0,detach_device,"boundary-rwa");
HOOK(0x007EFD80,create_device,"boundary-rwa");
HOOK(0x007F0E80,device_gain,"boundary-rwa");
HOOK(0x007F0EE0,register_listener,"boundary-rwa");
HOOK(0x007F0F00,device_mode,"boundary-rwa");
HOOK(0x007F0F30,speakers,"boundary-rwa");
HOOK(0x007F1850,create_sound,"boundary-rwa");
// Audio helpers misclassified as rw. These must share the adapter's objects.
HOOK(0x007DB6F0,open_engine,"boundary-rwa");
HOOK(0x007DB820,is_open,"boundary-rwa");
HOOK(0x007DB920,initialize,"boundary-rwa");
HOOK(0x007DB960,initialize,"boundary-rwa");
HOOK(0x007DBD20,named,"boundary-rwa");
HOOK(0x007DBD70,entry,"boundary-rwa");
HOOK(0x007DBD80,count,"boundary-rwa");
HOOK(0x007DBE70,get_default,"boundary-rwa");
HOOK(0x007DBE80,set_current,"boundary-rwa");
HOOK(0x007DE550,visit_active,"boundary-rwa");
HOOK(0x007DE590,visit_all,"boundary-rwa");
HOOK(0x007F17C0,initialize,"boundary-rwa");
HOOK(0x007F1810,initialize,"boundary-rwa");
#endif
