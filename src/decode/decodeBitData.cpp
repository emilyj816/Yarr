#include <iostream>
#include <vector>
#include <array>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>  
#include <time.h>
#include <chrono>
#include <algorithm>
#include <map>
#include <random>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <iterator>

/*

NOTE: I encoded maps according to the process described in the RD53B manual. This means encoding the binary tree by splitting the array first left right, then top bottom, then left right, then left right again. This order is arbitrary of course, but it is embedded into the code. 

When arrays are returned, they're returned in the following format:

0  1  2  3  8  9  10 11
4  5  6  7  12 13 14 15

Where the numbers represent the address of that pixel in the array. This slightly strange formatting aligns with the order of dividing the array into halves and encoding, and just makes everything a little easier and faster. 

 */

//decodes the hit map using a combination of lookup table and decoder
bool decodeHitMap(std::vector<bool> bin, std::vector<std::vector<bool> >sortedTableFinal, int toDec, int beginning, bool(&twoBitArr)[16]);
//converts binary to decimal
int binToDec (std::vector<bool> bin);
//IN PROGRESS: strings together the functions necessary to decode 64 bit streams
bool mastermind (std::vector<bool> bin);
//IN PROGRESS(ISH): finds when the encoded hit map begins (parses through tags, other things)
int findHitMapBeginning(std::vector<bool> bin);
//Adds a random 4 bit ToT value for every hit
std::vector<bool> addRandomToT(std::vector<bool>bin, int counter);
//generates random map...not used due to problems with seeding and randomness...
std::vector<bool> randomMapGen();
//Encodes a hit map
std::vector <bool> encode (std::vector<bool>twoBitArr0);
//Makes a lookup table but only for maps with encoded strings less than or equal to sizeLimit bits long
std::vector<std::vector<bool> > makeLookupTable(int sizeLimit);

int main () {
  //make lookup table
  //limits size of encoded bits in lookup table 
  int sizeLimit = 8;
  //table of encoded hit maps
  std::vector<std::vector<bool> > sortedTableFinal = makeLookupTable(sizeLimit);
  
  //start timing
  std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
  std::random_device dev;
  std::mt19937 rng(dev());
  int oneHundredPercent = 100;
  std::uniform_int_distribution<std::mt19937::result_type> dist6(0, oneHundredPercent-1);
  srand (time(0));

  //generate random hit maps
  
  //probability of any one pixel being hit
  double hitProb = 0.1;

  //probability that the pixel to the right will be hit, if the current pixel is hit
  double twoClumpProb = 0;

  //loop through many times to test speed of code
  for(unsigned int i=0; i<500000; i++){
    //to be filled with random map
    std::vector<bool> rando = {0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0};
    //tracks whether there are any hits in the map
    bool flag = 0;

    //loops through map
    for (unsigned int x=0; x<16; x++){
      if(dist6(rng)<oneHundredPercent*hitProb){//pixel x has been hit
	flag = 1; //there has been a hit
	rando[x] = 1; 
	if(dist6(rng)<twoClumpProb*oneHundredPercent){//this is a clump--two hits
	  //some exceptions due to the way I numbered the array
	  if(x==3){
	    rando[8] = 1;
	  }
	  else if(x==7){
	    rando[12] = 1;
	  }
	  else{ 
	    if(x!=11 && x!=15){//11 and 15 are on the right end of the map, and thus don't have a right neighbor to be hit. 
	      rando[x+1] = 1;
	    }
	  }
	}
      }
      //std::cout<<rando[x]<<" ";//can be used to print the map
    }
    //std::cout<<"\n";//can be used to print the map

    //Empty maps will never be sent to the decoder...run the loop again
    if(!flag){
      i--;
      continue;
    }
    
    //Count the number of hits in the map--stored in counter
    int counter = 0;
    for(std::vector<bool>::const_iterator j = rando.begin(); j!=rando.end(); ++j){ 
      if(*j){
	counter++;
      }
    }

    //Use the encoder to encode this hit map
    std::vector<bool>bin = encode(rando);
    //Add counter number of random 4 bit ToTs to the binary
    bin = addRandomToT(bin, counter);
    //IN PROGRESS: Add some tags and IDs to test the decoder for streams

    //Vector containing first sizeLimit digits of the encoded data
    std::vector<bool> lookingUp(bin.begin(), bin.begin()+sizeLimit);
    //Convert the first sizeLimit digits to decimal to feed into the lookup table later
    int toDec = binToDec(lookingUp);
    //Finds which digit of the stream the hit map begins at
    int begin = findHitMapBeginning(bin);

    //Make empty hit map to be filled by the decoder
    bool twoBitArr[16] = {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0};

    //Decodes the encoded hit map :). The decoded hit map is stored in twoBitArr
    bool hmm = decodeHitMap(bin, sortedTableFinal, toDec, begin, twoBitArr);
 
  }//and....repeat
  
  //end timing
  std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds> (t2 - t1).count();
  //Print time it took to run the encoding+decoding
  std::cout<<duration<<std::endl;
  
  return 0;

}

