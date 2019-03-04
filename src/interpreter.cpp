#include <interpreter.hpp>

#include <function.hpp>
#include <object.hpp>
#include <parser.hpp>

#include <cmath>
#include <iostream>
#include <utility>

uh_status::uh_status(uh_status&& status) noexcept
	: root_module_(std::move(status.root_module_))
{}

uh_status::uh_status()
{
	reset();
}

uh_status& uh_status::operator=(uh_status&& status) noexcept
{
	return root_module_ = std::move(status.root_module_), *this;
}

void uh_status::reset()
{
	root_module_ = std::unique_ptr<module>(new module(u""));

	root_module_->add_function(u"丑", function([](const native_function_param_t& args) -> native_function_res_t
	{
		if (args.size() == 0) return 0.;
		if (args.size() == 1) return args[0];

		double result = 1.;

		for (const auto& arg : args)
		{
			if (arg.type() == object_type::function) return 0.;
			else result += arg.cast_as_number().get_as_number();
		}

		return result;
	}));
	root_module_->add_function(u"之", function([](const native_function_param_t& args) -> native_function_res_t
	{
		if (args.size() == 0) return 0.;
		if (args.size() == 1) return args[0];

		double result = 0.;

		for (const auto& arg : args)
		{
			if (arg.type() == object_type::function) return 0.;
			else result += arg.cast_as_number().get_as_number();
		}

		return result;
	}));
	root_module_->add_function(u"今", function([](const native_function_param_t& args) -> native_function_res_t
	{
		if (args.size() == 0) return 0.;
		if (args.size() == 1) return args[0];

		const object& lhs = args[0];
		const object& rhs = args[1];

		if (lhs.type() == object_type::function || rhs.type() == object_type::function) return 0.;
		else return std::pow(lhs.cast_as_number().get_as_number(), rhs.cast_as_number().get_as_number());
	}));
	root_module_->add_function(u"中", function([](const native_function_param_t& args) -> native_function_res_t
	{
		if (args.size() < 2) return true;

		const object& lhs = args[0];
		const object& rhs = args[1];
		
		if (lhs.type() == object_type::function || rhs.type() == object_type::function) return false;
		else return lhs.cast_as_number().get_as_number() == rhs.cast_as_number().get_as_number();
	}));
	root_module_->add_function(u"元", function([](const native_function_param_t& args) -> native_function_res_t
	{
		if (args.size() < 2) return true;

		const object& lhs = args[0];
		const object& rhs = args[1];
		
		if (lhs.type() == object_type::function || rhs.type() == object_type::function) return false;
		else return lhs.cast_as_number().get_as_number() < rhs.cast_as_number().get_as_number();
	}));
	root_module_->add_function(u"仃", function([](const native_function_param_t& args) -> native_function_res_t
	{
		if (args.size() == 0) return true;

		const object& arg = args[0];

		if (arg.type() == object_type::function) return false;
		
		return !arg.cast_as_boolean().get_as_boolean();
	}));
	root_module_->add_function(u"元元", function([](const native_function_param_t&) -> native_function_res_t
	{
		return true;
	}));
	root_module_->add_function(u"丑元", function([](const native_function_param_t&) -> native_function_res_t
	{
		return false;
	}));
	root_module_->add_function(u"予", function([](const native_function_param_t&) -> native_function_res_t
	{
		std::string line;
		std::getline(std::cin, line);
		return std::stod(line);
	}));
}
void uh_status::swap(uh_status& status) noexcept
{
	root_module_.swap(status.root_module_);
}

::version uh_status::version() const noexcept
{
	return version_;
}

interpreter::interpreter(::version version)
{
	status_.version_ = version;
}

std::optional<object> interpreter::eval(const std::u16string& code)
{
	const command command = ::command::parse(code);
	const std::vector<::command> words = parser::make_words(command);
	node_ptr ast = parser::parse(words, status_.root_module_.get());

	if (!ast) return std::nullopt;
	else if (ast->type() != node_type::function_calling) return std::nullopt;
	
	return ast->eval(status_, ast);
}

const uh_status& interpreter::status() const noexcept
{
	return status_;
}
uh_status& interpreter::status() noexcept
{
	return status_;
}