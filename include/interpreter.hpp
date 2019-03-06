#pragma once

#include <module.hpp>

#include <memory>
#include <optional>
#include <string>

enum class version
{
	v0_3,

	latest = v0_3,
};

class argument_node;
class function;
class function_calling_node;
class interpreter;
class recursive_function_node;
class native_function_param_t;
class object;

class uh_status final
{
	friend class argument_node;
	friend class function;
	friend class function_calling_node;
	friend class interpreter;
	friend class recursive_function_node;
	friend class native_function_param_t;
	friend class object;

private:
	struct function_status
	{
		::function function;
		std::vector<object> arguments;
	};

public:
	uh_status(uh_status&& status) noexcept;
	~uh_status() = default;

private:
	uh_status();

public:
	uh_status& operator=(uh_status&& status) noexcept;
	
public:
	void reset();
	void swap(uh_status& status) noexcept;

public:
	::version version() const noexcept;

private:
	std::unique_ptr<module> root_module_;
	::version version_ = ::version::latest;
	std::vector<function_status> call_stack_;
};

class interpreter final
{
public:
	interpreter() = default;
	interpreter(::version version);
	interpreter(const interpreter&) = delete;
	~interpreter() = default;

public:
	interpreter& operator=(const interpreter&) = delete;

public:
	std::optional<object> eval(const std::u16string& code);

public:
	const uh_status& status() const noexcept;
	uh_status& status() noexcept;

private:
	uh_status status_;
};