#pragma once

#include <function.hpp>

#include <variant>

enum class object_type
{
	none,

	number,
	function,
	boolean,
};

class object final
{
public:
	object() noexcept = default;
	object(double number) noexcept;
	object(function function) noexcept;
	object(bool boolean) noexcept;
	object(const object& object);
	object(object&& object) noexcept;
	~object() = default;

public:
	object& operator=(double number) noexcept;
	object& operator=(function function) noexcept;
	object& operator=(bool boolean) noexcept;
	object& operator=(const object& object);
	object& operator=(object&& object) noexcept;
	friend bool operator==(const object& lhs, const object& rhs) noexcept;
	friend bool operator!=(const object& lhs, const object& rhs) noexcept;

public:
	void clear() noexcept;
	bool empty() const noexcept;
	void swap(object& other) noexcept;

	object_type type() const noexcept;
	double get_as_number() const noexcept;
	double& get_as_number() noexcept;
	function get_as_function() const noexcept;
	function& get_as_function() noexcept;
	bool get_as_boolean() const noexcept;
	bool& get_as_boolean() noexcept;

private:
	std::variant<std::monostate, double, function, bool> value_;

public:
	static const object true_object;
	static const object false_object;
};