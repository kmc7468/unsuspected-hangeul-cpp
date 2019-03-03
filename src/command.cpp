#include <command.hpp>

#include <encoder.hpp>

#include <algorithm>
#include <utility>

command::command(const_iterator begin, const_iterator end)
	: commands_(begin, end)
{}
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
bool operator==(const command& lhs, const command& rhs) noexcept
{
	if (lhs.commands_.size() != rhs.commands_.size()) return false;
	else return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}
bool operator!=(const command& lhs, const command& rhs) noexcept
{
	if (lhs.commands_.size() != rhs.commands_.size()) return true;
	else return !std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
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

bool command::is_integer_literal() const noexcept
{
	for (command_type cmd : *this)
	{
		if (command_type::r > cmd || cmd > command_type::w) return false;
	}
	return true;
}
bool command::is_function_defining() const noexcept
{
	return commands_.size() == 1 && commands_.front() == command_type::g;
}
bool command::is_function_calling() const noexcept
{
	return commands_.size() >= 2 && commands_.front() == command_type::g && command(commands_.begin() + 1, commands_.end()).is_integer_literal();
}
bool command::is_recursive_function() const noexcept
{
	return commands_.size() == 1 && commands_.front() == command_type::d;
}
bool command::is_argument() const noexcept
{
	return commands_.size() >= 2 && commands_.front() == command_type::d && command(commands_.begin() + 1, commands_.end()).is_integer_literal();
}

command command::parse(const std::u16string& code)
{
	command command;
	std::vector<command_type>& result = command.commands_;

	for (std::size_t i = 0; i < code.size(); ++i)
	{
		const char16_t c = code[i];
		const bool is_high_surrogate = ::is_high_surrogate(c);

		if ((is_high_surrogate || !is_hangul(c)) && result.size() && result.back() != command_type::none)
		{
			result.push_back(command_type::none);
			if (is_high_surrogate) ++i;
		}
		else if (is_hangul(c))
		{
			static const auto to_command_type = [](char16_t c)
			{
				switch (c)
				{
				case u'丑': return command_type::r;
				case u'中': return command_type::s;
				case u'之': return command_type::e;
				case u'予': return command_type::f;
				case u'仃': return command_type::a;
				case u'仆': return command_type::q;
				case u'今': return command_type::t;
				case u'仄': return command_type::d;
				case u'元': return command_type::w;
				case u'冗': return command_type::g;
				default: return command_type::none; // Dummy
				}
			};
			const char32_t temp = get_chosung(c);

			if (const char16_t first = static_cast<char16_t>(temp >> 16); first)
			{
				const char16_t second = static_cast<char16_t>(temp & 0xFFFF);

				const command_type first_t = to_command_type(first),
								   second_t = to_command_type(second);
				const bool is_first_t_insert_space = first_t == command_type::d || first_t == command_type::g,
						   is_second_t_insert_space = second_t == command_type::d || second_t == command_type::g;

				if (result.size() && result.back() != command_type::none && is_first_t_insert_space) result.push_back(command_type::none);
				result.push_back(first_t);
				if (result.back() != command_type::none && is_second_t_insert_space) result.push_back(command_type::none);
				result.push_back(second_t);
			}
			else
			{
				const command_type t = to_command_type(static_cast<char16_t>(temp));
				const bool is_t_insert_space = t == command_type::d || t == command_type::g;

				if (result.size() && result.back() != command_type::none && is_t_insert_space) result.push_back(command_type::none);
				result.push_back(t);
			}
		}
	}

	if (result.size() && result.back() == command_type::none)
	{
		result.erase(result.end() - 1);
	}
	return command;
}