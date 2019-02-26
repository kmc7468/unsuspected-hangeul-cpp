#include <function.hpp>

#include <cassert>
#include <utility>

function::function() noexcept
{}
function::function(command command) noexcept
	: function_(std::move(command))
{}
function::function(native_function_t function) noexcept
	: function_(std::move(function))
{}
function::function(const function& function)
	: function_(function.function_)
{}
function::function(function&& function) noexcept
	: function_(std::move(function.function_))
{}

function& function::operator=(const function& function)
{
	return function_ = function.function_, *this;
}
function& function::operator=(function&& function) noexcept
{
	return function_ = std::move(function.function_), *this;
}
bool operator==(const function& lhs, const function& rhs) noexcept
{
	if (lhs.function_.index() != rhs.function_.index()) return false;
	
	switch (lhs.function_.index())
	{
	case 0: return true;
	case 1: return lhs.get_as_command() == rhs.get_as_command();
	case 2: return lhs.get_as_native_function_t().target<native_function_ptr_t>() == rhs.get_as_native_function_t().target<native_function_ptr_t>();
	}
}
bool operator!=(const function& lhs, const function& rhs) noexcept
{
	if (lhs.function_.index() != rhs.function_.index()) return true;

	switch (lhs.function_.index())
	{
	case 0: return false;
	case 1: return lhs.get_as_command() != rhs.get_as_command();
	case 2: return lhs.get_as_native_function_t().target<native_function_ptr_t>() != rhs.get_as_native_function_t().target<native_function_ptr_t>();
	}
}

void function::clear() noexcept
{
	function_ = {};
}
bool function::empty() const noexcept
{
	return function_.index() == 0;
}
void function::swap(function& other) noexcept
{
	function_.swap(other.function_);
}

function_type function::type() const noexcept
{
	return static_cast<function_type>(function_.index());
}
const command& function::get_as_command() const noexcept
{
	assert(function_.index() == 1);
	return std::get<command>(function_);
}
command& function::get_as_command() noexcept
{
	assert(function_.index() == 1);
	return std::get<command>(function_);
}
const native_function_t& function::get_as_native_function_t() const noexcept
{
	assert(function_.index() == 2);
	return std::get<native_function_t>(function_);
}
native_function_t& function::get_as_native_function_t() noexcept
{
	assert(function_.index() == 2);
	return std::get<native_function_t>(function_);
}