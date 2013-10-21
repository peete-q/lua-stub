
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
		class indexer
		{
		public:
			indexer(const stack_holder &o) : m_index(o.m_index) {}
			operator int() {return m_index;}
		private:
			int m_index;
		};
	public:
		stack_holder();
		stack_holder(state *state, int index);
		void push() const;
	private:
		friend class stack_indexer;
		friend class registry_holder;
		state *m_state;
		int m_index;
	};

	class registry_holder
	{
	public:
		class indexer
		{
		public:
			indexer(const registry_holder &o);
			~indexer() {m_state->settop(m_index);}
			operator int() {return m_index;}
		private:
			state *m_state;
			int m_index;
		};
	public:
		registry_holder();
		registry_holder(state *state, int index);
		~registry_holder();
		registry_holder &operator =(const stack_holder &other);
		registry_holder &operator =(const registry_holder &other);
		void bind(state *state, int index);
		void push() const;
	private:
		friend class indexer;
		state *m_state;
		int *m_refcount;
		int m_ref;
	};

	template<typename holder>
	class basic_object
	{
	public:
		typedef typename holder::indexer indexer_t;
		typedef holder holder_t;
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
		void push() const
		{
			m_holder.push();
		}
		bool isnumber() const
		{
			indexer_t index(m_holder);
			return m_state->isnumber(index);
		}
		bool isstring() const
		{
			indexer_t index(m_holder);
			return m_state->isstring(index);
		}
		bool iscfunction() const
		{
			indexer_t index(m_holder);
			return m_state->iscfunction(index);
		}
		bool isuserdata() const
		{
			indexer_t index(m_holder);
			return m_state->isuserdata(index);
		}
		bool isfunction() const
		{
			indexer_t index(m_holder);
			return m_state->isfunction(index);
		}
		bool istable() const
		{
			indexer_t index(m_holder);
			return m_state->istable(index);
		}
		bool islightuserdata() const
		{
			indexer_t index(m_holder);
			return m_state->islightuserdata(index);
		}
		bool isnil() const
		{
			indexer_t index(m_holder);
			return m_state->isnil(index);
		}
		bool isboolean() const
		{
			indexer_t index(m_holder);
			return m_state->isboolean(index);
		}
		bool isthread() const
		{
			indexer_t index(m_holder);
			return m_state->isthread(index);
		}
		int type() const
		{
			indexer_t index(m_holder);
			return m_state->type(index);
		}
		
		bool rawequal(const basic_object &object) const
		{
			indexer_t index1(m_holder);
			indexer_t index2(object.m_holder);
			return m_state->rawequal(index1, index2);
		}
		int compare(const basic_object &object) const
		{
			indexer_t index1(m_holder);
			indexer_t index2(object.m_holder);
			return m_state->compare(index1, index2);
		}
		
		lua_Number tonumber() const
		{
			indexer_t index(m_holder);
			return m_state->tonumber(index);
		}
		lua_Integer tointeger() const
		{
			indexer_t index(m_holder);
			return m_state->tointeger(index);
		}
		bool toboolean() const
		{
			indexer_t index(m_holder);
			return m_state->toboolean(index);
		}
		const char *tolstring(size_t *len) const
		{
			indexer_t index(m_holder);
			return m_state->tolstring(index, len);
		}
		const char *tostring() const
		{
			indexer_t index(m_holder);
			return m_state->tostring(index);
		}
		size_t objlen() const
		{
			indexer_t index(m_holder);
			return m_state->objlen(index);
		}
		lua_CFunction tocfunction() const
		{
			indexer_t index(m_holder);
			return m_state->tocfunction(index);
		}
		void *touserdata() const
		{
			indexer_t index(m_holder);
			return m_state->touserdata(index);
		}
		lua_State *tothread() const
		{
			indexer_t index(m_holder);
			return m_state->tothread(index);
		}
		const void *topointer() const
		{
			indexer_t index(m_holder);
			return m_state->topointer(index);
		}
		template<typename T>
		T *toany() const
		{
			return (T*)touserdata();
		}
		
		template<typename K, typename V>
		void set(K key, V value)
		{
			indexer_t index(m_holder);
			luastub::push(m_state->cptr(), key);
			luastub::push(m_state->cptr(), value);
			m_state->settable(index);
		}
		template<typename T>
		void setmetatable(const T &value) const
		{
			indexer_t index(m_holder);
			value.push();
			m_state->setmetatable(index);
		}
		template<typename T>
		bool operator ==(T value) const
		{
			indexer_t index(m_holder);
			return match<T>(m_state, index) && read<T>(m_state, index) == value;
		}
		template<typename T>
		bool operator ==(const basic_object<T> &value) const
		{
			indexer_t index(m_holder);
			typename basic_object<T>::indexer_t index2(value.m_holer);
			return m_state->equal(index, index2);
		}
		
		// get
		template<typename K>
		basic_object get(K key) const
		{
			indexer_t index(m_holder);
			luastub::push(m_state->cptr(), key);
			m_state->gettable(index);
			return basic_object(m_state, m_state->gettop());
		}
		template<typename K>
		basic_object operator [](K key) const
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
		holder_t m_holder;
	};

	class stack_object : public basic_object<stack_holder>
	{
	public:
		stack_object() : basic_object()
		{
		}
		stack_object(state *state, int index) : basic_object(state, index)
		{
		}
		stack_object(const basic_object<stack_holder> &o)
		{
			m_state = o.m_state;
			m_holder = o.m_holder;
		}
	private:
		stack_object(const basic_object<registry_holder> &o);
	};

	class object : public basic_object<registry_holder>
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
			m_state = o.m_state;
			m_holder = o.m_holder;
		}
		template<typename T>
		object(const T &o)
		{
			m_state = o.m_state;
			m_holder = o.m_holder;
		}
		template<typename T>
		object &operator = (const T &o)
		{
			m_state = o.m_state;
			m_holder = o.m_holder;
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
	inline stack_holder::stack_holder() : m_state(0), m_index(0)
	{
	}
	inline stack_holder::stack_holder(state *state, int index) : m_state(state), m_index(state->absindex(index))
	{
	}
	inline void stack_holder::push() const
	{
		m_state->pushvalue(m_index);
	}
	
	// registry_holder
	inline registry_holder::registry_holder() : m_state(0), m_ref(LUA_REFNIL)
	{
	}
	inline registry_holder::registry_holder(state *state, int index)
	{
		bind(state, index);
	}
	inline registry_holder::~registry_holder()
	{
		if (m_refcount && --(*m_refcount) == 0)
		{
			delete m_refcount;
			m_state->unref(LUA_REGISTRYINDEX, m_ref);
			m_refcount = 0;
			m_state = 0;
		}
	}
	inline registry_holder &registry_holder::operator =(const stack_holder &other)
	{
		bind(other.m_state, other.m_index);
		return *this;
	}
	inline registry_holder &registry_holder::operator =(const registry_holder &other)
	{
		m_state = other.m_state;
		m_ref = other.m_ref;
		m_refcount = other.m_refcount;
		if (m_refcount)
			++(*m_refcount);
		return *this;
	}
	inline void registry_holder::bind(state *state, int index)
	{
		m_state = state;
		m_state->pushvalue(index);
		m_ref = m_state->ref(LUA_REGISTRYINDEX);
		m_refcount = new int(1);
		assert(m_ref != LUA_REFNIL);
	}
	inline void registry_holder::push() const
	{
		m_state->rawgeti(LUA_REGISTRYINDEX, m_ref);
		m_state->gettop();
	}
	registry_holder::indexer::indexer(const registry_holder &o) : m_state(o.m_state)
	{
		m_state->rawgeti(LUA_REGISTRYINDEX, o.m_ref);
		m_index = m_state->gettop();
	}
}
#endif