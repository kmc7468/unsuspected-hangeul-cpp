#include <object.hpp>

#include <cassert>
#include <utility>

object::object(double number) noexcept
	: value_(number)
{}
object::object(function function) noexcept
	: value_(std::move(function))
{}
object::object(bool boolean) noexcept
	: value_(boolean)
{}
object::object(const object& object)
	: value_(object.value_)
{}
object::object(object&& object) noexcept
	: value_(std::move(object.value_))
{}

object& object::operator=(double number) noexcept
{
	return value_ = number, *this;
}
object& object::operator=(function function) noexcept
{
	return value_ = std::move(function), *this;
}
object& object::operator=(bool boolean) noexcept
{
	return value_ = boolean, *this;
}
object& object::operator=(const object& object)
{
	return value_ = object.value_, *this;
}
object& object::operator=(object&& object) noexcept
{
	return value_ = std::move(object.value_), *this;
}
bool operator==(const object& lhs, const object& rhs) noexcept
{
	if (lhs.value_.index() != rhs.value_.index()) return false;
	
	switch (lhs.value_.index())
	{
	case 1: return lhs.get_as_number() == rhs.get_as_number();
	case 3: return lhs.get_as_boolean() == rhs.get_as_boolean();
	default: return true; // Dummy
	}
}
bool operator!=(const object& lhs, const object& rhs) noexcept
{
	if (lhs.value_.index() != rhs.value_.index()) return true;

	switch (lhs.value_.index())
	{
	case 1: return lhs.get_as_number() != rhs.get_as_number();
	case 3: return lhs.get_as_boolean() != rhs.get_as_boolean();
	default: return true; // Dummy
	}
}

void object::clear() noexcept
{
	value_ = {};
}
bool object::empty() const noexcept
{
	return value_.index() == 0;
}
void object::swap(object& other) noexcept
{
	value_.swap(other.value_);
}

object_type object::type() const noexcept
{
	return static_cast<object_type>(value_.index());
}
double object::get_as_number() const noexcept
{
	assert(type() == object_type::number);
	return std::get<double>(value_);
}
double& object::get_as_number() noexcept
{
	assert(type() == object_type::number);
	return std::get<double>(value_);
}
function object::get_as_function() const noexcept
{
	assert(type() == object_type::function);
	return std::get<function>(value_);
}
function& object::get_as_function() noexcept
{
	assert(type() == object_type::function || type() == object_type::boolean);

	static auto true_function = function([](const native_function_param_t& args) -> native_function_res_t
	{
		if (args.size() == 0) return true;
		else return args[0];
	});
	static auto false_function = function([](const native_function_param_t& args) -> native_function_res_t
	{
		if (args.size() == 0) return false;
		else if (args.size() == 1) return args[0];
		else return args[1];
	});

	if (type() == object_type::function) return std::get<function>(value_);
	else return std::get<bool>(value_) ? true_function : false_function;
}
bool object::get_as_boolean() const noexcept
{
	assert(type() == object_type::boolean);
	return std::get<bool>(value_);
}
bool& object::get_as_boolean() noexcept
{
	assert(type() == object_type::boolean);
	return std::get<bool>(value_);
}

object object::cast_as_number() const
{
	switch (type())
	{
	case object_type::boolean: return static_cast<double>(get_as_boolean());
	default: return *this;
	}
}
object object::cast_as_boolean() const
{
	switch (type())
	{
	case object_type::number: return static_cast<bool>(get_as_number());
	default: return *this;
	}
}

const object object::true_object = true;
const object object::false_object = false;