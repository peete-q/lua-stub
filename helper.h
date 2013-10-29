#ifndef __LUASTUB_HELPER_H__
#define __LUASTUB_HELPER_H__

extern "C" 
{
#include "lua.h"
#include "lauxlib.h"
}

#include <cstdlib>
#include <cstring>
#include <string>

#include <typeinfo>
#include <map>

namespace luastub
{
	template<typename T>
	class basic_object;
	template<typename T>
	class cclass;
	
	class cclass_manager
	{
		typedef std::map<const std::type_info*, std::string> TypeNameMap;
		TypeNameMap m_typenames;
	public:
		static cclass_manager *instance()
		{
			static cclass_manager self;
			return &self;
		}
		template<typename T>
		void add(const char *name)
		{
			m_typenames[&typeid(T)] = name;
		}
		template<typename T>
		const char *get()
		{
			TypeNameMap::iterator it = m_typenames.find(&typeid(T));
			if (it != m_typenames.end())
				return it->second.c_str();
			return NULL;
		}
	};
	
	template<typename T>
	struct lightuserdata
	{
		lightuserdata(T *ptr_) : ptr(ptr_) {}
		T *ptr;
	};
	template<typename T>
	struct userdata
	{
		userdata(T &value) : ptr(&value) {}
		T *ptr;
	};
	
	inline bool matchobject(lua_State *L, int index, const char *tname)
	{
		int type = lua_type(L, index);
		if (type == LUA_TUSERDATA || type == LUA_TTABLE)
		{
			if (lua_getmetatable(L, index))			/* does it have a metatable? */
			{
				lua_getfield(L, LUA_REGISTRYINDEX, tname);	/* get correct metatable */
				bool result = lua_rawequal(L, -1, -2);
				lua_pop(L, 2);
				return result;
			}
		}
		return false;
	}
	
	inline void *checkobject(lua_State *L, int index, const char *tname)
	{
		int type = lua_type(L, index);
		if (type == LUA_TUSERDATA)
			return luaL_checkudata(L, index, tname);
		else if (type == LUA_TTABLE) {
			if (!lua_getmetatable(L, index))			/* does it have a metatable? */
				luaL_typerror(L, index, tname);
			lua_getfield(L, LUA_REGISTRYINDEX, tname);	/* get correct metatable */
			if (!lua_rawequal(L, -1, -2))
				luaL_typerror(L, index, tname);
			lua_pop(L, 2);
			lua_pushstring(L, "__ptr");
			lua_rawget(L, index);
			if (!lua_isuserdata(L, -1))
			{
				lua_getglobal(L, "debug");
				lua_getfield(L, -1, "traceback");
				lua_pushfstring(L, "error: object{cclass:%s} (table: %p) has released", tname, lua_topointer(L, 1));
				lua_pushnumber(L, 3);
				lua_call(L, 2, 1);
				lua_error(L);
			}
			void *ptr = lua_touserdata(L, -1);
			lua_pop(L, 1);
			return ptr;
		} else {
			luaL_typerror(L, index, tname);
		}
		return NULL;
	}
	
	template<typename T>
	inline T *readobject(lua_State *L, int index)
	{
		int type = lua_type(L, index);
		if (type == LUA_TUSERDATA)
			return (T*)lua_touserdata(L, index);
		else if (type == LUA_TTABLE) {
			lua_pushstring(L, "__ptr");
			lua_rawget(L, index);
			if (!lua_isuserdata(L, -1))
			{
				const char *tname = cclass_manager::instance()->get<T>();
				lua_getglobal(L, "debug");
				lua_getfield(L, -1, "traceback");
				lua_pushfstring(L, "error: object{cclass:%s} (table: %p) has released", tname, lua_topointer(L, 1));
				lua_pushnumber(L, 3);
				lua_call(L, 2, 1);
				lua_error(L);
			}
			T *ptr = (T*)lua_touserdata(L, -1);
			lua_pop(L, 1);
			return ptr;
		} else {
			luaL_argerror(L, index, "expected userdata or table with __ptr");
		}
		return NULL;
	}

	template<bool C, typename A, typename B> struct if_ {};
	template<typename A, typename B> struct if_<true, A, B> {typedef A type;};
	template<typename A, typename B> struct if_<false, A, B> {typedef B type;};

	template<typename T>
	struct is_ptr {static const bool value = false;};
	template<typename T>
	struct is_ptr<T*> {static const bool value = true;};

	template<typename T>
	struct is_ref {static const bool value = false;};
	template<typename T>
	struct is_ref<T&> {static const bool value = true;};

	template<typename A>
	struct base_type {typedef A type;};
	template<typename A>
	struct base_type<A*> {typedef A type;};
	template<typename A>
	struct base_type<A&> {typedef A type;};
	
