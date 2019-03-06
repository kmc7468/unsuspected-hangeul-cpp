#pragma once

#include <function.hpp>

#include <cstddef>
#include <memory>
#include <tuple>
#include <variant>

enum class object_type
{
	none,

	number,
	function,
	boolean,
	lazy_eval,
};

class node;
class uh_status;

class object final
{
public:
	object() noexcept = default;
	object(double number) noexcept;
	object(function function) noexcept;
	object(bool boolean) noexcept;
	object(std::shared_ptr<node> node, std::size_t call_stack_index) noexcept;
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

	object cast_as_number() const;
	object cast_as_boolean() const;

	object& eval(uh_status& status);

private:
	std::variant<std::monostate, double, function, bool, std::tuple<std::shared_ptr<node>, std::size_t>> value_;

public:
	static const object true_object;
	static const object false_object;
};