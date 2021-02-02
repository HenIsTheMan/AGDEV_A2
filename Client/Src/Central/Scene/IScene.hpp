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
		virtual ~__VA_ARGS__() = default;
#endif

class IScene{
public:
	_6_DEFAULTED(IScene)

	virtual void Enter() = 0;
	virtual void Exit() = 0;

	virtual void EarlyInit() = 0;
	virtual void Init() = 0;

	virtual void FixedUpdate() = 0;
	virtual void Update() = 0;
	virtual void LateUpdate() = 0;

	virtual void PreRender() = 0;
	virtual void Render() = 0;
	virtual void PostRender() = 0;
};