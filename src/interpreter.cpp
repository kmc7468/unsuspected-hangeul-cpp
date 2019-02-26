#include <interpreter.hpp>

#include <object.hpp>

#include <cmath>
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

	root_module_->add_function(u"¤¡", function([](const native_function_param_t& args) -> native_function_res_t
	{
		const object& lhs = args[0];
		const object& rhs = args[1];

		if (lhs.type() != rhs.type() || lhs.type() != object_type::number) return 0; // TODO: Exception Handling
		
		return lhs.get_as_number() * rhs.get_as_number();
	}));
	root_module_->add_function(u"¤§", function([](const native_function_param_t & args) -> native_function_res_t
	{
		const object& lhs = args[0];
		const object& rhs = args[1];

		if (lhs.type() != rhs.type() || lhs.type() != object_type::number) return 0; // TODO: Exception Handling
		
		return lhs.get_as_number() + rhs.get_as_number();
	}));
	root_module_->add_function(u"¤µ", function([](const native_function_param_t & args) -> native_function_res_t
	{
		const object& lhs = args[0];
		const object& rhs = args[1];

		if (lhs.type() != rhs.type() || lhs.type() != object_type::number) return 0; // TODO: Exception Handling
		
		return std::pow(lhs.get_as_number(), rhs.get_as_number());
	}));
	// TODO: Equal, Less
}
void uh_status::swap(uh_status& status) noexcept
{
	root_module_.swap(status.root_module_);
}

const uh_status& interpreter::status() const noexcept
{
	return status_;
}
uh_status& interpreter::status() noexcept
{
	return status_;
}