	// c <=> lua
	template<typename T>
	struct value_type
	{
		static void push(lua_State *L, T &value)
		{
			cclass<T> c(state::from(L));
			if (c.valid())
				c.newobject(value);
			else
				new (lua_newuserdata(L, sizeof(T))) T(value);
		}
		static bool match(lua_State *L, int idx)
		{
			const char *name = cclass_manager::instance()->get<T>();
			if (name)
				return matchobject(L, idx, name);
			return lua_type(L, idx) == LUA_TUSERDATA;
		}
		static T read(lua_State *L, int idx)
		{
			const char *name = cclass_manager::instance()->get<T>();
			if (name)
				return *readobject<T>(L, idx);
			return *(T*)lua_touserdata(L, idx);
		}
	};
	template<typename T>
	struct ptr_type
	{
		static void push(lua_State *L, T *value)
		{
			cclass<T> c(state::from(L));
			if (c.valid())
				c.boxptr(value);
			else
				lua_pushlightuserdata(L, (void*)value);
		}
		static bool match(lua_State *L, int idx)
		{
			const char *name = cclass_manager::instance()->get<T>();
			if (name)
				return matchobject(L, idx, name);
			return lua_islightuserdata(L, idx);
		}
		static T *read(lua_State *L, int idx)
		{
			const char *name = cclass_manager::instance()->get<T>();
			if (name)
				return readobject<T>(L, idx);
			return (T*)lua_touserdata(L, idx);
		}
	};
	template<typename T>
	struct ref_type
	{
		static void push(lua_State *L, T &value)
		{
			cclass<T> c(state::from(L));
			if (c.valid())
				c.boxptr(&value);
			else
				lua_pushlightuserdata(L, (void*)&value);
		}
		static bool match(lua_State *L, int idx)
		{
			const char *name = cclass_manager::instance()->get<T>();
			if (name)
				return matchobject(L, idx, name);
			return lua_islightuserdata(L, idx);
		}
		static T &read(lua_State *L, int idx)
		{
			const char *name = cclass_manager::instance()->get<T>();
			if (name)
				return *readobject<T>(L, idx);
			return *(T*)lua_touserdata(L, idx);
		}
	};
	template<typename T>
	struct any_type
	{
		static void push(lua_State *L, T value)
		{
			if_<is_ptr<T>::value
				,ptr_type<typename base_type<T>::type>
				,typename if_<is_ref<T>::value
					,ref_type<typename base_type<T>::type>
					,value_type<typename base_type<T>::type>
				>::type
			>::type::push(L, value);
		}
		static bool match(lua_State *L, int idx)
		{
			return if_<is_ptr<T>::value
				,ptr_type<typename base_type<T>::type>
				,typename if_<is_ref<T>::value
					,ref_type<typename base_type<T>::type>
					,value_type<typename base_type<T>::type>
				>::type
			>::type::match(L, idx);
		}
		static T read(lua_State *L, int idx)
		{
			return if_<is_ptr<T>::value
				,ptr_type<typename base_type<T>::type>
				,typename if_<is_ref<T>::value
					,ref_type<typename base_type<T>::type>
					,value_type<typename base_type<T>::type>
				>::type
			>::type::read(L, idx);
		}
	};
	template<typename T>
	struct any_type <userdata<T> >
	{
		static void push(lua_State *L, userdata<T> value)
		{
			new (lua_newuserdata(L, sizeof(T))) T(*value.ptr);
		}
		static bool match(lua_State *L, int idx)
		{
			return lua_type(L, idx) == LUA_TUSERDATA;
		}
		static userdata<T> read(lua_State *L, int idx)
		{
			return userdata<T>(*(T*)lua_touserdata(L, idx));
		}
	};
	template<typename T>
	struct any_type <lightuserdata<T> >
	{
		static void push(lua_State *L, lightuserdata<T> value)
		{
			lua_pushlightuserdata(L, (void*)value.ptr);
		}
		static bool match(lua_State *L, int idx)
		{
			return lua_type(L, idx) == LUA_TLIGHTUSERDATA;
		}
		static lightuserdata<T> read(lua_State *L, int idx)
		{
			return lightuserdata<T>(lua_touserdata(L, idx));
		}
	};
	template<typename T>
	struct any_type <basic_object<T> >
	{
		static void push(lua_State *L, const basic_object<T> &value)
		{
			value.push();
		}
		static bool match(lua_State *L, int idx)
		{
			return true;
		}
		static basic_object<T> read(lua_State *L, int idx)
		{
			return basic_object<T>(state::from(L), idx);
		}
	};

	inline void push(lua_State *L, bool value)					{lua_pushboolean(L, value);}
	inline void push(lua_State *L, char value)					{lua_pushnumber(L, value);}
	inline void push(lua_State *L, unsigned char value)			{lua_pushnumber(L, value);}
	inline void push(lua_State *L, short value)					{lua_pushnumber(L, value);}
	inline void push(lua_State *L, unsigned short value)		{lua_pushnumber(L, value);}
	inline void push(lua_State *L, int value)					{lua_pushnumber(L, value);}
	inline void push(lua_State *L, unsigned int value)			{lua_pushnumber(L, value);}
	inline void push(lua_State *L, long value)					{lua_pushnumber(L, value);}
	inline void push(lua_State *L, unsigned long value)			{lua_pushnumber(L, value);}
	inline void push(lua_State *L, double value)				{lua_pushnumber(L, value);}
	inline void push(lua_State *L, float value)					{lua_pushnumber(L, value);}
	inline void push(lua_State *L, const char *value)			{lua_pushstring(L, value);}
	inline void push(lua_State *L, const char *value, int len)	{lua_pushlstring(L, value, len);}
	inline void push(lua_State *L, const nil_t&)				{lua_pushnil(L);}
	inline void push(lua_State *L, lua_CFunction value)			{lua_pushcclosure(L, value, 0);}
	inline void push(lua_State *L, void *value)					{lua_pushlightuserdata(L, value);}
	inline void push(lua_State *L, const void *value)			{lua_pushlightuserdata(L, (void*)value);}
	inline void push(lua_State *L, const std::string& value)	{lua_pushstring(L, value.c_str());}

	template<typename T>
	inline void push(lua_State *L, T value)						{any_type<T>::push(L, value);}

	template<typename T>
	inline bool match(lua_State *L, int idx)
		{return any_type<T>::match(L, idx);}
	
	template<typename T>
	inline bool match(state *L, int idx)
		{return match<T>(L->cptr(), idx);}
	
