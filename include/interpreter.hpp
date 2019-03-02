#pragma once

#include <module.hpp>

#include <map>
#include <memory>
#include <vector>

enum class version
{
	v0_3,

	latest = v0_3,
};

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

public:
	::version version() const noexcept;

private:
	std::unique_ptr<module> root_module_;
	::version version_ = ::version::latest;
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
	const uh_status& status() const noexcept;
	uh_status& status() noexcept;

private:
	uh_status status_;
};