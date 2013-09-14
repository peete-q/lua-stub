
#ifndef __LUASTUB_OBJECT_H__
#define __LUASTUB_OBJECT_H__

#include "assert.h"
#include "string.h"
#include "state.h"
#include "helper.h"

namespace luastub
{
	class stack_holder
	{
	public:
		stack_holder();
		stack_holder(state *state, int index);
	private:
		friend class stack_indexer;
		int m_value;
	};

	class registry_holder
	{
	public:
		registry_holder();
		registry_holder(state *state, int index);
		~registry_holder();
		void bind(state *state, int index);
	private:
		friend class registry_indexer;
		state *m_state;
		int m_value;
	};

	class stack_indexer
	{
	public:
		stack_indexer(const stack_holder &other);
		operator int();
	private:
		int m_value;
	};

	class registry_indexer
	{
	public:
		registry_indexer(const registry_holder &other);
		~registry_indexer();
		operator int();
	private:
		state *m_state;
		int m_value;
	};

	template<typename Indexer, typename Holder>
	class basic_object
	{
	public:
		typedef Indexer indexer_t;
		typedef Holder holder_t;
	public:
		basic_object() : m_state(0)
		{
		}
		basic_object(state *state, int index) : m_state(state), m_holder(state, index)
		{
		}
		state *getstate() const
		{
			return m_state;
		}
		lua_State *getcstate() const
		{
			return m_state ? m_state->cptr() : 0;
		}
		void push()
		{
			Indexer index(m_holder);
			m_state->pushvalue(index);
		}
		bool isnumber() const
		{
			Indexer index(m_holder);
			return m_state->isnumber(index);
		}
		bool isstring() const
		{
			Indexer index(m_holder);
			return m_state->isstring(index);
		}
		bool iscfunction() const
		{
			Indexer index(m_holder);
			return m_state->iscfunction(index);
		}
		bool isuserdata() const
		{
			Indexer index(m_holder);
			return m_state->isuserdata(index);
		}
		bool isfunction() const
		{
			Indexer index(m_holder);
			return m_state->isfunction(index);
		}
		bool istable() const
		{
			Indexer index(m_holder);
			return m_state->istable(index);
		}
		bool islightuserdata() const
		{
			Indexer index(m_holder);
			return m_state->islightuserdata(index);
		}
		bool isnil() const
		{
			Indexer index(m_holder);
			return m_state->isnil(index);
		}
		bool isboolean() const
		{
			Indexer index(m_holder);
			return m_state->isboolean(index);
		}
		bool isthread() const
		{
			Indexer index(m_holder);
			return m_state->isthread(index);
		}
		int type() const
		{
			Indexer index(m_holder);
			return m_state->type(index);
		}
		
		bool rawequal(const basic_object &object) const
		{
			Indexer index1(m_holder);
			Indexer index2(object.m_holder);
			return m_state->rawequal(index1, index2);
		}
		int compare(const basic_object &object) const
		{
			Indexer index1(m_holder);
			Indexer index2(object.m_holder);
			return m_state->compare(index1, index2);
		}
		
		lua_Number tonumber() const
		{
			Indexer index(m_holder);
			return m_state->tonumber(index);
		}
		lua_Integer tointeger() const
		{
			Indexer index(m_holder);
			return m_state->tointeger(index);
		}
		bool toboolean() const
		{
			Indexer index(m_holder);
			return m_state->toboolean(index);
		}
		const char *tolstring(size_t *len) const
		{
			Indexer index(m_holder);
			return m_state->tolstring(index, len);
		}
		const char *tostring() const
		{
			Indexer index(m_holder);
			return m_state->tostring(index);
		}
		size_t objlen() const
		{
			Indexer index(m_holder);
			return m_state->objlen(index);
		}
		lua_CFunction tocfunction() const
		{
			Indexer index(m_holder);
			return m_state->tocfunction(index);
		}
		void *touserdata() const
		{
			Indexer index(m_holder);
			return m_state->touserdata(index);
		}
		lua_State *tothread() const
		{
			Indexer index(m_holder);
			return m_state->tothread(index);
		}
		const void *topointer() const
		{
			Indexer index(m_holder);
			return m_state->topointer(index);
		}
		template<typename T>
		T *toobject() const
		{
			return *(T**)touserdata();
		}
		
