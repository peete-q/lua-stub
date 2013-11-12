
#include "state.h"
#include "function.h"
#include "cclass.h"

#define test(e,tab) printf("[test]	%s%s[%s]\n", #e, tab, (e)?"ok":"failed")

struct base
{
	base() {}
	virtual void whoami() {puts("I am base");}
};
struct point : base
{
	int x, y;
	const char *where;
	point(const char *_where, int _x, int _y) : where(_where), x(_x), y(_y) {}
	~point() {printf("[%s:0x%x]	released\n", where, this, x, y);}
	void whoami() {printf("I am point %s:0x%x (%d,%d)\n", where, this, x, y);}
	static const char *classname() {return "My classname is point";}
	static int dosomething(luastub::state *L) {
		L->pushstring("ok, do what you want to do");
		return 1;
	}
};
int main()
{
	luastub::state_proxy L;
	L->openlibs();
	L->dostring(
		"tb = {			"
		"	n = 1,		"
		"	[1] = true,	"
		"	[2] = _G,	"
		"}				"
		"tb[3] = tb		"
		"_G[tb] = tb	"
	);

	luastub::object _G = L->getglobals();
	luastub::object tb = _G["tb"];
	luastub::function<void> print(_G["print"]);
	
	tb.set(0, _G[tb]);
	tb.set("x", "xxx");
	
	print("---- test begin ----");
	for (luastub::table_iterator it(tb); it; ++it)
		print("[tb]", it.key(), it.value());

	test(tb["n"] == 1,			"				");
	test(tb["x"] == "xxx",			"			");
	test(tb[0].topointer() == tb.topointer(), "	");
	test(tb[1] == true,			"				");
	test(tb[2].topointer() == _G.topointer(), "	");
	test(tb[3].topointer() == tb.topointer(), "	");
	test(_G[tb].topointer() == tb.topointer(), "	");
	test(_G["print"].isfunction(),	"			");
	
	point pt("cpp", 0, 0);
	int n = 0;
	luastub::cclass<base>(L, "base", luastub::ctor<void>())
		.def_method("whoami", &base::whoami);
		
	luastub::cclass<point>(L, "point", luastub::ctor<const char*,int,int>())
		.inherit("base")
		.def("classname", &point::classname)
		.def("dosomething", &point::dosomething)
		.def_property("n", &n)
		.def_property("x", &point::x)
		.def_getter("y", &point::y);
		
	luastub::cclass<point> c(L);
	// new a point, manage life span in lua
	_G.set("o", c.newobject("cpp", 1, 1));
	// new a point, manage life span in c++
	_G.set("p", &pt);
	_G.def("whoami", &pt, &point::whoami);
	_G.def("classname", &point::classname);
	_G.def("dosomething", &point::dosomething);
	int err = L->dostring(
		"print('[functor]', classname())					\n"
		"print('[functor]', dosomething())					\n"
		"v = point('lua',2,2)								\n"
		"print('[method]', v.classname())					\n"
		"print('[method]', v.dosomething())					\n"
		"whoami()											\n"
		"v:whoami()											\n"
		"o:whoami()											\n"
		"p:whoami()											\n"
		"v.x = 1000											\n"
		"v.n = 2000											\n"
		"print('[v]', v, v.x, v.y, v.n)						\n"
		"print('[o]', o, o.x, o.y, o.n)						\n"
		"print('[p]', p, p.x, p.y, p.n)						\n"
		"print('now p.__self() is ', p.__self())			\n",
		"test chunk 1"
	);
	if (err)
		puts(L->tostring(-1));
	
	c.unboxptr(&pt);
	err = L->dostring(
		"print('now p.__self() is ', type(p.__self()))		\n"
		"function fun() p.x = 1000	end						\n"
		"fun()												\n",
		"test chunk 2"
	);
	if (err)
		puts(L->tostring(-1));
	print(true, false, luastub::nil, n);
	print("---- test end ----");
	return 1;
}