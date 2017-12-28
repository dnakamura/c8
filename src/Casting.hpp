#include <type_traits>
#include <assert.h>

template<class X, class Y, typename Enabler = void>
struct isa_impl {
	static inline bool impl(const Y * ptr) {
		assert(ptr && "isa<> called on null pointer");
		return X::classof(ptr);
	}
};

template<class X, class Y>
struct isa_impl<X, Y, typename std::enable_if<std::is_base_of<X, Y>::value>::type> {
	static inline bool impl(const Y* ptr) { 
		assert(ptr && "isa<> called on null pointer");
		return true; 
	}

};
/*
template<class X, class Y, typename Enabler = void>
struct isa_impl {
	static inline bool impl(const Y& obj) {
		assert(ptr && "isa<> called on null pointer");
		return X::classof(obj);
	}
};

template<class X, class Y>
struct isa_impl<X, Y, typename std::enable_if<std::is_base_of<X, Y>::value>::type> {
	static inline bool impl(const Y& ptr) { 
		assert(ptr && "isa<> called on null pointer");
		return true; 
	}

};*/

template<class X, class Y>
inline bool isa(const Y * ptr) {
	return isa_impl<X, Y>::impl(ptr);
}
/*
template<class X, class Y>
inline bool isa(const Y & obj) {
	return isa_impl<X, Y>::impl(obj);
}*/
