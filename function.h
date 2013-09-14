#ifndef __LUASTUB_FUNCTION_H__
#define __LUASTUB_FUNCTION_H__

#include "helper.h"

namespace luastub {

	template <typename RT>
	class function
	{
	public:
		function(object& func)
			: func(func) {
		}

		function(state* L, const char *name) {
			func = L->getglobal(name);
		}
		
		object * operator ->() {
			return &func;
		}

		RT operator()() {
			lua_State *L = func.getcstate();
			stack_protector sp(func.getstate());
			func.push();

			if (lua_pcall(L, 0, 1, 0)) {
				const char *message = lua_tostring(L, -1);
				state::function_error_cb(message);
			}
			return get(type_matcher<RT>(), L, -1);
		}

		template <typename P1>
		RT operator()(P1 p1) {
			lua_State *L = func.getcstate();
			stack_protector sp(func.getstate());
			func.push();

			push(L, p1);

			if (lua_pcall(L, 1, 1, 0)) {
				const char *message = lua_tostring(L, -1);
				state::function_error_cb(message);
			}
			return get(type_matcher<RT>(), L, -1);
		}

		template <typename P1, typename P2>
		RT operator()(P1 p1, P2 p2) {
			lua_State *L = func.getcstate();
			stack_protector sp(func.getstate());
			func.push();

			push(L, p1);
			push(L, p2);

			if (lua_pcall(L, 2, 1, 0)) {
				const char *message = lua_tostring(L, -1);
				state::function_error_cb(message);
			}
			return get(type_matcher<RT>(), L, -1);
		}

		template <typename P1, typename P2, typename P3>
		RT operator()(P1 p1, P2 p2, P3 p3) {
			lua_State *L = func.getcstate();
			stack_protector sp(func.getstate());
			func.push();

			push(L, p1);
			push(L, p2);
			push(L, p3);

			if (lua_pcall(L, 3, 1, 0)) {
				const char *message = lua_tostring(L, -1);
				state::function_error_cb(message);
			}
			return get(type_matcher<RT>(), L, -1);
		}

		template <typename P1, typename P2, typename P3, typename P4>
		RT operator()(P1 p1, P2 p2, P3 p3, P4 p4) {
			lua_State *L = func.getcstate();
			stack_protector sp(func.getstate());
			func.push();

			push(L, p1);
			push(L, p2);
			push(L, p3);
			push(L, p4);

			if (lua_pcall(L, 4, 1, 0)) {
				const char *message = lua_tostring(L, -1);
				state::function_error_cb(message);
			}
			return get(type_matcher<RT>(), L, -1);
		}

		template <typename P1, typename P2, typename P3, typename P4, typename P5>
		RT operator()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5) {
			lua_State *L = func.getcstate();
			stack_protector sp(func.getstate());
			func.push();

			push(L, p1);
			push(L, p2);
			push(L, p3);
			push(L, p4);
			push(L, p5);

			if (lua_pcall(L, 5, 1, 0)) {
				const char *message = lua_tostring(L, -1);
				state::function_error_cb(message);
			}
			return get(type_matcher<RT>(), L, -1);
		}

		template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
		RT operator()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6) {
			lua_State *L = func.getcstate();
			stack_protector sp(func.getstate());
			func.push();

			push(L, p1);
			push(L, p2);
			push(L, p3);
			push(L, p4);
			push(L, p5);
			push(L, p6);

			if (lua_pcall(L, 6, 1, 0)) {
				const char *message = lua_tostring(L, -1);
				state::function_error_cb(message);
			}
			return get(type_matcher<RT>(), L, -1);
		}

		template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
		RT operator()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7) {
			lua_State *L = func.getcstate();
			stack_protector sp(func.getstate());
			func.push();

			push(L, p1);
			push(L, p2);
			push(L, p3);
			push(L, p4);
			push(L, p5);
			push(L, p6);
			push(L, p7);

			if (lua_pcall(L, 7, 1, 0)) {
				const char *message = lua_tostring(L, -1);
				state::function_error_cb(message);
			}
			return get(type_matcher<RT>(), L, -1);
		}

	protected:
		object func;
	};


	template <>
	class function<void>
	{
	public:
		function(const object& func)
			: func(func) {
		}

		function(state* L, const char *name) {
			func = L->getglobal(name);
		}

		object * operator ->() {
			return &func;
		}
		
		void operator()() {
			lua_State *L = func.getcstate();
			stack_protector sp(func.getstate());
			func.push();

			if (lua_pcall(L, 0, 0, 0)) {
				const char *message = lua_tostring(L, -1);
				state::function_error_cb(message);
			}
		}

		template <typename P1>
		void operator()(P1 p1) {
			lua_State *L = func.getcstate();
			stack_protector sp(func.getstate());
			func.push();

			push(L, p1);

			if (lua_pcall(L, 1, 0, 0)) {
				const char *message = lua_tostring(L, -1);
				state::function_error_cb(message);
			}
		}

		template <typename P1, typename P2>
		void operator()(P1 p1, P2 p2) {
			lua_State *L = func.getcstate();
			stack_protector sp(func.getstate());
			func.push();

			push(L, p1);
			push(L, p2);

			if (lua_pcall(L, 2, 0, 0)) {
				const char *message = lua_tostring(L, -1);
				state::function_error_cb(message);
			}
		}

		template <typename P1, typename P2, typename P3>
		void operator()(P1 p1, P2 p2, P3 p3) {
			lua_State *L = func.getcstate();
			stack_protector sp(func.getstate());
			func.push();

			push(L, p1);
			push(L, p2);
			push(L, p3);

			if (lua_pcall(L, 3, 0, 0)) {
				const char *message = lua_tostring(L, -1);
				state::function_error_cb(message);
			}
		}

		template <typename P1, typename P2, typename P3, typename P4>
		void operator()(P1 p1, P2 p2, P3 p3, P4 p4) {
			lua_State *L = func.getcstate();
			stack_protector sp(func.getstate());
			func.push();

			push(L, p1);
			push(L, p2);
			push(L, p3);
			push(L, p4);

			if (lua_pcall(L, 4, 0, 0)) {
				const char *message = lua_tostring(L, -1);
				state::function_error_cb(message);
			}
		}

		template <typename P1, typename P2, typename P3, typename P4, typename P5>
		void operator()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5) {
			lua_State *L = func.getcstate();
			stack_protector sp(func.getstate());
			func.push();

			push(L, p1);
			push(L, p2);
			push(L, p3);
			push(L, p4);
			push(L, p5);

			if (lua_pcall(L, 5, 0, 0)) {
				const char *message = lua_tostring(L, -1);
				state::function_error_cb(message);
			}
		}

		template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
		void operator()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6) {
			lua_State *L = func.getcstate();
			stack_protector sp(func.getstate());
			func.push();

			push(L, p1);
			push(L, p2);
			push(L, p3);
			push(L, p4);
			push(L, p5);
			push(L, p6);

			if (lua_pcall(L, 6, 0, 0)) {
				const char *message = lua_tostring(L, -1);
				state::function_error_cb(message);
			}
		}

		template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
		void operator()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7) {
			lua_State *L = func.getcstate();
			stack_protector sp(func.getstate());
			func.push();

			push(L, p1);
			push(L, p2);
			push(L, p3);
			push(L, p4);
			push(L, p5);
			push(L, p6);
			push(L, p7);

			if (lua_pcall(L, 7, 0, 0)) {
				const char *message = lua_tostring(L, -1);
				state::function_error_cb(message);
			}
		}

	protected:
		object func;
	};

}

#endif
