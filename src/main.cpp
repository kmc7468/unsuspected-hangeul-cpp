#include <command.hpp>

int main(int argc, char** argv)
{
	std::u16string str = u"�� ���� �� �ߴ�.";
	command cmd = command::parse(str);

	return 0;
}