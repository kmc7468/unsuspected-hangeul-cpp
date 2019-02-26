#pragma once

#include <module.hpp>

#include <string>

class identifier final
{
public:
	identifier(std::u16string name, ::module* module) noexcept;
	identifier(const identifier& identifier);
	identifier(identifier&& identifier) noexcept;
	~identifier() = default;

public:
	identifier& operator=(const identifier& identifier);
	identifier& operator=(identifier&& identifier) noexcept;
	friend bool operator==(const identifier& lhs, const identifier& rhs) noexcept;
	friend bool operator!=(const identifier& lhs, const identifier& rhs) noexcept;

public:
	std::u16string name() const;
	void name(std::u16string new_name) noexcept;
	const ::module* module() const noexcept;
	::module* module() noexcept;
	void module(::module* new_module) noexcept;

private:
	std::u16string name_;
	::module* module_;
};