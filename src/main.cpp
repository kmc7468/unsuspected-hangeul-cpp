#include <interpreter.hpp>
#include <object.hpp>

int main(int argc, char** argv)
{
	interpreter i;
	const auto r = i.eval(u"今 [中 {丑仄丑 (丑仄丑 中丑 之冗之 丑仄冗中) 丑冗之} (丑仄丑 之 元冗之) 冗之] 冗 冗中");

	return 0;
}