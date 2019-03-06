#include <function.hpp>

#include <interpreter.hpp>
#include <object.hpp>

#include <cassert>
#include <utility>

native_function_param_t::const_iterator::const_iterator(const native_function_param_t* param) noexcept
	: param_(param)
{}
native_function_param_t::const_iterator::const_iterator(const native_function_param_t* param, std::int32_t index) noexcept
	: param_(param), index_(index)
{}

native_function_param_t::const_iterator& native_function_param_t::const_iterator::operator=(const const_iterator& iterator) noexcept
{
	return param_ = iterator.param_, index_ = iterator.index_, *this;
}
bool operator==(const native_function_param_t::const_iterator& lhs, const native_function_param_t::const_iterator& rhs) noexcept
{
	return lhs.param_ == rhs.param_ && lhs.index_ == rhs.index_;
}
bool operator!=(const native_function_param_t::const_iterator& lhs, const native_function_param_t::const_iterator& rhs) noexcept
{
	return lhs.param_ != rhs.param_ || lhs.index_ != rhs.index_;
}
bool operator>(const native_function_param_t::const_iterator& lhs, const native_function_param_t::const_iterator& rhs) noexcept
{
	assert(lhs.param_ == rhs.param_);
	return lhs.index_ > rhs.index_;
}
bool operator>=(const native_function_param_t::const_iterator& lhs, const native_function_param_t::const_iterator& rhs) noexcept
{
	assert(lhs.param_ == rhs.param_);
	return lhs.index_ >= rhs.index_;
}
bool operator<(const native_function_param_t::const_iterator& lhs, const native_function_param_t::const_iterator& rhs) noexcept
{
	assert(lhs.param_ == rhs.param_);
	return lhs.index_ < rhs.index_;
}
bool operator<=(const native_function_param_t::const_iterator& lhs, const native_function_param_t::const_iterator& rhs) noexcept
{
	assert(lhs.param_ == rhs.param_);
	return lhs.index_ <= rhs.index_;
}
native_function_param_t::const_iterator operator+(const native_function_param_t::const_iterator& iterator, std::int32_t v) noexcept
{
	return native_function_param_t::const_iterator(iterator.param_, iterator.index_ + v);
}
std::int32_t operator-(const native_function_param_t::const_iterator& iterator, std::int32_t v) noexcept
{
	return iterator.index_ - v;
}
native_function_param_t::const_iterator& native_function_param_t::const_iterator::operator++() noexcept
{
	return ++index_, *this;
}
native_function_param_t::const_iterator native_function_param_t::const_iterator::operator++(int) noexcept
{
	const const_iterator temp = *this;
	return ++index_, temp;
}
native_function_param_t::const_iterator& native_function_param_t::const_iterator::operator--() noexcept
{
	return --index_, * this;
}
native_function_param_t::const_iterator native_function_param_t::const_iterator::operator--(int) noexcept
{
	const const_iterator temp = *this;
	return --index_, temp;
}
const object& native_function_param_t::const_iterator::operator*() const noexcept
{
	return (*param_)[static_cast<std::size_t>(index_)];
}
const object* native_function_param_t::const_iterator::operator->() const noexcept
{
	return &(*param_)[static_cast<std::size_t>(index_)];
}

native_function_param_t::native_function_param_t(uh_status& status) noexcept
	: status_(status)
{}

const object& native_function_param_t::operator[](std::size_t index) const
{
	object& result = status_.call_stack_.back().arguments[index];
	while (result.type() == object_type::lazy_eval) result.eval(const_cast<uh_status&>(status_));
	return result;
}

std::size_t native_function_param_t::size() const noexcept
{
	return status_.call_stack_.size();
}

native_function_param_t::const_iterator native_function_param_t::begin() const noexcept
{
	return const_iterator(this);
}
native_function_param_t::const_iterator native_function_param_t::cbegin() const noexcept
{
	return const_iterator(this);
}
native_function_param_t::const_iterator native_function_param_t::end() const noexcept
{
	return const_iterator(this, status_.call_stack_.back().arguments.size());
}
native_function_param_t::const_iterator native_function_param_t::cend() const noexcept
{
	return const_iterator(this, status_.call_stack_.back().arguments.size());
}

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
	case function_type::native: return get_as_native_function_t()(status);
	default: return {}; // Dummy
	}
}