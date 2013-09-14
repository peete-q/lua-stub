
#ifndef __LUASTUB_OBJECT_H__
#include "object.h"

namespace luastub
{
	inline state *state::open() {return (state*)lua_open();}
	inline state *state::from(lua_State *L) {return (state*)(L);}
	inline void state::close(state *self) {lua_close(self->cptr());}
	
	inline int state::gettop() const {return lua_gettop(cptr());}
	inline void state::settop(int index) {lua_settop(cptr(), index);}
	inline void state::pushvalue(int index) {lua_pushvalue(cptr(), index);}
	inline void state::remove(int index) {lua_remove(cptr(), index);}
	inline void state::insert(int index) {lua_insert(cptr(), index);}
	inline void state::replace(int index) {lua_replace(cptr(), index);}
	inline void state::xmove(state *to, int n) {lua_xmove(cptr(), to->cptr(), n);}
	inline int state::checkstack(int size) const {return lua_checkstack(cptr(), size);}
		
	inline int state::isnumber(int index) const {return lua_isnumber(cptr(), index);}
	inline int state::isstring(int index) const {return lua_isstring(cptr(), index);}
	inline int state::iscfunction(int index) const {return lua_iscfunction(cptr(), index);}
	inline int state::isuserdata(int index) const {return lua_isuserdata(cptr(), index);}
	inline int state::isfunction(int index) const {return lua_isfunction(cptr(), index);}
	inline int state::istable(int index) const {return lua_istable(cptr(), index);}
	inline int state::islightuserdata(int index) const {return lua_islightuserdata(cptr(), index);}
	inline int state::isnil(int index) const {return lua_isnil(cptr(), index);}
	inline int state::isboolean(int index) const {return lua_isboolean(cptr(), index);}
	inline int state::isthread(int index) const {return lua_isthread(cptr(), index);}
	inline int state::isnone(int index) const {return lua_isnone(cptr(), index);}
	inline int state::isnoneornil(int index) const {return lua_isnoneornil(cptr(), index);}
	inline int state::type(int index) const {return lua_type(cptr(), index);}
	inline const char *state::tname(int type) const {return lua_typename(cptr(), type);}
		
	inline int state::equal(int index1, int index2) const {return lua_equal(cptr(), index1, index2);}
	inline int state::rawequal(int index1, int index2) const {return lua_rawequal(cptr(), index1, index2);}
	//inline int state::compare(int index1, int index2, int op) const;
		
	inline lua_Number state::tonumber(int index) const {return lua_tonumber(cptr(), index);}
	inline lua_Integer state::tointeger(int index) const {return lua_tointeger(cptr(), index);}
	inline int state::toboolean(int index) const {return lua_toboolean(cptr(), index);}
	inline const char *state::tolstring(int index, size_t *len) const {return lua_tolstring(cptr(), index, len);}
	inline const char *state::tostring(int index) const {return lua_tostring(cptr(), index);}
	inline size_t state::objlen(int index) const {return lua_objlen(cptr(), index);}
	inline lua_CFunction state::tocfunction(int index) const {return lua_tocfunction(cptr(), index);}
	inline void *state::touserdata(int index) const {return lua_touserdata(cptr(), index);}
	inline lua_State *state::tothread(int index) const {return lua_tothread(cptr(), index);}
	inline const void *state::topointer(int index) const {return lua_topointer(cptr(), index);}
		
	inline stack_object state::pushnil() {lua_pushnil(cptr()); return stack_object(this, gettop());}
	inline stack_object state::pushnumber(lua_Number n) {lua_pushnumber(cptr(), n); return stack_object(this, gettop());}
	inline stack_object state::pushinteger(int n) {lua_pushinteger(cptr(), n); return stack_object(this, gettop());}
	inline stack_object state::pushlstring(const char *s, size_t len) {lua_pushlstring(cptr(), s, len); return stack_object(this, gettop());}
	inline stack_object state::pushstring(const char *s) {lua_pushstring(cptr(), s); return stack_object(this, gettop());}
	inline stack_object state::pushvfstring(const char *fmt, va_list args) {lua_pushvfstring(cptr(), fmt, args); return stack_object(this, gettop());}
	inline stack_object state::pushfstring(const char *fmt, ...) {
		va_list args;
		va_start(args, fmt);
		lua_pushvfstring(cptr(), fmt, args);
		va_end(args);
		return stack_object(this, gettop());
	}
	inline stack_object state::pushcclosure(lua_CFunction fn, int n) {lua_pushcclosure(cptr(), fn, n); return stack_object(this, gettop());}
	inline stack_object state::pushcfunction(lua_CFunction f) {lua_pushcfunction(cptr(), f); return stack_object(this, gettop());}
	inline stack_object state::pushboolean(bool value) {lua_pushboolean(cptr(), value); return stack_object(this, gettop());}
	inline stack_object state::pushlightuserdata(void *p) {lua_pushlightuserdata(cptr(), p); return stack_object(this, gettop());}
	inline stack_object state::pushthread() {lua_pushthread(cptr()); return stack_object(this, gettop());}
		
	inline stack_object state::getglobals() {pushvalue(LUA_GLOBALSINDEX); return stack_object(this, gettop());}
	inline stack_object state::getglobal(const char *var) {getfield(LUA_GLOBALSINDEX, var); return stack_object(this, gettop());}
	inline stack_object state::getstack(int index) {return stack_object(this, index);}
	
