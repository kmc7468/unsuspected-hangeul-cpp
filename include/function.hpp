#pragma once

#include <command.hpp>

#include <functional>
#include <variant>
#include <vector>

class object;

using native_function_param_t = std::vector<object>;
using native_function_res_t = object;
using native_function_t = std::function<native_function_res_t(const native_function_param_t&)>;
using native_function_ptr_t = native_function_res_t(native_function_param_t);

enum class function_type
{
	none,

	script,
	native,
};

class function final
{
public:
	function() noexcept;
	explicit function(command command) noexcept;
	explicit function(native_function_t function) noexcept;
	function(command command, function* parent) noexcept;
	function(native_function_t function, ::function* parent) noexcept;
	function(const function& function);
	function(function&& function) noexcept;
	~function() = default;

public:
	function& operator=(const function& function);
	function& operator=(function&& function) noexcept;
	friend bool operator==(const function& lhs, const function& rhs) noexcept;
	friend bool operator!=(const function& lhs, const function& rhs) noexcept;

public:
	void clear() noexcept;
	bool empty() const noexcept;
	void swap(function& other) noexcept;
	
	function_type type() const noexcept;
	const command& get_as_command() const noexcept;
	command& get_as_command() noexcept;
	const native_function_t& get_as_native_function_t() const noexcept;
	native_function_t& get_as_native_function_t() noexcept;

public:
	const function& parent() const noexcept;
	function& parent() noexcept;

public:
	std::variant<std::monostate, command, native_function_t> function_;
	function* parent_ = nullptr;
};