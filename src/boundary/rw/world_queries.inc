// Independent spatial boundary: CPU PODs only, no Rx/native owning headers.
CSF_FUNC(rw::Sphere*,__cdecl,queryAtomicSphere,0x0058EC60,(Atomic*));
struct SectorTie { Link link; Atomic* atomic; };
using AtomicCB=Atomic* (__cdecl *)(Atomic*,void*);
Sector* __cdecl sectorAtomics(Sector* s,AtomicCB cb,void* data) {
    if(!s || !cb) return nullptr;
    Link* l=s->atomics.next;
    while(l!=&s->atomics) {
        // The original inserts a cursor AFTER the current tie. Removing the
        // current or next atomic in the callback must not leave a stale next.
        SectorTie cursor{{l->next,l},nullptr};
        l->next->prev=&cursor.link;l->next=&cursor.link;
        auto* a=reinterpret_cast<SectorTie*>(l)->atomic;
        bool keep=!a || cb(a,data)!=nullptr;
        l=cursor.link.next;remove(cursor.link);
        if(!keep) break;
    }
    return s;
}
uint32_t __cdecl pvsProgress(World* w) {
    pvsCurrentWorld=w;
    uint32_t result=0;
    if(w && pvsWorldOffset>=0) std::memcpy(&result,reinterpret_cast<char*>(w)+pvsWorldOffset,4);
    return result;
}
uint32_t __cdecl getLtStyle() { return ltMapStyle; }
Texture* lightMap(void* object,int offset) {
    Texture* texture=nullptr;
    if(offset>=0) std::memcpy(&texture,static_cast<char*>(object)+offset+20,sizeof(texture));
    return texture;
}
Atomic* __cdecl collectAtomicMap(Atomic* a,void* data) {
    auto& maps=*static_cast<std::vector<Texture*>*>(data);
    auto* t=lightMap(a,ltMapAtomicOffset);
    if(t && std::find(maps.begin(),maps.end(),t)==maps.end()) maps.push_back(t);
    return a;
}
Sector* __cdecl collectSectorMaps(Sector* s,void* data) {
    auto& maps=*static_cast<std::vector<Texture*>*>(data);
    auto* t=lightMap(s,ltMapSectorOffset);
    if(t && std::find(maps.begin(),maps.end(),t)==maps.end()) maps.push_back(t);
    sectorAtomics(s,collectAtomicMap,data);return s;
}
std::vector<Texture*> lightMaps(World* w) {
    std::vector<Texture*> maps;
    if(plugins(Registry::Sector).getPluginOffset(276)>=0)
        worldSectors(w,collectSectorMaps,&maps);
    return maps;
}
int __cdecl queryLightMaps(World* w) { return int(lightMaps(w).size()); }
int __cdecl setLtStyle(uint32_t style,World* w) {
    // Original changes the low byte of texture filterAddressing (+0x50),
    // preserving U/V addressing. It counts distinct sector/tied-atomic maps.
    uint32_t filter=(style&4)?1:(ltMapRasterFormat&0x8000)?6:2;
    for(auto* t:lightMaps(w)) {
        t->filterAddressing=(t->filterAddressing&0xffffff00)|filter;
        if(auto* n=textureNative(t)) n->filterAddressing=t->filterAddressing;
    }
    ltMapStyle=style;return 1;
}
using SectorIntersectionCB=Sector* (__cdecl *)(Intersection*,Sector*,void*);
bool sphereBox(const rw::Sphere& s,const rw::BBox& b) {
    float d=0;
    for(int i=0;i<3;++i) {
        float p=reinterpret_cast<const float*>(&s.center)[i];
        float lo=reinterpret_cast<const float*>(&b.inf)[i],hi=reinterpret_cast<const float*>(&b.sup)[i];
        float v=p<lo?lo-p:p>hi?p-hi:0;d+=v*v;
    }
    return d<=s.radius*s.radius;
}
bool tieAtomic(World* world,Atomic* atomic) {
    std::vector<Sector*> sectors;
    if(atomic->geometry && atomic->object.object.parent) {
        auto* sphere=queryAtomicSphere(atomic);if(!sphere) return false;
        struct Context { const rw::Sphere* sphere;std::vector<Sector*>* sectors; } context{sphere,&sectors};
        worldSectors(world,[](Sector* sector,void* data)->Sector* {
            auto& c=*static_cast<Context*>(data);if(sphereBox(*c.sphere,sector->bounds)) c.sectors->push_back(sector);return sector;
        },&context);
    }
    auto& old=atomicTies[atomic];
    if(old.size()==sectors.size()) {
        bool same=true;for(size_t i=0;i<old.size();++i) if(old[i]->sector!=sectors[i]) same=false;
        if(same) return true;
    }
    std::vector<std::unique_ptr<WorldTie>> fresh;fresh.reserve(sectors.size());
    for(auto* sector:sectors) { auto t=std::make_unique<WorldTie>();t->atomic=atomic;t->sector=sector;fresh.push_back(std::move(t)); }
    for(auto& t:old) { remove(t->sectorLink);remove(t->atomicLink); }
    old.swap(fresh);
    for(auto& t:old) { prepend(t->sector->atomics,t->sectorLink);prepend(atomic->worldSectors,t->atomicLink); }
    return true;
}
World* __cdecl sectorIntersections(World* w,Intersection* q,SectorIntersectionCB cb,void* data) {
    if(!worlds.count(w) || !q || !cb) return nullptr;
    if(q->type<1 || q->type>5) return w;
    Intersection local=*q;
    if(q->type==5) { auto* s=queryAtomicSphere(q->atomic);if(!s) return w;local.sphere=*s;local.type=3; }
    struct Visit { Sector* sector;float enter=0,leave=1; };
    std::vector<Visit> stack{{w->root}};
    while(!stack.empty()) {
        auto v=stack.back();stack.pop_back();auto* s=v.sector;
        if(s->type<0) {
            // Line/point/box walkers only prune BSP planes. They do NOT reject
            // the root leaf against the world box. Sphere adds a leaf-box test.
            if((local.type!=3 || sphereBox(local.sphere,s->bounds)) && !cb(q,s,data)) break;
        }
        else {
            auto* p=reinterpret_cast<Plane*>(s);
            int axis=p->type/4;
            if(local.type==1) {
                float start=reinterpret_cast<float*>(&local.line.start)[axis],end=reinterpret_cast<float*>(&local.line.end)[axis],delta=end-start;
                float epsilon=std::max(std::fabs(start),std::fabs(end))*1.e-6f;
                float left=p->leftValue+epsilon,right=p->rightValue-epsilon;
                float a=start+delta*v.enter,b=start+delta*v.leave;
                bool both=delta<=0?(left>=b && right<=a):(right<=b && left>=a);
                if(!both) {
                    bool chooseLeft=delta<=0?left>=b:right>b;
                    stack.push_back({chooseLeft?p->left:p->right,v.enter,v.leave});
                } else {
                    Visit l{p->left,v.enter,v.leave},r{p->right,v.enter,v.leave};
                    if(delta>0) { l.leave=std::min(l.leave,(left-start)/delta);r.enter=std::max(r.enter,(right-start)/delta); }
                    if(delta<0) { l.enter=std::max(l.enter,(left-start)/delta);r.leave=std::min(r.leave,(right-start)/delta); }
                    stack.push_back(delta<=0?l:r);stack.push_back(delta<=0?r:l);
                }
            } else {
                float inf,sup;
                if(local.type==2) inf=sup=reinterpret_cast<float*>(&local.point)[axis];
                else if(local.type==3) {
                    float center=reinterpret_cast<float*>(&local.sphere.center)[axis];inf=center-local.sphere.radius;sup=center+local.sphere.radius;
                } else { inf=reinterpret_cast<float*>(&local.box.inf)[axis];sup=reinterpret_cast<float*>(&local.box.sup)[axis]; }
                if(p->rightValue<=sup) {
                    stack.push_back({p->right});if(p->leftValue>=inf) stack.push_back({p->left});
                } else stack.push_back({p->left});
            }
        }
    }
    return w;
}
int __cdecl lineSphere(const rw::V3d* line,const rw::Sphere* sphere,float* distance) {
    auto delta=rw::sub(line[1],line[0]);float squared=rw::dot(delta,delta);
    float inverse=squared?1/std::sqrt(squared):0;
    auto direction=rw::scale(delta,inverse),offset=rw::sub(sphere->center,line[0]);
    float along=rw::dot(offset,direction),d2=rw::dot(offset,offset),r2=sphere->radius*sphere->radius;
    // Deliberately a forward RAY test: original never clamps at segment end.
    // Output is projected centre distance / segment length, not entry time.
    if(d2>=r2 && (along<=0 || r2-(d2-along*along)<0)) return 0;
    *distance=along*inverse;return 1;
}
int __cdecl lineTriangle(const rw::V3d* start,const rw::V3d* delta,const rw::V3d* a,const rw::V3d* b,const rw::V3d* c,float* distance) {
    auto e1=rw::sub(*b,*a),e2=rw::sub(*c,*a),p=rw::cross(*delta,e2);
    float det=rw::dot(p,e1);if(det<=1.e-8f) return 0;
    float low=det*-1.e-5f,high=det-low;
    auto s=rw::sub(*start,*a);float u=rw::dot(s,p);if(u<low || u>high) return 0;
    auto q=rw::cross(s,e1);float v=rw::dot(q,*delta);if(v<low || u+v>high) return 0;
    float t=rw::dot(q,e2);*distance=t;
    if(t<low || t>high) return 0;
    *distance=t/det;return 1;
}
rw::V3d closestTriangle(const rw::V3d& p,const rw::V3d& a,const rw::V3d& b,const rw::V3d& c) {
    // Independent Voronoi-region formulation, not library source.
    auto ab=rw::sub(b,a),ac=rw::sub(c,a),ap=rw::sub(p,a);
    float d1=rw::dot(ab,ap),d2=rw::dot(ac,ap);
    if(d1<=0 && d2<=0) return a;
    auto bp=rw::sub(p,b);float d3=rw::dot(ab,bp),d4=rw::dot(ac,bp);
    if(d3>=0 && d4<=d3) return b;
    float vc=d1*d4-d3*d2;
    if(vc<=0 && d1>=0 && d3<=0) return rw::add(a,rw::scale(ab,d1/(d1-d3)));
    auto cp=rw::sub(p,c);float d5=rw::dot(ab,cp),d6=rw::dot(ac,cp);
    if(d6>=0 && d5<=d6) return c;
    float vb=d5*d2-d1*d6;
    if(vb<=0 && d2>=0 && d6<=0) return rw::add(a,rw::scale(ac,d2/(d2-d6)));
    float va=d3*d6-d5*d4;
    if(va<=0 && d4-d3>=0 && d5-d6>=0) return rw::add(b,rw::scale(rw::sub(c,b),(d4-d3)/((d4-d3)+(d5-d6))));
    float inv=1/(va+vb+vc);
    return rw::add(a,rw::add(rw::scale(ab,vb*inv),rw::scale(ac,vc*inv)));
}
int __cdecl sphereTriangle(const rw::Sphere* s,const rw::V3d* a,const rw::V3d* b,const rw::V3d* c,rw::V3d* normal,float* distance) {
    for(int i=0;i<3;++i) {
        float x=reinterpret_cast<const float*>(a)[i]-reinterpret_cast<const float*>(&s->center)[i];
        float y=reinterpret_cast<const float*>(b)[i]-reinterpret_cast<const float*>(&s->center)[i];
        float z=reinterpret_cast<const float*>(c)[i]-reinterpret_cast<const float*>(&s->center)[i];
        // Positive-side axial tangency is excluded by the original quick reject.
        if((x>=0 && y>=0 && z>=0 && x>=s->radius && y>=s->radius && z>=s->radius) ||
           (x<0 && y<0 && z<0 && x<-s->radius && y<-s->radius && z<-s->radius)) return 0;
    }
    *normal=rw::cross(rw::sub(*b,*a),rw::sub(*c,*a));
    float squared=rw::dot(*normal,*normal);if(squared<=0) return 0;
    *normal=rw::scale(*normal,1/std::sqrt(squared));
    float plane=rw::dot(*normal,rw::sub(*a,s->center));
    if(plane<-s->radius || plane>s->radius) return 0;
    *distance=-plane;
    auto delta=rw::sub(s->center,closestTriangle(s->center,*a,*b,*c));
    return rw::dot(delta,delta)<=s->radius*s->radius;
}
bool boxTriangle(const rw::BBox& box,const rw::V3d& a,const rw::V3d& b,const rw::V3d& c) {
    auto center=rw::scale(rw::add(box.sup,box.inf),.5f),extent=rw::scale(rw::sub(box.sup,box.inf),.5f);
    rw::V3d v[3]={rw::sub(a,center),rw::sub(b,center),rw::sub(c,center)};
    rw::V3d edges[3]={rw::sub(v[1],v[0]),rw::sub(v[2],v[1]),rw::sub(v[0],v[2])};
    auto separated=[&](rw::V3d axis) {
        float p=rw::dot(v[0],axis),q=rw::dot(v[1],axis),r=rw::dot(v[2],axis);
        float radius=extent.x*std::fabs(axis.x)+extent.y*std::fabs(axis.y)+extent.z*std::fabs(axis.z);
        return std::min({p,q,r})>radius || std::max({p,q,r})<-radius;
    };
    for(auto axis:{rw::makeV3d(1,0,0),rw::makeV3d(0,1,0),rw::makeV3d(0,0,1)}) {
        if(separated(axis)) return false;
        for(auto edge:edges) if(separated(rw::cross(edge,axis))) return false;
    }
    return !separated(rw::cross(edges[0],edges[1]));
}
using WorldCollisionCB=CollisionTriangle* (__cdecl *)(Intersection*,Sector*,CollisionTriangle*,float,void*);
using AtomicCollisionCB=CollisionTriangle* (__cdecl *)(Intersection*,CollisionTriangle*,float,void*);
bool triangleHit(Intersection* query,rw::V3d* a,rw::V3d* b,rw::V3d* c,CollisionTriangle& hit,float& distance) {
    if(query->type==1) {
        auto delta=rw::sub(query->line.end,query->line.start);
        if(!lineTriangle(&query->line.start,&delta,a,b,c,&distance)) return false;
        auto n=rw::cross(rw::sub(*b,*a),rw::sub(*c,*a));float len=std::sqrt(rw::dot(n,n));
        hit.normal=len?rw::scale(n,1/len):n;
    } else if(query->type==3) {
        if(!sphereTriangle(&query->sphere,a,b,c,&hit.normal,&distance)) return false;
        distance/=query->sphere.radius;
    } else if(query->type==4) {
        if(!boxTriangle(query->box,*a,*b,*c)) return false;
        auto n=rw::cross(rw::sub(*b,*a),rw::sub(*c,*a));float len=std::sqrt(rw::dot(n,n));hit.normal=len?rw::scale(n,1/len):n;distance=0;
    } else return false;
    hit.point=*a;hit.vertices[0]=a;hit.vertices[1]=b;hit.vertices[2]=c;return true;
}
struct WorldCollisionContext { Intersection* original;WorldCollisionCB callback;void* data; };
Sector* __cdecl collideSector(Intersection* query,Sector* sector,void* data) {
    auto& ctx=*static_cast<WorldCollisionContext*>(data);
    for(unsigned i=0;i<sector->numTriangles;++i) {
        auto& t=sector->triangles[i];CollisionTriangle hit{};hit.index=i;float distance;
        if(triangleHit(query,sector->vertices+t.v[0],sector->vertices+t.v[1],sector->vertices+t.v[2],hit,distance) &&
           !ctx.callback(ctx.original,sector,&hit,distance,ctx.data)) return nullptr;
    }
    return sector;
}
World* __cdecl worldCollision(World* world,Intersection* query,WorldCollisionCB cb,void* data) {
    if(!worlds.count(world) || !query || !cb) return nullptr;
    if(query->type!=1 && query->type!=3 && query->type!=4 && query->type!=5) return world;
    Intersection local=*query;
    if(query->type==5) { auto* s=queryAtomicSphere(query->atomic);if(!s) return world;local.sphere=*s;local.type=3; }
    WorldCollisionContext ctx{query,cb,data};sectorIntersections(world,&local,collideSector,&ctx);return world;
}
Atomic* __cdecl atomicCollision(Atomic* atomic,Intersection* query,AtomicCollisionCB cb,void* data) {
    if(!atomic || !query || !cb) return nullptr;
    if(query->type!=1 && query->type!=3 && query->type!=5) return atomic;
    auto* frame=frameNative(static_cast<Frame*>(atomic->object.object.parent));
    auto* geometry=geometryNative(atomic->geometry);if(!frame || !geometry || !geometry->numMorphTargets) return atomic;
    Intersection local=*query;rw::Matrix inverse;rw::Matrix::invert(&inverse,frame->getLTM());
    if(query->type==1) rw::V3d::transformPoints(&local.line.start,&query->line.start,2,&inverse);
    else {
        auto* sphere=query->type==5?queryAtomicSphere(query->atomic):&query->sphere;if(!sphere) return atomic;
        rw::V3d::transformPoints(&local.sphere.center,&sphere->center,1,&inverse);
        local.sphere.radius=std::sqrt(rw::dot(inverse.at,inverse.at))*sphere->radius;local.type=3;
    }
    int first=atomic->interpolator.start,last=atomic->interpolator.end;
    if(first<0 || last<0 || first>=geometry->numMorphTargets || last>=geometry->numMorphTargets) first=last=0;
    auto* vertices=geometry->morphTargets[first].vertices;if(!vertices) return atomic;
    std::vector<rw::V3d> morphed;
    if(first!=last) {
        auto* end=geometry->morphTargets[last].vertices;if(!end) return atomic;
        float f=atomic->interpolator.position*atomic->interpolator.reciprocalTime;morphed.resize(geometry->numVertices);
        for(int i=0;i<geometry->numVertices;++i) morphed[i]=rw::add(vertices[i],rw::scale(rw::sub(end[i],vertices[i]),f));
        vertices=morphed.data();
    }
    for(int i=0;i<geometry->numTriangles;++i) {
        auto& t=geometry->triangles[i];CollisionTriangle hit{};hit.index=i;float distance;
        if(triangleHit(&local,vertices+t.v[0],vertices+t.v[1],vertices+t.v[2],hit,distance) && !cb(query,&hit,distance,data)) break;
    }
    return atomic;
}
Camera* __cdecl cameraSectors(Camera* camera,SectorCB cb,void* data) {
    if(!camera || !cb || cameraWorldOffset<0) return nullptr;
    auto* plugin=reinterpret_cast<char*>(camera)+cameraWorldOffset;
    Sector** sectors=nullptr;uint32_t count=0;
    std::memcpy(&sectors,plugin,sizeof(sectors));std::memcpy(&count,plugin+8,4);
    for(uint32_t i=0;i<count;++i) if(!cb(sectors[i],data)) break;
    return camera;
}
using WorldAtomicCB=Atomic* (__cdecl *)(Intersection*,Sector*,Atomic*,float,void*);
struct AtomicIntersectionContext {
    Intersection* query;Sector* sector;WorldAtomicCB callback;void* data;
    std::unordered_set<Atomic*> seen;bool stopped=false;
};
Atomic* __cdecl intersectAtomic(Atomic* a,void* data) {
    auto& c=*static_cast<AtomicIntersectionContext*>(data);auto* q=c.query;
    if(!(a->object.object.flags&1) || (q->type==5 && q->atomic==a) || !c.seen.insert(a).second) return a;
    auto* s=queryAtomicSphere(a);if(!s) return a;
    float distance=0;bool hit=false;
    if(q->type==1) {
        auto delta=rw::sub(q->line.end,q->line.start),offset=rw::sub(s->center,q->line.start);
        float squared=rw::dot(delta,delta);distance=squared?rw::dot(delta,offset)/squared:0;
        auto closest=rw::add(q->line.start,rw::scale(delta,std::clamp(distance,0.0f,1.0f)));
        auto separation=rw::sub(s->center,closest);hit=rw::dot(separation,separation)<=s->radius*s->radius;
    } else if(q->type==4) hit=sphereBox(*s,q->box);
    else {
        rw::Sphere other;
        if(q->type==2) other={q->point,0};
        else if(q->type==3) other=q->sphere;
        else if(q->type==5) { auto* sphere=queryAtomicSphere(q->atomic);if(!sphere) return a;other=*sphere; }
        else return a;
        auto delta=rw::sub(s->center,other.center);float squared=rw::dot(delta,delta),radius=s->radius+other.radius;
        hit=squared<=radius*radius;distance=radius?std::sqrt(squared)/radius:0;
    }
    if(hit && !c.callback(q,c.sector,a,distance,c.data)) { c.stopped=true;return nullptr; }
    return a;
}
Sector* __cdecl intersectSectorAtomics(Intersection*,Sector* sector,void* data) {
    auto& c=*static_cast<AtomicIntersectionContext*>(data);c.sector=sector;
    sectorAtomics(sector,intersectAtomic,data);return c.stopped?nullptr:sector;
}
CSF_FUNC(int,__stdcall,synchronizeWorldFrames,0x006327D0,());
World* __cdecl worldAtomicIntersections(World* world,Intersection* query,WorldAtomicCB cb,void* data) {
    if(!worlds.count(world) || !query || !cb) return nullptr;
    synchronizeWorldFrames();
    AtomicIntersectionContext context{query,nullptr,cb,data};
    sectorIntersections(world,query,intersectSectorAtomics,&context);return world;
}
