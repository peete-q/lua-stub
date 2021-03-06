#lua-stub

A lua wraper for c++, as flexible as LuaPlus, as convenient as luabind

##feature

* strong safety, detialed error info
* auto type conversion
* not invade into lua source code
* no dependency
* no need to compile
* smart life scope management

##example

use lua state:

    #include "state.h"
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

get lua object:

    luastub::object _G = L->getglobals();
    luastub::object tb = _G["tb"]

or

    luastub::object tb = L->getglobal("tb");
    
    
use lua object:

    tb.set(1, 1);       // tb[1] = 1
    tb.set("x", "xxx"); // tb.x = "xxx"
    tb.set("ref", tb);  // tb.ref = tb
    
traversal lua table:

    for (luastub::table_iterator it(tb); it; ++it)
      printf("key is %s, value is %s", it.key().tostring(), it.value().tostring());
    

export c++ class to lua:

    struct point
    {
    	int x, y;
    	point() : x(0), y(0) {}
    	void init(int _x, int _y) {x = _x; y = _y;}
    	void show() {printf("[0x%x]	is (%d,%d)\n", this, x, y);}
    	static const char *classname() {return "struct point";}
    	static int shared;
    };
	int global_var;
    luastub::cclass<point>(L, "point", &point::init)
      .def("classname", &point::classname)
      .def_method("show", &point::show)
      .def_property("x", &point::x)
      .def_getter("y", &point::y)
      .def_property("shared", &point::shared)
      .def_property("global_var", &global_var);

use exported class in lua

    pt = point(1, 1)
    pt.shared = 100	-- the var shared in all objects of the class point
    print(pt.x, pt.y, pt.shared)
    print(pt.classname())
    pt:show()
    
manage object life scope smartly:

	luastub::cclass<point> classpoint = luastub::cclass<point>::query(L, "point");
	// manage life scope in lua by gc
	luastub::stack_object a = classpoint.newobject();
	
	// manage life scope in c++ by yourself
	point pt;
	luastub::stack_object b = classpoint.newobject(&pt); // pass the pointer as a lightuserdata
	
	g.set("a", a);
	g.set("b", b);
	// use 'a' or 'b' like 'pt' in lua
	
run test.cpp for more
