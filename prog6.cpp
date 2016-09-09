// Program to find similarities between documents indicative of plagiarism
// Written by Matthew Williams and David Bumgarner
// Final Version: 11/25/14
// Large document set completed in 7 seconds

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>
using namespace std;

// function prototypes
int getdir (string dir, vector<string> &files);
void fileAccessor(vector<string>, int, int, string, vector<int> hashTable[], int tableLength);
string stringify(vector<string> words);
string fixWord( string );
bool isletter(char c);
void lowerCase( vector<string> &);
void key(string chunk, int fileNumber, vector<int> hashTable[], int length);
void hash(int key, int fileNumber, vector<int> hashTable[]);


int main(int argc, char * argv[])
{
  int length = 1000003; //length used for generation of hashTable and related arithmatic
  vector<int>* hashTable = new vector<int>[length]; //hash table
 	// the hash table is created on the heap due to memory constraints, and is designed
	// as an array of integer vectors
  string dir = string(argv[1]); //path to directory supplied at program call
  string directory = argv[1]; //appended with slash for easier call
  	directory += "/";
  vector<string> files = vector<string>(); //string vector storing file names

  // get all the file names in the directory
  getdir(dir,files); cout<<"\nFiles successfully located!\nParsing now..."<<endl;

  // #words/chunk <- (int)(2nd argv param)
  int numWords=atoi(argv[2]);

  // will get the number of files in the directory
  // the current file that we are working on
  int currFile = 0;

  // erase the first two files in the directory ( . and .. )
  files.erase(files.begin());
  files.erase(files.begin());
  
  // get the number of files in the files vector
  int numFiles = files.size();
    
  // to get the third argument needed: the number of collisions we care about
  int numCollide = atoi(argv[3]);

  // will go through each file in the directory
  while( currFile < numFiles )
  {
    // pass along the file names, number of words the user specified,
    // the current element, the ostream to write out to file, and the
    // directory name.
    fileAccessor(files, numWords, currFile, directory, hashTable, length );

    // increment to next file in the vector
    currFile++;
  } // end while

  // create a 2d array the size of the of the number of files in the directory
  // and fill in with 0's
  int detectionTable[numFiles][numFiles];
  for(int a=0; a<numFiles; a++) 
    for(int b=0;b<numFiles;b++)
      detectionTable[a][b]=0;

  // this process traverses the hashTable and pairs files hashed to the same location and 
  // updates the collisions in the detection table which is what is ultimately scanned for
  // incidents indicating plagiarism. This method allows the hash table to remain static and
  // unmodified should it need to be used again for another application within this program
  for(int i = 0; i < length; i++)
  {
     if((!hashTable[i].empty())&&(hashTable[i].size()!=1))
     {
        int x, y;
        int iLength;
        iLength=hashTable[i].size();

        if(hashTable[i].size()==2)
            detectionTable[hashTable[i][1]][hashTable[i][0]]++;
        else if (hashTable[i].size()>2)
        {
            int iSize=hashTable[i].size();
            for(int a=0; a<iSize;a++)
            {
		x=hashTable[i][a];
            	for(int b=(a+1); b<hashTable[i].size();b++)
		{
		   y=hashTable[i][b];
                   if(x!=y) 
			detectionTable[y][x]++;
                }
	    }
        }
     } // end if
  } // end for



  // Take the information in the detection table and move it into the consolidated
  // table where the first file, second file, and the number of collisions that are
  // above the user specified threshold are stored.
  //*********************************************

  int incidents=0; //storage for number of super-threshold values
  int consolidated[numFiles*2][3];
  	for (int a=0; a<(numFiles); a++)
  	  for (int b=0; b<(numFiles); b++)
  	    consolidated[a][b]=0;

  // move the data from detection table to consolidated table using a and b
  // as file one and file two (the colliding one) respectiveley.
  for (int a=0; a<numFiles; a++) 
  {
    for (int b=0; b<(numFiles-1); b++)
    {
       // if the number of collisions are over the threshold
       // the relevant files are stored in a consolidated table for reference
       if((detectionTable[a][b] > numCollide) && (a != b))
       { 
         consolidated[incidents][0]=a;
         consolidated[incidents][1]=b;
         consolidated[incidents][2]=detectionTable[a][b];
         incidents++;
       } 
    } // end for
  } // end for

//******************************************************
  // use selection sort to sort the consolidated table to put 
  // the largest number of collisions on top 
  int temp1, temp2, temp3, index, largest;
  for(int i=0; i<incidents; i++)
  {
    index=i;
    for(int j=i; j<incidents; j++)
    { 
      if(consolidated[j][2]>consolidated[index][2]) 
        index = j;
    } // end for
     
    // perform the swap
    temp1 = consolidated[index][0];
    temp2 = consolidated[index][1];
    temp3 = consolidated[index][2];

    consolidated[index][0] = consolidated[i][0];
    consolidated[index][1] = consolidated[i][1];
    consolidated[index][2] = consolidated[i][2];

    consolidated[i][0] = temp1;
    consolidated[i][1] = temp2;
    consolidated[i][2] = temp3;
  } // end for
//****************************************************


  // display the file names and the number of collisions between them from largest to smallest
  // and efforts to maintain formatting consistency are taken
  cout<<"Listing files by similarity:\n---------------------------"<<endl;
  for (int a=0; a<(incidents); a++)
  {
    if(( files[consolidated[a][0]] != files[consolidated[a][1]])&&(files[consolidated[a][1]].size()>13))
      cout<<files[consolidated[a][0]]<<"\t"<<files[consolidated[a][1]]<<"\t"<<consolidated[a][2];
    else  if( files[consolidated[a][0]] != files[consolidated[a][1]])
      cout<<files[consolidated[a][0]]<<"\t"<<files[consolidated[a][1]]<<"\t\t"<<consolidated[a][2];
    cout<<endl;
  } // end for

  cout<<"\n\n";
  return 0;

} // end main




