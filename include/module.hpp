#pragma once

#include <function.hpp>

#include <map>
#include <string>
#include <vector>

class module final
{
public:
	module(std::u16string name) noexcept;
	module(const module&) = delete;
	~module();

public:
	module& operator=(const module&) = delete;

public:
	void add_submodule(module* submodule);
	void add_function(std::u16string name, function function);

public:
	std::u16string name() const noexcept;
	std::vector<module*>& submodules() noexcept;
	const module* parent() const noexcept;
	module* parent() noexcept;

	std::map<std::u16string, function>& functions() noexcept;

private:
	std::u16string name_;
	std::vector<module*> submodules_;
	module* parent_ = nullptr;

	std::map<std::u16string, function> functions_;
};