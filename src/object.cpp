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