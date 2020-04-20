#include <iostream>
#include <filesystem>

using namespace std;
int main()
{
	namespace fs = filename;
	path p{"./makefile.sh"};
	cout << fs::exists(p) << endl;
	return 0;
}

