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

namespace luastub
{
	class nil_t;
	
	inline void push(lua_State *L, bool value)					{lua_pushboolean(L, value);}
	inline void push(lua_State *L, char value)					{lua_pushnumber(L, value);}
	inline void push(lua_State *L, unsigned char value)			{lua_pushnumber(L, value);}
	inline void push(lua_State *L, short value)					{lua_pushnumber(L, value);}
	inline void push(lua_State *L, unsigned short value)		{lua_pushnumber(L, value);}
	inline void push(lua_State *L, int value)					{lua_pushnumber(L, value);}
	inline void push(lua_State *L, unsigned int value)			{lua_pushnumber(L, value);}
	inline void push(lua_State *L, long value)					{lua_pushnumber(L, value);}
	inline void push(lua_State *L, unsigned long value)			{lua_pushnumber(L, value);}
	inline void push(lua_State *L, double value)				{lua_pushnumber(L, (lua_Number)value);}
	inline void push(lua_State *L, float value)					{lua_pushnumber(L, (lua_Number)value);}
	inline void push(lua_State *L, const char *value)			{lua_pushstring(L, value);}
	inline void push(lua_State *L, const char *value, int len)	{lua_pushlstring(L, value, len);}
	inline void push(lua_State *L, const nil_t&)				{lua_pushnil(L);}
	inline void push(lua_State *L, lua_CFunction value)			{lua_pushcclosure(L, value, 0);}
	inline void push(lua_State *L, const void *value)			{lua_pushlightuserdata(L, (void*)value);}
	inline void push(lua_State *L, const std::string& value)	{lua_pushstring(L, value.c_str());}
	template<class T>
	inline void push(lua_State *L, T value)						{value.push();}

	template<class T> 
	struct type_matcher {};

	inline bool	match(type_matcher<bool>, lua_State *L, int idx)
		{return lua_type(L, idx) == LUA_TBOOLEAN;}
	inline bool	match(type_matcher<char>, lua_State *L, int idx)
		{return lua_type(L, idx) == LUA_TNUMBER;}
	inline bool	match(type_matcher<unsigned char>, lua_State *L, int idx)
		{return lua_type(L, idx) == LUA_TNUMBER;}
	inline bool	match(type_matcher<short>, lua_State *L, int idx)
		{return lua_type(L, idx) == LUA_TNUMBER;}
	inline bool	match(type_matcher<unsigned short>, lua_State *L, int idx)
		{return lua_type(L, idx) == LUA_TNUMBER;}
	inline bool	match(type_matcher<int>, lua_State *L, int idx)
		{return lua_type(L, idx) == LUA_TNUMBER;}
	inline bool	match(type_matcher<unsigned int>, lua_State *L, int idx)
		{return lua_type(L, idx) == LUA_TNUMBER;}
	inline bool	match(type_matcher<long>, lua_State *L, int idx)
		{return lua_type(L, idx) == LUA_TNUMBER;}
	inline bool	match(type_matcher<unsigned long>, lua_State *L, int idx)
		{return lua_type(L, idx) == LUA_TNUMBER;}
	inline bool	match(type_matcher<float>, lua_State *L, int idx)
		{int type = lua_type(L, idx);  return type == LUA_TNUMBER;}
	inline bool	match(type_matcher<double>, lua_State *L, int idx)
		{int type = lua_type(L, idx);  return type == LUA_TNUMBER;}
	inline bool	match(type_matcher<const char*>, lua_State *L, int idx)
		{return lua_type(L, idx) == LUA_TSTRING;}
	inline bool	match(type_matcher<lua_State*>, lua_State *L, int idx)
		{return lua_type(L, idx) == LUA_TNONE;}
	inline bool	match(type_matcher<luastub::state*>, lua_State *L, int idx)
		{ return lua_type(L, idx) == LUA_TNONE;}
	inline bool	match(type_matcher<void*>, lua_State *L, int idx)
		{return lua_type(L, idx) == LUA_TLIGHTUSERDATA;}
	inline bool	match(type_matcher<std::string>, lua_State *L, int idx)
		{return lua_type(L, idx) == LUA_TSTRING;}

	inline void				get(type_matcher<void>, lua_State*, int)
		{}
	inline bool				get(type_matcher<bool>, lua_State *L, int idx)
		{return lua_toboolean(L, idx) != 0;}
	inline char				get(type_matcher<char>, lua_State *L, int idx)
		{return static_cast<char>(lua_tonumber(L, idx));}
	inline unsigned char	get(type_matcher<unsigned char>, lua_State *L, int idx)
		{return static_cast<unsigned char>(lua_tonumber(L, idx));}
	inline short			get(type_matcher<short>, lua_State *L, int idx)
		{return static_cast<short>(lua_tonumber(L, idx));}
	inline unsigned short	get(type_matcher<unsigned short>, lua_State *L, int idx)
		{return static_cast<unsigned short>(lua_tonumber(L, idx));}
	inline int				get(type_matcher<int>, lua_State *L, int idx)
		{return static_cast<int>(lua_tonumber(L, idx));}
	inline unsigned int		get(type_matcher<unsigned int>, lua_State *L, int idx)
		{return static_cast<unsigned int>(lua_tonumber(L, idx));}
	inline long				get(type_matcher<long>, lua_State *L, int idx)
		{return static_cast<long>(lua_tonumber(L, idx));}
	inline unsigned long	get(type_matcher<unsigned long>, lua_State *L, int idx)
		{return static_cast<unsigned long>(lua_tonumber(L, idx));}
	inline float			get(type_matcher<float>, lua_State *L, int idx)
		{return static_cast<float>(lua_tonumber(L, idx));}
	inline double			get(type_matcher<double>, lua_State *L, int idx)
		{return static_cast<double>(lua_tonumber(L, idx));}
	inline const char*		get(type_matcher<const char*>, lua_State *L, int idx)
		{return static_cast<const char*>(lua_tostring(L, idx));}
	inline nil_t			get(type_matcher<nil_t>, lua_State *L, int idx)
		{(void)L, (void)idx;  return nil_t();}
	inline lua_CFunction	get(type_matcher<lua_CFunction>, lua_State *L, int idx)
		{return static_cast<lua_CFunction>(lua_tocfunction(L, idx));}
	inline void*			get(type_matcher<void*>, lua_State *L, int idx)
		{return static_cast<void*>(lua_touserdata(L, idx));}
	inline lua_State*		get(type_matcher<lua_State*>, lua_State *L, int /*idx*/)
		{return L;}
	inline state*			get(type_matcher<luastub::state*>, lua_State *L, int /*idx*/)
		{return state::from(L);}
	inline const char*		get(type_matcher<std::string>, lua_State *L, int idx)
		{return static_cast<const char*>(lua_tostring(L, idx));}
	template<typename T>
	inline T				get(type_matcher<T>, lua_State *L, int idx)
		{return T(state::from(L), idx);}

