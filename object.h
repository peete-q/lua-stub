
#ifndef __LUASTUB_OBJECT_H__
#define __LUASTUB_OBJECT_H__

#include "state.h"
#include "helper.h"

namespace luastub
{
	class object
	{
	public:
		class indexer
		{
		public:
			inline indexer(const object &other)
			{
				m_state = other.m_state;
				m_state->rawgeti(LUA_REGISTRYINDEX, other.m_ref);
				m_index = m_state->gettop();
			}
			inline ~indexer() {m_state->settop(m_index - 1);}
			inline operator int() {return m_index;}
		private:
			state *m_state;
			int m_index;
		};
	public:
		object() : m_state(NULL), m_refcount(NULL), m_ref(LUA_NOREF)
		{
		}
		object(state *state, int index) : m_state(NULL), m_refcount(NULL), m_ref(LUA_NOREF)
		{
			state->pushvalue(index);
			pop(state, -1);
		}
		object(const object &other) : m_state(NULL), m_refcount(NULL), m_ref(LUA_NOREF)
		{
			*this = other;
		}
		object(const stack_index &other) : m_state(NULL), m_refcount(NULL), m_ref(LUA_NOREF)
		{
			*this = other;
		}
		object &operator =(const object &other)
		{
			release();
			m_state = other.m_state;
			m_ref = other.m_ref;
			m_refcount = other.m_refcount;
			if (m_refcount)
				++(*m_refcount);
			return *this;
		}
		object &operator =(const stack_index &other)
		{
			pop(other.m_state, other.m_index);
			return *this;
		}
		~object()
		{
			release();
		}
		void pop(state *state, int index)
		{
			release();
			
			if(state->type(index) != LUA_TNIL)
			{
				m_state = state;
				m_state->pushvalue(index);
				m_ref = m_state->ref(LUA_REGISTRYINDEX);
				m_refcount = new int(1);
			}
			state->remove(index);
		}
		void push() const
		{
			m_state->rawgeti(LUA_REGISTRYINDEX, m_ref);
		}
		void release()
		{
			if (m_refcount)
			{
				if (--(*m_refcount) == 0)
				{
					delete m_refcount;
					m_state->unref(LUA_REGISTRYINDEX, m_ref);
				}
				m_state = NULL;
				m_refcount = NULL;
				m_ref = LUA_NOREF;
			}
		}
	
		state *getstate() const
		{
			return m_state;
		}
		lua_State *getcstate() const
		{
			return m_state ? m_state->cptr() : NULL;
		}
		bool valid() const
		{
			return m_refcount != NULL;
		}
		bool isnumber() const
		{
			indexer index(*this);
			return m_state->isnumber(index);
		}
		bool isstring() const
		{
			indexer index(*this);
			return m_state->isstring(index);
		}
		bool iscfunction() const
		{
			indexer index(*this);
			return m_state->iscfunction(index);
		}
		bool isuserdata() const
		{
			indexer index(*this);
			return m_state->isuserdata(index);
		}
		bool isfunction() const
		{
			indexer index(*this);
			return m_state->isfunction(index);
		}
		bool istable() const
		{
			indexer index(*this);
			return m_state->istable(index);
		}
		bool islightuserdata() const
		{
			indexer index(*this);
			return m_state->islightuserdata(index);
		}
		bool isnil() const
		{
			if (valid())
				return true;
			indexer index(*this);
			return m_state->isnil(index);
		}
		bool isboolean() const
		{
			indexer index(*this);
			return m_state->isboolean(index);
		}
		bool isthread() const
		{
			indexer index(*this);
			return m_state->isthread(index);
		}
		int type() const
		{
			indexer index(*this);
			return m_state->type(index);
		}
		
		bool rawequal(const object &object) const
		{
			indexer index1(*this);
			indexer index2(object);
			return m_state->rawequal(index1, index2);
		}
		
