
#include "state.h"
#include "function.h"
#include "cclass.h"

#define test(e,tab) printf("[test]	%s%s[%s]\n", #e, tab, (e)?"ok":"failed")

struct base
{
	void showbase() {printf("[0x%x] showbase\n", this);}
};
struct point : base
{
	int x, y;
	const char *where;
	point(const char *_where, int _x, int _y) : where(_where), x(_x), y(_y) {}
	~point() {printf("[%s:0x%x]	released\n", where, this, x, y);}
	void showme() {printf("[%s:0x%x]	is (%d,%d)\n", where, this, x, y);}
	static const char *classname() {return "struct point";}
	static int f(luastub::state *L) {
		L->pushstring("return point::f");
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
	
	luastub::cclass<base>(L, "base", luastub::ctor<void>())
		.def_method("showbase", &base::showbase);
		
	point pt("cpp", 0, 0);
	int n = 0;
	luastub::cclass<point>(L, "point", luastub::ctor<const char*,int,int>())
		.inherit("base")
		.def("classname", &point::classname)
		.def("f", &point::f)
		.def_method("showme", &point::showme)
		.def_property("n", &n)
		.def_property("x", &point::x)
		.def_getter("y", &point::y);
		
	luastub::cclass<point> cl = luastub::cclass<point>::query(L, "point");
	// new a point, manage life scope in lua
	g.set("o", cl.newobject("cpp", 1, 1));
	// new a point, manage life scope in c++
	g.set("p", cl.boxptr(&pt));
	g.def("g_showme", &pt, &point::showme);
	g.def("g_classname", &point::classname);
	g.def("g_f", &point::f);
	int err = L->dostring(
		"print('[functor]', g_classname())					"
		"print('[functor]', g_f())						"
		"v = point('lua',2,2)								"
		"print('[method]', v.classname())					"
		"print('[method]', v.f())						"
		"g_showme()											"
		"v:showme()										"
		"o:showme()											"
		"p:showme()										"
		"v.x = 1000										"
		"v.n = 2000										"
		"print('[v]', v.x, v.y, v.n)					"
		"print('[o]', o.x, o.y, o.n)						"
		"print('[p]', p.x, p.y, p.n)				"
		"v:showbase()										"
	);
	if (err)
		puts(L->getstack(-1).tostring());
	
	cl.unboxptr(&pt);
	err = L->dostring(
		"p.x = 1000										"
	);
	if (err)
		puts(L->getstack(-1).tostring());
	print(true, false, luastub::nil, n);
	print("---- test end ----");
	return 1;
}