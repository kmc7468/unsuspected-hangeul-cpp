#pragma once

#include <parser.hpp>

#include <functional>
#include <variant>
#include <vector>

class object;
class uh_status;

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
	explicit function(node_ptr function) noexcept;
	explicit function(native_function_t function) noexcept;
	function(const function& function);
	function(function&& function) noexcept;
	~function() = default;

public:
	function& operator=(const function& function);
	function& operator=(function&& function) noexcept;

public:
	void clear() noexcept;
	bool empty() const noexcept;
	void swap(function& other) noexcept;
	
	function_type type() const noexcept;
	const node_ptr& get_as_node_ptr() const noexcept;
	node_ptr& get_as_node_ptr() noexcept;
	const native_function_t& get_as_native_function_t() const noexcept;
	native_function_t& get_as_native_function_t() noexcept;

	object eval(uh_status& status);

public:
	std::variant<std::monostate, node_ptr, native_function_t> function_;
};