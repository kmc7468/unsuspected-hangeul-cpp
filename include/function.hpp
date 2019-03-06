#pragma once

#include <parser.hpp>

#include <cstddef>
#include <cstdint>
#include <functional>
#include <variant>
#include <vector>

class object;
class uh_status;

class native_function_param_t final
{
public:
	class const_iterator final
	{
	public:
		const_iterator() noexcept = default;
		explicit const_iterator(const native_function_param_t* param) noexcept;
		const_iterator(const native_function_param_t* param, std::int32_t index) noexcept;
		~const_iterator() = default;
		
	public:
		const_iterator& operator=(const const_iterator& iterator) noexcept;
		friend bool operator==(const const_iterator& lhs, const const_iterator& rhs) noexcept;
		friend bool operator!=(const const_iterator& lhs, const const_iterator& rhs) noexcept;
		friend bool operator>(const const_iterator& lhs, const const_iterator& rhs) noexcept;
		friend bool operator>=(const const_iterator& lhs, const const_iterator& rhs) noexcept;
		friend bool operator<(const const_iterator& lhs, const const_iterator& rhs) noexcept;
		friend bool operator<=(const const_iterator& lhs, const const_iterator& rhs) noexcept;
		friend const_iterator operator+(const const_iterator& iterator, std::int32_t v) noexcept;
		friend std::int32_t operator-(const const_iterator& iterator, std::int32_t v) noexcept;
		const_iterator& operator++() noexcept;
		const_iterator operator++(int) noexcept;
		const_iterator& operator--() noexcept;
		const_iterator operator--(int) noexcept;
		const object& operator*() const noexcept;
		const object* operator->() const noexcept;

	private:
		const native_function_param_t* param_ = nullptr;
		std::int32_t index_ = 0;
	};

public:
	native_function_param_t(uh_status& status) noexcept;
	native_function_param_t(const native_function_param_t&) = delete;
	~native_function_param_t() = default;

public:
	native_function_param_t& operator=(const native_function_param_t&) = delete;
	const object& operator[](std::size_t index) const;

public:
	std::size_t size() const noexcept;

	const_iterator begin() const noexcept;
	const_iterator cbegin() const noexcept;
	const_iterator end() const noexcept;
	const_iterator cend() const noexcept;

private:
	uh_status& status_;
};

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