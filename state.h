
#ifndef __LUASTUB_STATE_H__
#define __LUASTUB_STATE_H__

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#define LUASTUB_VERSION			"luastub 1.0 beta"
#define LUASTUB_VERSION_NUM		100
#define LUASTUB_AUTHORS 		"Peete.Q"

namespace luastub
{
	static class nil_t {} nil;
	
	class stack_object;
	
	class state
	{
	public:
		static state *open();
		static state *from(lua_State *L);
		static void close(state *self);
		
		int gettop() const;
		void settop(int index);
		void pushvalue(int index);
		void remove(int index);
		void insert(int index);
		void replace(int index);
		void xmove(state *to, int n);
		int checkstack(int size) const;
		
		int isnumber(int index) const;
		int isstring(int index) const;
		int iscfunction(int index) const;
		int isuserdata(int index) const;
		int isfunction(int index) const;
		int istable(int index) const;
		int islightuserdata(int index) const;
		int isnil(int index) const;
		int isboolean(int index) const;
		int isthread(int index) const;
		int isnone(int index) const;
		int isnoneornil(int index) const;
		int type(int index) const;
		const char *tname(int type) const;
		
		int equal(int index1, int index2) const;
		int rawequal(int index1, int index2) const;
		int compare(int index1, int index2, int op) const;
		
		lua_Number tonumber(int index) const;
		lua_Integer tointeger(int index) const;
		int toboolean(int index) const;
		const char *tolstring(int index, size_t *len) const;
		const char *tostring(int index) const;
		size_t objlen(int index) const;
		lua_CFunction tocfunction(int index) const;
		void *touserdata(int index) const;
		lua_State *tothread(int index) const;
		const void *topointer(int index) const;
		
		stack_object pushnil();
		stack_object pushnumber(lua_Number n);
		stack_object pushinteger(int n);
		stack_object pushlstring(const char *s, size_t len);
		stack_object pushstring(const char *s);
		stack_object pushvfstring(const char *fmt, va_list args);
		stack_object pushfstring(const char *fmt, ...);
		stack_object pushcclosure(lua_CFunction fn, int n);
		stack_object pushcfunction(lua_CFunction f);
		stack_object pushboolean(bool value);
		stack_object pushlightuserdata(void *p);
		stack_object pushthread();
		
		stack_object getglobals();
		stack_object getglobal(const char *var);
		stack_object getstack(int index);
		
		stack_object gettable(int index);
		stack_object getfield(int index, const char *key);
		stack_object rawget(int index);
		stack_object rawgeti(int index, int n);
		stack_object createtable(int narr, int nrec);
		stack_object getmetatable(int objindex);
		stack_object newtable();
		stack_object upvalue(int n);
		
		void *newuserdata(size_t size);
		
		void settable(int index);
		void setfield(int index, const char *key);
		void rawset(int index);
		void rawseti(int index, int n);
		void setmetatable(int index);
		
		void call(int nargs, int nresults);
		int pcall(int nargs, int nresults, int errfunc);
		int load(lua_Reader reader, void *data, const char *chunkname);
		
		int yield(int nresults);
		int resume(int nargs);
		int status();
		
		int gc(int what, int data);
		int error();
		int next(int index);
		void concat(int n);
		void pop(int n);
		
		void registers(const char *libname, const luaL_Reg *l);
		int getmetafield(int obj, const char *e);
		int callmeta(int obj, const char *e);
		int typerror(int narg, const char* tname);
		int argerror(int narg, const char *extramsg);
		
		const char *checklstring(int narg, size_t *l) const;
		const char *optlstring(int narg, const char *def, size_t *l) const;
		lua_Number checknumber(int narg) const;
		lua_Number optnumber(int narg, lua_Number def) const;
		lua_Integer checkinteger(int narg) const;
		lua_Integer optinteger(int narg, lua_Integer def) const;
		void checkstack(int sz, const char *msg);
		void checktype(int narg, int t);
		void checkany(int narg);
		int newmetatable(const char *tname);
		void *checkudata(int narg, const char *tname);
		void where(int lvl);
		int error(const char *fmt, ...);
		int ref(int t);
		void unref(int t, int ref);
		
		int loadfile(const char *filename);
		int loadbuffer(const char *buff, size_t sz, const char *name);
		int loadstring(const char *s);
		int dofile(const char *filename);
		int dobuffer(const char *buff, size_t sz, const char *name);
		int dostring(const char *s);
		void openlibs();

		int absindex(int index);
		lua_State *cptr() const;
		
		typedef void (*function_error_t)(const char *);
		static function_error_t function_error_cb;
	};
	
	class state_proxy
	{
	public:
		state_proxy() {m_state = state::open();}
		~state_proxy() {state::close(m_state);}
		state *operator ->() {return m_state;}
		operator state *() {return m_state;}
	private:
		state_proxy(const state_proxy&);
		state_proxy &operator =(const state_proxy&);
	private:
		state *m_state;
	};

	class stack_protector
	{
	public:
		stack_protector(state *state) : m_state(state), m_top(state->gettop()) {}
		~stack_protector() {m_state->settop(m_top);}
	private:
		state *m_state;
		int m_top;
	};
	
}
#include "state_imp.h"
#endif