//IN PROGRESS: 
bool mastermind(std::vector<std::vector<bool> > bin){//each row is a 64 bit stream
  int sizeLimit = 8;
  for(int i=0; i<bin.size(); i++){//loop through the different 
    int beginning = findHitMapBeginning(bin[i]);
    std::vector<bool> hitMapAndStragglers(bin[i].begin()+beginning, bin[i].end());//first part
    if((beginning+sizeLimit)>63){//continue to next one as well
      i++;
      hitMapAndStragglers.insert(hitMapAndStragglers.end(), bin[i+1].begin(), bin[i+1].end());//add to second part
    
    }

    std::vector<std::vector<bool> > sortedTableFinal = makeLookupTable(sizeLimit);

    std::vector<bool> lookingUp(hitMapAndStragglers.begin(), hitMapAndStragglers.begin()+sizeLimit);

    int toDec = binToDec(lookingUp);
    
    bool twoBitArr[16] = {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0};

    bool hmm = decodeHitMap(hitMapAndStragglers, sortedTableFinal, toDec, beginning, twoBitArr);
  
    if(hmm){//Need to add on the next stream
      hitMapAndStragglers.insert(hitMapAndStragglers.end(), bin[i+1].begin(), bin[i+1].end());
      decodeHitMap(hitMapAndStragglers, sortedTableFinal, toDec, beginning, twoBitArr);
      
    }
    
  }

  return 0;


}

int findHitMapBeginning(std::vector<bool> bin)
{

  //this returns the stream containing the hit map, with the non hit map stuff at the beginning truncated. if the hit map spans two streams, we eliminate the irrelevant beginning of the first hit map, and then the irrelevant beginning of the 2nd hit map, and piece them together. feed this final stream to the hit map decoder
  int qrow=0;
  int ccol=0;
  int chipId = 2*bin[1]+bin[2];

  if(bin[0]){//new event, will have tags
    
    std::vector<bool> maybeTag(bin.begin()+3, bin.begin()+11); //tag
    int mightBeTag = binToDec(maybeTag);

    if(mightBeTag<252){//0-251
    }
    else if(mightBeTag = 252){//252
    }
    else if(mightBeTag = 253){//253
    }
    else if(mightBeTag = 254){//254
    }
    else if(mightBeTag = 255){//255
    }

    //read ccol
    std::vector<bool> ccolVec(bin.begin()+11, bin.begin()+17);
    int ccol = binToDec(ccolVec);

    if(bin[17]){
    }//islast

    int place = 19; 
      
    if(bin[18]){//isneighbor, qrow ommitted
      qrow++;
    }
    else{ //not isneighbor
      std::vector<bool> qrowVec(bin.begin()+18, bin.begin()+26);
      int qrow = binToDec(qrowVec);
      place+=8;
    }
    return place;
  }
  
  else{//previous one continues
    return 3;    
  }
}