		void set(int key, nil_t)
		{
			Indexer index(m_holder);
			m_state->pushnumber(key);
			m_state->pushnil();
			m_state->settable(index);
		}
		void set(const char *key, nil_t)
		{
			Indexer index(m_holder);
			m_state->pushstring(key);
			m_state->pushnil();
			m_state->settable(index);
		}
		template<typename K>
		void set(K key, nil_t)
		{
			Indexer index(m_holder);
			key.push();
			m_state->pushnil();
			m_state->settable(index);
		}
		void set(int key, bool value)
		{
			Indexer index(m_holder);
			m_state->pushnumber(key);
			m_state->pushboolean(value);
			m_state->settable(index);
		}
		void set(const char *key, bool value)
		{
			Indexer index(m_holder);
			m_state->pushstring(key);
			m_state->pushboolean(value);
			m_state->settable(index);
		}
		void set(int key, lua_Number value)
		{
			Indexer index(m_holder);
			m_state->pushnumber(key);
			m_state->pushnumber(value);
			m_state->settable(index);
		}
		void set(const char *key, lua_Number value)
		{
			Indexer index(m_holder);
			m_state->pushstring(key);
			m_state->pushnumber(value);
			m_state->settable(index);
		}
		template<typename K>
		void set(K key, lua_Number value)
		{
			Indexer index(m_holder);
			key.push();
			m_state->pushnumber(value);
			m_state->settable(index);
		}
		void set(int key, const char *value)
		{
			Indexer index(m_holder);
			m_state->pushnumber(key);
			m_state->pushstring(value);
			m_state->settable(index);
		}
		void set(const char *key, const char *value)
		{
			Indexer index(m_holder);
			m_state->pushstring(key);
			m_state->pushstring(value);
			m_state->settable(index);
		}
		template<typename K>
		void set(K key, const char *value)
		{
			Indexer index(m_holder);
			key.push();
			m_state->pushstring(value);
			m_state->settable(index);
		}
		template<typename V>
		void set(int key, V *value)
		{
			Indexer index(m_holder);
			m_state->pushnumber(key);
			m_state->pushlightuserdata(value);
			m_state->settable(index);
		}
		template<typename V>
		void set(const char *key, V *value)
		{
			Indexer index(m_holder);
			m_state->pushstring(key);
			m_state->pushlightuserdata(value);
			m_state->settable(index);
		}
		template<typename K, typename V>
		void set(K key, V *value)
		{
			Indexer index(m_holder);
			key.push();
			m_state->pushlightuserdata(value);
			m_state->settable(index);
		}
		template<typename V>
		void set(int key, V value)
		{
			Indexer index(m_holder);
			m_state->pushnumber(key);
			value.push();
			m_state->settable(index);
		}
		template<typename V>
		void set(const char *key, V value)
		{
			Indexer index(m_holder);
			m_state->pushstring(key);
			value.push();
			m_state->settable(index);
		}
		template<typename K, typename V>
		void set(K key, V value)
		{
			Indexer index(m_holder);
			key.push();
			value.push();
			m_state->settable(index);
		}
		basic_object get(int key) const
		{
			Indexer index(m_holder);
			m_state->pushnumber(key);
			m_state->gettable(index);
			return basic_object(m_state, m_state->gettop());
		}
		basic_object get(const char *key) const
		{
			Indexer index(m_holder);
			m_state->pushstring(key);
			m_state->gettable(index);
			return basic_object(m_state, m_state->gettop());
		}
		template<typename K>
		basic_object get(K key) const
		{
			Indexer index(m_holder);
			key.push();
			m_state->gettable(index);
			return basic_object(m_state, m_state->gettop());
		}
		void *newuserdata(int key, size_t size)
		{
			Indexer index(m_holder);
			m_state->pushnumber(key);
			void *ptr = m_state->newuserdata(size);
			m_state->settable(index);
			return ptr;
		}
		void *newuserdata(const char *key, size_t size)
		{
			Indexer index(m_holder);
			m_state->pushstring(key);
			void *ptr = m_state->newuserdata(size);
			m_state->settable(index);
			return ptr;
		}
		template<typename K>
		void *newuserdata(K key, size_t size)
		{
			Indexer index(m_holder);
			key.push();
			void *ptr = m_state->newuserdata(size);
			m_state->settable(index);
			return ptr;
		}
		basic_object newtable(int key)
		{
			Indexer index(m_holder);
			m_state->pushnumber(key);
			m_state->newtable();
			m_state->settable(index);
			return basic_object(m_state, m_state->gettop());
		}
		basic_object newtable(const char *key)
		{
			Indexer index(m_holder);
			m_state->pushstring(key);
			m_state->newtable();
			m_state->settable(index);
			return basic_object(m_state, m_state->gettop());
		}
		template<typename K>
		basic_object newtable(K key)
		{
			Indexer index(m_holder);
			key.push();
			m_state->newtable();
			m_state->settable(index);
			return basic_object(m_state, m_state->gettop());
		}
		template<typename V>
		void setmetatable(V value) const
		{
			Indexer index(m_holder);
			value.push();
			m_state->setmetatable(index);
		}
		//
		basic_object operator [](int key) const
		{
			return get(key);
		}
		basic_object operator [](const char *key) const
		{
			return get(key);
		}
		template<typename K>
		basic_object operator [](K key) const
		{
			return get(key);
		}
		bool operator ==(int value) const
		{
			return isnumber() && tonumber() == value;
		}
		bool operator ==(bool value) const
		{
			return isboolean() && toboolean() == value;
		}
		bool operator ==(const char *value) const
		{
			return isstring() && strcmp(tostring(), value) == 0;
		}
		template<typename V>
		bool operator ==(V value) const
		{
			Indexer index1(m_holder);
			typename V::indexer_t index2(value.m_holder);
			return m_state->equal(index1, index2);
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
			stack_protector sp(m_state);
			unsigned char *buffer = (unsigned char *)m_state->newuserdata(sizeof(func));
			memcpy(buffer, &func, sizeof(func));
			m_state->insert(-1 - nupvalues);
			stack_object o = m_state->pushcclosure(cclosure, nupvalues + 1);
			set(key, o);
		}
		template<typename Callee, typename Method>
		void _Def(const char *key, lua_CFunction cclosure, Callee *callee, Method method, int nupvalues)
		{
			stack_protector sp(m_state);
			unsigned char *buffer = (unsigned char *)m_state->newuserdata(sizeof(callee) + sizeof(method));
			memcpy(buffer, &callee, sizeof(callee));
			memcpy(buffer + sizeof(callee), &method, sizeof(method));
			m_state->insert(-1 - nupvalues);
			stack_object o = m_state->pushcclosure(cclosure, nupvalues + 1);
			set(key, o);
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

	protected:
		friend class stack_object;
		friend class object;
		
		state *m_state;
		Holder m_holder;
	};