	template<> inline bool match<bool>(lua_State *L, int idx)
		{return lua_type(L, idx) == LUA_TBOOLEAN;}
	template<> inline bool match<char>(lua_State *L, int idx)
		{return lua_type(L, idx) == LUA_TNUMBER;}
	template<> inline bool match<unsigned char>(lua_State *L, int idx)
		{return lua_type(L, idx) == LUA_TNUMBER;}
	template<> inline bool match<short>(lua_State *L, int idx)
		{return lua_type(L, idx) == LUA_TNUMBER;}
	template<> inline bool match<unsigned short>(lua_State *L, int idx)
		{return lua_type(L, idx) == LUA_TNUMBER;}
	template<> inline bool match<int>(lua_State *L, int idx)
		{return lua_type(L, idx) == LUA_TNUMBER;}
	template<> inline bool match<unsigned int>(lua_State *L, int idx)
		{return lua_type(L, idx) == LUA_TNUMBER;}
	template<> inline bool match<long>(lua_State *L, int idx)
		{return lua_type(L, idx) == LUA_TNUMBER;}
	template<> inline bool match<unsigned long>(lua_State *L, int idx)
		{return lua_type(L, idx) == LUA_TNUMBER;}
	template<> inline bool match<float>(lua_State *L, int idx)
		{return lua_type(L, idx) == LUA_TNUMBER;}
	template<> inline bool match<double>(lua_State *L, int idx)
		{return lua_type(L, idx) == LUA_TNUMBER;}
	template<> inline bool match<const char*>(lua_State *L, int idx)
		{return lua_type(L, idx) == LUA_TSTRING;}
	template<> inline bool match<lua_State*>(lua_State *L, int idx)
		{return lua_type(L, idx) == LUA_TNONE;}
	template<> inline bool match<state*>(lua_State *L, int idx)
		{return lua_type(L, idx) == LUA_TNONE;}
	template<> inline bool match<void*>(lua_State *L, int idx)
		{return lua_type(L, idx) == LUA_TLIGHTUSERDATA;}
	template<> inline bool match<std::string>(lua_State *L, int idx)
		{return lua_type(L, idx) == LUA_TSTRING;}

	template<typename T>
	inline T read(lua_State *L, int idx)
		{return any_type<T>::read(L, idx);}
	
	template<typename T>
	inline T read(state *L, int idx)
		{return read<T>(L->cptr(), idx);}
	
	template<> inline bool			read<bool>(lua_State *L, int idx)
		{return lua_toboolean(L, idx) != 0;}
	template<> inline char			read<char>(lua_State *L, int idx)
		{return static_cast<char>(lua_tonumber(L, idx));}
	template<> inline unsigned char	read<unsigned char>(lua_State *L, int idx)
		{return static_cast<unsigned char>(lua_tonumber(L, idx));}
	template<> inline short			read<short>(lua_State *L, int idx)
		{return static_cast<short>(lua_tonumber(L, idx));}
	template<> inline unsigned short	read<unsigned short>(lua_State *L, int idx)
		{return static_cast<unsigned short>(lua_tonumber(L, idx));}
	template<> inline int			read<int>(lua_State *L, int idx)
		{return static_cast<int>(lua_tonumber(L, idx));}
	template<> inline unsigned int	read<unsigned int>(lua_State *L, int idx)
		{return static_cast<unsigned int>(lua_tonumber(L, idx));}
	template<> inline long			read<long>(lua_State *L, int idx)
		{return static_cast<long>(lua_tonumber(L, idx));}
	template<> inline unsigned long	read<unsigned long>(lua_State *L, int idx)
		{return static_cast<unsigned long>(lua_tonumber(L, idx));}
	template<> inline float			read<float>(lua_State *L, int idx)
		{return static_cast<float>(lua_tonumber(L, idx));}
	template<> inline double		read<double>(lua_State *L, int idx)
		{return static_cast<double>(lua_tonumber(L, idx));}
	template<> inline const char*	read<const char*>(lua_State *L, int idx)
		{return static_cast<const char*>(lua_tostring(L, idx));}
	template<> inline std::string	read<std::string>(lua_State *L, int idx)
		{return std::string(lua_tostring(L, idx));}
	template<> inline nil_t			read<nil_t>(lua_State */*L*/, int /*idx*/)
		{return nil;}
	template<> inline lua_CFunction	read<lua_CFunction>(lua_State *L, int idx)
		{return static_cast<lua_CFunction>(lua_tocfunction(L, idx));}
	template<> inline void*			read<void*>(lua_State *L, int idx)
		{return static_cast<void*>(lua_touserdata(L, idx));}
	template<> inline lua_State*	read<lua_State*>(lua_State *L, int /*idx*/)
		{return L;}
	template<> inline state*		read<state*>(lua_State *L, int /*idx*/)
		{return state::from(L);}

	template<typename T> inline T check(lua_State *L, int idx)
	{
		if(!match<T>(L, idx))
			luaL_argerror(L, idx, "bad argument");
		return read<T>(L, idx);
	}
	template<typename T> inline T check(state *L, int idx)
	{
		return check<T>(L->cptr(), idx);
	}

	template<class RT>
	struct result_matcher
	{
		static inline int call(RT (*func)(), lua_State *L, int /*index*/)
		{
			RT ret = func();
			push(L, ret);
			return 1;
		}

		template <typename P1>
		static inline int call(RT (*func)(P1), lua_State *L, int index)
		{
			RT ret = func(
				check<P1>(L, index + 0)
			);
			push(L, ret);
			return 1;
		}

		template <typename P1, typename P2>
		static inline int call(RT (*func)(P1, P2), lua_State *L, int index)
		{
			RT ret = func(
				check<P1>(L, index + 0),
				check<P2>(L, index + 1)
			);
			push(L, ret);
			return 1;
		}

		template <typename P1, typename P2, typename P3>
		static inline int call(RT (*func)(P1, P2, P3), lua_State *L, int index)
		{
			RT ret = func(
				check<P1>(L, index + 0),
				check<P2>(L, index + 1),
				check<P3>(L, index + 2)
			);
			push(L, ret);
			return 1;
		}

		template <typename P1, typename P2, typename P3, typename P4>
		static inline int call(RT (*func)(P1, P2, P3, P4), lua_State *L, int index)
		{
			RT ret = func(
				check<P1>(L, index + 0),
				check<P2>(L, index + 1),
				check<P3>(L, index + 2),
				check<P4>(L, index + 3)
			);
			push(L, ret);
			return 1;
		}

