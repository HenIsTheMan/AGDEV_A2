#pragma once

#ifndef _6_DEFAULTED
#define _6_DEFAULTED(...) \
		__VA_ARGS__() = default; \
		\
		__VA_ARGS__(const __VA_ARGS__&) = default; \
		__VA_ARGS__(__VA_ARGS__&&) noexcept = default; \
		\
		__VA_ARGS__& operator=(const __VA_ARGS__&) = default; \
		__VA_ARGS__& operator=(__VA_ARGS__&&) noexcept = default; \
		\
		~__VA_ARGS__() = default;
#endif

class Scene{
public:
	_6_DEFAULTED(Scene)
};