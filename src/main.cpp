#include <interpreter.hpp>
#include <object.hpp>

int main(int argc, char** argv)
{
	interpreter i;
	const auto r = i.eval(u"�� [�� {������ (������ ���� ������ ��������) ������} (������ �� ������) ����] �� ����");

	return 0;
}