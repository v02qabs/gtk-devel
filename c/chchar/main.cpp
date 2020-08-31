#include <iostream>
#include <vector>
#include <regex>
#include <string>
#include <fstream>


using namespace std;


vector<string> split (string s, char pattern)
{
	int first = 0;
	int last = s.find_first_of(pattern);
	vector<string> result;
	while(first < s.size())
	{
		string subStr(s, first, last - first);
		result.push_back(subStr);

		first = last + 1;
		last = s.find_first_of(pattern, first);
		if(last == string::npos)
		{
			last = s.size();
		}
	}
	return result;
}



int main(int argc, char *argv[])
{
	cout << "Hello." << endl;
	string import_method = "import com.example.myapp";
	string remove_import = import_method.substr(7);
	cout << "remove_import: " << remove_import << endl;	// com.exmaple.myapp
	string commyapp = "com.example.myapp";
	//文字列を、'.'で分割する。
	char pattern = '.';

	for(const auto subStr : split(commyapp, pattern))
	{
		cout << "subStr : " << subStr << endl;
	}


	return 0;
}

