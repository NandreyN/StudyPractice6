#include "RParser.h"
#include <fstream>
#include <assert.h>
#include <sstream>
#include <iterator>
#include <algorithm>

std::vector<Question> RParser::getQuestionsCollection(int count)
{
	ifstream input;
	vector<Question> qCollection;
	for (int i = 1; i <= count * count; i++)
	{
		string path = "Testdata\\" + string("q") + to_string(i) + ".txt";
		input.open(path);
		assert(input.is_open());

		vector<string> qText;
		vector<int> qCorrect;
		string rData,text;
		char mode = 0;

		getline(input, text);
		getline(input, rData);
		mode = rData[0];
		getline(input, rData);
		istringstream iss(rData);
		copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter(qText));

		getline(input, rData);
		iss = istringstream(rData);
		copy(istream_iterator<int>(iss), istream_iterator<int>(),back_inserter(qCorrect));

		Question q;
		q.chosen = vector<int>();
		q.mode = mode;
		q.correct = qCorrect;
		q.variants = qText;
		q.text = text;
		qCollection.push_back(q);
		input.close();
	}
	return qCollection;
}