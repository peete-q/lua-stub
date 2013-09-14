#ifndef __LUASTUB_CCLASS_H__
#define __LUASTUB_CCLASS_H__

#include "state.h"

namespace luastub
{
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
		
		template<typename Method>
		cclass(state *L, const char* name, Method ctor = 0, int nupvalues = 0)
		{
			stack_protector sp(L);
			stack_object g = L->getglobals();
			if (!L->newmetatable(name))
				throw e_metatable_already_exist();
			m_metatable = L->getstack(-1);
			g.def(name, &__creator, 1);
			if (ctor)
				m_metatable.def_method<T, Method>("__ctor", ctor, nupvalues);
			L->pushstring(name);
			m_metatable.def("__gc", &__gc, 1);
			m_metatable.set("__index", m_metatable);
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
			T **ptr = (T **)L->newuserdata(sizeof(T**));
			*ptr = new T();
			stack_object o(L, -1);
			o.setmetatable(m_metatable);
			return o;
		}
		stack_object newobject(T* ptr)
		{
			state *L = m_metatable.getstate();
			stack_object o = L->newtable();
			o.set("__ptr", ptr);
			o.setmetatable(m_metatable);
			return o;
		}
	private:
		cclass(const stack_object &o) : m_metatable(o)
		{
		}
		static int __creator(state *L)
		{
			int args = L->gettop();
			stack_object mt = L->upvalue(2);
			T **ptr = (T **)L->newuserdata(sizeof(T**));
			*ptr = new T();
			stack_object o(mt.getstate(), -1);
			o.setmetatable(mt);
			{
				stack_protector sp(L);
				stack_object ctor = o.get("__ctor");
				if (!ctor.isnil())
				{
					o.push();
					for (int i = 1; i <= args; ++i)
						L->pushvalue(i);
					L->call(args + 1, 0);
				}
			}
			return 1;
		}
		static int __gc(state *L)
		{
			stack_object name = L->upvalue(2);
			if (T **ptr = (T **)L->checkudata(1, name.tostring()))
			{
				delete *ptr;
				*ptr = 0;
			}
			return 0;
		}
	private:
		object m_metatable;
	};
}

#endif