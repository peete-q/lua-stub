
#ifndef __LUASTUB_STATE_H__
#define __LUASTUB_STATE_H__

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include <string.h>

#define LUASTUB_VERSION			"luastub 1.2"
#define LUASTUB_VERSION_NUM		120
#define LUASTUB_AUTHORS 		"Peete.Q"

namespace luastub
{
	class state;
	
	const class nil_t {} nil;
	
	class stack_index
	{
	public:
		stack_index(state *state, int index) : m_state(state), m_index(index) {}
		state *m_state;
		int m_index;
	};
	
	class state
	{
	public:
		static inline state *open() {return (state*)lua_open();}
		static inline state *from(lua_State *L) {return (state*)(L);}
		
		inline void close() {lua_close(cptr());}
		inline lua_State *cptr() const {return (lua_State*)this;}
		
		inline int gettop() const {return lua_gettop(cptr());}
		inline void settop(int index) {lua_settop(cptr(), index);}
		inline void remove(int index) {lua_remove(cptr(), index);}
		inline void insert(int index) {lua_insert(cptr(), index);}
		inline void replace(int index) {lua_replace(cptr(), index);}
		inline void xmove(state *to, int n) {lua_xmove(cptr(), to->cptr(), n);}
		inline int checkstack(int size) const {return lua_checkstack(cptr(), size);}
			
		inline int isnumber(int index) const {return lua_isnumber(cptr(), index);}
		inline int isstring(int index) const {return lua_isstring(cptr(), index);}
		inline int iscfunction(int index) const {return lua_iscfunction(cptr(), index);}
		inline int isuserdata(int index) const {return lua_isuserdata(cptr(), index);}
		inline int isfunction(int index) const {return lua_isfunction(cptr(), index);}
		inline int istable(int index) const {return lua_istable(cptr(), index);}
		inline int islightuserdata(int index) const {return lua_islightuserdata(cptr(), index);}
		inline int isnil(int index) const {return lua_isnil(cptr(), index);}
		inline int isboolean(int index) const {return lua_isboolean(cptr(), index);}
		inline int isthread(int index) const {return lua_isthread(cptr(), index);}
		inline int isnone(int index) const {return lua_isnone(cptr(), index);}
		inline int isnoneornil(int index) const {return lua_isnoneornil(cptr(), index);}
		inline int type(int index) const {return lua_type(cptr(), index);}
		inline const char *typename_(int type) const {return lua_typename(cptr(), type);}
		inline const char *typename_of(int index) const {return luaL_typename(cptr(), index);}
			
		inline int equal(int index1, int index2) const {return lua_equal(cptr(), index1, index2);}
		inline int rawequal(int index1, int index2) const {return lua_rawequal(cptr(), index1, index2);}
			
		inline lua_Number tonumber(int index) const {return lua_tonumber(cptr(), index);}
		inline lua_Integer tointeger(int index) const {return lua_tointeger(cptr(), index);}
		inline int toboolean(int index) const {return lua_toboolean(cptr(), index);}
		inline const char *tolstring(int index, size_t *len) const {return lua_tolstring(cptr(), index, len);}
		inline const char *tostring(int index) const {return lua_tostring(cptr(), index);}
		inline size_t objlen(int index) const {return lua_objlen(cptr(), index);}
		inline lua_CFunction tocfunction(int index) const {return lua_tocfunction(cptr(), index);}
		inline void *touserdata(int index) const {return lua_touserdata(cptr(), index);}
		inline lua_State *tothread(int index) const {return lua_tothread(cptr(), index);}
		inline const void *topointer(int index) const {return lua_topointer(cptr(), index);}
			
