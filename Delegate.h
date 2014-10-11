#ifndef _XKS_DELEGATE_H_
#define _XKS_DELEGATE_H_
#include "stdafx.h"

template<class ParamType>
class Delegate {
 public:
    Delegate()
            : object_ptr(),
              stub_ptr(nullptr) {
    }

    template<class T, void (T::*TMethod)(const ParamType&)>
    static Delegate<ParamType> from_method(std::shared_ptr<T> object_ptr) {
        Delegate<ParamType> d;
        d.object_ptr = object_ptr;
        d.stub_ptr = &method_stub<T, TMethod>;  // #1
        return d;
    }

    void operator()(const ParamType& a1) const {
        (*stub_ptr)(object_ptr, a1);
    }

 private:
    typedef void (*stub_type)(std::weak_ptr<void> object_ptr, const ParamType&);

    std::weak_ptr<void> object_ptr;
    stub_type stub_ptr;

    template<class T, void (T::*TMethod)(const ParamType&)>
    static void method_stub(std::weak_ptr<void> object_ptr, const ParamType& a1) {
        if(auto p = object_ptr.lock()) {
            T* r = static_cast<T*>(p.get());
            (r->*TMethod)(a1);  // #2
        }
    }
};

#endif
