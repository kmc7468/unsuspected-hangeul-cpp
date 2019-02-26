#pragma once

#include <function.hpp>

#include <variant>

enum class object_type
{
	none,

	number,
	function,
};

class object final
{
public:
	object() noexcept = default;
	object(double number) noexcept;
	object(function function) noexcept;
	object(const object& object);
	object(object&& object) noexcept;
	~object() = default;

public:
	object& operator=(double number) noexcept;
	object& operator=(function function) noexcept;
	object& operator=(const object& object);
	object& operator=(object&& object) noexcept;

public:
	void clear() noexcept;
	bool empty() const noexcept;
	void swap(object& other) noexcept;

	object_type type() const noexcept;
	double get_as_number() const noexcept;
	double& get_as_number() noexcept;
	function get_as_function() const noexcept;
	function& get_as_function() noexcept;

private:
	std::variant<std::monostate, double, function> value_;
};