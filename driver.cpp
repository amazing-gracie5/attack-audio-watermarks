//This command-line C++ program utilizes the libsndfile library to carry out digital watermarking
//  attacks on an input file. The user can input a (.wav) file and select an attack (either 
//  a crop attack or shift attack). The result of the attack will be written to an output file.
//  For methodology and reasoning, please see the README

//Author: Grace Simonson
//Project: Junior Independent Study "Attacking Digital Audio Watermarks" (The College of Wooster)

//Source code: All code that reads input file information and writes to output file 
//  is based on example code at https://cindybui.me/pages/blogs/visual_studio_0 and 
//  https://github.com/libsndfile/libsndfile/blob/master/examples/sndfilehandle.cc

#include <iostream>
#include <sndfile.hh> //used for audio file handling

using namespace std;

//Functions

//Takes a string of a file name as input, returns a string containing file information
//  Current info returned: channels, frames, approximate length (in seconds)
string fileInfo(string file) {
    SndfileHandle fileHandler = SndfileHandle(file);
    string returnStr;

    returnStr += "File info for " + file + ":\n";
    //calculate channels
    returnStr += "\tChannels: " + to_string(fileHandler.channels()) + "\n";
    //calculate frams
    returnStr += "\tFrames: " + to_string(fileHandler.frames()) + "\n";
    //calculate length
    double length = static_cast<double>(fileHandler.frames())/static_cast<double>(fileHandler.samplerate());
    returnStr += "\tLength (seconds): " + to_string(length) + "\n";

    return returnStr;
}

//Takes a string of input file name, string of output file name, and double of length in seconds 
//  and crops the input file to be as long as the length specified (starting from the beginning).
//  For example, if 30 is submitted as the seconds parameter, the output file will be the first
//  30 seconds of the input file. Returns success message (string). Throws invalid_argument 
//  exception if seconds parameter is longer than file length
string cropAttack(string inFile, string outFile, double seconds) {
    string returnStr;
    SndfileHandle infileHandler = SndfileHandle(inFile); //error checking needed

    //calculate length of file
    double length = static_cast<double>(infileHandler.frames())/static_cast<double>(infileHandler.samplerate());
    
    //check that seconds parameter is valid
    if (seconds > length) {
        throw invalid_argument("Error. Seconds parameter must be shorter than file length");
    }
    
    //calculate size of input and output
    int inputSize = infileHandler.frames() * infileHandler.channels();
    //doing math as doubles to be more precise, but final product must be int
    int outputSize = (seconds * static_cast<double>(inputSize)) / length;

    //adjust size of outputSize to be divisible by number of channels
    while (outputSize % infileHandler.channels() != 0) {
        outputSize -= 1;
    }

    //create and populate array of inputfile info
    float* infileArray = new float[inputSize];
    infileHandler.readf(infileArray, inputSize);
    
    //create output file handler
    SndfileHandle outfileHandler = SndfileHandle(outFile, SFM_WRITE, SF_FORMAT_WAV | SF_FORMAT_PCM_24, infileHandler.channels(), infileHandler.samplerate());
    
    //write to output file
    outfileHandler.write(infileArray, outputSize);

    //deallocate memory
    delete[] infileArray;

    returnStr += "\nCrop Attack complete. First " + to_string(seconds) + " seconds of " + inFile + " copied to " + outFile + "\n";
    return returnStr;
}

//Takes a string of input file name and a string of output file name and copies the input file to the output
//  file, but with a very small amount of silence added to the beginning. Returns success message (string)
string shiftAttack(string inFile, string outFile) {
    string returnStr;
    size_t silentFrames;
    SndfileHandle infileHandler = SndfileHandle(inFile);
    
    //calculate how much silence to add (must be divisible by channels)
    silentFrames = 100 * infileHandler.channels(); //100 is arbitrary (small amount to be imperceptible)

    //create output array and set first x frames to silence
    float* outputArray = new float[silentFrames + (infileHandler.frames() * infileHandler.channels())];
    memset(outputArray, 0, silentFrames);

    //read in file after silence
    infileHandler.readf(&outputArray[silentFrames], infileHandler.frames());

    //write silence and input audio to output file
    SndfileHandle outfileHandler = SndfileHandle(outFile, SFM_WRITE, SF_FORMAT_WAV | SF_FORMAT_PCM_24, infileHandler.channels(), infileHandler.samplerate());
    outfileHandler.write(outputArray, (infileHandler.frames() * infileHandler.channels()) + silentFrames); //CHANGE VARIABLES

    //deallocate memory
    delete[] outputArray;
    
    returnStr = "\nShift Attack complete. Output file " + outFile + " populated\n";
    return returnStr;
}

