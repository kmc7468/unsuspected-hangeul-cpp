#pragma once

#include <cstddef>
#include <vector>

enum class command_type
{
	none,

	r,
	s,
	e,
	f,
	a,
	q,
	d,
	g,
};

class command final
{
public:
	using iterator = std::vector<command_type>::iterator;
	using const_iterator = std::vector<command_type>::const_iterator;
	using reverse_iterator = std::vector<command_type>::reverse_iterator;
	using const_reverse_iterator = std::vector<command_type>::const_reverse_iterator;

public:
	command() noexcept = default;
	command(const command& command);
	command(command&& command) noexcept;
	~command() = default;

public:
	command& operator=(const command& command);
	command& operator=(command&& command) noexcept;
	friend bool operator==(const command& lhs, const command& rhs) noexcept;
	friend bool operator!=(const command& lhs, const command& rhs) noexcept;
	command_type operator[](std::size_t index) const noexcept;
	command_type& operator[](std::size_t index) noexcept;

public:
	void clear() noexcept;
	bool empty() const noexcept;
	void swap(command& other) noexcept;
	std::size_t size() const noexcept;

	iterator begin() noexcept;
	const_iterator begin() const noexcept;
	const_iterator cbegin() const noexcept;
	iterator end() noexcept;
	const_iterator end() const noexcept;
	const_iterator cend() const noexcept;
	reverse_iterator rbegin() noexcept;
	const_reverse_iterator rbegin() const noexcept;
	const_reverse_iterator crbegin() const noexcept;
	reverse_iterator rend() noexcept;
	const_reverse_iterator rend() const noexcept;
	const_reverse_iterator crend() const noexcept;
	
private:
	std::vector<command_type> commands_;
};