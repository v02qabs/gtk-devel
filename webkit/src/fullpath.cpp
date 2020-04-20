#include <filesystem>
#include <iostream>

using namespace std;
int main()
{
	namespace fs = std::filename;
	fs::path p{"./makefile.sh"};
	cout << fs::exists(p) << endl;

	return 0;
}

