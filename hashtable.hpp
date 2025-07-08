/* Name: Arturo Calanche
 * Email: aac21p@fsu.edu
 * Class: COP4530-0001.fa24
All work was performed by Arturo Calanche */

#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

using namespace std;

//private interfaces

template<typename K, typename V>
void HashTable<K, V>::makeEmpty() {
    for (auto & thisList : theLists) {
        thisList.clear(); //delete each list
    }
}

template<typename K, typename V>
void HashTable<K, V>::rehash() {
    vector<list<pair<K, V>>> oldLists = theLists;

    //create new empty table with double the size
    theLists.resize(prime_below(2 * theLists.size()));
    for (auto & thisList : theLists) {
        thisList.clear();
    }

    //copy table over
    currentSize = 0;
    for (auto & thisList : oldLists) {
        for (auto & item : thisList) {
            insert(move(item));
        }
    }

}

template<typename K, typename V>
size_t HashTable<K, V>::myhash(const K &k) const {
    static hash<K> hf;
    return hf(k) % theLists.size(); //hash function from the lecture
}

// returns largest prime number <= n or zero if input is too large
// This is likely to be more efficient than prime_above(), because
// it only needs a vector of size n
template <typename K, typename V>
unsigned long HashTable<K, V>::prime_below (unsigned long n)
{
  if (n > max_prime)
    {
      cerr << "** input too large for prime_below()\nset to default capacity\n";
      return default_capacity;
    }
  if (n == max_prime)
    {
      return max_prime;
    }
  if (n <= 1)
    {
      cerr << "** input too small\nset to default capacity\n";
      return default_capacity;
    }

  // now: 2 <= n < max_prime
  vector <unsigned long> v (n+1);
  setPrimes(v);
  while (n > 2)
    {
      if (v[n] == 1)
	return n;
      --n;
    }

  return 2;
}

//Sets all prime number indexes to 1. Called by method prime_below(n)
template <typename K, typename V>
void HashTable<K, V>::setPrimes(vector<unsigned long>& vprimes)
{
  int i = 0;
  int j = 0;

  vprimes[0] = 0;
  vprimes[1] = 0;
  int n = vprimes.capacity();

  for (i = 2; i < n; ++i)
    vprimes[i] = 1;

  for( i = 2; i*i < n; ++i)
    {
      if (vprimes[i] == 1)
        for(j = i + i ; j < n; j += i)
          vprimes[j] = 0;
    }
}

//public interfaces

template<typename K, typename V>
HashTable<K, V>::HashTable(size_t size) :
        theLists(prime_below(size)), currentSize(0) {
            cout << theLists.size();
        } //sets size to prime number below input

template<typename K, typename V>
HashTable<K, V>::~HashTable() {
    makeEmpty();
}

template<typename K, typename V>
bool HashTable<K, V>::contains(const K &k) const {
    auto & whichList = theLists[myhash(k)];
    //returns true if an item with key k is found
    return find_if(begin(whichList), end(whichList),
                   [&k](const pair<K, V> &item) {return item.first == k;}) != end(whichList);
}

template<typename K, typename V>
bool HashTable<K, V>::match(const pair<K, V> &kv) const {
    auto & whichList = theLists[myhash(kv.first)];
    //returns true if an item with the same key and value is found
    return find(begin(whichList), end(whichList), kv) != end(whichList);
}

template<typename K, typename V>
bool HashTable<K, V>::insert(const pair<K, V> &kv) {
    auto & whichList = theLists[myhash(kv.first)];
    //iterator itr that points to the pair with same key as kv if it exists
    auto itr = find_if(begin(whichList), end(whichList),
                       [&kv](const pair<K, V> &item) {return item.first == kv.first;});

    if (itr != whichList.end()) {
        //if only the key exists, update value
        if (itr->second != kv.second) {
            itr->second = kv.second;
            return true;
        }
        //if key-value pair exists, don't add or update
        return false;
    }
    //if key doesn't exist, add new key-value pair
    whichList.push_back(kv);
    //rehash if table is already full
    if (++currentSize > theLists.size()) {
        rehash();
    }
    return true;
}

template<typename K, typename V>
bool HashTable<K, V>::insert(pair<K, V> &&kv) {
    auto & whichList = theLists[myhash(kv.first)];
    //iterator itr that points to the pair with same key as kv if it exists
    auto itr = find_if(begin(whichList), end(whichList),
                       [&kv](const pair<K, V> &item) {return item.first == kv.first;});

    if (itr != whichList.end()) {
        //if only the key exists, update value
        if (itr->second != kv.second) {
            itr->second = move(kv.second);
            return true;
        }
        //if key-value pair exists, don't add or update
        return false;
    }
    //if key doesn't exist, add new key-value pair
    whichList.push_back(move(kv));
    //rehash if table is already full
    if (++currentSize > theLists.size()) {
        rehash();
    }
    return true;
}

template<typename K, typename V>
bool HashTable<K, V>::remove(const K &k) {
    auto & whichList = theLists[myhash(k)];

    auto itr = find_if(begin(whichList), end(whichList), //find the pair with key k
                       [&k](const pair<K, V> &item) {return item.first == k;});

    if (itr == end(whichList)) {
        return false;
    } //erase pair pointed to by itr

    whichList.erase(itr);
    --currentSize;
    return true;
}

template<typename K, typename V>
void HashTable<K, V>::clear() {
    makeEmpty();
}

template<typename K, typename V>
bool HashTable<K, V>::load(const char *filename) {
    ifstream file(filename);
    if (!file) { //if file cannot be opened
        return false;
    }
    string line;
    while (getline(file, line)) {
        istringstream iss(line); //read file per line
        K key;
        V value;
        if (iss >> key >> value) {
            //if there's a key and a value in the same line, add a new pair to hash table
            insert(make_pair(key, value));
        } else {
            cout << "Error: Loading file entry is incorrect\n";
        }
    }
    file.close();
    return true;
}

template<typename K, typename V>
void HashTable<K, V>::dump() const {
    //print each list, even if empty
    for (size_t i = 0; i < theLists.size(); ++i) {
        cout << "v[" << i << "]: ";
        if (!theLists[i].empty()) { //if the list has items, print key and value
            for (auto itr = theLists[i].begin(); itr != theLists[i].end(); itr++) {
                cout << itr->first << " " << itr->second;
                //if there's more items in the list, separate them with a colon
                if (next(itr) != theLists[i].end()) {
                    cout << ":";
                }
            }
        }
        cout << endl;
    }
}

template<typename K, typename V>
size_t HashTable<K, V>::size() const {
    return currentSize;
}

template<typename K, typename V>
bool HashTable<K, V>::write_to_file(const char *filename) const {
    ofstream file(filename); //write items to a file (if file doesn't exist, create it)
    if (!file) {
        return false; //return false if file can't be opened
    }
    for (const auto & thisList : theLists) {
        for (const auto & item : thisList) {
            //write one item per line, key and value separated by white space
            file << item.first << " " << item.second << endl;
        }
    }
    return true;
}

#endif