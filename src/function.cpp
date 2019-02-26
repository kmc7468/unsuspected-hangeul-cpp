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
function::function(command command, function* parent) noexcept
	: function_(std::move(command)), parent_(parent)
{}
function::function(native_function_t function, ::function* parent) noexcept
	: function_(std::move(function)), parent_(parent)
{}
function::function(const function& function)
	: function_(function.function_), parent_(function.parent_)
{}
function::function(function&& function) noexcept
	: function_(std::move(function.function_)), parent_(function.parent_)
{
	function.parent_ = nullptr;
}

function& function::operator=(const function& function)
{
	return function_ = function.function_, parent_ = function.parent_, *this;
}
function& function::operator=(function&& function) noexcept
{
	return function_ = std::move(function.function_), parent_ = function.parent_, function.parent_ = nullptr, *this;
}

void function::clear() noexcept
{
	function_ = {};
	parent_ = nullptr;
}
bool function::empty() const noexcept
{
	return function_.index() == 0 && parent_ == nullptr;
}
void function::swap(function& other) noexcept
{
	function_.swap(other.function_);
	std::swap(parent_, other.parent_);
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

const function& function::parent() const noexcept
{
	return *parent_;
}
function& function::parent() noexcept
{
	return *parent_;
}