// pre: valid parameters are passed in to a valid location
// process: the directory supplied is opened and pushes all of the file names into the files
// vector for future use and reference
int getdir (string dir, vector<string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir <<endl;
        return errno;
    }
    while ((dirp = readdir(dp)) != NULL){
        files.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}



// pre: valid arguments are passed in to process files
// process: the temp string appends to create a specific path to a unique file, and is opened as a c_string.
// After the file is opened, the first $counter words are loaded into the vector after being stripped of 
// non-characters and then converted to lowercase. After the words are loaded, it is keyed which auto-hashes
// the set of words into the master hashTable. The file is then processed as by using the vector as a queue
// and processed the same way as the first six elements of the file and continues until EOF, then is repeated 
// for all of the remaining files.
void fileAccessor( vector<string> files, int numWords, int currFile, string dir, vector<int> hashTable[],int tableLength )
{
  // get a string to get the file path ex: sm_doc_set/a.txt
  string temp = dir + files[currFile];

  int counter =0; //tracks limit of user defined word segments
  bool endFile=false; //switch for determining at EOF

  // open the file we will be reading from, the string needs to be a c_string
  ifstream readFile(temp.c_str());


  // vector to hold the words from the file
  vector<string> words;
  string fileWord = "";

  // get the first n-number of words, convert to lowercase, and
  // write out to a file;
  while( counter < numWords )
  {
    readFile >> fileWord;
    fileWord = fixWord(fileWord);
    words.push_back(fileWord);
    lowerCase(words);
    counter++;
  } // end while

  // hash the current n number of words
  key(stringify(words), currFile, hashTable, tableLength);   

  // delete the first element and get the next word
  // then convert to lowercase and remove unneeded characters and finally send
  // the chunk off to be hashed.
  // If the end of the file is reached endFile will become true
  // and the loop will end.
  while( endFile == false )
  {
    words.erase(words.begin());
    readFile >> fileWord;
    fileWord = fixWord(fileWord);
    words.push_back(fileWord);
    lowerCase(words);
    key(stringify(words), currFile, hashTable, tableLength);
    if( readFile.eof() )
      endFile = true;
  } // end while

  // close the current file
  readFile.close();
  return;
} // end fileAccessor()



//pre: argument is a valid string vector
//process: returns a string created out of the content of the vector parameter
string stringify(vector<string> words)
{ 
  string temp;
  
  //iterates throguh vector and appends to the end of the temp variable
  for (int x = 0; x < words.size(); x++)
    temp += words[x];

  return temp;
} // end stringify()


//pre: valid string is passed into fixword for modification
//process: returns a modified version of the word that has had non characters stripped from it
//utilizing the is letter subroutine
string fixWord( string word )
{
  for( int x = 0; x < word.size(); x++ )
  {
    if( isletter(word[x]) == false )
    {
      word.erase( x, 1 );
      x = 0;
    }
  }
  return word;
} // end fixword()



// **Helper function for fixWord
//pre: valid character is passed in as argument
//process: returns true  if the character's ascii value is a letter
bool isletter(char c)
{
  if( ((c <91) && (c>64)) || ((c<123) && (c>96)) ) 
    return true;
  else 
    return false;
} // end isletter()



//pre: a valid string vector is passed by reference to the function
//process: the string vector is broken into individual words and the words broken into individual
//characters and passed to the tolower function as in the string library, modifying the words vector
void lowerCase( vector<string> &words )
{
  for( int x = 0; x < words.size(); x++ )
  {
    string temp = words[x];
    for( int y = 0; y < temp.length(); y++ )
         temp[y] = tolower(temp[y]); 
    words[x] = temp;
  }
} // end lowerCase()


//pre: all the chunk of words has been extracted from the file and the argumens are of valid length
//Process: adds up even variable index's ascii values times a scaling multiplier based on repition,
//then resets teh multiplier and passes backwards throuh adding to the total with a sliding multiplier.
//The total is then inverted if negative to have positive values only and modded by the hash table size
//giving valid indexes. The newly generated key is passed along with the filenumber and the hash table
//to the hash function which stores the file number in the respective has table location.
void key(string chunk, int fileNumber, vector<int>  hashTable[], int length)
{  //local variables
   long total = 0;
   long multiplier=3;

   for(int i = 0; i < chunk.length(); i += 2)
   {
      total+=(chunk[i]*multiplier);
      multiplier*=17;
   }
      multiplier = 9;
   for(int j = chunk.length(); j > 1; j -= 2)
   {
      total+=( chunk[j] * multiplier );
      multiplier+=61;
   } 

   //Invert if negative
   if( total < 0) 
     total *= (-1);

   //Generate 'key' then hash with key as argument
   long key = (total%length);
   hash(key, fileNumber, hashTable);

} // end key()

//pre: valid key between 0->hashTable length has been generated, an intiger file number and a created 
//hash table have been created and passed as arguments
//process: simply pushes the file number into the hashTable at index key
void hash(int key, int fileNumber, vector<int> hashTable[])
{
   hashTable[key].push_back((fileNumber));
} // end hash()