		template <typename P1, typename P2, typename P3, typename P4,
					typename P5>
		static inline int call(RT (*func)(P1, P2, P3, P4, P5), lua_State *L, int index)
		{
			RT ret = func(
				check<P1>(L, index + 0),
				check<P2>(L, index + 1),
				check<P3>(L, index + 2),
				check<P4>(L, index + 3),
				check<P5>(L, index + 4)
			);
			push(L, ret);
			return 1;
		}

		template <typename P1, typename P2, typename P3, typename P4,
					typename P5, typename P6>
		static inline int call(RT (*func)(P1, P2, P3, P4, P5, P6), lua_State *L, int index)
		{
			RT ret = func(
				check<P1>(L, index + 0),
				check<P2>(L, index + 1),
				check<P3>(L, index + 2),
				check<P4>(L, index + 3),
				check<P5>(L, index + 4),
				check<P6>(L, index + 5)
			);
			push(L, ret);
			return 1;
		}

		template <typename P1, typename P2, typename P3, typename P4,
					typename P5, typename P6, typename P7>
		static inline int call(RT (*func)(P1, P2, P3, P4, P5, P6, P7), lua_State *L, int index)
		{
			RT ret = func(
				check<P1>(L, index + 0),
				check<P2>(L, index + 1),
				check<P3>(L, index + 2),
				check<P4>(L, index + 3),
				check<P5>(L, index + 4),
				check<P6>(L, index + 5),
				check<P7>(L, index + 6)
			);
			push(L, ret);
			return 1;
		}

		template <typename P1, typename P2, typename P3, typename P4,
					typename P5, typename P6, typename P7, typename P8>
		static inline int call(RT (*func)(P1, P2, P3, P4, P5, P6, P7, P8), lua_State *L, int index)
		{
			RT ret = func(
				check<P1>(L, index + 0),
				check<P2>(L, index + 1),
				check<P3>(L, index + 2),
				check<P4>(L, index + 3),
				check<P5>(L, index + 4),
				check<P6>(L, index + 5),
				check<P7>(L, index + 6),
				check<P8>(L, index + 7)
			);
			push(L, ret);
			return 1;
		}

		//////////////////////////////////////////////////////////////////////////////
		template <typename Callee>
		static inline int call(Callee &callee, RT (Callee::*func)(), lua_State *L, int /*index*/)
		{
			RT ret = (callee.*func)();
			push(L, ret);
			return 1;
		}

		template <typename Callee>
		static inline int call(Callee &callee, RT (Callee::*func)() const, lua_State *L, int /*index*/)
		{
			RT ret = (callee.*func)();
			push(L, ret);
			return 1;
		}

		template <typename Callee, typename P1>
		static inline int call(Callee &callee, RT (Callee::*func)(P1), lua_State *L, int index)
		{
			RT ret = (callee.*func)(
				check<P1>(L, index + 0)
			);
			push(L, ret);
			return 1;
		}

		template <typename Callee, typename P1>
		static inline int call(Callee &callee, RT (Callee::*func)(P1) const, lua_State *L, int index)
		{
			RT ret = (callee.*func)(
				check<P1>(L, index + 0)
			);
			push(L, ret);
			return 1;
		}

		template <typename Callee, typename P1, typename P2>
		static inline int call(Callee &callee, RT (Callee::*func)(P1, P2), lua_State *L, int index)
		{
			RT ret = (callee.*func)(
				check<P1>(L, index + 0),
				check<P2>(L, index + 1)
			);
			push(L, ret);
			return 1;
		}

		template <typename Callee, typename P1, typename P2>
		static inline int call(Callee &callee, RT (Callee::*func)(P1, P2) const, lua_State *L, int index)
		{
			RT ret = (callee.*func)(
				check<P1>(L, index + 0),
				check<P2>(L, index + 1)
			);
			push(L, ret);
			return 1;
		}

