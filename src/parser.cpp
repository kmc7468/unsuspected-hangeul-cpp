#include <parser.hpp>

#include <utility>

node::node(node_type type) noexcept
	: type_(type)
{}

node_type node::type() const noexcept
{
	return type_;
}

integer_literal_node::integer_literal_node(long long value) noexcept
	: node(node_type::integer_literal), value(value)
{}

function_defining_node::function_defining_node(node_ptr body) noexcept
	: node(node_type::function_defining), body(std::move(body))
{}

function_calling_node::function_calling_node(node_ptr function) noexcept
	: node(node_type::function_calling), function(std::move(function))
{}
function_calling_node::function_calling_node(std::vector<node_ptr> arguments, node_ptr function) noexcept
	: node(node_type::function_calling), arguments(std::move(arguments)), function(std::move(function))
{}

recursive_function_node::recursive_function_node(node_ptr number) noexcept
	: node(node_type::recursive_function), number(std::move(number))
{}

argument_node::argument_node(node_ptr index, node_ptr function_number) noexcept
	: node(node_type::arugment), index(std::move(index)), function_number(std::move(function_number))
{}

identifier_node::identifier_node(::module* module, std::u16string name) noexcept
	: node(node_type::identifier), module(module), name(std::move(name))
{}