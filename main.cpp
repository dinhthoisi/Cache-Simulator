#include<iostream>
#include<fstream>  // need this for file I/O
#include<cstdlib>   // for exit()
#include "cache_class.cpp"


using namespace std;

const int MAX_SIZE = 1000;

  
int main(int argc, char*argv[]) {

  if (argc < 2) {
    cout << "Usage: " << endl;
    cout << "   ./file_process filename" << endl;
    return 0;
  }

  /* get base file name from command-line */
  string input_filename = argv[argc - 1];

  /* create input and output filenames */
  string output_filename = input_filename + "_output";

  /* create input and output files streams */
  ofstream output;
  ifstream input;


  /* open input stream for reading */
  input.open(input_filename);

  /* check if input stream has been succesfully opened; bail otherwise */
  if (!input.is_open()) {
    cerr << "Could not open input file " << input_filename << ". Exiting ..." << endl;
    exit(0);
  }
    
  unsigned long *nums = new unsigned long[MAX_SIZE];
  int count = 0;
  while (!input.eof() && count < MAX_SIZE) {
    input >> nums[count];
    count++;
  }

  /* done reading from input file; close the stream */
  input.close();
  
  /* open output file stream for writing */
  output.open(output_filename);
  
  // num_entry and assoc
  if (argc == 4) {
    unsigned entries = atoi(argv[1]);
    unsigned assoc = atoi(argv[2]);
    Cache* cache = new Cache(entries, assoc, 1);
    
    string miss_type;
    /* no need to check if output file is open; we will create new file if it doesn't exist; 
       overwrite otherwise */
    for (int i = 0; i < count; i++) {
      output << nums[i] << " : ";
      cache->hit(output, nums[i] * 4);
      output << endl;
    }
    delete cache;
  }
  else {
    const int n = (argc - 1) / 2;
    unsigned entries[n];
    unsigned assoc[n];
    unsigned num_word[n];
    for (int i = 1; i < argc; i += 2) {
      entries[(i - 1) / 2] = atoi(argv[i]);
      assoc[(i - 1) / 2] = atoi(argv[i + 1]);
      num_word[(i - 1) / 2] = 2;
    }
    
    MultilevelCache* caches = new MultilevelCache(n, entries, assoc, num_word);
    for (int i = 0; i < count; i++) {
      output << nums[i] << " : ";
      caches->hit(output, nums[i] * 4);
      output << endl;
    }
  }

  /* close output stream */
  output.close();
  return 0;
}