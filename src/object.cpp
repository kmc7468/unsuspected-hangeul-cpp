#include <object.hpp>

#include <cassert>
#include <utility>

object::object(double number) noexcept
	: value_(number)
{}
object::object(function function) noexcept
	: value_(std::move(function))
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
	case 0: return true;
	case 1: return lhs.get_as_number() == rhs.get_as_number();
	case 2: return lhs.get_as_function() == rhs.get_as_function();
	default: return true; // Dummy
	}
}
bool operator!=(const object& lhs, const object& rhs) noexcept
{
	if (lhs.value_.index() != rhs.value_.index()) return true;

	switch (lhs.value_.index())
	{
	case 0: return false;
	case 1: return lhs.get_as_number() != rhs.get_as_number();
	case 2: return lhs.get_as_function() != rhs.get_as_function();
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
	assert(type() == object_type::function);
	return std::get<function>(value_);
}