//the hit map decoder
bool decodeHitMap(std::vector<bool> bin, std::vector<std::vector<bool> >sortedTableFinal, int toDec, int beginning, bool(&twoBitArr)[16])
{
  bool flag = 0;
  unsigned int nextLevel = 2;
  unsigned int sizeLimit = 8;

  //Lookup Table
  if(sortedTableFinal[toDec].size()!=1){//Lookup table worked--the elements of the lookup table return {0} if it does not correspond to a map
    //if(0){
    std::copy(sortedTableFinal[toDec].begin(), sortedTableFinal[toDec].end(), twoBitArr); //Convert to array
    
    //Exit function
    return 0;
  }

  //Algorithm
  //Count number of binary digits corresponding to each level
  //Number of levels in the binary tree
  unsigned int numLevels = 4;
  //Array of number of bits on each level
  unsigned int levelCounts[numLevels] = {2, 0, 0, 0};
  //Array of number of bits on a level, plus all the bits on the levels before
  unsigned int levelCountsSum[numLevels] = {2, 0, 0, 0};
  //Used to calculate what to put into levelCountsSum
  unsigned int nextLevelSum = 2;
  //Will be the number of bits in the huffman-decoded hit map 
  unsigned int binSize = 2;
  //Will be the number of bits added to get the huffman decoded hit map
  unsigned int numEncoded = 0;

  //Loop through the number of levels, fill with next level before moving on to the next one
  for (unsigned int i = 0; i<numLevels; i++){
    //Dummy to help track the place we're scanning through
    unsigned int dum = 0;
    //Huffman decoding
    for (unsigned int j = 0; j<i; j++){
      //Get the current place in bin before looping through this level
      dum+=levelCounts[j];
    }

    //Loop through this level to Huffman decode it. If 0 occurs in an odd numbered position, it means it must be decoded, since 01 and 00 do not appear in the huffman decoded string.
  
    for(unsigned int j=0; j<levelCounts[i]; j++){
      //Flag means the previous bit was 1
      if(flag){
	flag = 0;
	continue;
      }
      //Flag this one if it is 1
      else if(bin[dum+j]){
	flag=1;
      }
      //Insert 1, 0->01
      else{
	std::vector<bool>::iterator it = bin.begin();
	std::advance(it, dum+j+1);
	bin.insert(it, 1);
	flag=1;
	numEncoded++;
      }
    }
      
    unsigned int nextLevel = 0;
    //Loop through the binary digits on each level to compute how many will be in the next level  
    for (unsigned int j = 0; j<levelCounts[i]; j++){
      //There must be two more bits on the next level, since this one has 1
      if (bin[dum+j]){ 
	nextLevel += 2;
      }	
    }
    //Fill things in
    levelCounts[i+1] = nextLevel;
    binSize+=nextLevel;
    nextLevelSum+=nextLevel;
    levelCountsSum[i+1]=nextLevelSum;
  }
  //The for loop overdoes things, the last level is counted twice
  binSize-=levelCounts[4];
  
  //Get the size before huffman decoding
  unsigned int binSizeEncoded = binSize - numEncoded;

  //If the hit map continues into the next stream--not implemented in encoder yet, so no need for this yet
  /*if((binSizeEncoded+beginning)>63){
    //return twoBitArr;
    return 0;
    }*/
    
  //Find coordinates
  //Loop through the bit pairs on the bottom level
  for(unsigned int i=0; i<levelCounts[numLevels-1]/2; i++){ 
    //Tracks position on hit map
    unsigned int x=0; 
    //Use this to track which pair on the level we're currently analyzing so we can get the one above, we're resetting it here to be the bottom bit pair, but as we move through the next for loop we want to update it to always be the order of the pair that was just put into the coords 
    unsigned int temp=i; 
	
    //Loop through the levels  
    for(unsigned int j=0; j<(numLevels-1); j++){
      //Keeps track of which pixel we're on, decrement every time we fill coords
      unsigned int counter = 0;
      //Tracks which level we're on
      unsigned int levelTracker=numLevels-2-j;
      int binCoord = -1;
      if(j<(numLevels-2)){
	binCoord = levelCountsSum[1-j]-1;
      }
	
      bool flag=0;

      //Move through the level. We're looking for which 1 on the level above generates the pair below it. 
      for (unsigned int k=0; k<levelCounts[numLevels-2-j]; k++){
	binCoord++;
	  
	if(bin[binCoord]==1){
	  //This means it's the right pair that generated the pair we're looking at
	  if(counter==temp){
	    //Update temp to be the pair that generated the pair we're on. Now we look for its predecessors in the next loop
	    temp=floor(k/2);  
	    if(binCoord%2==1){
	      //Get the correct coordinate of the bit pair
	      //First division
	      if(levelTracker==0){
		x=x+4;
	      }
	      //Second division
	      else if(levelTracker==1){
		x=x+2;
	      }
	      //Third division
	      else{
		x=x+1;
	      }
	    }
	    break;
	  }
	  counter++;
	}
      }	
    }
    //Fill the array with the two bits
    twoBitArr[2*x]=bin[binSize-levelCounts[numLevels-1]+2*i]; 
    twoBitArr[2*x+1]=bin[binSize-levelCounts[numLevels-1]+2*i+1];
  }
    
  //Can print out the array to see if it matches the one encoded
  /*for (unsigned int x=0; x<16; x++){
    std::cout<<twoBitArr[x]<<" ";
    }
    std::cout<<" "<<std::endl;*/

  //Exit function
  return 0;
}