		template <typename Callee, typename P1, typename P2, typename P3>
		static inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3), lua_State *L, int index)
		{
			RT ret = (callee.*func)(
				check<P1>(L, index + 0),
				check<P2>(L, index + 1),
				check<P3>(L, index + 2)
			);
			push(L, ret);
			return 1;
		}

		template <typename Callee, typename P1, typename P2, typename P3>
		static inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3) const, lua_State *L, int index)
		{
			RT ret = (callee.*func)(
				check<P1>(L, index + 0),
				check<P2>(L, index + 1),
				check<P3>(L, index + 2)
			);
			push(L, ret);
			return 1;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4>
		static inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3, P4), lua_State *L, int index)
		{
			RT ret = (callee.*func)(
				check<P1>(L, index + 0),
				check<P2>(L, index + 1),
				check<P3>(L, index + 2),
				check<P4>(L, index + 3)
			);
			push(L, ret);
			return 1;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4>
		static inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3, P4) const, lua_State *L, int index)
		{
			RT ret = (callee.*func)(
				check<P1>(L, index + 0),
				check<P2>(L, index + 1),
				check<P3>(L, index + 2),
				check<P4>(L, index + 3)
			);
			push(L, ret);
			return 1;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5>
		static inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3, P4, P5), lua_State *L, int index)
		{
			RT ret = (callee.*func)(
				check<P1>(L, index + 0),
				check<P2>(L, index + 1),
				check<P3>(L, index + 2),
				check<P4>(L, index + 3),
				check<P5>(L, index + 4)
			);
			push(L, ret);
			return 1;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5>
		static inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3, P4, P5) const, lua_State *L, int index)
		{
			RT ret = (callee.*func)(
				check<P1>(L, index + 0),
				check<P2>(L, index + 1),
				check<P3>(L, index + 2),
				check<P4>(L, index + 3),
				check<P5>(L, index + 4)
			);
			push(L, ret);
			return 1;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6>
		static inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6), lua_State *L, int index)
		{
			RT ret = (callee.*func)(
				check<P1>(L, index + 0),
				check<P2>(L, index + 1),
				check<P3>(L, index + 2),
				check<P4>(L, index + 3),
				check<P5>(L, index + 4),
				check<P6>(L, index + 5)
			);
			push(L, ret);
			return 1;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6>
		static inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6) const, lua_State *L, int index)
		{
			RT ret = (callee.*func)(
				check<P1>(L, index + 0),
				check<P2>(L, index + 1),
				check<P3>(L, index + 2),
				check<P4>(L, index + 3),
				check<P5>(L, index + 4),
				check<P6>(L, index + 5)
			);
			push(L, ret);
			return 1;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6, typename P7>
		static inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6, P7), lua_State *L, int index)
		{
			RT ret = (callee.*func)(
				check<P1>(L, index + 0),
				check<P2>(L, index + 1),
				check<P3>(L, index + 2),
				check<P4>(L, index + 3),
				check<P5>(L, index + 4),
				check<P6>(L, index + 5),
				check<P7>(L, index + 6)
			);
			push(L, ret);
			return 1;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6, typename P7>
		static inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6, P7) const, lua_State *L, int index)
		{
			RT ret = (callee.*func)(
				check<P1>(L, index + 0),
				check<P2>(L, index + 1),
				check<P3>(L, index + 2),
				check<P4>(L, index + 3),
				check<P5>(L, index + 4),
				check<P6>(L, index + 5),
				check<P7>(L, index + 6)
			);
			push(L, ret);
			return 1;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6, typename P7, typename P8>
		static inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6, P7, P8), lua_State *L, int index)
		{
			RT ret = (callee.*func)(
				check<P1>(L, index + 0),
				check<P2>(L, index + 1),
				check<P3>(L, index + 2),
				check<P4>(L, index + 3),
				check<P5>(L, index + 4),
				check<P6>(L, index + 5),
				check<P7>(L, index + 6),
				check<P8>(L, index + 7)
			);
			push(L, ret);
			return 1;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6, typename P7, typename P8>
		static inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6, P7, P8) const, lua_State *L, int index)
		{
			RT ret = (callee.*func)(
				check<P1>(L, index + 0),
				check<P2>(L, index + 1),
				check<P3>(L, index + 2),
				check<P4>(L, index + 3),
				check<P5>(L, index + 4),
				check<P6>(L, index + 5),
				check<P7>(L, index + 6),
				check<P8>(L, index + 7)
			);
			push(L, ret);
			return 1;
		}
	};

	template<>
	struct result_matcher<void>
	{
		static inline int call(void (*func)(), lua_State *L, int /*index*/)
		{
			(void)L;
			func();
			return 0;
		}

		template <typename P1>
		static inline int call(void (*func)(P1), lua_State *L, int index)
		{
			func(
				check<P1>(L, index + 0)
			);
			return 0;
		}

		template <typename P1, typename P2>
		static inline int call(void (*func)(P1, P2), lua_State *L, int index)
		{
			func(
				check<P1>(L, index + 0),
				check<P2>(L, index + 1)
			);
			return 0;
		}

		template <typename P1, typename P2, typename P3>
		static inline int call(void (*func)(P1, P2, P3), lua_State *L, int index)
		{
			func(
				check<P1>(L, index + 0),
				check<P2>(L, index + 1),
				check<P3>(L, index + 2)
			);
			return 0;
		}

		template <typename P1, typename P2, typename P3, typename P4>
		static inline int call(void (*func)(P1, P2, P3, P4), lua_State *L, int index)
		{
			func(
				check<P1>(L, index + 0),
				check<P2>(L, index + 1),
				check<P3>(L, index + 2),
				check<P4>(L, index + 3)
			);
			return 0;
		}

		template <typename P1, typename P2, typename P3, typename P4, typename P5>
		static inline int call(void (*func)(P1, P2, P3, P4, P5), lua_State *L, int index)
		{
			func(
				check<P1>(L, index + 0),
				check<P2>(L, index + 1),
				check<P3>(L, index + 2),
				check<P4>(L, index + 3),
				check<P5>(L, index + 4)
			);
			return 0;
		}

		template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
		static inline int call(void (*func)(P1, P2, P3, P4, P5, P6), lua_State *L, int index)
		{
			func(
				check<P1>(L, index + 0),
				check<P2>(L, index + 1),
				check<P3>(L, index + 2),
				check<P4>(L, index + 3),
				check<P5>(L, index + 4),
				check<P6>(L, index + 5)
			);
			return 0;
		}

		template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
		static inline int call(void (*func)(P1, P2, P3, P4, P5, P6, P7), lua_State *L, int index)
		{
			func(
				check<P1>(L, index + 0),
				check<P2>(L, index + 1),
				check<P3>(L, index + 2),
				check<P4>(L, index + 3),
				check<P5>(L, index + 4),
				check<P6>(L, index + 5),
				check<P7>(L, index + 6)
			);
			return 0;
		}

		template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
		static inline int call(void (*func)(P1, P2, P3, P4, P5, P6, P7, P8), lua_State *L, int index)
		{
			func(
				check<P1>(L, index + 0),
				check<P2>(L, index + 1),
				check<P3>(L, index + 2),
				check<P4>(L, index + 3),
				check<P5>(L, index + 4),
				check<P6>(L, index + 5),
				check<P7>(L, index + 6),
				check<P8>(L, index + 7)
			);
			return 0;
		}

		//////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////
		template <typename Callee>
		static inline int call(Callee &callee, void (Callee::*func)(), lua_State */*L*/, int /*index*/)
		{
			(callee.*func)();
			return 0;
		}

		template <typename Callee>
		static inline int call(Callee &callee, void (Callee::*func)() const, lua_State */*L*/, int /*index*/)
		{
			(callee.*func)();
			return 0;
		}

		template <typename Callee, typename P1>
		static inline int call(Callee &callee, void (Callee::*func)(P1), lua_State *L, int index)
		{
			(callee.*func)(
				check<P1>(L, index + 0)
			);
			return 0;
		}

		template <typename Callee, typename P1>
		static inline int call(Callee &callee, void (Callee::*func)(P1) const, lua_State *L, int index)
		{
			(callee.*func)(
				check<P1>(L, index + 0)
			);
			return 0;
		}

		template <typename Callee, typename P1, typename P2>
		static inline int call(Callee &callee, void (Callee::*func)(P1, P2), lua_State *L, int index)
		{			(callee.*func)(
				check<P1>(L, index + 0),
				check<P2>(L, index + 1)
			);
			return 0;
		}

		template <typename Callee, typename P1, typename P2>
		static inline int call(Callee &callee, void (Callee::*func)(P1, P2) const, lua_State *L, int index)
		{			(callee.*func)(
				check<P1>(L, index + 0),
				check<P2>(L, index + 1)
			);
			return 0;
		}

		template <typename Callee, typename P1, typename P2, typename P3>
		static inline int call(Callee &callee, void (Callee::*func)(P1, P2, P3), lua_State *L, int index)
		{
			(callee.*func)(
				check<P1>(L, index + 0),
				check<P2>(L, index + 1),
				check<P3>(L, index + 2)
			);
			return 0;
		}

		template <typename Callee, typename P1, typename P2, typename P3>
		static inline int call(Callee &callee, void (Callee::*func)(P1, P2, P3) const, lua_State *L, int index)
		{
			(callee.*func)(
				check<P1>(L, index + 0),
				check<P2>(L, index + 1),
				check<P3>(L, index + 2)
			);
			return 0;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4>
		static inline int call(Callee &callee, void (Callee::*func)(P1, P2, P3, P4), lua_State *L, int index)
		{			(callee.*func)(
				check<P1>(L, index + 0),
				check<P2>(L, index + 1),
				check<P3>(L, index + 2),
				check<P4>(L, index + 3)
			);
			return 0;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4>
		static inline int call(Callee &callee, void (Callee::*func)(P1, P2, P3, P4) const, lua_State *L, int index)
		{			(callee.*func)(
				check<P1>(L, index + 0),
				check<P2>(L, index + 1),
				check<P3>(L, index + 2),
				check<P4>(L, index + 3)
			);
			return 0;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5>
		static inline int call(Callee &callee, void (Callee::*func)(P1, P2, P3, P4, P5), lua_State *L, int index)
		{
			(callee.*func)(
				check<P1>(L, index + 0),
				check<P2>(L, index + 1),
				check<P3>(L, index + 2),
				check<P4>(L, index + 3),
				check<P5>(L, index + 4)
			);
			return 0;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5>
		static inline int call(Callee &callee, void (Callee::*func)(P1, P2, P3, P4, P5) const, lua_State *L, int index)
		{
			(callee.*func)(
				check<P1>(L, index + 0),
				check<P2>(L, index + 1),
				check<P3>(L, index + 2),
				check<P4>(L, index + 3),
				check<P5>(L, index + 4)
			);
			return 0;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6>
		static inline int call(Callee &callee, void (Callee::*func)(P1, P2, P3, P4, P5, P6), lua_State *L, int index)
		{			(callee.*func)(
				check<P1>(L, index + 0),
				check<P2>(L, index + 1),
				check<P3>(L, index + 2),
				check<P4>(L, index + 3),
				check<P5>(L, index + 4),
				check<P6>(L, index + 5)
			);
			return 0;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6>
		static inline int call(Callee &callee, void (Callee::*func)(P1, P2, P3, P4, P5, P6) const, lua_State *L, int index)
		{			(callee.*func)(
				check<P1>(L, index + 0),
				check<P2>(L, index + 1),
				check<P3>(L, index + 2),
				check<P4>(L, index + 3),
				check<P5>(L, index + 4),
				check<P6>(L, index + 5)
			);
			return 0;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6, typename P7>
		static inline int call(Callee &callee, void (Callee::*func)(P1, P2, P3, P4, P5, P6, P7), lua_State *L, int index)
		{
			(callee.*func)(
				check<P1>(L, index + 0),
				check<P2>(L, index + 1),
				check<P3>(L, index + 2),
				check<P4>(L, index + 3),
				check<P5>(L, index + 4),
				check<P6>(L, index + 5),
				check<P7>(L, index + 6)
			);
			return 0;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6, typename P7>
		static inline int call(Callee &callee, void (Callee::*func)(P1, P2, P3, P4, P5, P6, P7) const, lua_State *L, int index)
		{
			(callee.*func)(
				check<P1>(L, index + 0),
				check<P2>(L, index + 1),
				check<P3>(L, index + 2),
				check<P4>(L, index + 3),
				check<P5>(L, index + 4),
				check<P6>(L, index + 5),
				check<P7>(L, index + 6)
			);
			return 0;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6, typename P7, typename P8>
		static inline int call(Callee &callee, void (Callee::*func)(P1, P2, P3, P4, P5, P6, P7, P8), lua_State *L, int index)
		{
			(callee.*func)(
				check<P1>(L, index + 0),
				check<P2>(L, index + 1),
				check<P3>(L, index + 2),
				check<P4>(L, index + 3),
				check<P5>(L, index + 4),
				check<P6>(L, index + 5),
				check<P7>(L, index + 6),
				check<P8>(L, index + 7)
			);
			return 0;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6, typename P7, typename P8>
		static inline int call(Callee &callee, void (Callee::*func)(P1, P2, P3, P4, P5, P6, P7, P8) const, lua_State *L, int index)
		{
			(callee.*func)(
				check<P1>(L, index + 0),
				check<P2>(L, index + 1),
				check<P3>(L, index + 2),
				check<P4>(L, index + 3),
				check<P5>(L, index + 4),
				check<P6>(L, index + 5),
				check<P7>(L, index + 6),
				check<P8>(L, index + 7)
			);
			return 0;
		}
	};

	template <typename RT>
	inline int call(RT (*func)(), lua_State *L, int index)
		{return result_matcher<RT>::call(func, L, index);}

	template <typename RT, typename P1>
	inline int call(RT (*func)(P1), lua_State *L, int index)
		{return result_matcher<RT>::call(func, L, index);}

	template <typename RT, typename P1, typename P2>
	inline int call(RT (*func)(P1, P2), lua_State *L, int index)
		{return result_matcher<RT>::call(func, L, index);}

	template <typename RT, typename P1, typename P2, typename P3>
	inline int call(RT (*func)(P1, P2, P3), lua_State *L, int index)
		{return result_matcher<RT>::call(func, L, index);}

	template <typename RT, typename P1, typename P2, typename P3, typename P4>
	inline int call(RT (*func)(P1, P2, P3, P4), lua_State *L, int index)
		{return result_matcher<RT>::call(func, L, index);}

	template <typename RT, typename P1, typename P2, typename P3, typename P4,
				typename P5>
	inline int call(RT (*func)(P1, P2, P3, P4, P5), lua_State *L, int index)
		{return result_matcher<RT>::call(func, L, index);}

	template <typename RT, typename P1, typename P2, typename P3, typename P4,
				typename P5, typename P6>
	inline int call(RT (*func)(P1, P2, P3, P4, P5, P6), lua_State *L, int index)
		{return result_matcher<RT>::call(func, L, index);}

	template <typename RT, typename P1, typename P2, typename P3, typename P4,
				typename P5, typename P6, typename P7>
	inline int call(RT (*func)(P1, P2, P3, P4, P5, P6, P7), lua_State *L, int index)
		{return result_matcher<RT>::call(func, L, index);}

	template <typename RT, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
	inline int call(RT (*func)(P1, P2, P3, P4, P5, P6, P7, P8), lua_State *L, int index)
		{return result_matcher<RT>::call(func, L, index);}

	template <typename Callee, typename RT>
	inline int call(Callee &callee, RT (Callee::*func)(), lua_State *L, int index)
		{return result_matcher<RT>::call(callee, func, L, index);}

	template <typename Callee, typename RT>
	inline int call(Callee &callee, RT (Callee::*func)() const, lua_State *L, int index)
		{return result_matcher<RT>::call(callee, func, L, index);}

	template <typename Callee, typename RT, typename P1>
	inline int call(Callee &callee, RT (Callee::*func)(P1), lua_State *L, int index)
		{return result_matcher<RT>::call(callee, func, L, index);}

	template <typename Callee, typename RT, typename P1>
	inline int call(Callee &callee, RT (Callee::*func)(P1) const, lua_State *L, int index)
		{return result_matcher<RT>::call(callee, func, L, index);}

	template <typename Callee, typename RT, typename P1, typename P2>
	inline int call(Callee &callee, RT (Callee::*func)(P1, P2), lua_State *L, int index)
		{return result_matcher<RT>::call(callee, func, L, index);}

	template <typename Callee, typename RT, typename P1, typename P2>
	inline int call(Callee &callee, RT (Callee::*func)(P1, P2) const, lua_State *L, int index)
		{return result_matcher<RT>::call(callee, func, L, index);}

	template <typename Callee, typename RT, typename P1, typename P2, typename P3>
	inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3), lua_State *L, int index)
		{return result_matcher<RT>::call(callee, func, L, index);}

	template <typename Callee, typename RT, typename P1, typename P2, typename P3>
	inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3) const, lua_State *L, int index)
		{return result_matcher<RT>::call(callee, func, L, index);}

	template <typename Callee, typename RT, typename P1, typename P2, typename P3,
				typename P4>
	inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3, P4), lua_State *L, int index)
		{return result_matcher<RT>::call(callee, func, L, index);}

	template <typename Callee, typename RT, typename P1, typename P2, typename P3,
				typename P4>
	inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3, P4) const, lua_State *L, int index)
		{return result_matcher<RT>::call(callee, func, L, index);}

	template <typename Callee, typename RT, typename P1, typename P2, typename P3,
				typename P4, typename P5>
	inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3, P4, P5), lua_State *L, int index)
		{return result_matcher<RT>::call(callee, func, L, index);}

	template <typename Callee, typename RT, typename P1, typename P2, typename P3,
				typename P4, typename P5>
	inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3, P4, P5) const, lua_State *L, int index)
		{return result_matcher<RT>::call(callee, func, L, index);}

	template <typename Callee, typename RT, typename P1, typename P2, typename P3,
				typename P4, typename P5, typename P6>
	inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6), lua_State *L, int index)
		{return result_matcher<RT>::call(callee, func, L, index);}

	template <typename Callee, typename RT, typename P1, typename P2, typename P3,
				typename P4, typename P5, typename P6>
	inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6) const, lua_State *L, int index)
		{return result_matcher<RT>::call(callee, func, L, index);}

	template <typename Callee, typename RT, typename P1, typename P2, typename P3,
				typename P4, typename P5, typename P6, typename P7>
	inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6, P7), lua_State *L, int index)
		{return result_matcher<RT>::call(callee, func, L, index);}

	template <typename Callee, typename RT, typename P1, typename P2, typename P3,
				typename P4, typename P5, typename P6, typename P7>
	inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6, P7) const, lua_State *L, int index)
		{return result_matcher<RT>::call(callee, func, L, index);}

	template <typename Callee, typename RT, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
	inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6, P7, P8), lua_State *L, int index)
		{return result_matcher<RT>::call(callee, func, L, index);}

	template <typename Callee, typename RT, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
	inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6, P7, P8) const, lua_State *L, int index)
		{return result_matcher<RT>::call(callee, func, L, index);}

} // namespace luastub

