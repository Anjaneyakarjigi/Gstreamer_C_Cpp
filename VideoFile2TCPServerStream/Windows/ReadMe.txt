/*************/
Project records audio from default mike source and stores it into file.

Run this project by below Command

/* Build project */
cmake -H. -Bbuild

/* Compile project */
cmake --build build  --config Release

// compilation adds cvImageToGstreamerFile into folder
/* Run Project */
Just run MikeRecoderToFile in the terminal, which creates mikeRecorded.mp3 audio file 

