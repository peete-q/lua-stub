#ifndef __LUASTUB_FUNCTION_H__
#define __LUASTUB_FUNCTION_H__

#include <vector>
#include "object.h"

namespace luastub {

	typedef std::vector<object> multi_result;
	
	template <typename RT>
	class function
	{
	public:
		function(const object &o)
			: m_object(o) {
		}

		function(state* L, const char *name) {
			m_object = L->getglobal(name);
		}
		
		void push() const {
			m_object.push();
		}
		
		bool operator ==(const function &f) const {
			return m_object == f.m_object;
		}
		
		bool operator ==(const object &o) const {
			return m_object == o;
		}

		RT operator()() {
			state *L = m_object.getstate();
			stack_protector sp(L);
			m_object.push();

			if (L->pcall(0, 1, 0)) {
				const char *message = L->tostring(-1);
				L->traceback(message);
				L->error();
			}
			return read<RT>(L, -1);
		}

		template <typename P1>
		RT operator()(P1 p1) {
			state *L = m_object.getstate();
			stack_protector sp(L);
			m_object.push();

			luastub::push(L, p1);

			if (L->pcall(1, 1, 0)) {
				const char *message = L->tostring(-1);
				L->traceback(message);
				L->error();
			}
			return read<RT>(L, -1);
		}

		template <typename P1, typename P2>
		RT operator()(P1 p1, P2 p2) {
			state *L = m_object.getstate();
			stack_protector sp(L);
			m_object.push();

			luastub::push(L, p1);
			luastub::push(L, p2);

			if (L->pcall(2, 1, 0)) {
				const char *message = L->tostring(-1);
				L->traceback(message);
				L->error();
			}
			return read<RT>(L, -1);
		}

		template <typename P1, typename P2, typename P3>
		RT operator()(P1 p1, P2 p2, P3 p3) {
			state *L = m_object.getstate();
			stack_protector sp(L);
			m_object.push();

			luastub::push(L, p1);
			luastub::push(L, p2);
			luastub::push(L, p3);

			if (L->pcall(3, 1, 0)) {
				const char *message = L->tostring(-1);
				L->traceback(message);
				L->error();
			}
			return read<RT>(L, -1);
		}

		template <typename P1, typename P2, typename P3, typename P4>
		RT operator()(P1 p1, P2 p2, P3 p3, P4 p4) {
			state *L = m_object.getstate();
			stack_protector sp(L);
			m_object.push();

			luastub::push(L, p1);
			luastub::push(L, p2);
			luastub::push(L, p3);
			luastub::push(L, p4);

			if (L->pcall(4, 1, 0)) {
				const char *message = L->tostring(-1);
				L->traceback(message);
				L->error();
			}
			return read<RT>(L, -1);
		}

		template <typename P1, typename P2, typename P3, typename P4, typename P5>
		RT operator()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5) {
			state *L = m_object.getstate();
			stack_protector sp(L);
			m_object.push();

			luastub::push(L, p1);
			luastub::push(L, p2);
			luastub::push(L, p3);
			luastub::push(L, p4);
			luastub::push(L, p5);

			if (L->pcall(5, 1, 0)) {
				const char *message = L->tostring(-1);
				L->traceback(message);
				L->error();
			}
			return read<RT>(L, -1);
		}