namespace luastub
{
	inline unsigned char *getupvalue(lua_State *L, int n)
	{
		return (unsigned char*)lua_touserdata(L, lua_upvalueindex(n));
	}

	// dispatchers
	template <typename Func>
	class function_dispatcher
	{
	public:
		static inline int dispatch(lua_State *L)
		{
 			unsigned char *buffer = getupvalue(L, 1);
			return call(*(Func*)(buffer), L, 1);
		}
	};

	template <typename Callee, typename Method>
	class method_dispatcher
	{
	public:
		static inline int dispatch(lua_State *L)
		{
 			unsigned char *buffer = getupvalue(L, 1);
			Callee &callee = **(Callee**)buffer;
			Method &method = *(Method*)(buffer + sizeof(Callee*));
			return call(callee, method, L, 1);
		}
	};

	inline int state_function_dispatcher(lua_State *L)
	{
		typedef int (*Functor)(state*);
		unsigned char *buffer = getupvalue(L, 1);
		Functor &func = *(Functor*)(buffer);
 		return (*func)(state::from(L));
	}

	template <typename Callee>
	class state_method_dispatcher
	{
	public:
		static inline int dispatch(lua_State *L)
		{
			typedef int (Callee::*Functor)(state*);
 			unsigned char *buffer = getupvalue(L, 1);
			Callee &callee = **(Callee**)buffer;
			Functor &func = *(Functor*)(buffer + sizeof(Callee*));
			return (callee.*func)(state::from(L));
		}
	};