		lua_Number tonumber() const
		{
			indexer index(*this);
			return m_state->tonumber(index);
		}
		lua_Integer tointeger() const
		{
			indexer index(*this);
			return m_state->tointeger(index);
		}
		bool toboolean() const
		{
			indexer index(*this);
			return m_state->toboolean(index);
		}
		const char *tolstring(size_t *len) const
		{
			indexer index(*this);
			return m_state->tolstring(index, len);
		}
		const char *tostring() const
		{
			indexer index(*this);
			return m_state->tostring(index);
		}
		size_t objlen() const
		{
			indexer index(*this);
			return m_state->objlen(index);
		}
		lua_CFunction tocfunction() const
		{
			indexer index(*this);
			return m_state->tocfunction(index);
		}
		void *touserdata() const
		{
			indexer index(*this);
			return m_state->touserdata(index);
		}
		lua_State *tothread() const
		{
			indexer index(*this);
			return m_state->tothread(index);
		}
		const void *topointer() const
		{
			indexer index(*this);
			return m_state->topointer(index);
		}
		template<typename T>
		T *to() const
		{
			return (T*)touserdata();
		}
		
		template<typename K, typename V>
		void set(K key, V value)
		{
			indexer index(*this);
			luastub::push(m_state->cptr(), key);
			luastub::push(m_state->cptr(), value);
			m_state->settable(index);
		}
		template<typename K, typename V>
		void rawset(K key, V value)
		{
			indexer index(*this);
			luastub::push(m_state->cptr(), key);
			luastub::push(m_state->cptr(), value);
			m_state->rawset(index);
		}
		template<typename T>
		void setmetatable(const T &value) const
		{
			indexer index(*this);
			value.push();
			m_state->setmetatable(index);
		}
		template<typename T>
		bool operator !=(T value) const
		{
			return !(*this == value);
		}
		bool operator ==(const nil_t&) const
		{
			return !valid() || isnil();
		}
		bool operator ==(const char *value) const
		{
			indexer index(*this);
			return isstring() && strcmp(tostring(), value) == 0;
		}
		template<typename T>
		bool operator ==(T value) const
		{
			indexer index(*this);
			return match<T>(m_state, index) && read<T>(m_state, index) == value;
		}
		bool operator ==(const object &value) const
		{
			if (valid() && value.valid())
			{
				indexer index(*this);
				indexer index2(value);
				return m_state->equal(index, index2);
			}
			return !valid() && !value.valid();
		}
		
		// get
		template<typename K>
		object get(K key) const
		{
			indexer index(*this);
			luastub::push(m_state->cptr(), key);
			return m_state->gettable(index);
		}
		template<typename K>
		object rawget(K key) const
		{
			indexer index(*this);
			luastub::push(m_state->cptr(), key);
			return m_state->rawget(index);
		}
		template<typename K>
		object operator [](K key) const
		{
			return get(key);
		}
		// def
		void def(const char *key, int (*func)(state*), int nupvalues = 0)
		{
			_Def(key, state_function_dispatcher, func, nupvalues);
		}
		template<typename Function>
		void def(const char *key, Function func, int nupvalues = 0)
		{
			_Def(key, function_dispatcher<Function>::dispatch, func, nupvalues);
		}
		template<typename Callee, typename Method>
		void def(const char *key, Callee *callee, Method method, int nupvalues = 0)
		{
			_Def(key, method_dispatcher<Callee, Method>::dispatch, callee, method, nupvalues);
		}
		template<typename Callee>
		void def(const char *key, Callee *callee, int (Callee::*method)(state*), int nupvalues = 0)
		{
			_Def(key, state_method_dispatcher<Callee>::dispatch, callee, method, nupvalues);
		}
		template<typename Callee, typename Method>
		void def_method(const char *key, Method method, int nupvalues = 0)
		{
			_Def(key, oo_method_dispatcher<Callee, Method>::dispatch, method, nupvalues);
		}
		template<typename Callee>
		void def_method(const char *key, int (Callee::*method)(state*), int nupvalues = 0)
		{
			_Def(key, oo_state_method_dispatcher<Callee>::dispatch, method, nupvalues);
		}
		template <typename Callee, typename Var>
		void def_property(const char *key, Var Callee:: *var)
		{
			def_setter(key, var);
			def_getter(key, var);
		}
		template <typename Var>
		void def_property(const char *key, Var *var)
		{
			def_setter(key, var);
			def_getter(key, var);
		}
		template <typename Callee, typename Var>
		void def_setter(const char *key, Var Callee:: *var)
		{
			stack_protector sp(m_state);
			_Def(key);
			m_state->pushlightuserdata((void*)&(((Callee*)0)->*var));
			m_state->pushcclosure(&member_variable_dispatcher<Callee, Var>::setter, 1);
			m_state->rawseti(-2, 2);
		}
		template <typename Callee, typename Var>
		void def_getter(const char *key, Var Callee:: *var)
		{
			stack_protector sp(m_state);
			_Def(key);
			m_state->pushlightuserdata((void*)&(((Callee*)0)->*var));
			m_state->pushcclosure(&member_variable_dispatcher<Callee, Var>::getter, 1);
			m_state->rawseti(-2, 1);
		}
		template <typename Var>
		void def_setter(const char *key, Var *var)
		{
			stack_protector sp(m_state);
			_Def(key);
			m_state->pushlightuserdata(var);
			m_state->pushcclosure(&global_variable_dispatcher<Var>::setter, 1);
			m_state->rawseti(-2, 2);
		}
		template <typename Var>
		void def_getter(const char *key, Var *var)
		{
			stack_protector sp(m_state);
			_Def(key);
			m_state->pushlightuserdata(var);
			m_state->pushcclosure(&global_variable_dispatcher<Var>::getter, 1);
			m_state->rawseti(-2, 1);
		}
		
