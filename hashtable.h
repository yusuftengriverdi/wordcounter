using namespace std;
#include <iostream>
#include <ctime>
#include <fstream>
#include <string> 

#define PRIME 23


int coll_no = 0;
int first_Size = pow(2, 12);
int TABLE_SIZE = pow(2, 12);



int compute_hash(string const& str) {
	const int p = 31;
	const int modulo = 1e9 + 9;
	long long value = 0;
	long long p_pow = 1;
	for (char letter : str) {
		value = (value + (letter - 'a' + 1) * p_pow) % modulo;
		p_pow = (p_pow * p) % modulo;
	}
	if (value < 0) value = -value;

	return value;
}
// QUOTE : https://cp-algorithms.com/string/string-hashing.html 

class HashNode {
	
public:
	string k;
	int v;
	HashNode() {
		this->k = k;
		this->v = v;
	}
	HashNode(string k, int v) {
		this->k = k;
		this->v = v;
	}


};

class HashTable {
	int curr_size;
	int sad_Face=0;
private:
	HashNode **t;
public:
	HashTable() 
	{
		t = new HashNode *[TABLE_SIZE];
		for (int i = 0; i < TABLE_SIZE; i++) 
		{
			t[i] = NULL;
		}
	}

	// function to check if hash table is full
	bool isFull()
	{
		// if hash size reaches maximum size
		return (curr_size == TABLE_SIZE);
	}
	bool almostFull()
	{
		// if hash size reaches maximum size
		return (curr_size > TABLE_SIZE / 2);
	}
	// the simplest hash functions
	int hashFN(int key)
	{
		return(key % TABLE_SIZE);
	}
	int hashFN2(int key)
	{
		return (PRIME - (key % PRIME));
	}

	int hashFN3(int key)
	{	
		int real_key = (key * 17 - 32);
		if ((key * 17 - 32) < 0)
		{
			real_key = -(key * 17 - 32);
		}
		return (real_key % TABLE_SIZE);
	}
		// QUOTE: https://www.codeproject.com/Articles/32829/Hash-Functions-An-Empirical-Comparison

	void insertPerfect(string word) 
	{
		if (isFull()) {
			//cout << "full" << endl;
			//cout << "rehashing by doubling size" << endl;
			reHash();
		};
		for (int i = 0; i < TABLE_SIZE; i++)
		{
			if (t[i] == NULL)
			{
				//frequency++;
				//hashTable[index] = (key, frequency);
				int value = 1;
				t[i] = new HashNode(word, value);
				curr_size++;
				sad_Face--;
				break;
			}
			else if (t[i]->k == word)
			{
				(t[i]->v)++;
				sad_Face--;			
				break;
			}

		}
	}

	// function to insert key into hash table
	void insertHash(string word)
	{	
		int index = checkWord(word, t, TABLE_SIZE);

		if (index != -1) {
			(t[index]->v)++;
			return;
		}
		//int key = HashCode(word);
		int key = compute_hash(word);
		
		// if hash table is full
		if (isFull()) {
			//cout << "full" << endl;
			//cout << "rehashing by doubling size" << endl;
			reHash();
		};

		
		// get index from first hash
		index = hashFN2(key);
		int pcount = 0;

		// if collision occurs
		if (t[index] != NULL && t[index]->k != word)
		{
			//cout << "collusion!!!" << endl;
			coll_no++;
		};
		//double hashing
		int newindex = index;
		int c = 1;
		while (t[index] != NULL && t[index]->k != word
			&& (pcount < (TABLE_SIZE / 2)))
		{	
			pcount++;
			// get index2 from second hash
			int index2 = hashFN3(key);
			// get newIndex
			newindex = (index + c * index2) % TABLE_SIZE;
			c++;
			index = newindex;
			
		}

		//if (pcount > TABLE_SIZE / 2 || almostFull()) {
		//	reHash();
		//}
		coll_no += pcount; 

		// Larson's Hash
		pcount = 0;
		int h = 0;
		while ((t[index] != NULL)
			&& (t[index]->k != word)
			&& (pcount < (TABLE_SIZE / 2)))
		{
			pcount++;
			h = (h * 101 + (unsigned)key++) % TABLE_SIZE;
			index = h;
		};
		coll_no += pcount;

		// QUOTE :https://stackoverflow.com/questions/628790/have-a-good-hash-function-for-a-c-hash-table

		//still collision?
		if (t[index] != NULL && t[index]->k != word)
		{
			sad_Face++;
			insertPerfect(word);
		};

		if (t[index] != NULL && t[index]->k == word  )
		{
			// increment value of that key since word counting
			(t[index]->v)++;

			//cout << "duplicate found..." << endl; // t[index]->v << endl;
		}
		// if no collision occurs
		else if (t[index] == NULL) // hashtable(index) is empty
		{
			//frequency++;
			//hashTable[index] = (key, frequency);
			int value = 1;
			t[index] = new HashNode(word, value);
			curr_size++;
		}
	}
	void printSadFace() {
		cout << ":( x " << sad_Face << endl;
	}
	// function to display the hash table
	void displayHash()
	{
		for (int i = 0; i < TABLE_SIZE; i++)
		{
			if (t[i] != NULL)
				cout << i << " --> "
				<< t[i] ->k << " " << t[i] ->v << endl;
			else
				cout << i << endl;
		}
		cout << "Current size :" << curr_size << endl;
		cout << "Collusion number :" << coll_no << endl;
	}