		template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
		RT operator()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6) {
			state *L = m_object.getstate();
			stack_protector sp(L);
			m_object.push();

			luastub::push(L, p1);
			luastub::push(L, p2);
			luastub::push(L, p3);
			luastub::push(L, p4);
			luastub::push(L, p5);
			luastub::push(L, p6);

			if (L->pcall(6, 1, 0)) {
				const char *message = L->tostring(-1);
				L->traceback(message);
				L->error();
			}
			return read<RT>(L, -1);
		}

		template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
		RT operator()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7) {
			state *L = m_object.getstate();
			stack_protector sp(L);
			m_object.push();

			luastub::push(L, p1);
			luastub::push(L, p2);
			luastub::push(L, p3);
			luastub::push(L, p4);
			luastub::push(L, p5);
			luastub::push(L, p6);
			luastub::push(L, p7);

			if (L->pcall(7, 1, 0)) {
				const char *message = L->tostring(-1);
				L->traceback(message);
				L->error();
			}
			return read<RT>(L, -1);
		}

	protected:
		object m_object;
	};

	template <>
	class function<void>
	{
	public:
		function(const object &o)
			: m_object(o) {
		}

		function(state* L, const char *name) {
			m_object = L->getglobal(name);
		}

		void push() const {
			m_object.push();
		}
		
		bool operator ==(const function &f) const {
			return m_object == f.m_object;
		}
		
		bool operator ==(const object &o) const {
			return m_object == o;
		}
		
		void operator()() {
			state *L = m_object.getstate();
			stack_protector sp(L);
			m_object.push();

			if (L->pcall(0, 0, 0)) {
				const char *message = L->tostring(-1);
				L->traceback(message);
				L->error();
			}
		}

		template <typename P1>
		void operator()(P1 p1) {
			state *L = m_object.getstate();
			stack_protector sp(L);
			m_object.push();

			luastub::push(L, p1);

			if (L->pcall(1, 0, 0)) {
				const char *message = L->tostring(-1);
				L->traceback(message);
				L->error();
			}
		}

		template <typename P1, typename P2>
		void operator()(P1 p1, P2 p2) {
			state *L = m_object.getstate();
			stack_protector sp(L);
			m_object.push();

			luastub::push(L, p1);
			luastub::push(L, p2);

			if (L->pcall(2, 0, 0)) {
				const char *message = L->tostring(-1);
				L->traceback(message);
				L->error();
			}
		}

		template <typename P1, typename P2, typename P3>
		void operator()(P1 p1, P2 p2, P3 p3) {
			state *L = m_object.getstate();
			stack_protector sp(L);
			m_object.push();

			luastub::push(L, p1);
			luastub::push(L, p2);
			luastub::push(L, p3);

			if (L->pcall(3, 0, 0)) {
				const char *message = L->tostring(-1);
				L->traceback(message);
				L->error();
			}
		}

		template <typename P1, typename P2, typename P3, typename P4>
		void operator()(P1 p1, P2 p2, P3 p3, P4 p4) {
			state *L = m_object.getstate();
			stack_protector sp(L);
			m_object.push();

			luastub::push(L, p1);
			luastub::push(L, p2);
			luastub::push(L, p3);
			luastub::push(L, p4);

			if (L->pcall(4, 0, 0)) {
				const char *message = L->tostring(-1);
				L->traceback(message);
				L->error();
			}
		}

		template <typename P1, typename P2, typename P3, typename P4, typename P5>
		void operator()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5) {
			state *L = m_object.getstate();
			stack_protector sp(L);
			m_object.push();

			luastub::push(L, p1);
			luastub::push(L, p2);
			luastub::push(L, p3);
			luastub::push(L, p4);
			luastub::push(L, p5);

			if (L->pcall(5, 0, 0)) {
				const char *message = L->tostring(-1);
				L->traceback(message);
				L->error();
			}
		}

		template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
		void operator()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6) {
			state *L = m_object.getstate();
			stack_protector sp(L);
			m_object.push();

			luastub::push(L, p1);
			luastub::push(L, p2);
			luastub::push(L, p3);
			luastub::push(L, p4);
			luastub::push(L, p5);
			luastub::push(L, p6);

			if (L->pcall(6, 0, 0)) {
				const char *message = L->tostring(-1);
				L->traceback(message);
				L->error();
			}
		}

		template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
		void operator()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7) {
			state *L = m_object.getstate();
			stack_protector sp(L);
			m_object.push();

			luastub::push(L, p1);
			luastub::push(L, p2);
			luastub::push(L, p3);
			luastub::push(L, p4);
			luastub::push(L, p5);
			luastub::push(L, p6);
			luastub::push(L, p7);

			if (L->pcall(7, 0, 0)) {
				const char *message = L->tostring(-1);
				L->traceback(message);
				L->error();
			}
		}

	protected:
		object m_object;
	};

	template <>
	class function<multi_result>
	{
	public:
		function(const object &o)
			: m_object(o) {
		}

		function(state* L, const char *name) {
			m_object = L->getglobal(name);
		}

		void push() const {
			m_object.push();
		}
		
		bool operator ==(const function &f) const {
			return m_object == f.m_object;
		}
		
		bool operator ==(const object &o) const {
			return m_object == o;
		}
		
		multi_result operator()() {
			state *L = m_object.getstate();
			stack_protector sp(L);
			m_object.push();

			if (L->pcall(0, LUA_MULTRET, 0)) {
				const char *message = L->tostring(-1);
				L->traceback(message);
				L->error();
			}
			multi_result res;
			for (int n = sp.gettop() + 1; n <= L->gettop(); ++n)
				res.push_back(object(L, n));
			return res;
		}

		template <typename P1>
		multi_result operator()(P1 p1) {
			state *L = m_object.getstate();
			stack_protector sp(L);
			m_object.push();

			luastub::push(L, p1);

			if (L->pcall(1, LUA_MULTRET, 0)) {
				const char *message = L->tostring(-1);
				L->traceback(message);
				L->error();
			}
			multi_result res;
			for (int n = sp.gettop() + 1; n <= L->gettop(); ++n)
				res.push_back(object(L, n));
			return res;
		}

		template <typename P1, typename P2>
		multi_result operator()(P1 p1, P2 p2) {
			state *L = m_object.getstate();
			stack_protector sp(L);
			m_object.push();

			luastub::push(L, p1);
			luastub::push(L, p2);

			if (L->pcall(2, LUA_MULTRET, 0)) {
				const char *message = L->tostring(-1);
				L->traceback(message);
				L->error();
			}
			multi_result res;
			for (int n = sp.gettop() + 1; n <= L->gettop(); ++n)
				res.push_back(object(L, n));
			return res;
		}

		template <typename P1, typename P2, typename P3>
		multi_result operator()(P1 p1, P2 p2, P3 p3) {
			state *L = m_object.getstate();
			stack_protector sp(L);
			m_object.push();

			luastub::push(L, p1);
			luastub::push(L, p2);
			luastub::push(L, p3);

			if (L->pcall(3, LUA_MULTRET, 0)) {
				const char *message = L->tostring(-1);
				L->traceback(message);
				L->error();
			}
			multi_result res;
			for (int n = sp.gettop() + 1; n <= L->gettop(); ++n)
				res.push_back(object(L, n));
			return res;
		}

		template <typename P1, typename P2, typename P3, typename P4>
		multi_result operator()(P1 p1, P2 p2, P3 p3, P4 p4) {
			state *L = m_object.getstate();
			stack_protector sp(L);
			m_object.push();

			luastub::push(L, p1);
			luastub::push(L, p2);
			luastub::push(L, p3);
			luastub::push(L, p4);

			if (L->pcall(4, LUA_MULTRET, 0)) {
				const char *message = L->tostring(-1);
				L->traceback(message);
				L->error();
			}
			multi_result res;
			for (int n = sp.gettop() + 1; n <= L->gettop(); ++n)
				res.push_back(object(L, n));
			return res;
		}

		template <typename P1, typename P2, typename P3, typename P4, typename P5>
		multi_result operator()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5) {
			state *L = m_object.getstate();
			stack_protector sp(L);
			m_object.push();

			luastub::push(L, p1);
			luastub::push(L, p2);
			luastub::push(L, p3);
			luastub::push(L, p4);
			luastub::push(L, p5);

			if (L->pcall(5, LUA_MULTRET, 0)) {
				const char *message = L->tostring(-1);
				L->traceback(message);
				L->error();
			}
			multi_result res;
			for (int n = sp.gettop() + 1; n <= L->gettop(); ++n)
				res.push_back(object(L, n));
			return res;
		}

		template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
		multi_result operator()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6) {
			state *L = m_object.getstate();
			stack_protector sp(L);
			m_object.push();

			luastub::push(L, p1);
			luastub::push(L, p2);
			luastub::push(L, p3);
			luastub::push(L, p4);
			luastub::push(L, p5);
			luastub::push(L, p6);

			if (L->pcall(6, LUA_MULTRET, 0)) {
				const char *message = L->tostring(-1);
				L->traceback(message);
				L->error();
			}
			multi_result res;
			for (int n = sp.gettop() + 1; n <= L->gettop(); ++n)
				res.push_back(object(L, n));
			return res;
		}

		template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
		multi_result operator()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7) {
			state *L = m_object.getstate();
			stack_protector sp(L);
			m_object.push();

			luastub::push(L, p1);
			luastub::push(L, p2);
			luastub::push(L, p3);
			luastub::push(L, p4);
			luastub::push(L, p5);
			luastub::push(L, p6);
			luastub::push(L, p7);

			if (L->pcall(7, LUA_MULTRET, 0)) {
				const char *message = L->tostring(-1);
				L->traceback(message);
				L->error();
			}
			multi_result res;
			for (int n = sp.gettop() + 1; n <= L->gettop(); ++n)
				res.push_back(object(L, n));
			return res;
		}

	protected:
		object m_object;
	};

	template<typename T>
	struct any_type <function<T> >
	{
		static const char *type() {return "object";}
		static void push(lua_State *L, const function<T> &value) {value.push();}
		static bool match(lua_State *L, int index) {return true;}
		static function<T> read(lua_State *L, int index) {return object(state::from(L), index);}
	};
	template<typename T>
	struct any_type <function<T>&>;
	template<typename T>
	struct any_type <const function<T>&>;
}

#endif