	template <typename Callee>
	class oo_state_method_dispatcher
	{
	public:
		static inline int dispatch(lua_State *L)
		{
			typedef int (Callee::*Functor)(state*);
 			unsigned char *buffer = getupvalue(L, 1);
			Functor &func = *(Functor*)(buffer);
			Callee &callee = *readobject<Callee>(L, 1);
			return (callee.*func)(state::from(L));
		}
	};
	
	template <typename Callee, typename Method>
	class oo_method_dispatcher
	{
	public:
		static inline int dispatch(lua_State *L)
		{
			typedef int (Callee::*Functor)(lua_State*);
 			unsigned char *buffer = getupvalue(L, 1);
			Method &method = *(Method*)(buffer);
			Callee &callee = *readobject<Callee>(L, 1);
			return call(callee, method, L, 2);
		}
	};
	
	inline int property_newindex(lua_State *L)
	{
													// t k v
		lua_getmetatable(L, 1);						// t k v m
		lua_pushstring(L, "__props");				// t k v m p
		lua_rawget(L, -2);							// t k v m pt
		if (lua_istable(L, -1))
		{
			lua_pushvalue(L, 2);					// t k v m pt k
			lua_rawget(L, -2);						// t k v m pt prop
			if (lua_isnil(L, -1))
				luaL_error(L, "property '%s' is nil", lua_tostring(L, 2));

			lua_rawgeti(L, -1, 2);					// t k v m pt prop setf
			if (lua_isnil(L, -1))
				luaL_error(L, "attempt to write property '%s'", lua_tostring(L, 2));
			
			lua_pushvalue(L, 1);					// t k v m pt prop setf t
			lua_pushvalue(L, 3);					// t k v m pt prop setf t v
			lua_call(L, 2, 1);
			return 1;
		}

		return 0;
	}

