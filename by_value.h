#pragma once
#include <algorithm>

namespace poly
{

template<class Base,
	std::size_t Size = sizeof (Base),
	std::size_t Alignment = alignof (Base)>
class by_value
{
public:
	by_value(by_value<Base, Size, Alignment>&& other)
	{
		std::swap(_data, other._data);
	}

	template<class Derived,
		typename std::enable_if<
			std::is_base_of<Base, Derived>::value &&
			sizeof (Derived) <= Size &&
			alignof (Derived) <= Alignment>::type* = nullptr>
	by_value(const Derived& derived)
	{
		new (**this) Derived(derived);
	}

	template<class Derived,
		typename std::enable_if<
			std::is_base_of<Base, Derived>::value &&
			sizeof (Derived) <= Size &&
			alignof (Derived) <= Alignment>::type* = nullptr>
	by_value(const Derived&& derived)
	{
		new (**this) Derived(derived);
	}

	~by_value()
	{
		(*this)->~Base();
	}

	by_value<Base, Size, Alignment>& operator=(by_value<Base, Size, Alignment>&& other)
	{
		std::swap(_data, other._data);
		return *this;
	}

	Base* operator*()
	{
		return reinterpret_cast<Base*>(_data);
	}

	const Base* operator*() const
	{
		return reinterpret_cast<const Base*>(_data);
	}

	Base* operator->()
	{
		return **this;
	}

	const Base* operator->() const
	{
		return **this;
	}

	by_value(const by_value<Base, Size, Alignment>&) = delete;
	by_value<Base, Size, Alignment>& operator=(const by_value<Base, Size, Alignment>&) = delete;
	by_value() = delete;

private:
	alignas(Alignment) unsigned char _data[Size];
};

template<class...Ts>
struct max;

template<>
struct max<>
{
	static constexpr std::size_t size = 0;
	static constexpr std::size_t alignment = 0;
};

template<class T, class...Ts>
struct max<T, Ts...>
{
	static constexpr std::size_t size = (sizeof (T) > max<Ts...>::size)
		? sizeof (T)
		: max<Ts...>::size;
	static constexpr std::size_t alignment = (alignof (T) > max<Ts...>::alignment)
		? alignof (T)
		: max<Ts...>::alignment;
};

} // namespace poly