	inline stack_object state::gettable(int index) {lua_gettable(cptr(), index); return stack_object(this, gettop());}
	inline stack_object state::getfield(int index, const char *key) {lua_getfield(cptr(), index, key); return stack_object(this, gettop());}
	inline stack_object state::rawget(int index) {lua_rawget(cptr(), index); return stack_object(this, gettop());}
	inline stack_object state::rawgeti(int index, int n) {lua_rawgeti(cptr(), index, n); return stack_object(this, gettop());}
	inline stack_object state::createtable(int narr, int nrec) {lua_createtable(cptr(), narr, nrec); return stack_object(this, gettop());}
	inline stack_object state::getmetatable(int objindex) {lua_getmetatable(cptr(), objindex); return stack_object(this, gettop());}
	inline stack_object state::newtable() {lua_newtable(cptr()); return stack_object(this, gettop());}
	inline stack_object state::upvalue(int n) {return stack_object(this, lua_upvalueindex(n));}
	inline void *state::newuserdata(size_t size) {return lua_newuserdata(cptr(), size);}
		
	inline void state::settable(int index) {lua_settable(cptr(), index);}
	inline void state::setfield(int index, const char *key) {lua_setfield(cptr(), index, key);}
	inline void state::rawset(int index) {lua_rawset(cptr(), index);}
	inline void state::rawseti(int index, int n) {lua_rawseti(cptr(), index, n);}
	inline void state::setmetatable(int index) {lua_setmetatable(cptr(), index);}
		
	inline void state::call(int nargs, int nresults) {lua_call(cptr(), nargs, nresults);}
	inline int state::pcall(int nargs, int nresults, int errfunc) {return lua_pcall(cptr(), nargs, nresults, errfunc);}
	inline int state::load(lua_Reader reader, void *data, const char *chunkname) {return lua_load(cptr(), reader, data, chunkname);}
		
	inline int state::yield(int nresults) {return lua_yield(cptr(), nresults);}
	inline int state::resume(int nargs) {return lua_resume(cptr(), nargs);}
	inline int state::status() {return lua_status(cptr());}
		
	inline int state::gc(int what, int data) {return lua_gc(cptr(), what, data);}
	inline int state::error() {return lua_error(cptr());}
	inline int state::next(int index) {return lua_next(cptr(), index);}
	inline void state::concat(int n) {lua_concat(cptr(), n);}
	inline void state::pop(int n) {lua_pop(cptr(), n);}
		
	inline void state::registers(const char *libname, const luaL_Reg *l) {luaL_register(cptr(), libname, l);}
	inline int state::getmetafield(int obj, const char *e) {return luaL_getmetafield(cptr(), obj, e);}
	inline int state::callmeta(int obj, const char *e) {return luaL_callmeta(cptr(), obj, e);}
	inline int state::typerror(int narg, const char* tname) {return luaL_typerror(cptr(), narg, tname);}
	inline int state::argerror(int narg, const char *extramsg) {return luaL_argerror(cptr(), narg, extramsg);}
		
	inline const char *state::checklstring(int narg, size_t *l) const {return luaL_checklstring(cptr(), narg, l);}
	inline const char *state::optlstring(int narg, const char *def, size_t *l) const {return luaL_optlstring(cptr(), narg, def, l);}
	inline lua_Number state::checknumber(int narg) const {return luaL_checknumber(cptr(), narg);}
	inline lua_Number state::optnumber(int narg, lua_Number def) const {return luaL_optnumber(cptr(), narg, def);}
	inline lua_Integer state::checkinteger(int narg) const {return luaL_checkinteger(cptr(), narg);}
	inline lua_Integer state::optinteger(int narg, lua_Integer def) const {return luaL_optinteger(cptr(), narg, def);}
	inline void state::checkstack(int sz, const char *msg) {luaL_checkstack(cptr(), sz, msg);}
	inline void state::checktype(int narg, int t) {luaL_checktype(cptr(), narg, t);}
	inline void state::checkany(int narg) {luaL_checkany(cptr(), narg);}
	inline int state::newmetatable(const char *tname) {return luaL_newmetatable(cptr(), tname);}
	inline void *state::checkudata(int ud, const char *tname) {return luaL_checkudata(cptr(), ud, tname);}
	inline void state::where(int lvl) {luaL_where(cptr(), lvl);}
	inline int state::error(const char *fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		luaL_where(cptr(), 1);
		lua_pushvfstring(cptr(), fmt, args);
		va_end(args);
		lua_concat(cptr(), 2);
		return lua_error(cptr());
	}
	inline int state::ref(int t) {return luaL_ref(cptr(), t);}
	inline void state::unref(int t, int ref) {luaL_unref(cptr(), t, ref);}
	
	inline int state::loadfile(const char *filename) {return luaL_loadfile(cptr(), filename);}
	inline int state::loadbuffer(const char *buff, size_t sz, const char *name) {return luaL_loadbuffer(cptr(), buff, sz, name);}
	inline int state::loadstring(const char *s) {return luaL_loadstring(cptr(), s);}
	inline int state::dofile(const char *filename) {return luaL_loadfile(cptr(), filename) || lua_pcall(cptr(), 0, LUA_MULTRET, 0);}
	inline int state::dobuffer(const char *buff, size_t sz, const char *name) {return luaL_loadbuffer(cptr(), buff, sz, name) || lua_pcall(cptr(), 0, LUA_MULTRET, 0);}
	inline int state::dostring(const char *s) {return luaL_loadstring(cptr(), s) || lua_pcall(cptr(), 0, LUA_MULTRET, 0);}
	inline void state::openlibs() {luaL_openlibs(cptr());}
	
	inline int state::absindex(int i) {return i > 0 || i < LUA_REGISTRYINDEX ? i : gettop() + i + 1;}
	inline lua_State *state::cptr() const {return (lua_State*)this;}
	
	inline void print_error(const char *s) {puts(s);}
	state::function_error_t state::function_error_cb = &print_error;
}
#endif