	//////////////////////////////////////////////////////////////////////////

	#define luaL_argassert(arg, index) if (!match(type_matcher<P##arg>(), L, index)) \
				luaL_argerror(L, index, "bad argument")

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
			luaL_argassert(1, index + 0);

			RT ret = func(
				get(type_matcher<P1>(), L, index + 0)
			);
			push(L, ret);
			return 1;
		}


		template <typename P1, typename P2>
		static inline int call(RT (*func)(P1, P2), lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);

			RT ret = func(
				get(type_matcher<P1>(), L, index + 0),
				get(type_matcher<P2>(), L, index + 1)
			);
			push(L, ret);
			return 1;
		}


		template <typename P1, typename P2, typename P3>
		static inline int call(RT (*func)(P1, P2, P3), lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);

			RT ret = func(
				get(type_matcher<P1>(), L, index + 0),
				get(type_matcher<P2>(), L, index + 1),
				get(type_matcher<P3>(), L, index + 2)
			);
			push(L, ret);
			return 1;
		}


		template <typename P1, typename P2, typename P3, typename P4>
		static inline int call(RT (*func)(P1, P2, P3, P4), lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);

			RT ret = func(
				get(type_matcher<P1>(), L, index + 0),
				get(type_matcher<P2>(), L, index + 1),
				get(type_matcher<P3>(), L, index + 2),
				get(type_matcher<P4>(), L, index + 3)
			);
			push(L, ret);
			return 1;
		}


		template <typename P1, typename P2, typename P3, typename P4,
					typename P5>
		static inline int call(RT (*func)(P1, P2, P3, P4, P5), lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);

			RT ret = func(
				get(type_matcher<P1>(), L, index + 0),
				get(type_matcher<P2>(), L, index + 1),
				get(type_matcher<P3>(), L, index + 2),
				get(type_matcher<P4>(), L, index + 3),
				get(type_matcher<P5>(), L, index + 4)
			);
			push(L, ret);
			return 1;
		}


		template <typename P1, typename P2, typename P3, typename P4,
					typename P5, typename P6>
		static inline int call(RT (*func)(P1, P2, P3, P4, P5, P6), lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);

			RT ret = func(
				get(type_matcher<P1>(), L, index + 0),
				get(type_matcher<P2>(), L, index + 1),
				get(type_matcher<P3>(), L, index + 2),
				get(type_matcher<P4>(), L, index + 3),
				get(type_matcher<P5>(), L, index + 4),
				get(type_matcher<P6>(), L, index + 5)
			);
			push(L, ret);
			return 1;
		}


		template <typename P1, typename P2, typename P3, typename P4,
					typename P5, typename P6, typename P7>
		static inline int call(RT (*func)(P1, P2, P3, P4, P5, P6, P7), lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);
			luaL_argassert(7, index + 6);

			RT ret = func(
				get(type_matcher<P1>(), L, index + 0),
				get(type_matcher<P2>(), L, index + 1),
				get(type_matcher<P3>(), L, index + 2),
				get(type_matcher<P4>(), L, index + 3),
				get(type_matcher<P5>(), L, index + 4),
				get(type_matcher<P6>(), L, index + 5),
				get(type_matcher<P7>(), L, index + 6)
			);
			push(L, ret);
			return 1;
		}


		template <typename P1, typename P2, typename P3, typename P4,
					typename P5, typename P6, typename P7, typename P8>
		static inline int call(RT (*func)(P1, P2, P3, P4, P5, P6, P7, P8), lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);
			luaL_argassert(7, index + 6);
			luaL_argassert(8, index + 7);

			RT ret = func(
				get(type_matcher<P1>(), L, index + 0),
				get(type_matcher<P2>(), L, index + 1),
				get(type_matcher<P3>(), L, index + 2),
				get(type_matcher<P4>(), L, index + 3),
				get(type_matcher<P5>(), L, index + 4),
				get(type_matcher<P6>(), L, index + 5),
				get(type_matcher<P7>(), L, index + 6),
				get(type_matcher<P8>(), L, index + 7)
			);
			push(L, ret);
			return 1;
		}


		template <typename P1, typename P2, typename P3, typename P4,
					typename P5, typename P6, typename P7, typename P8,
					typename P9>
		static inline int call(RT (*func)(P1, P2, P3, P4, P5, P6, P7, P8, P9), lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);
			luaL_argassert(7, index + 6);
			luaL_argassert(8, index + 7);
			luaL_argassert(9, index + 8);

			RT ret = func(
				get(type_matcher<P1>(), L, index + 0),
				get(type_matcher<P2>(), L, index + 1),
				get(type_matcher<P3>(), L, index + 2),
				get(type_matcher<P4>(), L, index + 3),
				get(type_matcher<P5>(), L, index + 4),
				get(type_matcher<P6>(), L, index + 5),
				get(type_matcher<P7>(), L, index + 6),
				get(type_matcher<P8>(), L, index + 7),
				get(type_matcher<P9>(), L, index + 8)
			);
			push(L, ret);
			return 1;
		}


		template <typename P1, typename P2, typename P3, typename P4,
					typename P5, typename P6, typename P7, typename P8,
					typename P9, typename P10>
		static inline int call(RT (*func)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10), lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);
			luaL_argassert(7, index + 6);
			luaL_argassert(8, index + 7);
			luaL_argassert(9, index + 8);
			luaL_argassert(10, index + 9);

			RT ret = func(
				get(type_matcher<P1>(), L, index + 0),
				get(type_matcher<P2>(), L, index + 1),
				get(type_matcher<P3>(), L, index + 2),
				get(type_matcher<P4>(), L, index + 3),
				get(type_matcher<P5>(), L, index + 4),
				get(type_matcher<P6>(), L, index + 5),
				get(type_matcher<P7>(), L, index + 6),
				get(type_matcher<P8>(), L, index + 7),
				get(type_matcher<P9>(), L, index + 8),
				get(type_matcher<P10>(), L, index + 9)
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
			luaL_argassert(1, index + 0);

			RT ret = (callee.*func)(
				get(type_matcher<P1>(), L, index + 0)
			);
			push(L, ret);
			return 1;
		}


		template <typename Callee, typename P1>
		static inline int call(Callee &callee, RT (Callee::*func)(P1) const, lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);

			RT ret = (callee.*func)(
				get(type_matcher<P1>(), L, index + 0)
			);
			push(L, ret);
			return 1;
		}


		template <typename Callee, typename P1, typename P2>
		static inline int call(Callee &callee, RT (Callee::*func)(P1, P2), lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);

			RT ret = (callee.*func)(
				get(type_matcher<P1>(), L, index + 0),
				get(type_matcher<P2>(), L, index + 1)
			);
			push(L, ret);
			return 1;
		}

		template <typename Callee, typename P1, typename P2>
		static inline int call(Callee &callee, RT (Callee::*func)(P1, P2) const, lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);

			RT ret = (callee.*func)(
				get(type_matcher<P1>(), L, index + 0),
				get(type_matcher<P2>(), L, index + 1)
			);
			push(L, ret);
			return 1;
		}

		template <typename Callee, typename P1, typename P2, typename P3>
		static inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3), lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);

			RT ret = (callee.*func)(
				get(type_matcher<P1>(), L, index + 0),
				get(type_matcher<P2>(), L, index + 1),
				get(type_matcher<P3>(), L, index + 2)
			);
			push(L, ret);
			return 1;
		}

		template <typename Callee, typename P1, typename P2, typename P3>
		static inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3) const, lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);

			RT ret = (callee.*func)(
				get(type_matcher<P1>(), L, index + 0),
				get(type_matcher<P2>(), L, index + 1),
				get(type_matcher<P3>(), L, index + 2)
			);
			push(L, ret);
			return 1;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4>
		static inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3, P4), lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);

			RT ret = (callee.*func)(
				get(type_matcher<P1>(), L, index + 0),
				get(type_matcher<P2>(), L, index + 1),
				get(type_matcher<P3>(), L, index + 2),
				get(type_matcher<P4>(), L, index + 3)
			);
			push(L, ret);
			return 1;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4>
		static inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3, P4) const, lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);

			RT ret = (callee.*func)(
				get(type_matcher<P1>(), L, index + 0),
				get(type_matcher<P2>(), L, index + 1),
				get(type_matcher<P3>(), L, index + 2),
				get(type_matcher<P4>(), L, index + 3)
			);
			push(L, ret);
			return 1;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5>
		static inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3, P4, P5), lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);

			RT ret = (callee.*func)(
				get(type_matcher<P1>(), L, index + 0),
				get(type_matcher<P2>(), L, index + 1),
				get(type_matcher<P3>(), L, index + 2),
				get(type_matcher<P4>(), L, index + 3),
				get(type_matcher<P5>(), L, index + 4)
			);
			push(L, ret);
			return 1;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5>
		static inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3, P4, P5) const, lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);

			RT ret = (callee.*func)(
				get(type_matcher<P1>(), L, index + 0),
				get(type_matcher<P2>(), L, index + 1),
				get(type_matcher<P3>(), L, index + 2),
				get(type_matcher<P4>(), L, index + 3),
				get(type_matcher<P5>(), L, index + 4)
			);
			push(L, ret);
			return 1;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6>
		static inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6), lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);

			RT ret = (callee.*func)(
				get(type_matcher<P1>(), L, index + 0),
				get(type_matcher<P2>(), L, index + 1),
				get(type_matcher<P3>(), L, index + 2),
				get(type_matcher<P4>(), L, index + 3),
				get(type_matcher<P5>(), L, index + 4),
				get(type_matcher<P6>(), L, index + 5)
			);
			push(L, ret);
			return 1;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6>
		static inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6) const, lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);

			RT ret = (callee.*func)(
				get(type_matcher<P1>(), L, index + 0),
				get(type_matcher<P2>(), L, index + 1),
				get(type_matcher<P3>(), L, index + 2),
				get(type_matcher<P4>(), L, index + 3),
				get(type_matcher<P5>(), L, index + 4),
				get(type_matcher<P6>(), L, index + 5)
			);
			push(L, ret);
			return 1;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6, typename P7>
		static inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6, P7), lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);
			luaL_argassert(7, index + 6);

			RT ret = (callee.*func)(
				get(type_matcher<P1>(), L, index + 0),
				get(type_matcher<P2>(), L, index + 1),
				get(type_matcher<P3>(), L, index + 2),
				get(type_matcher<P4>(), L, index + 3),
				get(type_matcher<P5>(), L, index + 4),
				get(type_matcher<P6>(), L, index + 5),
				get(type_matcher<P7>(), L, index + 6)
			);
			push(L, ret);
			return 1;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6, typename P7>
		static inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6, P7) const, lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);
			luaL_argassert(7, index + 6);

			RT ret = (callee.*func)(
				get(type_matcher<P1>(), L, index + 0),
				get(type_matcher<P2>(), L, index + 1),
				get(type_matcher<P3>(), L, index + 2),
				get(type_matcher<P4>(), L, index + 3),
				get(type_matcher<P5>(), L, index + 4),
				get(type_matcher<P6>(), L, index + 5),
				get(type_matcher<P7>(), L, index + 6)
			);
			push(L, ret);
			return 1;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6, typename P7, typename P8>
		static inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6, P7, P8), lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);
			luaL_argassert(7, index + 6);
			luaL_argassert(8, index + 7);

			RT ret = (callee.*func)(
				get(type_matcher<P1>(), L, index + 0),
				get(type_matcher<P2>(), L, index + 1),
				get(type_matcher<P3>(), L, index + 2),
				get(type_matcher<P4>(), L, index + 3),
				get(type_matcher<P5>(), L, index + 4),
				get(type_matcher<P6>(), L, index + 5),
				get(type_matcher<P7>(), L, index + 6),
				get(type_matcher<P8>(), L, index + 7)
			);
			push(L, ret);
			return 1;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6, typename P7, typename P8>
		static inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6, P7, P8) const, lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);
			luaL_argassert(7, index + 6);
			luaL_argassert(8, index + 7);

			RT ret = (callee.*func)(
				get(type_matcher<P1>(), L, index + 0),
				get(type_matcher<P2>(), L, index + 1),
				get(type_matcher<P3>(), L, index + 2),
				get(type_matcher<P4>(), L, index + 3),
				get(type_matcher<P5>(), L, index + 4),
				get(type_matcher<P6>(), L, index + 5),
				get(type_matcher<P7>(), L, index + 6),
				get(type_matcher<P8>(), L, index + 7)
			);
			push(L, ret);
			return 1;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6, typename P7, typename P8,
					typename P9>
		static inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6, P7, P8, P9), lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);
			luaL_argassert(7, index + 6);
			luaL_argassert(8, index + 7);
			luaL_argassert(9, index + 8);

			RT ret = (callee.*func)(
				get(type_matcher<P1>(), L, index + 0),
				get(type_matcher<P2>(), L, index + 1),
				get(type_matcher<P3>(), L, index + 2),
				get(type_matcher<P4>(), L, index + 3),
				get(type_matcher<P5>(), L, index + 4),
				get(type_matcher<P6>(), L, index + 5),
				get(type_matcher<P7>(), L, index + 6),
				get(type_matcher<P8>(), L, index + 7),
				get(type_matcher<P9>(), L, index + 8)
			);
			push(L, ret);
			return 1;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6, typename P7, typename P8,
					typename P9>
		static inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6, P7, P8, P9) const, lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);
			luaL_argassert(7, index + 6);
			luaL_argassert(8, index + 7);
			luaL_argassert(9, index + 8);

			RT ret = (callee.*func)(
				get(type_matcher<P1>(), L, index + 0),
				get(type_matcher<P2>(), L, index + 1),
				get(type_matcher<P3>(), L, index + 2),
				get(type_matcher<P4>(), L, index + 3),
				get(type_matcher<P5>(), L, index + 4),
				get(type_matcher<P6>(), L, index + 5),
				get(type_matcher<P7>(), L, index + 6),
				get(type_matcher<P8>(), L, index + 7),
				get(type_matcher<P9>(), L, index + 8)
			);
			push(L, ret);
			return 1;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6, typename P7, typename P8,
					typename P9, typename P10>
		static inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10), lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);
			luaL_argassert(7, index + 6);
			luaL_argassert(8, index + 7);
			luaL_argassert(9, index + 8);
			luaL_argassert(10, index + 9);

			RT ret = (callee.*func)(
				get(type_matcher<P1>(), L, index + 0),
				get(type_matcher<P2>(), L, index + 1),
				get(type_matcher<P3>(), L, index + 2),
				get(type_matcher<P4>(), L, index + 3),
				get(type_matcher<P5>(), L, index + 4),
				get(type_matcher<P6>(), L, index + 5),
				get(type_matcher<P7>(), L, index + 6),
				get(type_matcher<P8>(), L, index + 7),
				get(type_matcher<P9>(), L, index + 8),
				get(type_matcher<P10>(), L, index + 9)
			);
			push(L, ret);
			return 1;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6, typename P7, typename P8,
					typename P9, typename P10>
		static inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10) const, lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);
			luaL_argassert(7, index + 6);
			luaL_argassert(8, index + 7);
			luaL_argassert(9, index + 8);
			luaL_argassert(10, index + 9);

			RT ret = (callee.*func)(
				get(type_matcher<P1>(), L, index + 0),
				get(type_matcher<P2>(), L, index + 1),
				get(type_matcher<P3>(), L, index + 2),
				get(type_matcher<P4>(), L, index + 3),
				get(type_matcher<P5>(), L, index + 4),
				get(type_matcher<P6>(), L, index + 5),
				get(type_matcher<P7>(), L, index + 6),
				get(type_matcher<P8>(), L, index + 7),
				get(type_matcher<P9>(), L, index + 8),
				get(type_matcher<P10>(), L, index + 9)
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
			luaL_argassert(1, index + 0);

			func(
				get(type_matcher<P1>(), L, index + 0)
			);
			return 0;
		}


		template <typename P1, typename P2>
		static inline int call(void (*func)(P1, P2), lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);

			func(
				get(type_matcher<P1>(), L, index + 0),
				get(type_matcher<P2>(), L, index + 1)
			);
			return 0;
		}


		template <typename P1, typename P2, typename P3>
		static inline int call(void (*func)(P1, P2, P3), lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);

			func(
				get(type_matcher<P1>(), L, index + 0),
				get(type_matcher<P2>(), L, index + 1),
				get(type_matcher<P3>(), L, index + 2)
			);
			return 0;
		}

		template <typename P1, typename P2, typename P3, typename P4>
		static inline int call(void (*func)(P1, P2, P3, P4), lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);

			func(
				get(type_matcher<P1>(), L, index + 0),
				get(type_matcher<P2>(), L, index + 1),
				get(type_matcher<P3>(), L, index + 2),
				get(type_matcher<P4>(), L, index + 3)
			);
			return 0;
		}

		template <typename P1, typename P2, typename P3, typename P4, typename P5>
		static inline int call(void (*func)(P1, P2, P3, P4, P5), lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);

			func(
				get(type_matcher<P1>(), L, index + 0),
				get(type_matcher<P2>(), L, index + 1),
				get(type_matcher<P3>(), L, index + 2),
				get(type_matcher<P4>(), L, index + 3),
				get(type_matcher<P5>(), L, index + 4)
			);
			return 0;
		}

		template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
		static inline int call(void (*func)(P1, P2, P3, P4, P5, P6), lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);

			func(
				get(type_matcher<P1>(), L, index + 0),
				get(type_matcher<P2>(), L, index + 1),
				get(type_matcher<P3>(), L, index + 2),
				get(type_matcher<P4>(), L, index + 3),
				get(type_matcher<P5>(), L, index + 4),
				get(type_matcher<P6>(), L, index + 5)
			);
			return 0;
		}

		template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
		static inline int call(void (*func)(P1, P2, P3, P4, P5, P6, P7), lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);
			luaL_argassert(7, index + 6);

			func(
				get(type_matcher<P1>(), L, index + 0),
				get(type_matcher<P2>(), L, index + 1),
				get(type_matcher<P3>(), L, index + 2),
				get(type_matcher<P4>(), L, index + 3),
				get(type_matcher<P5>(), L, index + 4),
				get(type_matcher<P6>(), L, index + 5),
				get(type_matcher<P7>(), L, index + 6)
			);
			return 0;
		}

		template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
		static inline int call(void (*func)(P1, P2, P3, P4, P5, P6, P7, P8), lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);
			luaL_argassert(7, index + 6);
			luaL_argassert(8, index + 7);

			func(
				get(type_matcher<P1>(), L, index + 0),
				get(type_matcher<P2>(), L, index + 1),
				get(type_matcher<P3>(), L, index + 2),
				get(type_matcher<P4>(), L, index + 3),
				get(type_matcher<P5>(), L, index + 4),
				get(type_matcher<P6>(), L, index + 5),
				get(type_matcher<P7>(), L, index + 6),
				get(type_matcher<P8>(), L, index + 7)
			);
			return 0;
		}


		template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>
		static inline int call(void (*func)(P1, P2, P3, P4, P5, P6, P7, P8, P9), lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);
			luaL_argassert(7, index + 6);
			luaL_argassert(8, index + 7);
			luaL_argassert(9, index + 8);

			func(
				get(type_matcher<P1>(), L, index + 0),
				get(type_matcher<P2>(), L, index + 1),
				get(type_matcher<P3>(), L, index + 2),
				get(type_matcher<P4>(), L, index + 3),
				get(type_matcher<P5>(), L, index + 4),
				get(type_matcher<P6>(), L, index + 5),
				get(type_matcher<P7>(), L, index + 6),
				get(type_matcher<P8>(), L, index + 7),
				get(type_matcher<P9>(), L, index + 8)
			);
			return 0;
		}


		template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10>
		static inline int call(void (*func)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10), lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);
			luaL_argassert(7, index + 6);
			luaL_argassert(8, index + 7);
			luaL_argassert(9, index + 8);
			luaL_argassert(10, index + 9);

			func(
				get(type_matcher<P1>(), L, index + 0),
				get(type_matcher<P2>(), L, index + 1),
				get(type_matcher<P3>(), L, index + 2),
				get(type_matcher<P4>(), L, index + 3),
				get(type_matcher<P5>(), L, index + 4),
				get(type_matcher<P6>(), L, index + 5),
				get(type_matcher<P7>(), L, index + 6),
				get(type_matcher<P8>(), L, index + 7),
				get(type_matcher<P9>(), L, index + 8),
				get(type_matcher<P10>(), L, index + 9)
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
			luaL_argassert(1, index + 0);

			(callee.*func)(
				get(type_matcher<P1>(), L, index + 0)
			);
			return 0;
		}


		template <typename Callee, typename P1>
		static inline int call(Callee &callee, void (Callee::*func)(P1) const, lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);

			(callee.*func)(
				get(type_matcher<P1>(), L, index + 0)
			);
			return 0;
		}


		template <typename Callee, typename P1, typename P2>
		static inline int call(Callee &callee, void (Callee::*func)(P1, P2), lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);

			(callee.*func)(
				get(type_matcher<P1>(), L, index + 0),
				get(type_matcher<P2>(), L, index + 1)
			);
			return 0;
		}

		template <typename Callee, typename P1, typename P2>
		static inline int call(Callee &callee, void (Callee::*func)(P1, P2) const, lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);

			(callee.*func)(
				get(type_matcher<P1>(), L, index + 0),
				get(type_matcher<P2>(), L, index + 1)
			);
			return 0;
		}

		template <typename Callee, typename P1, typename P2, typename P3>
		static inline int call(Callee &callee, void (Callee::*func)(P1, P2, P3), lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);

			(callee.*func)(
				get(type_matcher<P1>(), L, index + 0),
				get(type_matcher<P2>(), L, index + 1),
				get(type_matcher<P3>(), L, index + 2)
			);
			return 0;
		}

		template <typename Callee, typename P1, typename P2, typename P3>
		static inline int call(Callee &callee, void (Callee::*func)(P1, P2, P3) const, lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);

			(callee.*func)(
				get(type_matcher<P1>(), L, index + 0),
				get(type_matcher<P2>(), L, index + 1),
				get(type_matcher<P3>(), L, index + 2)
			);
			return 0;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4>
		static inline int call(Callee &callee, void (Callee::*func)(P1, P2, P3, P4), lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);

			(callee.*func)(
				get(type_matcher<P1>(), L, index + 0),
				get(type_matcher<P2>(), L, index + 1),
				get(type_matcher<P3>(), L, index + 2),
				get(type_matcher<P4>(), L, index + 3)
			);
			return 0;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4>
		static inline int call(Callee &callee, void (Callee::*func)(P1, P2, P3, P4) const, lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);

			(callee.*func)(
				get(type_matcher<P1>(), L, index + 0),
				get(type_matcher<P2>(), L, index + 1),
				get(type_matcher<P3>(), L, index + 2),
				get(type_matcher<P4>(), L, index + 3)
			);
			return 0;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5>
		static inline int call(Callee &callee, void (Callee::*func)(P1, P2, P3, P4, P5), lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);

			(callee.*func)(
				get(type_matcher<P1>(), L, index + 0),
				get(type_matcher<P2>(), L, index + 1),
				get(type_matcher<P3>(), L, index + 2),
				get(type_matcher<P4>(), L, index + 3),
				get(type_matcher<P5>(), L, index + 4)
			);
			return 0;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5>
		static inline int call(Callee &callee, void (Callee::*func)(P1, P2, P3, P4, P5) const, lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);

			(callee.*func)(
				get(type_matcher<P1>(), L, index + 0),
				get(type_matcher<P2>(), L, index + 1),
				get(type_matcher<P3>(), L, index + 2),
				get(type_matcher<P4>(), L, index + 3),
				get(type_matcher<P5>(), L, index + 4)
			);
			return 0;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6>
		static inline int call(Callee &callee, void (Callee::*func)(P1, P2, P3, P4, P5, P6), lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);

			(callee.*func)(
				get(type_matcher<P1>(), L, index + 0),
				get(type_matcher<P2>(), L, index + 1),
				get(type_matcher<P3>(), L, index + 2),
				get(type_matcher<P4>(), L, index + 3),
				get(type_matcher<P5>(), L, index + 4),
				get(type_matcher<P6>(), L, index + 5)
			);
			return 0;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6>
		static inline int call(Callee &callee, void (Callee::*func)(P1, P2, P3, P4, P5, P6) const, lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);

			(callee.*func)(
				get(type_matcher<P1>(), L, index + 0),
				get(type_matcher<P2>(), L, index + 1),
				get(type_matcher<P3>(), L, index + 2),
				get(type_matcher<P4>(), L, index + 3),
				get(type_matcher<P5>(), L, index + 4),
				get(type_matcher<P6>(), L, index + 5)
			);
			return 0;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6, typename P7>
		static inline int call(Callee &callee, void (Callee::*func)(P1, P2, P3, P4, P5, P6, P7), lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);
			luaL_argassert(7, index + 6);

			(callee.*func)(
				get(type_matcher<P1>(), L, index + 0),
				get(type_matcher<P2>(), L, index + 1),
				get(type_matcher<P3>(), L, index + 2),
				get(type_matcher<P4>(), L, index + 3),
				get(type_matcher<P5>(), L, index + 4),
				get(type_matcher<P6>(), L, index + 5),
				get(type_matcher<P7>(), L, index + 6)
			);
			return 0;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6, typename P7>
		static inline int call(Callee &callee, void (Callee::*func)(P1, P2, P3, P4, P5, P6, P7) const, lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);
			luaL_argassert(7, index + 6);

			(callee.*func)(
				get(type_matcher<P1>(), L, index + 0),
				get(type_matcher<P2>(), L, index + 1),
				get(type_matcher<P3>(), L, index + 2),
				get(type_matcher<P4>(), L, index + 3),
				get(type_matcher<P5>(), L, index + 4),
				get(type_matcher<P6>(), L, index + 5),
				get(type_matcher<P7>(), L, index + 6)
			);
			return 0;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6, typename P7, typename P8>
		static inline int call(Callee &callee, void (Callee::*func)(P1, P2, P3, P4, P5, P6, P7, P8), lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);
			luaL_argassert(7, index + 6);
			luaL_argassert(8, index + 7);

			(callee.*func)(
				get(type_matcher<P1>(), L, index + 0),
				get(type_matcher<P2>(), L, index + 1),
				get(type_matcher<P3>(), L, index + 2),
				get(type_matcher<P4>(), L, index + 3),
				get(type_matcher<P5>(), L, index + 4),
				get(type_matcher<P6>(), L, index + 5),
				get(type_matcher<P7>(), L, index + 6),
				get(type_matcher<P8>(), L, index + 7)
			);
			return 0;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6, typename P7, typename P8>
		static inline int call(Callee &callee, void (Callee::*func)(P1, P2, P3, P4, P5, P6, P7, P8) const, lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);
			luaL_argassert(7, index + 6);
			luaL_argassert(8, index + 7);

			(callee.*func)(
				get(type_matcher<P1>(), L, index + 0),
				get(type_matcher<P2>(), L, index + 1),
				get(type_matcher<P3>(), L, index + 2),
				get(type_matcher<P4>(), L, index + 3),
				get(type_matcher<P5>(), L, index + 4),
				get(type_matcher<P6>(), L, index + 5),
				get(type_matcher<P7>(), L, index + 6),
				get(type_matcher<P8>(), L, index + 7)
			);
			return 0;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6, typename P7, typename P8,
					typename P9>
		static inline int call(Callee &callee, void (Callee::*func)(P1, P2, P3, P4, P5, P6, P7, P8, P9), lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);
			luaL_argassert(7, index + 6);
			luaL_argassert(8, index + 7);
			luaL_argassert(9, index + 8);

			(callee.*func)(
				get(type_matcher<P1>(), L, index + 0),
				get(type_matcher<P2>(), L, index + 1),
				get(type_matcher<P3>(), L, index + 2),
				get(type_matcher<P4>(), L, index + 3),
				get(type_matcher<P5>(), L, index + 4),
				get(type_matcher<P6>(), L, index + 5),
				get(type_matcher<P7>(), L, index + 6),
				get(type_matcher<P8>(), L, index + 7),
				get(type_matcher<P9>(), L, index + 8)
			);
			return 0;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6, typename P7, typename P8,
					typename P9>
		static inline int call(Callee &callee, void (Callee::*func)(P1, P2, P3, P4, P5, P6, P7, P8, P9) const, lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);
			luaL_argassert(7, index + 6);
			luaL_argassert(8, index + 7);
			luaL_argassert(9, index + 8);

			(callee.*func)(
				get(type_matcher<P1>(), L, index + 0),
				get(type_matcher<P2>(), L, index + 1),
				get(type_matcher<P3>(), L, index + 2),
				get(type_matcher<P4>(), L, index + 3),
				get(type_matcher<P5>(), L, index + 4),
				get(type_matcher<P6>(), L, index + 5),
				get(type_matcher<P7>(), L, index + 6),
				get(type_matcher<P8>(), L, index + 7),
				get(type_matcher<P9>(), L, index + 8)
			);
			return 0;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6, typename P7, typename P8,
					typename P9, typename P10>
		static inline int call(Callee &callee, void (Callee::*func)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10), lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);
			luaL_argassert(7, index + 6);
			luaL_argassert(8, index + 7);
			luaL_argassert(9, index + 8);
			luaL_argassert(10, index + 9);

			(callee.*func)(
				get(type_matcher<P1>(), L, index + 0),
				get(type_matcher<P2>(), L, index + 1),
				get(type_matcher<P3>(), L, index + 2),
				get(type_matcher<P4>(), L, index + 3),
				get(type_matcher<P5>(), L, index + 4),
				get(type_matcher<P6>(), L, index + 5),
				get(type_matcher<P7>(), L, index + 6),
				get(type_matcher<P8>(), L, index + 7),
				get(type_matcher<P9>(), L, index + 8),
				get(type_matcher<P10>(), L, index + 9)
			);
			return 0;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6, typename P7, typename P8,
					typename P9, typename P10>
		static inline int call(Callee &callee, void (Callee::*func)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10) const, lua_State *L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);
			luaL_argassert(7, index + 6);
			luaL_argassert(8, index + 7);
			luaL_argassert(9, index + 8);
			luaL_argassert(10, index + 9);

			(callee.*func)(
				get(type_matcher<P1>(), L, index + 0),
				get(type_matcher<P2>(), L, index + 1),
				get(type_matcher<P3>(), L, index + 2),
				get(type_matcher<P4>(), L, index + 3),
				get(type_matcher<P5>(), L, index + 4),
				get(type_matcher<P6>(), L, index + 5),
				get(type_matcher<P7>(), L, index + 6),
				get(type_matcher<P8>(), L, index + 7),
				get(type_matcher<P9>(), L, index + 8),
				get(type_matcher<P10>(), L, index + 9)
			);
			return 0;
		}
	};


	template <typename RT>
	inline int call(RT (*func)(), lua_State *L, int index)
	{
		return result_matcher<RT>::call(func, L, index);
	}


	template <typename RT, typename P1>
	inline int call(RT (*func)(P1), lua_State *L, int index)
	{
		return result_matcher<RT>::call(func, L, index);
	}


	template <typename RT, typename P1, typename P2>
	inline int call(RT (*func)(P1, P2), lua_State *L, int index)
	{
		return result_matcher<RT>::call(func, L, index);
	}


	template <typename RT, typename P1, typename P2, typename P3>
	inline int call(RT (*func)(P1, P2, P3), lua_State *L, int index)
	{
		return result_matcher<RT>::call(func, L, index);
	}


	template <typename RT, typename P1, typename P2, typename P3, typename P4>
	inline int call(RT (*func)(P1, P2, P3, P4), lua_State *L, int index)
	{
		return result_matcher<RT>::call(func, L, index);
	}


	template <typename RT, typename P1, typename P2, typename P3, typename P4,
				typename P5>
	inline int call(RT (*func)(P1, P2, P3, P4, P5), lua_State *L, int index)
	{
		return result_matcher<RT>::call(func, L, index);
	}


	template <typename RT, typename P1, typename P2, typename P3, typename P4,
				typename P5, typename P6>
	inline int call(RT (*func)(P1, P2, P3, P4, P5, P6), lua_State *L, int index)
	{
		return result_matcher<RT>::call(func, L, index);
	}


	template <typename RT, typename P1, typename P2, typename P3, typename P4,
				typename P5, typename P6, typename P7>
	inline int call(RT (*func)(P1, P2, P3, P4, P5, P6, P7), lua_State *L, int index)
	{
		return result_matcher<RT>::call(func, L, index);
	}


	template <typename RT, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
	inline int call(RT (*func)(P1, P2, P3, P4, P5, P6, P7, P8), lua_State *L, int index)
	{
		return result_matcher<RT>::call(func, L, index);
	}


	template <typename RT, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>
	inline int call(RT (*func)(P1, P2, P3, P4, P5, P6, P7, P8, P9), lua_State *L, int index)
	{
		return result_matcher<RT>::call(func, L, index);
	}


	template <typename RT, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10>
	inline int call(RT (*func)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10), lua_State *L, int index)
	{
		return result_matcher<RT>::call(func, L, index);
	}


	template <typename Callee, typename RT>
	inline int call(Callee &callee, RT (Callee::*func)(), lua_State *L, int index)
	{
		return result_matcher<RT>::call(callee, func, L, index);
	}


	template <typename Callee, typename RT>
	inline int call(Callee &callee, RT (Callee::*func)() const, lua_State *L, int index)
	{
		return result_matcher<RT>::call(callee, func, L, index);
	}


	template <typename Callee, typename RT, typename P1>
	inline int call(Callee &callee, RT (Callee::*func)(P1), lua_State *L, int index)
	{
		return result_matcher<RT>::call(callee, func, L, index);
	}


	template <typename Callee, typename RT, typename P1>
	inline int call(Callee &callee, RT (Callee::*func)(P1) const, lua_State *L, int index)
	{
		return result_matcher<RT>::call(callee, func, L, index);
	}


	template <typename Callee, typename RT, typename P1, typename P2>
	inline int call(Callee &callee, RT (Callee::*func)(P1, P2), lua_State *L, int index)
	{
		return result_matcher<RT>::call(callee, func, L, index);
	}


	template <typename Callee, typename RT, typename P1, typename P2>
	inline int call(Callee &callee, RT (Callee::*func)(P1, P2) const, lua_State *L, int index)
	{
		return result_matcher<RT>::call(callee, func, L, index);
	}


	template <typename Callee, typename RT, typename P1, typename P2, typename P3>
	inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3), lua_State *L, int index)
	{
		return result_matcher<RT>::call(callee, func, L, index);
	}


	template <typename Callee, typename RT, typename P1, typename P2, typename P3>
	inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3) const, lua_State *L, int index)
	{
		return result_matcher<RT>::call(callee, func, L, index);
	}


	template <typename Callee, typename RT, typename P1, typename P2, typename P3,
				typename P4>
	inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3, P4), lua_State *L, int index)
	{
		return result_matcher<RT>::call(callee, func, L, index);
	}


	template <typename Callee, typename RT, typename P1, typename P2, typename P3,
				typename P4>
	inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3, P4) const, lua_State *L, int index)
	{
		return result_matcher<RT>::call(callee, func, L, index);
	}


	template <typename Callee, typename RT, typename P1, typename P2, typename P3,
				typename P4, typename P5>
	inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3, P4, P5), lua_State *L, int index)
	{
		return result_matcher<RT>::call(callee, func, L, index);
	}


	template <typename Callee, typename RT, typename P1, typename P2, typename P3,
				typename P4, typename P5>
	inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3, P4, P5) const, lua_State *L, int index)
	{
		return result_matcher<RT>::call(callee, func, L, index);
	}


	template <typename Callee, typename RT, typename P1, typename P2, typename P3,
				typename P4, typename P5, typename P6>
	inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6), lua_State *L, int index)
	{
		return result_matcher<RT>::call(callee, func, L, index);
	}


	template <typename Callee, typename RT, typename P1, typename P2, typename P3,
				typename P4, typename P5, typename P6>
	inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6) const, lua_State *L, int index)
	{
		return result_matcher<RT>::call(callee, func, L, index);
	}


	template <typename Callee, typename RT, typename P1, typename P2, typename P3,
				typename P4, typename P5, typename P6, typename P7>
	inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6, P7), lua_State *L, int index)
	{
		return result_matcher<RT>::call(callee, func, L, index);
	}


	template <typename Callee, typename RT, typename P1, typename P2, typename P3,
				typename P4, typename P5, typename P6, typename P7>
	inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6, P7) const, lua_State *L, int index)
	{
		return result_matcher<RT>::call(callee, func, L, index);
	}


	template <typename Callee, typename RT, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
	inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6, P7, P8), lua_State *L, int index)
	{
		return result_matcher<RT>::call(callee, func, L, index);
	}


	template <typename Callee, typename RT, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
	inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6, P7, P8) const, lua_State *L, int index)
	{
		return result_matcher<RT>::call(callee, func, L, index);
	}


	template <typename Callee, typename RT, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>
	inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6, P7, P8, P9), lua_State *L, int index)
	{
		return result_matcher<RT>::call(callee, func, L, index);
	}


	template <typename Callee, typename RT, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>
	inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6, P7, P8, P9) const, lua_State *L, int index)
	{
		return result_matcher<RT>::call(callee, func, L, index);
	}


	template <typename Callee, typename RT, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10>
	inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10), lua_State *L, int index)
	{
		return result_matcher<RT>::call(callee, func, L, index);
	}


	template <typename Callee, typename RT, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10>
	inline int call(Callee &callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10) const, lua_State *L, int index)
	{
		return result_matcher<RT>::call(callee, func, L, index);
	}

} // namespace luastub

