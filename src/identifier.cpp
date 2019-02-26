#include <identifier.hpp>

#include <utility>

identifier::identifier(std::u16string name, ::module* module) noexcept
	: name_(std::move(name)), module_(module)
{}
identifier::identifier(const identifier& identifier)
	: name_(identifier.name_), module_(identifier.module_)
{}
identifier::identifier(identifier&& identifier) noexcept
	: name_(std::move(identifier.name_)), module_(identifier.module_)
{
	identifier.module_ = nullptr;
}

identifier& identifier::operator=(const identifier& identifier)
{
	return name_ = identifier.name_, module_ = identifier.module_, *this;
}
identifier& identifier::operator=(identifier&& identifier) noexcept
{
	return name_ = std::move(identifier.name_), module_ = identifier.module_, identifier.module_ = nullptr, *this;
}
bool operator==(const identifier& lhs, const identifier& rhs) noexcept
{
	return lhs.name() == rhs.name() && lhs.module() == rhs.module();
}
bool operator!=(const identifier& lhs, const identifier& rhs) noexcept
{
	return lhs.name() != rhs.name() || lhs.module() != rhs.module();
}

std::u16string identifier::name() const
{
	return name_;
}
void identifier::name(std::u16string new_name) noexcept
{
	name_ = new_name;
}
const ::module* identifier::module() const noexcept
{
	return module_;
}
::module* identifier::module() noexcept
{
	return module_;
}
void identifier::module(::module* new_module) noexcept
{
	module_ = new_module;
}