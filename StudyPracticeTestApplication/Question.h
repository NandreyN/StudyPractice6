#pragma once
#include <string>
#include <vector>
using namespace std;

struct Question
{
	string text;
	vector<string> variants;
	vector<int> correct;
	vector<int> chosen;
	char mode;
};