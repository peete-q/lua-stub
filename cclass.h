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
		static T *construct(state *L)
		{
			return new(L->newuserdata(sizeof(T))) T(check<P1>(L,1), check<P2>(L,2), check<P3>(L,3), check<P4>(L,4));
		}
	};

	template<typename P1, typename P2, typename P3>
	struct ctor<P1, P2, P3>
	{
		template<typename T>
		static T *construct(state *L)
		{
			return new(L->newuserdata(sizeof(T))) T(check<P1>(L,1), check<P2>(L,2), check<P3>(L,3));
		}
	};

	template<typename P1, typename P2>
	struct ctor<P1, P2>
	{
		template<typename T>
		static T *construct(state *L)
		{
			return new(L->newuserdata(sizeof(T))) T(check<P1>(L,1), check<P2>(L,2));
		}
	};

	template<typename P1>
	struct ctor<P1>
	{
		template<typename T>
		static T *construct(state *L)
		{
			return new(L->newuserdata(sizeof(T))) T(check<P1>(L,1));
		}
	};

	template<>
	struct ctor<void>
	{
		template<typename T>
		static T *construct(state *L)
		{
			return new(L->newuserdata(sizeof(T))) T();
		}
	};

	template<typename T>
	class cclass
	{
	public:
		struct e_name_already_exist {};
		struct e_type_already_exist {};
	public:
		cclass(state *L)
		{
			const char *name = cclass_manager::instance()->get<T>();
			if (name)
			{
				m_metatable = L->getregistry(name);
			}
		}
		bool isnone() const
		{
			return m_metatable.isnone();
		}
		template<typename Constructor>
		cclass(state *L, const char* name, Constructor = ctor<void>())
		{
			if (cclass_manager::instance()->get<T>())
				throw e_type_already_exist();
			stack_protector sp(L);
			object _G = L->getglobals();
			if (!L->newmetatable(name))
				throw e_name_already_exist();
			m_metatable = L->getstack(-1);
			m_metatable.push();
			_G.def(name, &__ctor<Constructor>, 1);
			L->pushstring(name);
			m_metatable.def("__gc", &__gc, 1);
			L->pushstring(name);
			m_metatable.def("__tostring", &__tostring, 1);
			m_metatable.set("__index", m_metatable);
			cclass_manager::instance()->add<T>(name);
		}
        cclass &inherit(const cclass &c)
		{
			state *L = m_metatable.getstate();
			object meta = L->newtable();
			meta.set("__index", c.m_metatable); 
			m_metatable.setmetatable(meta);
			return *this;
		}
        cclass &inherit(const object &o)
		{
			state *L = m_metatable.getstate();
			object meta = L->newtable();
			meta.set("__index", o); 
			m_metatable.setmetatable(meta);
			return *this;
		}
		cclass &inherit(const char *name)
		{
			state *L = m_metatable.getstate();
			object R = L->getregistrys();
			object meta = L->newtable();
			meta.set("__index", R.get(name));
			m_metatable.setmetatable(meta);
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
		object newobject()
		{
			state *L = m_metatable.getstate();
			T *ptr = new (L->newuserdata(sizeof(T))) T();
			object o = L->getstack(-1);
			o.setmetatable(m_metatable);
			return o;
		}
		object newobject(const T &value)
		{
			state *L = m_metatable.getstate();
			T *ptr = new (L->newuserdata(sizeof(T))) T(value);
			object o = L->getstack(-1);
			o.setmetatable(m_metatable);
			return o;
		}
		template<typename P1>
		object newobject(P1 p1)
		{
			state *L = m_metatable.getstate();
			T *ptr = new (L->newuserdata(sizeof(T))) T(p1);
			object o = L->getstack(-1);
			o.setmetatable(m_metatable);
			return o;
		}
		template<typename P1, typename P2>
		object newobject(P1 p1, P2 p2)
		{
			state *L = m_metatable.getstate();
			T *ptr = new (L->newuserdata(sizeof(T))) T(p1, p2);
			object o = L->getstack(-1);
			o.setmetatable(m_metatable);
			return o;
		}
		template<typename P1, typename P2, typename P3>
		object newobject(P1 p1, P2 p2, P3 p3)
		{
			state *L = m_metatable.getstate();
			T *ptr = new (L->newuserdata(sizeof(T))) T(p1, p2, p3);
			object o = L->getstack(-1);
			o.setmetatable(m_metatable);
			return o;
		}
		template<typename P1, typename P2, typename P3, typename P4>
		object newobject(P1 p1, P2 p2, P3 p3, P4 p4)
		{
			state *L = m_metatable.getstate();
			T *ptr = new (L->newuserdata(sizeof(T))) T(p1, p2, p3, p4);
			object o = L->getstack(-1);
			o.setmetatable(m_metatable);
			return o;
		}
		object boxptr(T *ptr)
		{
			state *L = m_metatable.getstate();
			object R = L->getregistrys();
			object o = R.get(lightuserdata<T>(ptr));
			if (o != nil)
				return o;
			
			o = L->newtable();
			L->pushlightuserdata(ptr);
			o.def("__self", &__self, 1);
			o.setmetatable(m_metatable);
			R.set(lightuserdata<T>(ptr), o);
			return o;
		}
		void unboxptr(T *ptr)
		{
			state *L = m_metatable.getstate();
			object R = L->getregistrys();
			R.set(lightuserdata<T>(ptr), nil);
		}
	private:
		cclass() {}
		cclass(const object &o) : m_metatable(o) {}
		
		template<typename Constructor>
		static int __ctor(state *L)
		{
			T *(*ctor)(state*) = &Constructor::construct;
			T *ptr = (*ctor)(L);
			L->pushvalue(upvalueindex(1));
			L->setmetatable(-2);
			return 1;
		}
		static int __gc(state *L)
		{
			if (T *ptr = (T*)L->checkudata(1, L->tostring(upvalueindex(1))))
				ptr->~T();
			return 0;
		}
		static int __tostring(state *L)
		{
			L->pushfstring("object{cclass:%s} (%s: %p)", L->tostring(upvalueindex(1)), L->typename_of(1), L->topointer(1));
			return 1;
		}
		static int __self(state *L)
		{
			L->pushvalue(upvalueindex(1));
			L->gettable(LUA_REGISTRYINDEX);
			if (L->isnil(-1))
			{
				L->pop(1);
				L->pushnil();
				return 1;
			}
			L->pop(1);
			L->pushvalue(upvalueindex(1));
			return 1;
		}
	private:
		object m_metatable;
	};
}

#endif
