#include <command.hpp>

int main(int argc, char** argv)
{
	std::u16string str = u"ㄴ ㄲ제 다 했다.";
	command cmd = command::parse(str);

	return 0;
}