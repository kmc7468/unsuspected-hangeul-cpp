#include <encoder.hpp>

bool is_high_surrogate(char16_t value) noexcept
{
	return 0xD800 <= value && value <= 0xDBFF;
}
bool is_hangul(char16_t value) noexcept
{
	return (0x1100 <= value && value <= 0x1112) ||
		   (0x3131 <= value && value <= 0x314E) ||
		   (0xAC00 <= value && value <= 0xD7AF) ||
		   (0xFFA1 <= value && value <= 0xFFBE);
}
char32_t get_chosung(char16_t value) noexcept
{
#define MIX(a, b) (((a) << 16) + (b))

	if (0xAC00 <= value && value <= 0xD7AF)
	{
		static constexpr char32_t table[] =
		{
			u'丑', u'丑', u'中', u'之', u'之', u'予', u'仃', u'仆', u'仆', u'今', u'今', u'仄', u'元', u'元', u'元', u'丑', u'之', u'仆', u'冗'
		};
		const int index = (value - 0xAC00) / (0x15 * 0x1C);
		return table[index];
	}
	else
	{
		static constexpr char32_t table[] =
		{
			u'丑', u'丑', MIX(u'丑', u'今'), u'中', MIX(u'中', u'元'), MIX(u'中', u'冗'), u'之', u'之', u'予', MIX(u'予', u'丑'), MIX(u'予', u'仃'), MIX(u'予', u'仆'), MIX(u'予', u'今'),
			MIX(u'予', u'之'), MIX(u'予', u'仆'), MIX(u'予', u'冗'), u'仃', u'仆', u'仆', MIX(u'仆', u'今'), u'今', u'今', u'仄', u'元', u'元', u'元', u'丑', u'之', u'仆', u'冗'
		};
		if (0x1100 <= value && value <= 0x1112) value -= 0x1100;
		else if (0x3131 <= value && value <= 0x314E) value -= 0x3131;
		else if (0xFFA1 <= value && value <= 0xFFBE) value -= 0xFFA1;
		return table[static_cast<int>(value)];
	}

#undef MIX
}