	void saveHash()
	{
		ofstream myfile;
		myfile.open("hash.txt");

		for (int i = 0; i < TABLE_SIZE; i++)
		{
			if (t[i] != NULL)
				myfile << i << " --> "
				<< t[i]->k << " " << t[i]->v << endl;
			else
				myfile << i << endl;
		}
		myfile << "Current size :" << curr_size << endl;
		myfile << "Collusion number :" << coll_no << endl;
		myfile.close();
	}

	void infoHashMap()
	{
		cout << "Current size :" << curr_size << endl;
		cout << "Collusion number :" << coll_no << endl;
	}

	void reHash()
	{
		int OldCapacity = TABLE_SIZE;
		TABLE_SIZE = TABLE_SIZE * 2 + 1;

		// allocate new bed. note: uses () to value-initialize nullptr entries
		HashNode** newHashTable = new HashNode*[TABLE_SIZE]();

		//fill in the new temp table with old info
		for (int i = 0; i < OldCapacity; ++i)
		{
			HashNode *n = t[i];
			if (n != nullptr)
			{
				newHashTable[i] = n;
			}
		}

		delete[] t;
		t = newHashTable;
	}

	int checkWord(string word, HashNode** sam, int n)
	{
		//cout << "checking" << endl;
		for (int i = 0; i < n; i++)
		{
			if (sam[i] != NULL && sam[i]->k == word)
			{ 
				//cout << "found" << endl;  
				return i; 
			}
		}
		//cout << "not found" << endl;
		return -1;
	}
	void printTopWords(int desired_number_of_words)
	{
		int counter = 0;
		cout << "starting process" << endl;
		HashNode** cache = new HashNode*[desired_number_of_words]();
		while (  counter != desired_number_of_words)
		{
			string most_fword;
			int max = 0;
			int argmax;
			for (int i = 0; i < TABLE_SIZE; i++)
			{
				if (t[i] !=NULL)
				{
					//cout << "checking..." << endl;
					if (checkWord(t[i]->k, cache, desired_number_of_words) == -1)
					{
						if (t[i]->v > max) 
						{
							max = t[i]->v;
							argmax = i; 
							//cout << " max is : " << max << endl;				
						}
					}
					//else { cout << "pass" << endl; }
				}
			}
			//cout << "listing word.." << endl;
			most_fword = t[argmax]->k;
			//cout << "word is: " << most_fword << endl;
			
			//cout << "assigning..." << endl;
			if (cache[counter] == NULL)
			{
				cache[counter] = new HashNode(most_fword, max);
			}
			
			//cout << "looping..." << endl;
			counter++;
		}


		cout << "printing" << endl;
		for (int i = 0; i < desired_number_of_words; i++)
		{
			if (cache[i] != NULL)
				cout << i+1 << " --> "
				<< cache[i]->k << " " << cache[i]->v << endl;
			else
				cout << i+1 << endl;
		}
	}
};