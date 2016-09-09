#Written by David Bumgarner for COSC3358, Fall 2014
#Email me with questions @  BraxtonDGray@gmail.com

#I've left example data in the file consistent with program #3a

# To use Extra Functions, type 'make' followed by the function name
#
# clean        --removes all object, .dat, and executable files 
# package      --packages directory into .zip for submission
# repackage    --deletes the existing .zip and replaces it
# backup       --backs up program in a tarball 
# OSbackup     --Off-site backup: same as backup but changes it's
#               ending destination out of the current
#               directory **change path required**


#Note about OSbackup:
        #/media/backups refers to a specific location on my home server
        #replace /media/backups with /your/path/to/some/directory3
 
#------------------------------------------------------------------------


#Specify program number and ID below for 'make package' function
PROGRAMNUMBER = 6
ID = plag_proj2_A04047800
#&&A04039719

#Set Compiler and compiler specific object switch
CC = g++
SWITCHES = -c -WALL

#Set Target Name for Executable
TARGET = plagiarismCatcher




#    CPP files get listed below: 
#  ***DO NOT INCLUDE EXTENSIONS***

#File containing main should be listed next to CLIENT
CLIENT = prog6

#Class files are listed next to CPP 1->4 AS THEY ARE NEEDED
CPP1 = 
CPP2 = 
CPP3 = 
CPP4 = 

 

#Object files for linking 
#  *****LEAVE BLANK*****
#------------------------
OF1 =
OF2 = 
OF3 = 
OF4 = 
OF5 = 
#------------------------


#Append appropriate class object variables to end of first and third following lines,
#once again, AS NEEDED. Do not add in more files than you actually have!
#Format should be $(CPPx).o 
$(TARGET): $(CLIENT).o 
	clear
	$(CC) -o $(TARGET) $(CLIENT).o 


$(OF1): $(CLIENT).cpp $(CPP1).h
	$(CC) $(SWITCHES) $(CLIENT).cpp 
	 
$(OF2): $(CPP1).cpp $(CPP1).h
	$(CC) $(SWITCHES) $(CPP1).cpp

$(OF3): $(CPP2).cpp $(CP2).h
	$(CC) $(SWITCHES) $(CPP2).cpp

$(OF4): $(CPP3).cpp $(CPP3).h
	$(CC) $(SWITCHES) $(CPP3).cpp

$(OF5): $(CPP4).cpp $(CPP4).h
	$(CC) -c $(CPP4).cpp


clean:
	rm -f *.o $(TARGET)

package:
	zip $(ID)*.*

repackage:
	rm -f prog$(PROGRAMNUMBER)_$(ID).zip
	zip prog$(PROGRAMNUMBER)_$(ID) *.cpp *.h

OSbackup:
	tar -zcvf prog$(PROGRAMNUMBER)Backup.tar.gz *.*
	mv prog$(PROGRAMNUMBER)Backup.tar.gz /media/backups

backup:
	tar -zcvf prog$(PROGRAMNUMBER)Backup.tar.gz *
	

