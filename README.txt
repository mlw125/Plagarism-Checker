Milestone 2 - Plagiarism Catcher
David Bumgarner & Matthew Williams
Final Version: 11/25/14


Relevant Files: project6.cpp 
                Makefile
                README.txt (this document)

Compiling:	Simply type "make" from bash to create the executable "plagiarismCatcher"

Running:	"./plagiarismCatcher path/to/files #wordsToSegment thresholdOfSimilarity"
Example:	./plagiarismCatcher /media/studentFiles/Essay1 6 200

Notes:
	-No bugs known as of yet, though without knowledge of actual overlap in files, there is no way 
	 to determine absolute effectiveness of keying/hashing algorithm.

	-Works on all document sets
	-Large document set completed in 7 seconds





Program logic flow and process is as follows:

The program is launched with three parameters: a path to files to be examined, a number of words to use as a comparable basis, and a threshold of similarity between documents that might warrant further investigation. 

After the program is called, the directory is opened assuming it exists and the files are located, loaded into a vector, and opened sequentially.

Once a file has been opened, a vector queue is utilized to read in sequential segments of the individual document (eg: if the parameter is 6, it reads words 1->6, 2->7, 3->8 and so on) and runs them through a battery of processes to simplify them down to lowercase characters without spaces. 

Once the words have been made 'uniform' they are passed into the keying algorithm which does a forward multiplication summation, and then adds a reverse additive summation, and is normalized to a positive value before being modded by the size of the created hash table, giving an index in the table for the file number of the current keyed phrase to bestored.

After the hash table has been created and filled, a loop determines if there is something at that index. If the vector is either empty or has only one element hashed there (meaning there are no duplicate phrases and therefore no potential for plagiarism) it is ignored. If two elements exist, they are paired and the corresponding position in an incident matrix is incrimented indicating duplicate phrasing across the two files. If three or more hashes share a key, they are paired mutually with each other (eg; the set {1,4,5,9} would contain pairs {1,4}, {1,5}, {1,9}, {4,5}, {4,9}, and {5,9}) and their respective matrix location incrimented.

After the incident matrix has been completed, pairs exceeding the likely plagiarism variable (supplied at program call) are stored in another simplified matrix containing the two files and the number of similarities between the two of them. These are then sorted with respect to the greatest number of similarities between the files, and displayed in descending order down to the established threshold, before releasing the program from memory and returning the user to the bash console.
