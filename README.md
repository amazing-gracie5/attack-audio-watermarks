# attack-audio-watermarks
This C++ program will attempt to erase a digital audio watermark from an audio file. This is a work-in-progress. 

When complete, this command-line program will ask the user for an input file (presumably watermarked), attempt to erase any digital audio watermark that may be in the file, and write the result to an output file. This program may be used to test watermarking tools by submitting a watermarked file and attempting to extract the watermark from the output produced by this program. 

This program is incomplete--presently, it will take an input file and print some information about it to show the file has been processed, using the sound processing library libsndfile. This code is created as part of the junior independent study project "Attacking Digital Audio Watermarks" by Grace Simonson.