//This is the encoder...nothing fancy, very makeshift, very ugly, but it works. 
std::vector <bool> encode (std::vector<bool>twoBitArr0){
  unsigned int rows = 2;
  unsigned int cols = 8;
  bool twoBitArr[rows][cols]={{twoBitArr0[0],twoBitArr0[1],twoBitArr0[2],twoBitArr0[3],twoBitArr0[8],twoBitArr0[9],twoBitArr0[10],twoBitArr0[11]},
			      {twoBitArr0[4],twoBitArr0[5],twoBitArr0[6],twoBitArr0[7],twoBitArr0[12],twoBitArr0[13],twoBitArr0[14],twoBitArr0[15]}};
  srand (time(NULL));
  bool flag = 1;
  //encode array
  //binary encoding
  std::vector<bool> bin;

  if(!flag) {
    bin = {0};
    return bin;
  }
  else {
    std::vector<bool> lvl4;
    std::vector<bool> lvl3; 
    std::vector<bool> lvl2;
    std::vector<bool> lvl1;

    for (unsigned int y=0; y<rows; y++){//left
      for (unsigned int x=0; x<cols/4; x++){
	if (!twoBitArr[y][2*x]&& !twoBitArr[y][2*x+1]){//both sections 0, skip
	  continue;
	}
	else if (!twoBitArr[y][2*x] && !twoBitArr[y][2*x+1]){
	  continue;
	}
	else {
	  lvl4.emplace_back(twoBitArr[y][2*x]);
	  lvl4.emplace_back(twoBitArr[y][2*x+1]);
	}
      }
    }

    for (unsigned int y=0; y<rows; y++){//right
      for (unsigned int x=cols/4; x<cols/2; x++){
	if (!twoBitArr[y][2*x]&& !twoBitArr[y][2*x+1]){//both sections 0, skip
	  continue;
	}
	else if (!twoBitArr[y][2*x] && !twoBitArr[y][2*x+1]){//this one 0, but next one won't be
	  continue;
	}
	else {
	  lvl4.emplace_back(twoBitArr[y][2*x]);
	  lvl4.emplace_back(twoBitArr[y][2*x+1]);
	}
      }
    }

    //let's just manually encode level 2: seems easiest
    //left side
    if((twoBitArr[0][0]||twoBitArr[0][1]||twoBitArr[0][2]||twoBitArr[0][3])
       &&(twoBitArr[1][0]||twoBitArr[1][1]||twoBitArr[1][2]||twoBitArr[1][3])){
      lvl2.emplace_back(1);
      lvl2.emplace_back(1);
    }
    else if((twoBitArr[0][0]||twoBitArr[0][1]||twoBitArr[0][2]||twoBitArr[0][3])
	    &&!(twoBitArr[1][0]||twoBitArr[1][1]||twoBitArr[1][2]||twoBitArr[1][3])){
      lvl2.emplace_back(1);
      lvl2.emplace_back(0);
    }
    else if(!(twoBitArr[0][0]||twoBitArr[0][1]||twoBitArr[0][2]||twoBitArr[0][3])
	    &&(twoBitArr[1][0]||twoBitArr[1][1]||twoBitArr[1][2]||twoBitArr[1][3])){
      lvl2.emplace_back(0);
      lvl2.emplace_back(1);
    }

    //right side
    if((twoBitArr[0][4]||twoBitArr[0][5]||twoBitArr[0][6]||twoBitArr[0][7])
       &&(twoBitArr[1][4]||twoBitArr[1][5]||twoBitArr[1][6]||twoBitArr[1][7])){
      lvl2.emplace_back(1);
      lvl2.emplace_back(1);
    }
    else if((twoBitArr[0][4]||twoBitArr[0][5]||twoBitArr[0][6]||twoBitArr[0][7])
	    &&!(twoBitArr[1][4]||twoBitArr[1][5]||twoBitArr[1][6]||twoBitArr[1][7])){
      lvl2.emplace_back(1);
      lvl2.emplace_back(0);
    }
    else if(!(twoBitArr[0][4]||twoBitArr[0][5]||twoBitArr[0][6]||twoBitArr[0][7])
	    &&(twoBitArr[1][4]||twoBitArr[1][5]||twoBitArr[1][6]||twoBitArr[1][7])){
      lvl2.emplace_back(0);
      lvl2.emplace_back(1);
    }

    //we stupid so we also manually encode level 3, this is probably really slow but the important thing is to get it working first!

    for(unsigned int x=0; x<cols/4; x++){
      for (unsigned int y=0; y<rows; y++){
	if((twoBitArr[y][4*x]||twoBitArr[y][4*x+1])&&(twoBitArr[y][4*x+2]||twoBitArr[y][4*x+3])){
	  lvl3.emplace_back(1);
	  lvl3.emplace_back(1);
	  continue;
	}

	else if((twoBitArr[y][4*x]||twoBitArr[y][4*x+1])&&!(twoBitArr[y][4*x+2]||twoBitArr[y][4*x+3])){
	  lvl3.emplace_back(1);
	  lvl3.emplace_back(0);
	  continue;
	}

	else if(!(twoBitArr[y][4*x]||twoBitArr[y][4*x+1])&&(twoBitArr[y][4*x+2]||twoBitArr[y][4*x+3])){
	  lvl3.emplace_back(0);
	  lvl3.emplace_back(1);
	  continue;
	}
      }
    }
  

    bool flag1=0; //lvl1
    for (unsigned int y=0; y<rows; y++){
      for (unsigned int x=0; x<cols/2; x++){
	if(twoBitArr[y][x]){
	  flag1=1;
	  lvl1.emplace_back(1);
	  break;
	}
      }
      if(flag1){
	break;
      }
    }

    if(!flag1){
      lvl1.emplace_back(0);
    }

    flag1=0;
    for (unsigned int y=0; y<rows; y++){
      for (unsigned int x=cols/2; x<cols; x++){
	if(twoBitArr[y][x]){
	  flag1=1;
	  lvl1.emplace_back(1);
	  break;
	}
      }
      if(flag1){
	break;
      }
    }
    if(!flag1){
      lvl1.emplace_back(0);
    }
  
    //concatenate all levels tgt
    bin=lvl1;
    bin.reserve(lvl1.size()+lvl2.size()+lvl3.size()+lvl4.size());
    bin.insert(bin.end(), lvl2.begin(), lvl2.end());
    bin.insert(bin.end(), lvl3.begin(), lvl3.end());
    bin.insert(bin.end(), lvl4.begin(), lvl4.end());

    std::vector<bool> bin2;
    std::vector<unsigned int>indices;
    bool flag2=0;
  
    //huffman encoding
    for(unsigned int i=0; i<bin.size()/2; i++){

      if ((!bin[2*i])&&bin[2*i+1]){
	bin2.emplace_back(0);
      }
      else{
	bin2.emplace_back(bin[2*i]);
	bin2.emplace_back(bin[2*i+1]);
      }
    }

    //You can print the encoded binary strings, both with and without huffman encoding
    /*for(std::vector<bool>::const_iterator i = bin.begin(); i!=bin.end(); ++i){ 
      std::cout<<*i;
    }
    std::cout<<" "<<std::endl;*/

    /*for(std::vector<bool>::const_iterator i = bin2.begin(); i!=bin2.end(); ++i){ 
      std::cout<<*i;
    }
    std::cout<<" "<<std::endl;*/

    return bin2;
  }
}