		inline stack_index pushvalue(int index) {lua_pushvalue(cptr(), index); return stack_index(this, gettop());}
		inline stack_index pushnil() {lua_pushnil(cptr()); return stack_index(this, gettop());}
		inline stack_index pushnumber(lua_Number n) {lua_pushnumber(cptr(), n); return stack_index(this, gettop());}
		inline stack_index pushinteger(int n) {lua_pushinteger(cptr(), n); return stack_index(this, gettop());}
		inline stack_index pushlstring(const char *s, size_t len) {lua_pushlstring(cptr(), s, len); return stack_index(this, gettop());}
		inline stack_index pushstring(const char *s) {lua_pushstring(cptr(), s); return stack_index(this, gettop());}
		inline stack_index pushvfstring(const char *fmt, va_list args) {lua_pushvfstring(cptr(), fmt, args); return stack_index(this, gettop());}
		inline stack_index pushcclosure(lua_CFunction fn, int n) {lua_pushcclosure(cptr(), fn, n); return stack_index(this, gettop());}
		inline stack_index pushcfunction(lua_CFunction f) {lua_pushcfunction(cptr(), f); return stack_index(this, gettop());}
		inline stack_index pushboolean(bool value) {lua_pushboolean(cptr(), value); return stack_index(this, gettop());}
		inline stack_index pushlightuserdata(void *ptr) {lua_pushlightuserdata(cptr(), ptr); return stack_index(this, gettop());}
		inline stack_index pushthread() {lua_pushthread(cptr()); return stack_index(this, gettop());}
			
		inline stack_index getglobals() {pushvalue(LUA_GLOBALSINDEX); return stack_index(this, gettop());}
		inline stack_index getglobal(const char *var) {getfield(LUA_GLOBALSINDEX, var); return stack_index(this, gettop());}
		inline stack_index getstack(int index) {return stack_index(this, index);}
		inline stack_index getregistry() {lua_getregistry(cptr()); return stack_index(this, gettop());}
		
		inline stack_index gettable(int index) {lua_gettable(cptr(), index); return stack_index(this, gettop());}
		inline stack_index getfield(int index, const char *key) {lua_getfield(cptr(), index, key); return stack_index(this, gettop());}
		inline stack_index rawget(int index) {lua_rawget(cptr(), index); return stack_index(this, gettop());}
		inline stack_index rawgeti(int index, int n) {lua_rawgeti(cptr(), index, n); return stack_index(this, gettop());}
		inline stack_index createtable(int narr, int nrec) {lua_createtable(cptr(), narr, nrec); return stack_index(this, gettop());}
		inline stack_index getmetatable(int objindex) {lua_getmetatable(cptr(), objindex); return stack_index(this, gettop());}
		inline stack_index newtable() {lua_newtable(cptr()); return stack_index(this, gettop());}
		inline stack_index upvalue(int n) {return stack_index(this, lua_upvalueindex(n));}
		inline void *newuserdata(size_t size) {return lua_newuserdata(cptr(), size);}
			
		inline void settable(int index) {lua_settable(cptr(), index);}
		inline void setfield(int index, const char *key) {lua_setfield(cptr(), index, key);}
		inline void rawset(int index) {lua_rawset(cptr(), index);}
		inline void rawseti(int index, int n) {lua_rawseti(cptr(), index, n);}
		inline void setmetatable(int index) {lua_setmetatable(cptr(), index);}
			
		inline void call(int nargs, int nresults) {lua_call(cptr(), nargs, nresults);}
		inline int pcall(int nargs, int nresults, int errfunc) {return lua_pcall(cptr(), nargs, nresults, errfunc);}
		inline int load(lua_Reader reader, void *data, const char *chunkname) {return lua_load(cptr(), reader, data, chunkname);}
			
		inline int yield(int nresults) {return lua_yield(cptr(), nresults);}
		inline int resume(int nargs) {return lua_resume(cptr(), nargs);}
		inline int status() {return lua_status(cptr());}
			
		inline int gc(int what, int data) {return lua_gc(cptr(), what, data);}
		inline int next(int index) {return lua_next(cptr(), index);}
		inline void concat(int n) {lua_concat(cptr(), n);}
		inline void pop(int n) {lua_pop(cptr(), n);}
			
		inline void register_(const char *libname, const luaL_Reg *l) {luaL_register(cptr(), libname, l);}
		inline int getmetafield(int obj, const char *e) {return luaL_getmetafield(cptr(), obj, e);}
		inline int callmeta(int obj, const char *e) {return luaL_callmeta(cptr(), obj, e);}
		inline int typerror(int narg, const char* tname) {return luaL_typerror(cptr(), narg, tname);}
		inline int argerror(int narg, const char *extramsg) {return luaL_argerror(cptr(), narg, extramsg);}
			
