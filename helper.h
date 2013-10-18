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
	inline void push(lua_State *L, T *value)					{lua_pushlightuserdata(L, (void*)value);}
	template<typename T>
	inline void push(lua_State *L, const T *value)				{lua_pushlightuserdata(L, (void*)value);}
	template<typename T>
	inline void push(lua_State *L, T value)						{value.push();}

	template<typename T>
	inline bool match(lua_State *L, int idx);
	
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
	inline T read(lua_State *L, int idx);
	
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
			lua_gettable(L, index);
			if (!lua_isuserdata(L, -1))
				luaL_error(L, "expected __ptr field");
			lua_getregistry(L);
			lua_pushvalue(L, -2);
			lua_rawget(L, -2);
			if (!lua_isuserdata(L, -1))
				luaL_error(L, "__ptr (0x%p) has released", lua_touserdata(L, -3));
			void *ptr = lua_touserdata(L, -1);
			lua_pop(L, 3);
			return ptr;
		} else {
			luaL_typerror(L, index, tname);
		}
		return NULL;
	}
	
	inline void *getobject(lua_State *L, int index)
	{
		int type = lua_type(L, index);
		if (type == LUA_TUSERDATA)
			return lua_touserdata(L, index);
		else if (type == LUA_TTABLE) {
			lua_pushstring(L, "__ptr");
			lua_gettable(L, index);
			if (!lua_isuserdata(L, -1))
				luaL_error(L, "expected __ptr field");
			lua_getregistry(L);
			lua_pushvalue(L, -2);
			lua_rawget(L, -2);
			if (!lua_isuserdata(L, -1))
				luaL_error(L, "__ptr (0x%p) has released", lua_touserdata(L, -3));
			void *ptr = lua_touserdata(L, -1);
			lua_pop(L, 3);
			return ptr;
		} else {
			luaL_argerror(L, index, "expected userdata or table with __ptr");
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
			Callee &callee = *(Callee*)getobject(L, 1);
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
			Callee &callee = *(Callee*)getobject(L, 1);
			return call(callee, method, L, 2);
		}
	};
	
	inline int property_newindex(lua_State *L)
	{
													// t k v
		lua_getmetatable(L, 1);						// t k v m
		lua_pushstring(L, "__props");				// t k v m p
		lua_gettable(L, -2);						// t k v m pt
		if (lua_istable(L, -1))
		{
			lua_pushvalue(L, 2);					// t k v m pt k
			lua_gettable(L, -2);					// t k v m pt prop
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
		lua_gettable(L, -2);						// t v m lookup
		if (!lua_isnil(L, -1))
			return 1;

		lua_pop(L, 1);								// t v m
		lua_pushstring(L, "__props");				// t k v m __props
		lua_gettable(L, -2);						// t k v m pt

		if (lua_istable(L, -1))
		{
			lua_pushvalue(L, 2);					// t k v m pt k
			lua_gettable(L, -2);					// t k v m pt prop
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
		lua_gettable(L, index);
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
			Callee* callee = (Callee*)getobject(L, 1);
			push(L, *(Var*)((unsigned char*)callee + (unsigned int)offset));
			return 1;
		}

		static inline int setter(lua_State *L)
		{
			void *offset = getupvalue(L, 1);
			Callee* callee = (Callee*)getobject(L, 1);
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

