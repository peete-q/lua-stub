
#ifndef __LUASTUB_OBJECT_H__
#define __LUASTUB_OBJECT_H__

#include <assert.h>

#include "state.h"
#include "helper.h"

namespace luastub
{
	class stack_object;
	class object;
	
	class stack_holder : public stack_ref
	{
	public:
		class indexer
		{
		public:
			inline indexer(const stack_holder &other) : m_index(other.m_index) {}
			inline operator int() {return m_index;}
		private:
			int m_index;
		};
	public:
		inline stack_holder() : stack_ref(NULL, 0) {}
		inline stack_holder(state *state, int index) : stack_ref(state, state->absindex(index)) {}
		inline void push() const {m_state->pushvalue(m_index);}
		inline void pop() const {m_state->remove(m_index);}
	};

	class registry_holder
	{
	public:
		class indexer
		{
		public:
			inline indexer(const registry_holder &other)
			{
				m_state = other.m_state;
				m_state->rawgeti(LUA_REGISTRYINDEX, other.m_ref);
				m_index = m_state->gettop();
			}
			inline ~indexer() {m_state->settop(m_index);}
			inline operator int() {return m_index;}
		private:
			state *m_state;
			int m_index;
		};
	public:
		inline registry_holder() : m_state(NULL), m_refcount(NULL), m_ref(LUA_REFNIL)
		{
		}
		inline registry_holder(state *state, int index) : m_state(NULL), m_refcount(NULL), m_ref(LUA_REFNIL)
		{
			bind(state, index);
		}
		inline ~registry_holder()
		{
			release();
		}
		inline void push() const
		{
			m_state->rawgeti(LUA_REGISTRYINDEX, m_ref);
		}
		inline void bind(state *state, int index)
		{
			release();
			if(state)
			{
				m_state = state;
				m_state->pushvalue(index);
				m_ref = m_state->ref(LUA_REGISTRYINDEX);
				m_refcount = new int(1);
				assert(m_ref != LUA_REFNIL);
			}
			else
			{
				m_state = NULL;
				m_refcount = NULL;
				m_ref = LUA_REFNIL;
			}
		}
		inline void ref(const registry_holder& other)
		{
			release();
			m_state = other.m_state;
			m_ref = other.m_ref;
			m_refcount = other.m_refcount;
			if (m_refcount)
				++(*m_refcount);
		}
		inline void release()
		{
			if (m_refcount && --(*m_refcount) == 0)
			{
				delete m_refcount;
				m_state->unref(LUA_REGISTRYINDEX, m_ref);
				m_refcount = NULL;
				m_state = NULL;
				m_ref = LUA_REFNIL;
			}
		}
	
	protected:
		friend class indexer;
		friend class object;
		
		state *m_state;
		int *m_refcount;
		int m_ref;
	};

	template<typename holder>
	class basic_object : public holder
	{
	public:
		typedef holder base_type;
		typedef typename base_type::indexer indexer_type;
	public:
		basic_object() : base_type(NULL, 0)
		{
		}
		basic_object(state *state, int index) : base_type(state, index)
		{
		}
		state *getstate() const
		{
			return this->m_state;
		}
		lua_State *getcstate() const
		{
			return this->m_state ? this->m_state->cptr() : NULL;
		}
		bool valid() const
		{
			return this->m_state != NULL;
		}
		bool isnumber() const
		{
			indexer_type index(*this);
			return this->m_state->isnumber(index);
		}
		bool isstring() const
		{
			indexer_type index(*this);
			return this->m_state->isstring(index);
		}
		bool iscfunction() const
		{
			indexer_type index(*this);
			return this->m_state->iscfunction(index);
		}
		bool isuserdata() const
		{
			indexer_type index(*this);
			return this->m_state->isuserdata(index);
		}
		bool isfunction() const
		{
			indexer_type index(*this);
			return this->m_state->isfunction(index);
		}
		bool istable() const
		{
			indexer_type index(*this);
			return this->m_state->istable(index);
		}
		bool islightuserdata() const
		{
			indexer_type index(*this);
			return this->m_state->islightuserdata(index);
		}
		bool isnil() const
		{
			indexer_type index(*this);
			return this->m_state->isnil(index);
		}
		bool isboolean() const
		{
			indexer_type index(*this);
			return this->m_state->isboolean(index);
		}
		bool isthread() const
		{
			indexer_type index(*this);
			return this->m_state->isthread(index);
		}
		int type() const
		{
			indexer_type index(*this);
			return this->m_state->type(index);
		}
		
