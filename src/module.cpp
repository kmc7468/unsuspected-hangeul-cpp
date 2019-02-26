#include <module.hpp>

#include <utility>

module::module(std::u16string name) noexcept
	: name_(std::move(name))
{}
module::~module()
{
	for (module* mod : submodules_)
	{
		delete mod;
	}
}

void module::add_submodule(module* submodule)
{
	submodule->parent_ = this;
	submodules_.push_back(submodule);
}
void module::add_function(function function)
{
	functions_.push_back(std::move(function));
}

std::u16string module::name() const noexcept
{
	return name_;
}
std::vector<module*>& module::submodules() noexcept
{
	return submodules_;
}
const module* module::parent() const noexcept
{
	return parent_;
}
module* module::parent() noexcept
{
	return parent_;
}

std::vector<function>& module::functions() noexcept
{
	return functions_;
}