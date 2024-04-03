//Note: this code is incomplete. To-dos are listed at the bottom. Commented
//  out lines are not currently in use (but may be used in future implementations
//  and testing)

//Author: Grace Simonson
//Project: Junior Independent Study "Attacking Digital Audio Watermarks"

#include <iostream>

//to-do: ensure I'm using this library (libsndfile) according to its license
#include <sndfile.hh> //used for audio file handling

using namespace std;

int main()
{
    cout << "Starting program, initializing variables\n\n";
    
    //initialize variables
    char usrInput[4096]; //this is the largest character limit on file path names I could find
    string inputFile;
    //string outputFile = "outputFile.wav";
    //static short buffer[1024]; //used by file handler in some cases
    SndfileHandle inputHandler;
    
    //handle user input (grab input file)
    cout << "Enter the file name you would like to manipulate: ";
    cin.getline(usrInput, 4096);
    cout << "\nYou entered: " << usrInput << endl;

    inputFile = usrInput; //to-do: check if file actually exists
    inputHandler = SndfileHandle(inputFile);

    //print file information to show successful loading
    cout << "File info: " << endl;
    cout << "\tChannels: " << inputHandler.channels() << endl;
    cout << "\tFrames: " << inputHandler.frames() << endl;
    cout << endl;

    //To-do:
    //  Create and write to output file successfully
    //  Implement watermark attacks (compression, conversion, etc.)
    //  Test attacks using existing audio watermarking tools
    //  Implement error handling (check if input file exists, etc.)
    //  Ensure all shared code (libraries, etc.) is cited and used properly

}