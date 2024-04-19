# attack-audio-watermarks
This C++ program will attack (attempt to erase) a digital audio watermark from a WAV audio file. 

This command-line program will ask the user for an input file (presumably watermarked), attempt to erase any digital audio watermark that may be in the file, and write the result to an output file. This program may be used to test watermarking tools by submitting a watermarked file and attempting to extract the watermark from the output produced by this program. Currently, two attacks are implemented: crop attack and shift attack. The user may choose which of these attacks to perform. 

## Crop Attack
The crop attack will take an input file and an amount of time (in seconds) and crops the input file to the length specified (starting at the beginning of the file). For example, if the time submitted was 30 seconds, the first 30 seconds of the input file would be written to the output file. 

This attack targets watermarks that may be spread too thin--if the information needed to extract the full watermark is spread across the whole file, then the whole file is needed for extraction. If this is the case, then this attack would successfully prevent successful watermark extraction. This attack could also be used to test how small a portion of the watermarked file is needed for extraction. For example, one could continually crop a watermarked file and attempt extraction after each cropping. The shortest file that yields a successful extraction gives insight into how much of the file is needed to convey the watermark. 

## Shift Attack
The shift attack will take an input file and copy its entire contents to an output file, but with a very short amount of silence added to the beginning. 

This attack targets watermarks that require the watermark embedder and extractor to visit the same points when embedding and extracting the watermark, respectively. This can be classified as a synchronization attack--that is, an attack that does not destroy the watermark, but instead prevents the extractor from locating watermark information. By adding a small piece of silence to the start of the audio file, this attack shifts the watermark information to a later point in the file, making it more difficult for an extractor to find it. This attack also has the advantage of being fairly imperceptible, as the amount of silence is very short. 

## Other Functionality and Details
This program also prints information on the input file and resulting output file after each attack.

This program utilizes the sound processing library `libsndfile` (found at https://github.com/libsndfile/libsndfile). This code is created as part of the junior independent study project "Attacking Digital Audio Watermarks" by Grace Simonson at the College of Wooster.

For to-dos and future work, see open issues and comments in `driver.cpp`.