//Adds random 4 bit ToT values to an encoded hit map
std::vector<bool> addRandomToT(std::vector<bool>bin, int counter) {
  //generates random ToT value, each hit has its own 4 bit ToT value, but not really random--each loop has the same beginnings...whatever
  int ToTPermNum = std::pow(2.0, 4.0*counter);//num possible ToT permutations
  int tempToT = rand() % ToTPermNum; //choose random ToT dec value
  for(unsigned int k=0; k<(4*counter); k++){
    bin.emplace_back(tempToT%2);
    //std::cout<<tempToT%2;
    tempToT=tempToT/2;
  }
  //std::cout<<"\n";
  return bin;
}

int binToDec (std::vector<bool> bin){
  //convert to int
  unsigned int toDec = 0;
  for (int i=0; i<bin.size(); i++){
    if(bin[i]){
      toDec+=pow(2.0, bin.size()-1-i);
    }
  } 
  return toDec;
}

std::vector<bool> decToBin (int dec, int binSize){
  

}

//Makes a lookup table
std::vector<std::vector<bool> > makeLookupTable(int sizeLimit){
  
  //make lookup table
  int permNum = std::pow(2.0, 16.0);

  std::vector<std::vector<bool> > perms;
  std::vector<std::vector<bool> > permsEncoded;
  perms.reserve(permNum);
  permsEncoded.reserve(permNum);

  //int sizeLimit = 8;

  //for file storage purposes
  std::ofstream table;
  table.open("table.txt");
  table<<"int \t binary \n";

  //this will be a preliminary table
  std::vector<std::string > sortedTable;
  sortedTable.reserve(3);

  //declare map for later convenience in sorting
  std::map<std::string, std::vector<bool> > sortingAid;//matches sorted binaries to their int
  std::vector<std::vector<std::string> > helper;//row: length of binary string, sorted in order of largeness. col: sorted "alphabetically"
  helper.reserve(sizeLimit+1);

  for(unsigned int i=0; i<permNum; i++){//make map of all possible hit maps with their binary codes
    int flag =0;
    int temp=i;
    std::vector<bool>perm;
    for(unsigned int j=0; j<16; j++){
      perms[i].push_back(temp%2);
      //perm.push_back(temp%2);
      flag+=(temp%2);//limits to 3 hits or less
      temp=temp/2;
    }
    //perms[i]=perm;
    
    if(encode(perms[i]).size()<(sizeLimit+1) && encode(perms[i]).size()>2){//we only want binary strings that are 9 or less, and 3 events or less, and omit the impossible 0 one
      //std::cout<<encode(perms[i]).size();
      permsEncoded[i]=encode(perms[i]);      

      //make into strings
      std::ostringstream oss;
      std::copy(permsEncoded[i].begin(), permsEncoded[i].end(), std::ostream_iterator<int>(oss));
      int size = oss.str().size();
      std::string str = oss.str();
      //std::cout<<str<<std::endl;

      //add ToT values
      int ToTPermNum = std::pow(2.0, sizeLimit-permsEncoded[i].size());//num possible ToT permutations
      for(unsigned int j=0; j<ToTPermNum; j++){//make map of all possible hit maps with their binary codes
	int tempToT=j;
	std::string strToT = str;
	for(unsigned int k=0; k<(sizeLimit-permsEncoded[i].size()); k++){
	  strToT=strToT+std::to_string(tempToT%2);
	  tempToT=tempToT/2;
	}
	helper[size].push_back(strToT);
	sortingAid[strToT] = perms[i];
      }      
    }
  }

  //sort table
  std::vector<std::string> help; //used to concatenate everything together
  for (int i=0; i<(sizeLimit+1); i++){
    std::sort(helper[i].begin(), helper[i].end());
    help.insert(help.end(), helper[i].begin(), helper[i].end());
  }
  sortedTable=help;
  
  //give int to encoded binary+ToT value
  std::vector<std::vector<bool> > sortedTableFinal(256);//maybe array?
  std::vector<bool> zero {0};
  fill(sortedTableFinal.begin(), sortedTableFinal.end(), zero);
  
  for (int i=0; i<sortedTable.size(); i++){
    int toDec = 0;
    for(int j=0; j<sizeLimit; j++){
      int dig = static_cast<int>(sortedTable[i].at(j));
      dig-=48;
      if(dig==1){
	toDec+=pow(2.0, sizeLimit-1-j);
      }
    }
    sortedTableFinal[toDec]=sortingAid[sortedTable[i] ];
  }

  //put into table file
  int num = pow(2.0, sizeLimit);
  for(int i=0; i<num; i++){
    table<<i<<"\t";
    //std::cout<<i<<"\t";

    for(std::vector<bool>::const_iterator j = sortedTableFinal[i].begin(); j!=sortedTableFinal[i].end(); ++j){ 
      table<<*j;
      //std::cout<<*j;
    }
    table<<"\n";

    //std::cout<<" "<<std::endl;
  }  

  table.close();

  return sortedTableFinal;
}