	private:
		template<typename Function>
		void _Def(const char *key, lua_CFunction cclosure, Function func, int nupvalues)
		{
			int top = m_state->gettop() - nupvalues;
			unsigned char *buffer = (unsigned char *)m_state->newuserdata(sizeof(func));
			memcpy(buffer, &func, sizeof(func));
			m_state->insert(-1 - nupvalues);
			object other = m_state->pushcclosure(cclosure, nupvalues + 1);
			set(key, other);
			m_state->settop(top);
		}
		template<typename Callee, typename Method>
		void _Def(const char *key, lua_CFunction cclosure, Callee *callee, Method method, int nupvalues)
		{
			int top = m_state->gettop() - nupvalues;
			unsigned char *buffer = (unsigned char *)m_state->newuserdata(sizeof(callee) + sizeof(method));
			memcpy(buffer, &callee, sizeof(callee));
			memcpy(buffer + sizeof(callee), &method, sizeof(method));
			m_state->insert(-1 - nupvalues);
			object other = m_state->pushcclosure(cclosure, nupvalues + 1);
			set(key, other);
			m_state->settop(top);
		}
		void _Def(const char *key)
		{
			push();
			property_support(m_state->cptr(), m_state->gettop());
			m_state->pushstring("__props");
			m_state->rawget(-2);
			m_state->pushstring(key);
			m_state->rawget(-2);
			if (m_state->isnil(-1))
			{
				m_state->pop(1);
				m_state->newtable();
				m_state->pushstring(key);
				m_state->pushvalue(-2);
				m_state->rawset(-4);
			}
		}
	private:
		state *m_state;
		int *m_refcount;
		int m_ref;
	};

	class table_iterator
	{
	public:
		class e_invalid_iterator {};
	public:
		template<typename T>
		table_iterator(T other)
		{
			m_end = false;
			m_state = other.getstate();
			m_top = m_state->gettop();
			other.push();
			m_state->pushnil();
			next();
		}
		~table_iterator()
		{
			m_state->settop(m_top);
		}
		void next()
		{
			if (!m_end)
			{
				m_state->settop(m_top + 2);
				if (m_state->next(m_top + 1))
					return;
				m_end = true;
				m_state->settop(m_top);
			}
		}
		bool end()
		{
			return m_end;
		}
		table_iterator &operator ++()
		{
			next();
			return *this;
		}
		operator bool()
		{
			return !m_end;
		}
		object key()
		{
			if (m_end)
				throw e_invalid_iterator();
			return object(m_state, m_top + 2);
		}
		object value()
		{
			if (m_end)
				throw e_invalid_iterator();
			return object(m_state, m_top + 3);
		}
	private:
		table_iterator();
		table_iterator(const table_iterator &);
		table_iterator& operator=(const table_iterator &);
	private:
		state *m_state;
		int m_top;
		bool m_end;
	};
	
	//
	template<>
	struct any_type <object>
	{
		static const char *type() {return "object";}
		static void push(lua_State *L, const object &value) {value.push();}
		static bool match(lua_State *L, int index) {return true;}
		static object read(lua_State *L, int index) {return object(state::from(L), index);}
	};
}
#endif