#pragma once

#include <module.hpp>

#include <map>
#include <memory>
#include <vector>

class interpreter;

class uh_status final
{
	friend class interpreter;

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

private:
	std::unique_ptr<module> root_module_;
};

class interpreter final
{
public:
	interpreter() = default;
	interpreter(const interpreter&) = delete;
	~interpreter() = default;

public:
	interpreter& operator=(const interpreter&) = delete;

public:
	const uh_status& status() const noexcept;
	uh_status& status() noexcept;

private:
	uh_status status_;
};