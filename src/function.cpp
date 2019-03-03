#include <function.hpp>

#include <interpreter.hpp>
#include <object.hpp>

#include <cassert>
#include <utility>

function::function() noexcept
{}
function::function(node_ptr command) noexcept
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
const node_ptr& function::get_as_node_ptr() const noexcept
{
	assert(function_.index() == 1);
	return std::get<node_ptr>(function_);
}
node_ptr& function::get_as_node_ptr() noexcept
{
	assert(function_.index() == 1);
	return std::get<node_ptr>(function_);
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

object function::eval(uh_status& status)
{
	switch (type())
	{
	case function_type::script: return std::static_pointer_cast<function_defining_node>(get_as_node_ptr())->body->eval(status, get_as_node_ptr());
	case function_type::native: return get_as_native_function_t()(status.call_stack_.back().arguments);
	default: return {}; // Dummy
	}
}