	inline int property_index(lua_State *L)
	{
													// t v
		lua_getmetatable(L, 1);						// t v m
		lua_pushvalue(L, 2);						// t v m v
		lua_rawget(L, -2);							// t v m lookup
		if (!lua_isnil(L, -1))
			return 1;

		lua_pop(L, 1);								// t v m
		lua_pushstring(L, "__props");				// t k v m __props
		lua_rawget(L, -2);							// t k v m pt

		if (lua_istable(L, -1))
		{
			lua_pushvalue(L, 2);					// t k v m pt k
			lua_rawget(L, -2);						// t k v m pt prop
			if (lua_isnil(L, -1))
				luaL_error(L, "property '%s' is nil", lua_tostring(L, 2));

			lua_rawgeti(L, -1, 1);					// t k v m pt prop getf
			if (lua_isnil(L, -1))
				luaL_error(L, "attempt to get property '%s'", lua_tostring(L, 2));
			
			lua_pushvalue(L, 1);					// t k v m pt prop getf t
			lua_call(L, 1, 1);
			return 1;
		}

		return 0;
	}

	inline int property_support(lua_State *L, int index)
	{
		lua_pushstring(L, "__props");
		lua_rawget(L, index);
		if (lua_isnil(L, -1))
		{
			lua_pushstring(L, "__props");
			lua_newtable(L);
			lua_rawset(L, index);
			
			lua_pushstring(L, "__newindex");
			lua_pushcfunction(L, property_newindex);
			lua_rawset(L, index);
			
			lua_pushstring(L, "__index");
			lua_pushcfunction(L, property_index);
			lua_rawset(L, index);
		}
		lua_pop(L, 1);
		return 0;
	}

	template <typename Callee, typename Var>
	class member_variable_dispatcher
	{
	public:
		static inline int getter(lua_State *L)
		{
			void *offset = getupvalue(L, 1);
			Callee* callee = readobject<Callee>(L, 1);
			push(L, *(Var*)((unsigned char*)callee + (unsigned int)offset));
			return 1;
		}

		static inline int setter(lua_State *L)
		{
			void *offset = getupvalue(L, 1);
			Callee* callee = readobject<Callee>(L, 1);
			if (!match<Var>(L, 2))
				luaL_argerror(L, 2, "bad argument");
			*(Var*)((unsigned char*)callee + (unsigned int)offset) = read<Var>(L, 2);
			return 1;
		}
	};

	template <typename Var>
	class global_variable_dispatcher
	{
	public:
		static inline int getter(lua_State *L)
		{
			void *offset = getupvalue(L, 1);
			push(L, *(Var*)offset);
			return 1;
		}

		static inline int setter(lua_State *L)
		{
			void *offset = getupvalue(L, 1);
			if (!match<Var>(L, 2))
				luaL_argerror(L, 2, "bad argument");
			*(Var*)offset = read<Var>(L, 2);
			return 1;
		}
	};
} // namespace luastub

#endif