		bool rawequal(const basic_object &object) const
		{
			indexer_type index1(*this);
			indexer_type index2(object.*this);
			return this->m_state->rawequal(index1, index2);
		}
		int compare(const basic_object &object) const
		{
			indexer_type index1(*this);
			indexer_type index2(object.*this);
			return this->m_state->compare(index1, index2);
		}
		
		lua_Number tonumber() const
		{
			indexer_type index(*this);
			return this->m_state->tonumber(index);
		}
		lua_Integer tointeger() const
		{
			indexer_type index(*this);
			return this->m_state->tointeger(index);
		}
		bool toboolean() const
		{
			indexer_type index(*this);
			return this->m_state->toboolean(index);
		}
		const char *tolstring(size_t *len) const
		{
			indexer_type index(*this);
			return this->m_state->tolstring(index, len);
		}
		const char *tostring() const
		{
			indexer_type index(*this);
			return this->m_state->tostring(index);
		}
		size_t objlen() const
		{
			indexer_type index(*this);
			return this->m_state->objlen(index);
		}
		lua_CFunction tocfunction() const
		{
			indexer_type index(*this);
			return this->m_state->tocfunction(index);
		}
		void *touserdata() const
		{
			indexer_type index(*this);
			return this->m_state->touserdata(index);
		}
		lua_State *tothread() const
		{
			indexer_type index(*this);
			return this->m_state->tothread(index);
		}
		const void *topointer() const
		{
			indexer_type index(*this);
			return this->m_state->topointer(index);
		}
		template<typename T>
		T *to() const
		{
			return (T*)touserdata();
		}
		
		template<typename K, typename V>
		void set(K key, V value)
		{
			indexer_type index(*this);
			luastub::push(this->m_state->cptr(), key);
			luastub::push(this->m_state->cptr(), value);
			this->m_state->settable(index);
		}
		template<typename K, typename V>
		void rawset(K key, V value)
		{
			indexer_type index(*this);
			luastub::push(this->m_state->cptr(), key);
			luastub::push(this->m_state->cptr(), value);
			this->m_state->rawset(index);
		}
		template<typename T>
		void setmetatable(const T &value) const
		{
			indexer_type index(*this);
			value.push();
			this->m_state->setmetatable(index);
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
			indexer_type index(*this);
			return isstring() && strcmp(tostring(), value) == 0;
		}
		template<typename T>
		bool operator ==(T value) const
		{
			indexer_type index(*this);
			return match<T>(this->m_state, index) && read<T>(this->m_state, index) == value;
		}
		template<typename T>
		bool operator ==(const basic_object<T> &value) const
		{
			if (valid() && value.valid())
			{
				indexer_type index(*this);
				typename basic_object<T>::indexer_type index2(value.m_holer);
				return this->m_state->equal(index, index2);
			}
			return !valid() && !value.valid();
		}
		
		// get
		template<typename K>
		basic_object get(K key) const
		{
			indexer_type index(*this);
			luastub::push(this->m_state->cptr(), key);
			this->m_state->gettable(index);
			return basic_object(this->m_state, this->m_state->gettop());
		}
		template<typename K>
		basic_object rawget(K key) const
		{
			indexer_type index(*this);
			luastub::push(this->m_state->cptr(), key);
			this->m_state->rawget(index);
			return basic_object(this->m_state, this->m_state->gettop());
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
			stack_protector sp(this->m_state);
			_Def(key);
			this->m_state->pushlightuserdata((void*)&(((Callee*)0)->*var));
			this->m_state->pushcclosure(&member_variable_dispatcher<Callee, Var>::setter, 1);
			this->m_state->rawseti(-2, 2);
		}
		template <typename Callee, typename Var>
		void def_getter(const char *key, Var Callee:: *var)
		{
			stack_protector sp(this->m_state);
			_Def(key);
			this->m_state->pushlightuserdata((void*)&(((Callee*)0)->*var));
			this->m_state->pushcclosure(&member_variable_dispatcher<Callee, Var>::getter, 1);
			this->m_state->rawseti(-2, 1);
		}
		template <typename Var>
		void def_setter(const char *key, Var *var)
		{
			stack_protector sp(this->m_state);
			_Def(key);
			this->m_state->pushlightuserdata(var);
			this->m_state->pushcclosure(&global_variable_dispatcher<Var>::setter, 1);
			this->m_state->rawseti(-2, 2);
		}
		template <typename Var>
		void def_getter(const char *key, Var *var)
		{
			stack_protector sp(this->m_state);
			_Def(key);
			this->m_state->pushlightuserdata(var);
			this->m_state->pushcclosure(&global_variable_dispatcher<Var>::getter, 1);
			this->m_state->rawseti(-2, 1);
		}
		
