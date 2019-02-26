#include <command.hpp>

#include <utility>

command::command(const command& command)
	: commands_(command.commands_)
{}
command::command(command&& command) noexcept
	: commands_(std::move(command.commands_))
{}

command& command::operator=(const command& command)
{
	return commands_ = command.commands_, *this;
}
command& command::operator=(command&& command) noexcept
{
	return commands_ = std::move(command.commands_), *this;
}
command_type command::operator[](std::size_t index) const noexcept
{
	return commands_[index];
}
command_type& command::operator[](std::size_t index) noexcept
{
	return commands_[index];
}

void command::clear() noexcept
{
	commands_.clear();
}
bool command::empty() const noexcept
{
	return commands_.empty();
}
void command::swap(command& other) noexcept
{
	commands_.swap(other.commands_);
}
std::size_t command::size() const noexcept
{
	return commands_.size();
}

command::iterator command::begin() noexcept
{
	return commands_.begin();
}
command::const_iterator command::begin() const noexcept
{
	return commands_.begin();
}
command::const_iterator command::cbegin() const noexcept
{
	return commands_.cbegin();
}
command::iterator command::end() noexcept
{
	return commands_.end();
}
command::const_iterator command::end() const noexcept
{
	return commands_.end();
}
command::const_iterator command::cend() const noexcept
{
	return commands_.cend();
}
command::reverse_iterator command::rbegin() noexcept
{
	return commands_.rbegin();
}
command::const_reverse_iterator command::rbegin() const noexcept
{
	return commands_.rbegin();
}
command::const_reverse_iterator command::crbegin() const noexcept
{
	return commands_.crbegin();
}
command::reverse_iterator command::rend() noexcept
{
	return commands_.rend();
}
command::const_reverse_iterator command::rend() const noexcept
{
	return commands_.rend();
}
command::const_reverse_iterator command::crend() const noexcept
{
	return commands_.crend();
}