std::vector<bool> randomMapGen(){
  //fill array with random hits
  unsigned int rows = 2;
  unsigned int cols = 8;
  std::vector<bool> twoBitArr={0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0};
  srand (time(NULL));
  bool flag = 0;
  for (unsigned int x=0; x<16; x++){
    int random = rand()%10;
    if(random<5){
      twoBitArr[x] = 1;
      flag = 1;
    }
    //std::cout<<twoBitArr[x];
  }
  //std::cout<<" "<<std::endl;
  return twoBitArr;
}


//The following commented code was intended to be a prettier version of the current encoder, but it didn't work and I abandoned the debugging effort to work on other things...maybe you can give it a home if you want. 

/*std::vector <bool> encode (std::vector<bool>twoBitArr){

  //srand (time(NULL));
  bool flag = 0;
  if(twoBitArr[0]||twoBitArr[1]||twoBitArr[2]||twoBitArr[3]||twoBitArr[4]||twoBitArr[5]||twoBitArr[6]||twoBitArr[7]||twoBitArr[8]||twoBitArr[9]||twoBitArr[10]||twoBitArr[11]||twoBitArr[12]||twoBitArr[13]||twoBitArr[14]||twoBitArr[15]){
    flag = 1;
    std::cout<<flag<<std::endl;
  }

  //encode array
  std::vector<bool> bin;

  if(!flag) {
    bin = {0};
    return bin;
  }
  else {
    std::vector<bool> lvl4;
    std::vector<bool> lvl3; 
    std::vector<bool> lvl2;
    std::vector<bool> lvl1;

    //lvl1
    bool flag1 = 0;
    for(int i=0; i<8; i++){
      if(twoBitArr[i]){
	flag1 = 1;
	break;
      }
    }
    bool flag2 = 0;
    for(int i=8; i<16; i++){
      if(twoBitArr[i]){
	flag2 = 1;
	break;
      }
    }
    lvl1.emplace_back(flag1);
    lvl1.emplace_back(flag2);

    //lvl2
    bool flag3=0;
    bool flag4=0;
    if(flag1){
      for(int i=0; i<4; i++){
	if(twoBitArr[i]){
	  flag3 = 1;
	}
      }
      for(int i=4; i<8; i++){
	if(twoBitArr[i]){
	  flag4 = 1;
	}
      }
      lvl2.emplace_back(flag3);
      lvl2.emplace_back(flag4);
    }

    bool flag5=0;
    bool flag6=0;
    if(flag1){
      for(int i=8; i<12; i++){
	if(twoBitArr[i]){
	  flag5 = 1;
	}
      }
      for(int i=12; i<16; i++){
	if(twoBitArr[i]){
	  flag6 = 1;
	}
      }
      lvl2.emplace_back(flag5);
      lvl2.emplace_back(flag6);
    }

    //lvl3
    bool flag7=0;
    bool flag8=0;
    bool flag9=0;
    bool flag10=0;
    if(flag3){
      if(twoBitArr[0]||twoBitArr[1]){
	flag7=1;
      }
      if(twoBitArr[2]||twoBitArr[3]){
	flag8=1;
      }
      lvl3.emplace_back(flag7);
      lvl3.emplace_back(flag8);
    }

    if(flag4){
      if(twoBitArr[4]||twoBitArr[5]){
	flag9=1;
      }
      if(twoBitArr[6]||twoBitArr[7]){
	flag10=1;
      }
      lvl3.emplace_back(flag9);
      lvl3.emplace_back(flag10);
    }

    bool flag11=0;
    bool flag12=0;
    bool flag13=0;
    bool flag14=0;

    if(flag5){
      if(twoBitArr[8]||twoBitArr[9]){
	flag11=1;
      }
      if(twoBitArr[10]||twoBitArr[11]){
	flag12=1;
      }
      lvl3.emplace_back(flag11);
      lvl3.emplace_back(flag12);
    }

    if(flag6){
      if(twoBitArr[12]||twoBitArr[13]){
	flag13=1;
      }
      if(twoBitArr[14]||twoBitArr[15]){
	flag14=1;
      }
      lvl3.emplace_back(flag13);
      lvl3.emplace_back(flag14);
    }

    //lvl4

    if(flag7){
      lvl4.emplace_back(twoBitArr[0]);
      lvl4.emplace_back(twoBitArr[1]);
    }
    if(flag8){
      lvl4.emplace_back(twoBitArr[2]);
      lvl4.emplace_back(twoBitArr[3]);
    }    
    if(flag9){
      lvl4.emplace_back(twoBitArr[4]);
      lvl4.emplace_back(twoBitArr[5]);
    }    
    if(flag10){
      lvl4.emplace_back(twoBitArr[6]);
      lvl4.emplace_back(twoBitArr[7]);
    }
    if(flag11){
      lvl4.emplace_back(twoBitArr[8]);
      lvl4.emplace_back(twoBitArr[9]);
    }
    if(flag12){
      lvl4.emplace_back(twoBitArr[10]);
      lvl4.emplace_back(twoBitArr[11]);
    }    
    if(flag13){
      lvl4.emplace_back(twoBitArr[12]);
      lvl4.emplace_back(twoBitArr[13]);
    }    
    if(flag14){
      lvl4.emplace_back(twoBitArr[14]);
      lvl4.emplace_back(twoBitArr[15]);
    }
    
    //concatenate all levels tgt
    bin=lvl1;
    bin.reserve(lvl1.size()+lvl2.size()+lvl3.size()+lvl4.size());
    bin.insert(bin.end(), lvl2.begin(), lvl2.end());
    bin.insert(bin.end(), lvl3.begin(), lvl3.end());
    bin.insert(bin.end(), lvl4.begin(), lvl4.end());

    std::vector<bool> bin2;
  
    //huffman encoding
    for(unsigned int i=0; i<bin.size()/2; i++){
      if ((!bin[2*i])&&bin[2*i+1]){
	bin2.emplace_back(0);
      }
      else{
	bin2.emplace_back(bin[2*i]);
	bin2.emplace_back(bin[2*i+1]);
      }
    }

    //ToT (this only adds 4 digits, not huffman encoded--ask if this is accurate later). totally randomly generated
    /* int ToTPermNum = std::pow(2.0, 4.0);//num possible ToT permutations
    int tempToT = rand() % ToTPermNum; //choose random ToT dec value
    for(unsigned int k=0; k<4; k++){
      bin2.emplace_back(tempToT%2);
      tempToT=tempToT/2;
      }*/
    

    /*for(std::vector<bool>::const_iterator i = bin.begin(); i!=bin.end(); ++i){ 
      std::cout<<*i;
    }

    std::cout<<" "<<std::endl;*/

    /*for(std::vector<bool>::const_iterator i = bin2.begin(); i!=bin2.end(); ++i){ 
      std::cout<<*i;
    }

    std::cout<<" "<<std::endl;*/

/*    return bin2;
  }
  }*/