	class stack_object : public basic_object<stack_indexer, stack_holder>
	{
	public:
		stack_object() : basic_object()
		{
		}
		stack_object(state *state, int index) : basic_object(state, index)
		{
		}
		stack_object(const basic_object<stack_indexer, stack_holder> &o)
		{
			m_state = o.m_state;
			m_holder = o.m_holder;
		}
	private:
		stack_object(const basic_object<registry_indexer, registry_holder> &o);
	};

	class object : public basic_object<registry_indexer, registry_holder>
	{
	public:
		object() : basic_object()
		{
		}
		object(state *state, int index) : basic_object(state, index)
		{
		}
		object(const object &o)
		{
			assign(o);
		}
		template<typename T>
		object(const T &o)
		{
			assign(o);
		}
		template<typename T>
		object &operator = (const T &o)
		{
			assign(o);
		}
		template<typename T>
		void assign(const T &o)
		{
			typename T::indexer_t index(o.m_holder);
			m_state = o.m_state;
			m_holder.bind(m_state, index);
		}
	};
	
	class table_iterator
	{
	public:
		class e_invalid_iterator {};
	public:
		template<typename T>
		table_iterator(T o)
		{
			m_state = o.getstate();
			m_top = m_state->gettop();
			o.push();
			m_index = m_state->gettop();
			m_state->pushnil();
			next();
		}
		~table_iterator()
		{
			m_state->settop(m_top);
		}
		bool next()
		{
			if (valid())
			{
				m_state->settop(m_index + 1);
				if (m_state->next(m_index))
					return true;
				else
					m_index = 0;
			}
			return false;
		}
		bool valid()
		{
			return m_index != 0;
		}
		table_iterator &operator ++()
		{
			next();
			return *this;
		}
		operator bool()
		{
			return valid();
		}
		stack_object key()
		{
			if (!valid())
				throw e_invalid_iterator();
			return stack_object(m_state, m_index + 1);
		}
		stack_object value()
		{
			if (!valid())
				throw e_invalid_iterator();
			return stack_object(m_state, m_index + 2);
		}
	private:
		table_iterator& operator=(const table_iterator &);
		table_iterator(const table_iterator &);
	private:
		state *m_state;
		int m_top;
		int m_index;
	};

	// stack_holder
	inline stack_holder::stack_holder() : m_value(0)
	{
	}
	inline stack_holder::stack_holder(state *state, int index) : m_value(state->absindex(index))
	{
	}
	// registry_holder
	inline registry_holder::registry_holder() : m_state(0), m_value(LUA_REFNIL)
	{
	}
	inline registry_holder::registry_holder(state *state, int index)
	{
		bind(state, index);
	}
	inline registry_holder::~registry_holder()
	{
		m_state->unref(LUA_REGISTRYINDEX, m_value);
	}
	inline void registry_holder::bind(state *state, int index)
	{
		m_state = state;
		m_state->pushvalue(m_state->absindex(index));
		m_value = m_state->ref(LUA_REGISTRYINDEX);
		assert(m_value != LUA_REFNIL);
	}
	// stack_indexer
	inline stack_indexer::stack_indexer(const stack_holder &other) : m_value(other.m_value)
	{
	}
	inline stack_indexer::operator int()
	{
		return m_value;
	}
	// registry_indexer
	inline registry_indexer::registry_indexer(const registry_holder &other) : m_state(other.m_state)
	{
		m_state->rawgeti(LUA_REGISTRYINDEX, other.m_value);
		m_value = m_state->gettop();
	}
	inline registry_indexer::~registry_indexer()
	{
		m_state->remove(m_value);
	}
	inline registry_indexer::operator int()
	{
		return m_value;
	}
}
#endif