int main()
{
    cout << "Starting program, initializing variables\n\n";
    
    //initialize variables
    char usrInput[4096]; //this is the largest character limit on file path names I could find
    string inputFile;
    string outputFile = "outputFile.wav";
    char usrAction[2];
    SndfileHandle inputHandler;
    SndfileHandle outputHandler;
    
    //handle user input (grab input file)
    cout << "Please note, for correct file processing, include the entire path or run the source code in the same directory as the file. Also, the file must be a .wav file." << endl;
    cout << "Enter the file name you would like to manipulate: ";
    cin.getline(usrInput, 4096);
    cout << endl;

    inputFile = usrInput; //DONE to-do: check if file actually exists
    inputHandler = SndfileHandle(inputFile);

    //checks validity of input file (no frames means no audio information)
    if (inputHandler.frames() == 0) {
        cout << "Error opening file\n";
        return -1;
    }

    //print some basic file information about input file
    cout << fileInfo(inputFile) << endl;

    //handle user input (grab action)
    cout << "Actions:" << endl;
    cout << "[C] Crop Attack (shorten a file to the first n seconds)" << endl;
    cout << "[S] Shift Attack (add a short bit of silence to the beginning of the file)" << endl;
    cout << "[Q] Quit program" << endl;
    cout << "Note: All actions will not affect the orginal input file. Each action will be written to the output file " << outputFile << "\n";
    cout << "\nPlease enter the letter of the action you would like to take: ";

    cin.getline(usrAction, 2); 

    char usrChar = toupper(usrAction[0]);
    cout << endl;

    if (usrChar == 'C') {
        cout << "\nBeginning Crop Attack" << endl;
        cout << "This attack will write the first n seconds of the input file to the output file." << endl;
        
        //handle user input (grab seconds parameter for cropAttack)
        cout << "Enter n (it must be shorter than the overall file length): ";
        char usrSecondsString[16]; //double in c++ is 15 places long max
        cin.getline(usrSecondsString, 16);
        
        //change string to double
        char * endPtr;
        double usrSeconds = strtod(usrSecondsString, &endPtr);
        
        //check that user input is valid (strtod will return 0 if unable to find double)
        if (usrSeconds == 0) {
            cout << "\nError. Must enter a valid double or number greater than 0" << endl;
            return -1;
        }

        //Now that all input has been checked, run crop attack with input
        //Using try block because cropAttack checks if seconds > length of file
        try {
            cout << cropAttack(inputFile, outputFile, usrSeconds) << endl;
        } catch (invalid_argument& e) {
            cout << "\n" << e.what() << endl;
            return -1;
        }
        
        //print info on populated output file
        cout << fileInfo(outputFile);

    } else if (usrChar == 'S') {
        cout << "\nBeginning Shift Attack" << endl;
        cout << "This attack will copy the input file to an output file, with a very short bit of silence added to the beginning." << endl;

        //user input has already been checked, so run shift attack
        cout << shiftAttack(inputFile, outputFile) << endl;

        //print info on populated output file
        cout << fileInfo(outputFile);

    } else if (usrChar == 'Q') {
        cout << "\nGood bye!" << endl;
    } else {
        cout << "\nError. Invalid input" << endl;
        return -1;
    }

    //Possible improvements/future work
    //  Implement more attacks
    //  Clean structure of code to make it more loosely coupled
    //  Handle errors better/more consistently throughout program
    //  Have crop attack crop from a specified starting point (instead of the beginning)
}