	private:
		template<typename Function>
		void _Def(const char *key, lua_CFunction cclosure, Function func, int nupvalues)
		{
			stack_protector sp(this->m_state);
			unsigned char *buffer = (unsigned char *)this->m_state->newuserdata(sizeof(func));
			memcpy(buffer, &func, sizeof(func));
			this->m_state->insert(-1 - nupvalues);
			stack_object other = this->m_state->pushcclosure(cclosure, nupvalues + 1);
			set(key, other);
		}
		template<typename Callee, typename Method>
		void _Def(const char *key, lua_CFunction cclosure, Callee *callee, Method method, int nupvalues)
		{
			stack_protector sp(this->m_state);
			unsigned char *buffer = (unsigned char *)this->m_state->newuserdata(sizeof(callee) + sizeof(method));
			memcpy(buffer, &callee, sizeof(callee));
			memcpy(buffer + sizeof(callee), &method, sizeof(method));
			this->m_state->insert(-1 - nupvalues);
			stack_object other = this->m_state->pushcclosure(cclosure, nupvalues + 1);
			set(key, other);
		}
		void _Def(const char *key)
		{
			this->push();
			property_support(this->m_state->cptr(), this->m_state->gettop());
			this->m_state->pushstring("__props");
			this->m_state->rawget(-2);
			this->m_state->pushstring(key);
			this->m_state->rawget(-2);
			if (this->m_state->isnil(-1))
			{
				this->m_state->pop(1);
				this->m_state->newtable();
				this->m_state->pushstring(key);
				this->m_state->pushvalue(-2);
				this->m_state->rawset(-4);
			}
		}

	protected:
		friend class stack_object;
		friend class object;
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
		stack_object(const stack_ref &ref) : basic_object(ref.m_state, ref.m_index)
		{
		}
		stack_object(const basic_object<stack_holder> &other) : basic_object(other.m_state, other.m_index)
		{
		}
		stack_object &operator = (const stack_ref &ref)
		{
			m_state = ref.m_state;
			m_index = ref.m_index;
		}
		stack_object &operator = (const basic_object<stack_holder> &other)
		{
			m_state = other.m_state;
			m_index = other.m_index;
		}
	private:
		stack_object(const basic_object<registry_holder> &other);
	};

	class object : public basic_object<registry_holder>
	{
	public:
		object() : basic_object()
		{
		}
		object(state *state, int index)
		{
			bind(state, index);
		}
		object(const object &other)
		{
			ref(other);
		}
		object(const stack_object &other)
		{
			bind(other.m_state, other.m_index);
		}
		object(const stack_ref &ref)
		{
			bind(ref.m_state, ref.m_index);
			this->m_state->pop(1);
		}
		object(const basic_object<stack_holder> &other)
		{
			bind(other.m_state, other.m_index);
		}
		object(const basic_object<registry_holder> &other)
		{
			ref(other);
		}
		object &operator = (const basic_object<registry_holder> &other)
		{
			ref(other);
			return *this;
		}
		object &operator = (const object &other)
		{
			ref(other);
			return *this;
		}
		object &operator = (const stack_object &other)
		{
			bind(other.m_state, other.m_index);
		}
		object &operator = (const stack_ref &ref)
		{
			bind(ref.m_state, ref.m_index);
		}
	};

	class table_iterator
	{
	public:
		class e_invalid_iterator {};
	public:
		template<typename T>
		table_iterator(T other)
		{
			m_valid = true;
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
			if (valid())
			{
				m_state->settop(m_top + 2);
				if (m_state->next(m_top + 1))
					return;
				m_valid = false;
				m_state->settop(m_top);
			}
		}
		bool valid()
		{
			return m_valid;
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
			return stack_object(m_state, m_top + 2);
		}
		stack_object value()
		{
			if (!valid())
				throw e_invalid_iterator();
			return stack_object(m_state, m_top + 3);
		}
	private:
		table_iterator& operator=(const table_iterator &);
		table_iterator(const table_iterator &);
	private:
		state *m_state;
		int m_top;
		bool m_valid;
	};
	
	//
	template<>
	struct any_type <stack_object>
	{
		static void push(lua_State *L, const stack_object &value)
		{
			value.push();
		}
		static bool match(lua_State *L, int idx)
		{
			return true;
		}
		static stack_object read(lua_State *L, int idx)
		{
			return stack_object(state::from(L), idx);
		}
	};
	template<>
	struct any_type <object>
	{
		static void push(lua_State *L, const object &value)
		{
			value.push();
		}
		static bool match(lua_State *L, int idx)
		{
			return true;
		}
		static object read(lua_State *L, int idx)
		{
			return object(state::from(L), idx);
		}
	};
}
#endif