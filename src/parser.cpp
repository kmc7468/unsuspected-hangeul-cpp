#include <parser.hpp>

#include <function.hpp>
#include <interpreter.hpp>
#include <module.hpp>
#include <object.hpp>

#include <algorithm>
#include <cstddef>
#include <stack>
#include <utility>

#ifdef _MSC_VER
#	pragma warning(disable: 4458)
#endif

node::node(node_type type) noexcept
	: type_(type)
{}

node_type node::type() const noexcept
{
	return type_;
}

integer_literal_node::integer_literal_node(long long value, std::u16string original) noexcept
	: node(node_type::integer_literal), value(value), original(original)
{}

object integer_literal_node::eval(uh_status&, const std::shared_ptr<node>&)
{
	return object(static_cast<double>(value));
}

function_defining_node::function_defining_node(node_ptr body) noexcept
	: node(node_type::function_defining), body(std::move(body))
{}

object function_defining_node::eval(uh_status&, const std::shared_ptr<node>& current_node)
{
	return function(current_node);
}

function_calling_node::function_calling_node(node_ptr function) noexcept
	: node(node_type::function_calling), function(std::move(function))
{}
function_calling_node::function_calling_node(std::vector<node_ptr> arguments, node_ptr function) noexcept
	: node(node_type::function_calling), arguments(std::move(arguments)), function(std::move(function))
{}

object function_calling_node::eval(uh_status& status, const std::shared_ptr<node>&)
{
	std::vector<object> arguments;
	for (const auto& arg : this->arguments)
	{
		if (arg->type() == node_type::function_calling) arguments.push_back(object(arg, status.call_stack_.size() - 1)/* lazy eval */);
		else arguments.push_back(arg->eval(status, arg));
	}

	status.call_stack_.push_back(uh_status::function_status{ ::function(function->eval(status, function).get_as_function()), arguments });
	object result = status.call_stack_.back().function.eval(status);
	while (result.type() == object_type::lazy_eval) result = result.eval(status);
	return status.call_stack_.erase(status.call_stack_.end() - 1), result;
}

recursive_function_node::recursive_function_node(node_ptr number) noexcept
	: node(node_type::recursive_function), number(std::move(number))
{}

object recursive_function_node::eval(uh_status& status, const std::shared_ptr<node>&)
{
	const object number = this->number->eval(status, this->number);
	const object number_c = number.type() == object_type::boolean ? static_cast<double>(number.get_as_boolean()) : number;

	if (number_c.type() != object_type::number) return false;
	if (status.call_stack_.size() <= number_c.get_as_number()) return false;

	return status.call_stack_[static_cast<std::size_t>(number_c.get_as_number())].function;
}

argument_node::argument_node(node_ptr index, node_ptr function_number) noexcept
	: node(node_type::argument), index(std::move(index)), function_number(std::move(function_number))
{}

object argument_node::eval(uh_status& status, const std::shared_ptr<node>&)
{
	const object index = this->index->eval(status, this->index);
	const object index_c = index.type() == object_type::boolean ? static_cast<double>(index.get_as_number()) : index;
	const object function_number = this->function_number->eval(status, this->function_number);
	const object function_number_c = function_number.type() == object_type::boolean ? static_cast<double>(function_number.get_as_number()) : function_number;

	if (index_c.type() != object_type::number || function_number_c.type() != object_type::number) return false;
	if (status.call_stack_.size() <= function_number_c.get_as_number()) return false;
	uh_status::function_status& fstatus = status.call_stack_[status.call_stack_.size() - static_cast<std::size_t>(function_number_c.get_as_number()) - 1];
	
	if (fstatus.arguments.size() <= index_c.get_as_number()) return false;

	object& argument = fstatus.arguments[fstatus.arguments.size() - static_cast<std::size_t>(index_c.get_as_number()) - 1];
	if (argument.type() == object_type::lazy_eval) argument.eval(status);
	return argument;
}

identifier_node::identifier_node(::module* module, std::u16string name) noexcept
	: node(node_type::identifier), module(module), name(std::move(name))
{}

object identifier_node::eval(uh_status&, const std::shared_ptr<node>&)
{
	return module->functions().at(name);
}

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
node_ptr parser::parse(const std::vector<command>& words, module* root_module)
{
	static const auto pop = [](std::stack<node_ptr>& nodes)
	{
		const node_ptr temp = nodes.top();
		return nodes.pop(), temp;
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
	static const auto command_to_string = [](command::const_iterator begin, command::const_iterator end)
	{
		std::u16string result;
		for (; begin < end; ++begin)
		{
			switch (*begin)
			{
			case command_type::r: result += u'丑'; break;
			case command_type::s: result += u'中'; break;
			case command_type::e: result += u'之'; break;
			case command_type::f: result += u'予'; break;
			case command_type::a: result += u'仃'; break;
			case command_type::q: result += u'仆'; break;
			case command_type::t: result += u'今'; break;
			case command_type::w: result += u'元'; break;
			case command_type::d: result += u'仄'; break;
			case command_type::g: result += u'冗'; break;
			default: break; // Dummy
			}
		}

		return result.empty() ? u"丑" : result;
	};

	std::stack<node_ptr> nodes;

	for (auto word_iter = words.begin(); word_iter < words.end(); ++word_iter)
	{
		if (word_iter->is_integer_literal())
		{
			node_ptr lit = std::make_shared<integer_literal_node>(
				parse_integer_literal(word_iter->begin(), word_iter->end()),
				command_to_string(word_iter->begin(), word_iter->end()));
			const auto next_word_iter = word_iter + 1;
			if (next_word_iter == words.end()) goto push_integer_literal;

			if (next_word_iter->is_argument())
			{
				nodes.push(std::make_shared<argument_node>(
					std::move(lit), std::make_shared<integer_literal_node>(
						parse_integer_literal(next_word_iter->begin() + 1, next_word_iter->end()),
						command_to_string(next_word_iter->begin() + 1, next_word_iter->end()))));
				++word_iter;
			}
			else if (next_word_iter->is_recursive_function())
			{
				nodes.push(std::make_shared<recursive_function_node>(std::move(lit)));
				++word_iter;
			}
			else
			{
			push_integer_literal:
				nodes.push(std::move(lit));
			}
		}
		else if (word_iter->is_function_defining())
		{
			nodes.push(std::make_shared<function_defining_node>(pop(nodes)));
		}
		else if (word_iter->is_function_calling())
		{
			const std::shared_ptr<integer_literal_node> argument_size =
				std::make_shared<integer_literal_node>(
					parse_integer_literal(word_iter->begin() + 1, word_iter->end()),
					command_to_string(word_iter->begin() + 1, word_iter->end()));
			const node_ptr function = pop(nodes);
			std::vector<node_ptr> arguments;

			for (long long i = 0; i < argument_size->value; ++i)
			{
				arguments.push_back(pop(nodes));
			}
			std::reverse(arguments.begin(), arguments.end());

			node_ptr function_c = function;
			if (function_c->type() == node_type::integer_literal)
			{
				function_c = std::make_shared<identifier_node>(root_module, std::move(std::static_pointer_cast<integer_literal_node>(function)->original));
			}

			nodes.push(std::make_shared<function_calling_node>(std::move(arguments), std::move(function_c)));
		}
	}

	if (nodes.size() == 1) return nodes.top();
	else return nullptr;
}