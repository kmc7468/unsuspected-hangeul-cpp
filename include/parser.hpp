#pragma once

#include <command.hpp>
#include <module.hpp>

#include <memory>
#include <ostream>
#include <string>
#include <vector>

enum class node_type
{
	none,

	integer_literal,
	function_defining,
	function_calling,
	recursive_function,
	arugment,
	identifier,
};

class node
{
public:
	node(const node&) = delete;
	virtual ~node() = default;

protected:
	node(node_type type) noexcept;

public:
	node& operator=(const node&) = delete;

public:
	node_type type() const noexcept;

private:
	node_type type_;
};

using node_ptr = std::shared_ptr<node>;

class integer_literal_node final : public node
{
public:
	integer_literal_node() noexcept = default;
	integer_literal_node(long long value) noexcept;
	integer_literal_node(const integer_literal_node&) = delete;
	virtual ~integer_literal_node() override = default;

public:
	integer_literal_node& operator=(const integer_literal_node&) = delete;

public:
	long long value = 0;
};

class function_defining_node final : public node
{
public:
	function_defining_node() noexcept = default;
	function_defining_node(node_ptr body) noexcept;
	function_defining_node(const function_defining_node&) = delete;
	virtual ~function_defining_node() override = default;

public:
	function_defining_node& operator=(const function_defining_node&) = delete;

public:
	node_ptr body;
};

class function_calling_node final : public node
{
public:
	function_calling_node() noexcept = default;
	function_calling_node(node_ptr function) noexcept;
	function_calling_node(std::vector<node_ptr> arguments, node_ptr function) noexcept;
	function_calling_node(const function_calling_node&) = delete;
	virtual ~function_calling_node() override = default;

public:
	function_calling_node& operator=(const function_calling_node&) = delete;

public:
	std::vector<node_ptr> arguments;
	node_ptr function;
};

class recursive_function_node final : public node
{
public:
	recursive_function_node() noexcept = default;
	recursive_function_node(node_ptr number) noexcept;
	recursive_function_node(const recursive_function_node&) = delete;
	virtual ~recursive_function_node() override = default;

public:
	recursive_function_node& operator=(const recursive_function_node&) = delete;

public:
	node_ptr number;
};

class argument_node final : public node
{
public:
	argument_node() noexcept = default;
	argument_node(node_ptr index, node_ptr function_number) noexcept;
	argument_node(const argument_node&) = delete;
	virtual ~argument_node() override = default;

public:
	argument_node& operator=(const argument_node&) = delete;

public:
	node_ptr index;
	node_ptr function_number;
};

class identifier_node final : public node
{
public:
	identifier_node() noexcept = default;
	identifier_node(::module* module, std::u16string name) noexcept;
	identifier_node(const identifier_node&) = delete;
	virtual ~identifier_node() override = default;

public:
	identifier_node& operator=(const identifier_node&) = delete;

public:
	::module* module;
	std::u16string name;
};

class parser final
{
public:
	parser() = delete;
	parser(const parser&) = delete;
	~parser() = delete;

public:
	parser& operator=(const parser&) = delete;

public:
	static std::vector<command> make_words(const command& command);
	static node_ptr parse(const std::vector<command>& words);
};