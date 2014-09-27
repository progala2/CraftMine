#ifndef _XKS_DELEGATE_H_
#define _XKS_DELEGATE_H_
#include "stdafx.h"

template <class ParamType>
class Delegate
{
public:
    Delegate()
        : object_ptr(0), stub_ptr(0)
    {}

	template <class T, void (T::*TMethod)(const ParamType&)>
	static Delegate<ParamType> from_method(T* object_ptr)
    {
		Delegate<ParamType> d;
        d.object_ptr = object_ptr;
        d.stub_ptr = &method_stub<T, TMethod>; // #1
        return d;
    }

    void operator()(ParamType&& a1) const
    {
        return (*stub_ptr)(object_ptr, a1);
    }

	void operator()(const ParamType& a1) const
    {
        return (*stub_ptr)(object_ptr, a1);
    }

private:
    typedef void (*stub_type)(void* object_ptr, const ParamType&);

    void* object_ptr;
    stub_type stub_ptr;

    template <class T, void (T::*TMethod)(const ParamType&)>
    static void method_stub(void* object_ptr, const ParamType& a1)
    {
        T* p = static_cast<T*>(object_ptr);
        return (p->*TMethod)(a1); // #2
    }
};

#endif