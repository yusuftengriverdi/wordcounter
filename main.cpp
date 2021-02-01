// CMP_PROJECT.cpp : Bu dosya 'main' işlevi içeriyor. Program yürütme orada başlayıp biter.
//

using namespace std;
#include <iostream>
#include <ctime>
#include <fstream>
#include "hashtable.h"
#pragma warning( disable : 4996 )


HashTable h;

int BodyCount(string str, string s)
{
	int length = 0;
	for (int i = 0; i < str.length(); i++)
	{
		if (str.substr(i, s.length()) == s)
		{
			length++;
		}
	}
	return length;
}
bool CompareifDiff(string text, string stop)
{
	if (text == stop)
	{
		return true;
	}

	else return false;
}

void Count(ifstream &inputFile)
{
	string text;
	string read;

	while (getline(inputFile, read))
	{
		text += (read + " ");
	}

	const string start_body = "<BODY>";
	const string end_body = "</BODY>";
	int body_count = BodyCount(text, "<BODY>");
	string* inner_text_s = new string[body_count];

	auto first = text.find(start_body);
	auto last = text.find(end_body, first + 1);
	string StopWords;
	ifstream inputStop;
	string read2;
	inputStop.open("C:\\Users\\ASUS\\Desktop\\Reuters-21578\\stopwords.txt");

	while (getline(inputStop, read2))
	{
		StopWords += (read2 + " ");
	}

	string stopword;
	int deletecount2 = 0;

	string wordlist;


	for (int q = 0; q < StopWords.length() + 1; q++)
	{
		if (StopWords[q] == ' ')
		{
			StopWords.erase(0, stopword.length() + deletecount2);


			wordlist += ("+" + stopword + "-");
			stopword = "";
			q = 0;
			deletecount2 = 1;

		}
		else {
			stopword = stopword + StopWords[q];
		}
	}

	for (int i = 0; i < body_count; i++)
	{
		inner_text_s[i] = text.substr(first + start_body.length(), last - start_body.length() - first);
		int length_inner_text = inner_text_s[i].length();

		for (int j = 0; j < length_inner_text; j++)
		{
			inner_text_s[i][j] = tolower(inner_text_s[i][j]);
		}

		first = text.find(start_body, last + 1);
		last = text.find(end_body, first + 1);

		string inner_text = inner_text_s[i];
		string newword = "";

		int delete_count = 0;
		for (int z = 0; z < inner_text.length() + 1; z++)
		{
			if (inner_text[z] < 97 || inner_text[z] > 122 || inner_text[z] == 39)
			{
				if (newword != "")
				{
					if (wordlist.find("+" + newword + "-") == std::string::npos)
					{

						h.insertHash(newword);

						//std::cout << newword << '\t';
						//Word word(newword);
						//std::cout << word.frequency << endl;
					}
					inner_text.erase(0, newword.length() + delete_count);
					newword = "";
					z = 0;
					delete_count = 1;

				}

				else {
					delete_count++;
				}
			}
			else {
				newword = newword + (inner_text[z]);
			}
		}
	}
}

int main()
{ 
	clock_t start, stop;
	ifstream inputFile;

	double totalTime;

	start = clock();
	string FilePath;
	for (int i = 0; i < 22; i++)
	{
		//cout << (i < 10) << endl;
		if (i < 10) 
		{
			FilePath = "C:\\Users\\ASUS\\Desktop\\Reuters-21578\\reut2-00" + to_string(i) + ".sgm"; 
		}
		else 
		{
			FilePath = "C:\\Users\\ASUS\\Desktop\\Reuters-21578\\reut2-0" + to_string(i) + ".sgm"; 
		}
		//cout << FilePath << endl;
		inputFile.open(FilePath);
		Count(inputFile);
		inputFile.close();
		cout << i << endl;
	}
	h.printSadFace();
	h.printTopWords(10);


	stop = clock();
	totalTime = (stop - start) / (double)CLOCKS_PER_SEC;
	cout << "Total Time Elapsed: " << totalTime << endl;
	h.infoHashMap();
	//h.saveHash();

	return 0;
}