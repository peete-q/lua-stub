#ifndef __LUASTUB_CCLASS_H__
#define __LUASTUB_CCLASS_H__

#include "state.h"

namespace luastub
{
	// ctor
	template<typename P1=void, typename P2=void, typename P3=void, typename P4=void>
	struct ctor
	{
		template<typename T>
		static T* construct(state *L)
		{
			return new(L->newuserdata(sizeof(T))) T(check<P1>(L,1), check<P2>(L,2), check<P3>(L,3), check<P4>(L,4));
		}
	};

	template<typename P1, typename P2, typename P3>
	struct ctor<P1, P2, P3>
	{
		template<typename T>
		static T* construct(state *L)
		{
			return new(L->newuserdata(sizeof(T))) T(check<P1>(L,1), check<P2>(L,2), check<P3>(L,3));
		}
	};


	template<typename P1, typename P2>
	struct ctor<P1, P2>
	{
		template<typename T>
		static T* construct(state *L)
		{
			return new(L->newuserdata(sizeof(T))) T(check<P1>(L,1), check<P2>(L,2));
		}
	};

	template<typename P1>
	struct ctor<P1>
	{
		template<typename T>
		static T* construct(state *L)
		{
			return new(L->newuserdata(sizeof(T))) T(check<P1>(L,1));
		}
	};

	template<>
	struct ctor<void>
	{
		template<typename T>
		static T* construct(state *L)
		{
			return new(L->newuserdata(sizeof(T))) T();
		}
	};

	template<typename T>
	class cclass
	{
	public:
		struct e_metatable_already_exist
		{
		};
	public:
		static cclass query(state *L, const char *name)
		{
			L->pushstring(name);
			return cclass(L->gettable(LUA_REGISTRYINDEX));
		}
		
		template<typename Constructor>
		cclass(state *L, const char *name, Constructor)
		{
			stack_protector sp(L);
			stack_object g = L->getglobals();
			if (!L->newmetatable(name))
				throw e_metatable_already_exist();
			m_metatable = L->getstack(-1);
			g.def(name, &__ctor<Constructor>, 1);
			L->pushstring(name);
			m_metatable.def("__gc", &__gc, 1);
			m_metatable.set("__index", m_metatable);
		}
		cclass &inherit(const char *name)
		{
			state *L = m_metatable.getstate();
			stack_protector sp(L);
			cclass cl = query(L, name);
			stack_object mt = L->newtable();
			mt.set("__index", cl.m_metatable);
			m_metatable.setmetatable(mt);
			return *this;
		}
		template<typename Func>
		cclass &def(const char *key, Func func, int nupvalues = 0)
		{
			m_metatable.def<Func>(key, func, nupvalues);
			return *this;
		}
		cclass &def(const char *key, int (*func)(state*), int nupvalues = 0)
		{
			m_metatable.def(key, func, nupvalues);
			return *this;
		}
		template<typename Method>
		cclass &def_method(const char *key, Method method, int nupvalues = 0)
		{
			m_metatable.def_method<T, Method>(key, method, nupvalues);
			return *this;
		}
		cclass &def_method(const char *key, int (T::*method)(state*), int nupvalues = 0)
		{
			m_metatable.def_method<T>(key, method, nupvalues);
			return *this;
		}
		template <typename Var>
		cclass &def_property(const char *key, Var T:: *var)
		{
			m_metatable.def_property<T, Var>(key, var);
			return *this;
		}
		template <typename Var>
		cclass &def_property(const char *key, Var *var)
		{
			m_metatable.def_property<Var>(key, var);
			return *this;
		}
		template <typename Var>
		cclass &def_setter(const char *key, Var T:: *var)
		{
			m_metatable.def_setter<T, Var>(key, var);
			return *this;
		}
		template <typename Var>
		cclass &def_getter(const char *key, Var T:: *var)
		{
			m_metatable.def_getter<T, Var>(key, var);
			return *this;
		}
		template <typename Var>
		cclass &def_setter(const char *key, Var *var)
		{
			m_metatable.def_setter<Var>(key, var);
			return *this;
		}
		template <typename Var>
		cclass &def_getter(const char *key, Var *var)
		{
			m_metatable.def_getter<Var>(key, var);
			return *this;
		}
		stack_object newobject()
		{
			state *L = m_metatable.getstate();
			T *ptr = new (L->newuserdata(sizeof(T))) T();
			stack_object o(L, -1);
			o.setmetatable(m_metatable);
			return o;
		}
		template<typename P1>
		stack_object newobject(P1 p1)
		{
			state *L = m_metatable.getstate();
			T *ptr = new (L->newuserdata(sizeof(T))) T(p1);
			stack_object o(L, -1);
			o.setmetatable(m_metatable);
			return o;
		}
		template<typename P1, typename P2>
		stack_object newobject(P1 p1, P2 p2)
		{
			state *L = m_metatable.getstate();
			T *ptr = new (L->newuserdata(sizeof(T))) T(p1, p2);
			stack_object o(L, -1);
			o.setmetatable(m_metatable);
			return o;
		}
		template<typename P1, typename P2, typename P3>
		stack_object newobject(P1 p1, P2 p2, P3 p3)
		{
			state *L = m_metatable.getstate();
			T *ptr = new (L->newuserdata(sizeof(T))) T(p1, p2, p3);
			stack_object o(L, -1);
			o.setmetatable(m_metatable);
			return o;
		}
		template<typename P1, typename P2, typename P3, typename P4>
		stack_object newobject(P1 p1, P2 p2, P3 p3, P4 p4)
		{
			state *L = m_metatable.getstate();
			T *ptr = new (L->newuserdata(sizeof(T))) T(p1, p2, p3, p4);
			stack_object o(L, -1);
			o.setmetatable(m_metatable);
			return o;
		}
		stack_object boxptr(T* ptr)
		{
			state *L = m_metatable.getstate();
			stack_object o = L->newtable();
			o.set("__ptr", ptr);
			o.setmetatable(m_metatable);
			stack_object r = L->getregistry();
			r.set(ptr, ptr);
			return o;
		}
		void unboxptr(T* ptr)
		{
			state *L = m_metatable.getstate();
			stack_object r = L->getregistry();
			r.set(ptr, nil);
		}
		object &getmetatable()
		{
			return m_metatable;
		}
	private:
		cclass(const stack_object &o) : m_metatable(o)
		{
		}
		template<typename Constructor>
		static int __ctor(state *L)
		{
			int args = L->gettop();
			stack_object mt = L->upvalue(2);
			T *(*ctor)(state*) = &Constructor::construct;
			T *ptr = (*ctor)(L);
			stack_object o(mt.getstate(), -1);
			o.setmetatable(mt);
			return 1;
		}
		static int __gc(state *L)
		{
			stack_object name = L->upvalue(2);
			if (T *ptr = (T*)L->checkudata(1, name.tostring()))
				ptr->~T();
			return 0;
		}
	private:
		object m_metatable;
	};
}

#endif