#include <iostream>
#include <vector>
#include <array>
#include <math.h>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <chrono>

bool decode(std::vector<bool> bin)
{
  unsigned int rows = 2;
  unsigned int cols = 8;
  bool twoBitArr[rows][cols]={{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}};
  bool flag = 0;
  unsigned int nextLevel = 2;
  
  if(bin.size()>1){

    //expand from Huffman coding back from 0->01
    for (unsigned int i=0; i<bin.size(); i++){
      if(flag){
	flag = 0;
	continue;
      }
      else if(bin[i]){

	flag=1;
      }
      else{
	std::vector<bool>::iterator it = bin.begin();
	std::advance(it, i+1);
	bin.insert(it, 1+1);
	flag=1;
      }
    }

    //count number of binary digits corresponding to each level
    unsigned int numLevels = 4+1;
    int levelCounts[numLevels] = {0, 2, 0, 0, 0};
    unsigned int nextLevel = levelCounts[0];

    for (unsigned int i = 1; i<numLevels; i++){
      //loop through levelCounts, fill with next level before going to next one
      unsigned int dum = 0;

      for (unsigned int k = 1; k<i; k++){//just get the place of current place in bin before looping through this level
	dum+=levelCounts[k];
      }

      nextLevel = 0;

      for (unsigned int j = 0; j<levelCounts[i]; j++){
	//loop through the individual binary digits in each level to compute how many will be in the next level

	if (bin[dum+j]){ //every true on one level will spawn 2 more spots on the next level
	
	  nextLevel += 2;
	}
      }
      levelCounts[i+1] = nextLevel;
    }

    bool twoBits[levelCounts[numLevels-1]/2][2];

    //get coordinates of the two bits    
    for(unsigned int i=0; i<levelCounts[numLevels-1]/2; i++){ //loop through 2bits
      //get the twoBits on the bottom level, in order
      twoBits[levelCounts[numLevels-1]/2-i-1][0]=bin[bin.size()-2*i-2];
      twoBits[levelCounts[numLevels-1]/2-i-1][1]=bin[bin.size()-2*i-1];
      unsigned int x=0;
      unsigned int y=0;
      unsigned int temp=i; //use this to track which pair on the level we're currently analyzing so we can get the one above, we're resetting it here to be the bottom bit pair, but as we move through the next for loop we want to update it to always be the order of the pair that was just put into the coords 	
      
      for(unsigned int j=0; j<(numLevels-2); j++){//loop through levels
	//keeps track of which pixel we're on, decrement every time we fill coords
	unsigned int counter=0; 
	unsigned int levelTracker=numLevels-2-1-j;
	unsigned int binCoord=bin.size()-levelCounts[numLevels-1]-levelCounts[numLevels-2]; 
	unsigned int binCoordBase=binCoord;
	bool flag=0;

	for (unsigned int k=0; k<j; k++){
	  binCoordBase = binCoordBase-levelCounts[numLevels-3-k];
	}

	for (unsigned int k=0; k<levelCounts[numLevels-2-j]; k++){
	  binCoord = binCoordBase+k;
	  
	  if(bin[binCoord]==1){
	    if(counter==temp){//this means it's the pair we're looking for!
	      temp=floor(k/2);  //we update temp to mean the pair we just found
	      if(binCoord%2==1){
		if(levelTracker==0){
		  x=x+2;
		}
		else if(levelTracker==1){
		  y=y+1;
		}
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
      twoBitArr[y][2*x]=bin[bin.size()-levelCounts[numLevels-1]+2*i]; //fill with twoBits
      twoBitArr[y][2*x+1]=bin[bin.size()-levelCounts[numLevels-1]+2*i+1];
    }
    
    for (unsigned int y=0; y<rows; y++){
      for (unsigned int x=0; x<cols; x++){
	std::cout<<twoBitArr[y][x]<<" ";
      }
      std::cout<<" "<<std::endl;
    }
  }
  return twoBitArr;
}

std::vector <bool> encode (){
  //fill array with random hits
  unsigned int rows = 2;
  unsigned int cols = 8;
  bool twoBitArr[rows][cols]={{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}};
  srand (time(NULL));
  bool flag = 0;
  for (unsigned int y=0; y<rows; y++){
    for (unsigned int x=0; x<cols; x++){
      int random = rand()%10;
      if(random<5){
	twoBitArr[y][x] = 1;
	flag = 1;
      }
      std::cout<<twoBitArr[y][x]<<" ";
    }
    std::cout<<" "<<std::endl;
  }

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

    
    //alternatively: let's start from the bottom level, and work our way up
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

    for(std::vector<bool>::const_iterator i = bin.begin(); i!=bin.end(); ++i){ 
      std::cout<<*i;
    }

    std::cout<<" "<<std::endl;

    for(std::vector<bool>::const_iterator i = bin2.begin(); i!=bin2.end(); ++i){ 
      std::cout<<*i;
    }

    std::cout<<" "<<std::endl;


    //huffman encoding

    return bin2;
  }
}



int main () {
  std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
  
  //for(unsigned int i=0; i<10000; i++){
    std::vector<bool>bin = encode();
    bool twoBitArr = decode(bin);
    //}

  std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

  auto duration = std::chrono::duration_cast<std::chrono::microseconds> (t2 - t1).count();
  std::cout<<duration<<std::endl;
  return 0;

}
