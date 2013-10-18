
#include "state.h"
#include "function.h"
#include "cclass.h"

#define test(e,tab) printf("[test]	%s%s[%s]\n", #e, tab, (e)?"ok":"failed")

struct point
{
	int x, y;
	const char *where;
	point(const char *_where, int _x, int _y) : where(_where), x(_x), y(_y) {}
	~point() {printf("[%s:0x%x]	released\n", where, this, x, y);}
	void output() {printf("[%s:0x%x]	is (%d,%d)\n", where, this, x, y);}
	static const char *classname() {return "struct point";}
	static int foo(luastub::state *L) {
		L->pushstring("return point::foo");
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

	luastub::object g = L->getglobals();
	luastub::object tb = g["tb"];
	luastub::function<void> print(g["print"]);
	
	tb.set(0, g[tb]);
	tb.set("x", "xxx");
	
	print("---- test begin ----");
	for (luastub::table_iterator it(tb); it; ++it)
		print("[tb]", it.key(), it.value());
	test(tb["n"] == 1,			"				");
	test(tb["x"] == "xxx",			"			");
	test(tb[0].topointer() == tb.topointer(), "	");
	test(tb[1] == true,			"				");
	test(tb[2].topointer() == g.topointer(), "	");
	test(tb[3].topointer() == tb.topointer(), "	");
	test(g["print"].isfunction(),	"			");
	
	point pt("cpp", 0, 0);
	int n = 0;
	luastub::cclass<point>(L, "point", luastub::ctor<const char*,int,int>())
		.def("classname", &point::classname)
		.def("foo", &point::foo)
		.def_method("output", &point::output)
		.def_property("n", &n)
		.def_property("x", &point::x)
		.def_getter("y", &point::y);
		
	luastub::cclass<point> cl = luastub::cclass<point>::query(L, "point");
	// new a point, manage life scope in lua
	g.set("o", cl.newobject("cpp", 1, 1));
	// new a point, manage life scope in c++
	g.set("ptr", cl.boxptr(&pt));
	g.def("g_output", &pt, &point::output);
	g.def("g_classname", &point::classname);
	g.def("g_foo", &point::foo);
	int err = L->dostring(
		"print('[functor]', g_classname())					"
		"print('[functor]', g_foo())						"
		"pt = point('lua',2,2)								"
		"print('[method]', pt.classname())					"
		"print('[method]', pt.foo())						"
		"g_output()											"
		"pt:output()										"
		"o:output()											"
		"ptr:output()										"
		"pt.x = 1000										"
		"pt.n = 2000										"
		"print('[pt]', pt.x, pt.y, pt.n)					"
		"print('[o]', o.x, o.y, o.n)						"
		"print('[ptr]', ptr.x, ptr.y, ptr.n)				"
	);
	if (err)
		puts(L->getstack(-1).tostring());
	
	cl.unboxptr(&pt);
	err = L->dostring(
		"ptr.x = 1000										"
	);
	if (err)
		puts(L->getstack(-1).tostring());
	print(true, false, luastub::nil, n);
	print("---- test end ----");
	return 1;
}