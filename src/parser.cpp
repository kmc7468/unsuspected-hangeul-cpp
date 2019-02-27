#include <parser.hpp>

#include <algorithm>
#include <cstddef>
#include <deque>
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

std::vector<command> parser::make_words(const command& command)
{
	std::vector<::command> result;
	::command* word = &result.emplace_back();

	auto begin = command.begin();

	for (auto iter = command.begin(); iter < command.end(); ++iter)
	{
		if (*iter == command_type::none)
		{
			*word = ::command(begin, iter);
			begin = iter + 1;
			word = &result.emplace_back();
		}
	}

	if (begin != command.end())* word = ::command(begin, command.end());
	return result;
}
node_ptr parser::parse(const std::vector<command>& words)
{
	static const auto pop_back_nodes = [](std::deque<node_ptr>& nodes)
	{
		const node_ptr temp = nodes.back();
		return nodes.pop_back(), temp;
	};
	static const auto parse_integer_literal = [](command::const_iterator begin, command::const_iterator end) -> long long
	{
		long long result = 0, e = 0;
		int sign = ((end - begin) & 1) ? 1 : -1;

		for (; begin < end; ++begin)
		{
			result += (static_cast<long long>(*begin) - static_cast<long long>(command_type::r)) * static_cast<long long>(std::pow(8., e++));
		}

		return sign * result;
	};

	std::deque<node_ptr> nodes;

	for (auto word_iter = words.begin(); word_iter < words.end(); ++word_iter)
	{
		if (word_iter->is_integer_literal())
		{
			node_ptr lit = std::make_shared<integer_literal_node>(parse_integer_literal(word_iter->begin(), word_iter->end()));
			const auto next_word_iter = word_iter + 1;
			if (next_word_iter == words.end()) goto push_integer_literal;

			if (next_word_iter->is_argument())
			{
				nodes.push_back(std::make_shared<argument_node>(
					std::move(lit), std::make_shared<integer_literal_node>(parse_integer_literal(next_word_iter->begin() + 1, next_word_iter->end()))));
				++word_iter;
			}
			else if (next_word_iter->is_recursive_function())
			{
				nodes.push_back(std::make_shared<recursive_function_node>(std::move(lit)));
				++word_iter;
			}
			else
			{
			push_integer_literal:
				nodes.push_back(std::move(lit));
			}
		}
		else if (word_iter->is_function_defining())
		{
			nodes.push_back(std::make_shared<function_defining_node>(pop_back_nodes(nodes)));
		}
		else if (word_iter->is_function_calling())
		{
			const std::shared_ptr<integer_literal_node> argument_size =
				std::make_shared<integer_literal_node>(parse_integer_literal(word_iter->begin() + 1, word_iter->end()));
			const node_ptr function = pop_back_nodes(nodes);
			std::vector<node_ptr> arguments;

			for (long long i = 0; i < argument_size->value; ++i)
			{
				arguments.push_back(pop_back_nodes(nodes));
			}
			std::reverse(arguments.begin(), arguments.end());

			nodes.push_back(std::make_shared<function_calling_node>(std::move(arguments), std::move(function)));
		}
	}

	if (nodes.size() == 1) return nodes.front();
	else return nullptr;
}