		inline const char *checklstring(int narg, size_t *l) const {return luaL_checklstring(cptr(), narg, l);}
		inline const char *optlstring(int narg, const char *def, size_t *l) const {return luaL_optlstring(cptr(), narg, def, l);}
		inline lua_Number checknumber(int narg) const {return luaL_checknumber(cptr(), narg);}
		inline lua_Number optnumber(int narg, lua_Number def) const {return luaL_optnumber(cptr(), narg, def);}
		inline lua_Integer checkinteger(int narg) const {return luaL_checkinteger(cptr(), narg);}
		inline lua_Integer optinteger(int narg, lua_Integer def) const {return luaL_optinteger(cptr(), narg, def);}
		inline void checkstack(int sz, const char *msg) {luaL_checkstack(cptr(), sz, msg);}
		inline void checktype(int narg, int t) {luaL_checktype(cptr(), narg, t);}
		inline void checkany(int narg) {luaL_checkany(cptr(), narg);}
		inline int newmetatable(const char *tname) {return luaL_newmetatable(cptr(), tname);}
		inline void *checkudata(int ud, const char *tname) {return luaL_checkudata(cptr(), ud, tname);}
		inline void where(int lvl) {luaL_where(cptr(), lvl);}
		inline int ref(int t) {return luaL_ref(cptr(), t);}
		inline void unref(int t, int ref) {luaL_unref(cptr(), t, ref);}
		
		inline int loadfile(const char *filename) {return luaL_loadfile(cptr(), filename);}
		inline int loadbuffer(const char *buff, size_t sz, const char *name) {return luaL_loadbuffer(cptr(), buff, sz, name);}
		inline int loadstring(const char *s) {return luaL_loadstring(cptr(), s);}
		inline int dofile(const char *filename) {return luaL_loadfile(cptr(), filename) || lua_pcall(cptr(), 0, LUA_MULTRET, 0);}
		inline int dobuffer(const char *buff, size_t sz, const char *name) {return luaL_loadbuffer(cptr(), buff, sz, name) || lua_pcall(cptr(), 0, LUA_MULTRET, 0);}
		inline int dostring(const char *s, const char *name) {return dobuffer(s, strlen(s), name);}
		inline int dostring(const char *s) {return luaL_loadstring(cptr(), s) || lua_pcall(cptr(), 0, LUA_MULTRET, 0);}
		inline void openlibs() {luaL_openlibs(cptr());}
		inline int absindex(int i) {return i > 0 || i < LUA_REGISTRYINDEX ? i : gettop() + i + 1;}
		inline int error() {return lua_error(cptr());}
		
		inline stack_index pushfstring(const char *fmt, ...)
		{
			va_list args;
			va_start(args, fmt);
			lua_pushvfstring(cptr(), fmt, args);
			va_end(args);
			return stack_index(this, gettop());
		}
		inline int error(const char *fmt, ...)
		{
			va_list args;
			va_start(args, fmt);
			luaL_where(cptr(), 1);
			lua_pushvfstring(cptr(), fmt, args);
			va_end(args);
			lua_concat(cptr(), 2);
			return lua_error(cptr());
		}
		inline void traceback(const char *fmt, ...)
		{
			lua_getglobal(cptr(), "debug");
			lua_getfield(cptr(), -1, "traceback");
			va_list args;
			va_start(args, fmt);
			luaL_where(cptr(), 1);
			lua_pushvfstring(cptr(), fmt, args);
			va_end(args);
			lua_call(cptr(), 1, 1);
		}
	};
	
	inline int upvalueindex(int n) {return lua_upvalueindex(n + 1);}
	
	class state_proxy
	{
	public:
		state_proxy() {m_state = state::open();}
		~state_proxy() {m_state->close();}
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
		~stack_protector() {restore();}
		void restore() {if (m_state) m_state->settop(m_top), m_state = NULL;}
		int gettop() {return m_top;}
	private:
		state *m_state;
		int m_top;
	};
	
}
#endif