namespace luastub
{
	inline void *checkobject(lua_State *L, int index, const char *tname)
	{
		int type = lua_type(L, index);
		if (type == LUA_TUSERDATA)
			return *(void **)luaL_checkudata(L, index, tname);
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
				luaL_error(L, "expected a table with a lightuserdata field called __ptr.");
			void *ptr = lua_touserdata(L, -1);
			lua_pop(L, 1);
			return ptr;
		} else {
			luaL_typerror(L, index, tname);
		}

		return NULL;
	}
	
	inline void *getobject(lua_State *L)
	{
		int type = lua_type(L, 1);
		if (type == LUA_TUSERDATA)
			return *(void **)lua_touserdata(L, 1);
		else if (type == LUA_TTABLE) {
			lua_pushstring(L, "__ptr");
			lua_rawget(L, 1);
			if (!lua_isuserdata(L, -1))
				luaL_error(L, "expected a table with a lightuserdata field called __ptr.");
			void *ptr = lua_touserdata(L, -1);
			lua_pop(L, 1);
			return ptr;
		} else {
			luaL_argerror(L, 1, "expected a userdata or a table with a lightuserdata field called __ptr");
		}

		return NULL;
	}

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

	inline int state_function_dispatcher(lua_State *L) {
		typedef int (*Functor)(state*);
		unsigned char *buffer = getupvalue(L, 1);
		Functor &func = *(Functor*)(buffer);
 		return (*func)(state::from(L));
	}

	template <typename Callee>
	class state_method_dispatcher {
	public:
		static inline int dispatch(lua_State *L) {
			typedef int (Callee::*Functor)(state*);
 			unsigned char *buffer = getupvalue(L, 1);
			Callee &callee = **(Callee**)buffer;
			Functor &func = *(Functor*)(buffer + sizeof(Callee*));
			return (callee.*func)(state::from(L));
		}
	};

	template <typename Callee>
	class oo_state_method_dispatcher {
	public:
		static inline int dispatch(lua_State *L) {
			typedef int (Callee::*Functor)(state*);
 			unsigned char *buffer = getupvalue(L, 1);
			Functor &func = *(Functor*)(buffer);
			Callee &callee = *(Callee*)getobject(L);
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
			Callee &callee = *(Callee*)getobject(L);
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
				luaL_error(L, "attempt to read property '%s'", lua_tostring(L, 2));
			
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
			Callee* callee = (Callee*)getobject(L);
			push(L, *(Var*)((unsigned char*)callee + (unsigned int)offset));
			return 1;
		}

		static inline int setter(lua_State *L)
		{
			void *offset = getupvalue(L, 1);
			Callee* callee = (Callee*)getobject(L);
			if (!match(type_matcher<Var>(), L, 2))
				luaL_argerror(L, 2, "bad argument");
			*(Var*)((unsigned char*)callee + (unsigned int)offset) = get(type_matcher<Var>(), L, 2);
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
			if (!match(type_matcher<Var>(), L, 2))
				luaL_argerror(L, 2, "bad argument");
			*(Var*)offset = get(luastub::type_matcher<Var>(), L, 2);
			return 1;
		}
	};
} // namespace luastub

#endif

