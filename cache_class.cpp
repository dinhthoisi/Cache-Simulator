#include<iostream>
#include<fstream>
#include<list>
#include<unordered_map>
#include<math.h>

using namespace std;

class Entry {
public:
  Entry() {
    valid = false;
  }
  ~Entry() {
    
  };
  void display(ofstream& outfile);

  void set_tag(int _tag) { tag = _tag; }
  int get_tag() { return tag; }

  void set_valid(bool _valid) { valid = _valid; }
  bool get_valid() { return valid; }

  void set_ref(int _ref) { ref = _ref; }
  int get_ref() { return ref; }

private:  
  bool valid;
  unsigned tag;
  int ref;
};

class LRUCache {
  list<pair<int,int>> l;

  unordered_map<int, list<pair<int,int>>::iterator> ma;
  int size; 
 
public:
  LRUCache() = default;

  LRUCache(int n) {
    size = n;
  }
  
  bool refer(int tag, int& lru) {
    bool res = false;
    if (ma.find(tag) == ma.end()) {
      if (l.size() == size) {
        auto last = l.back();
        ma.erase(last.first);
        lru = last.second;
        l.pop_back();
      }
      else {
        lru = l.size();
      }
      l.push_front({tag, lru});
      ma[tag] = l.begin();
    }
    else {
      l.splice(l.begin(), l, ma[tag]);
      res = true;
    }
    return res;
  }
};

// 2 words/block
class Cache {
public:
  Cache() = default;
  Cache(int _num_entries, int _assoc, int _num_word) {
    num_entries = _num_entries;
    assoc = _assoc;
    num_entries_aval = _num_entries;
    num_word = _num_word;
    
    entries = new Entry*[num_entries/assoc];
    lru = new LRUCache[num_entries/assoc];
    for (int i = 0; i < num_entries/assoc; i++) {
      entries[i] = new Entry[assoc];
      lru[i] = LRUCache(assoc);
    }
  }
  
  ~Cache() {
    for(int i = 0; i < num_entries/assoc; i++)
      delete[] entries[i];
    delete[] entries;
    delete[] lru;
  }

  int get_index(unsigned long addr) {
    int n = 2 + log2(num_word);
    return (addr >> n) & (num_entries / assoc - 1);
  }
  
  int get_tag(unsigned long addr) {
    int n = log2(num_entries / assoc) + 2 + log2(num_word);
    return addr >> n;
  }
  
  bool hit(ofstream& outfile, unsigned long addr) {
    int index = get_index(addr);
    unsigned tag = get_tag(addr);
    
    string miss_type;
    int lru_pos = 0;
    bool ref = lru[index].refer(tag, lru_pos);
    if (!ref) {
      entries[index][lru_pos].set_tag(tag);
      if (!entries[index][lru_pos].get_valid()) {
        entries[index][lru_pos].set_valid(true);
        miss_type = "Compulsory";
        num_entries_aval--;
      }
      else {
        if (num_entries_aval == 0)
          miss_type = "Capacity";
        else
          miss_type = "Conflict";
      }
      outfile << "MISS " << miss_type;
    }
    else
      outfile << "HIT";
    return ref;
  }
  
private:
  int assoc;
  unsigned num_entries;
  
  int num_word;
  int num_sets;
  Entry **entries;
  LRUCache* lru;
  int num_entries_aval;
};

class MultilevelCache {
public:
  MultilevelCache();
  MultilevelCache(int n, unsigned num_entries[], unsigned assoc[], unsigned num_word[]) {
    num_caches = n;
    caches = new Cache[n];
    for (int i = 0; i < n; i++) {
      caches[i] = Cache(num_entries[i], assoc[i], num_word[i]);
    }
  }
  
  ~MultilevelCache() {
    delete caches;
  }
  
  bool hit(ofstream& outfile, unsigned long addr) {
    for (int i = 0; i < num_caches; i++) {
      if (caches[i].hit(outfile, addr))
        return true;
    }
    return false;
  }
  
private:
  int num_caches = 1;
  Cache* caches;
};
