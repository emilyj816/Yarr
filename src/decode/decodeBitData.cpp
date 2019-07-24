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

std::map<std::vector<bool>, std::vector<bool> > bin5;
std::map<std::vector<bool>, std::vector<bool> > bin6;
std::map<std::vector<bool>, std::vector<bool> > bin7;
std::map<std::vector<bool>, std::vector<bool> > bin8;
std::map<std::vector<bool>, std::vector<bool> > bin9;
std::map<std::vector<bool>, std::vector<bool> > bin10;
std::map<std::vector<bool>, std::vector<bool> > bin11;
std::map<std::vector<bool>, std::vector<bool> > bin12;
std::map<std::vector<bool>, std::vector<bool> > bin13;
std::map<std::vector<bool>, std::vector<bool> > bin14;
std::map<std::vector<bool>, std::vector<bool> > bin15;
std::map<std::vector<bool>, std::vector<bool> > bin16;
std::map<std::vector<bool>, std::vector<bool> > bin17;
std::map<std::vector<bool>, std::vector<bool> > bin18;
std::map<std::vector<bool>, std::vector<bool> > bin19;
std::map<std::vector<bool>, std::vector<bool> > bin20;
std::map<std::vector<bool>, std::vector<bool> > bin21;
std::map<std::vector<bool>, std::vector<bool> > bin22;


std::map<unsigned int, std::map<std::vector<bool>, std::vector<bool> > > bins = { {5, bin5},{6, bin6},{7, bin7},{8,bin8},{9,bin9},{10,bin10},{11,bin11},{12,bin12},{13,bin13},{14,bin14},{16,bin16},{17,bin17},{18,bin18},{19,bin19},{20,bin20},{21,bin21},{22,bin22}};//this is a map of all the binary strings, sorted by length

std::map<std::vector<bool>, std::vector<unsigned int> > begin4;//key: the 4 digit long beginning, value: a vector containing the lengths of binary codes that have this
std::map<std::vector<bool>, std::vector<unsigned int> > begin5;
std::map<std::vector<bool>, std::vector<unsigned int> > begin6;
std::map<std::vector<bool>, std::vector<unsigned int> > begin7;
std::map<std::vector<bool>, std::vector<unsigned int> > begin8;
std::map<std::vector<bool>, std::vector<unsigned int> > begin9;
std::map<std::vector<bool>, std::vector<unsigned int> > begin10;
std::map<std::vector<bool>, std::vector<unsigned int> > begin11;
std::map<std::vector<bool>, std::vector<unsigned int> > begin12;
std::map<std::vector<bool>, std::vector<unsigned int> > begin13;
std::map<std::vector<bool>, std::vector<unsigned int> > begin14;
std::map<std::vector<bool>, std::vector<unsigned int> > begin15;
std::map<std::vector<bool>, std::vector<unsigned int> > begin16;
std::map<std::vector<bool>, std::vector<unsigned int> > begin17;
std::map<std::vector<bool>, std::vector<unsigned int> > begin18;
std::map<std::vector<bool>, std::vector<unsigned int> > begin19;
std::map<std::vector<bool>, std::vector<unsigned int> > begin20;

std::map<unsigned int, std::map<std::vector<bool>, std::vector<unsigned int> > > begins={{5, begin5}, {6, begin6}, {7, begin7}, {8, begin8}, {9, begin9}, {10, begin10},{11, begin11},{12, begin12},{13, begin13},{14, begin14},{15, begin15},{16, begin16},{17, begin17},{18, begin18},{19, begin18},{20, begin20}};

std::map<std::vector<bool>, std::vector<std::vector<bool> > > end4; //maps vector of binaries(even) and vector of decoded map(odd) to its 4 digit beginning

bool flag=0;

bool decode(std::vector<bool> bin)
{
  unsigned int rows = 2;
  unsigned int cols = 8;
  bool twoBitArr[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  bool flag = 0;
  unsigned int nextLevel = 2;
  std::vector<bool> bin4 = {0,0,0,0};
  if(bin==bin4){
    return twoBitArr;
  }
  else{
    std::vector<bool>binBegin(bin.begin(),bin.begin()+5);
    if(bins[5].find(binBegin)!=bins[5].end()){//found matching 5 digit long one, nothing to do now but return
      //std::cout<<5<<std::endl;
      std::copy(binBegin.begin(), binBegin.end(), twoBitArr); //convert to array
      return twoBitArr;
      }

    else {//second level
      if(begins[5].find(binBegin)!=begins[5].end()){//look for beginning like the first 5 digits of our binary string
	unsigned int next = *std::min_element(begins[5][binBegin].begin(), begins[5][binBegin].end());//size of next to be tried element
	for(int i=5; i<next; i++){//update binBegin to be new length
	  binBegin.push_back(bin[i]);
	}
	if(bins[next].find(binBegin)!=bins[next].end()){//look for the new beginning as a full bin
	  //std::cout<<"next"<<std::endl;
	  std::copy(binBegin.begin(), binBegin.end(), twoBitArr); //convert to array
	  return twoBitArr;
	}
	/*else{//third level, stop here?
	  if(begins[next].find(binBegin)!=begins[5].end()){
	  }
	  }*/
      }
      }
    
    

    /*std::vector<bool>beginning(bin.begin(),bin.begin()+4);
    
    if(end4.find(beginning)!=end4.end()){//find the corresponding beginning 4 digits
      
      for(unsigned int i=0; i<end4[beginning].size()/2; i++){//find the map
	std::vector<bool>temp(bin.begin()+4, bin.begin()+end4[beginning][2*i].size()+4);//we compare only the non beginning part to save space

	if(temp==end4[beginning][2*i]){//does it belong in this?

	  std::copy(end4[beginning][2*i+1].begin(), end4[beginning][2*i+1].end(), twoBitArr); //convert to array
	  return twoBitArr;

	}
      }
    }*/
    
    //count number of binary digits corresponding to each level
	    
    unsigned int numLevels = 4;
    unsigned int levelCounts[numLevels] = {2, 0, 0, 0};
    unsigned int levelCountsSum[numLevels] = {2, 0, 0, 0};
    unsigned int nextLevelSum = 2;
    unsigned int binSize = 2;
    unsigned int binSizeEncoded = 2; 

    for (unsigned int i = 0; i<numLevels; i++){
      //loop through levelCounts, fill with next level before going to next one
      unsigned int dum = 0;

      for (unsigned int j = 0; j<i; j++){//just get the place of current place in bin before looping through this level
	dum+=levelCounts[j];
      }

      for(unsigned int j=0; j<levelCounts[i]; j++){//huffman decode the coming level
	if(flag){//previously 1, this one 
	  flag = 0;
	  continue;
	}
	else if(bin[dum+j]){//this one is 1, flag it
	  flag=1;
	}
	else{//need to insert 1
	  std::vector<bool>::iterator it = bin.begin();
	  std::advance(it, dum+j+1);
	  bin.insert(it, 1);
	  flag=1;
	  binSizeEncoded--;
	}
      }
      
      unsigned int nextLevel = 0;
      
      for (unsigned int j = 0; j<levelCounts[i]; j++){
	//loop through the individual binary digits in each level to compute how many will be in the next level

	if (bin[dum+j]){ //every true on one level will spawn 2 more spots on the next level	
	  nextLevel += 2;
	}	
      }
      levelCounts[i+1] = nextLevel;
      binSize+=nextLevel;
      binSizeEncoded+=nextLevel;
      nextLevelSum+=nextLevel;
      levelCountsSum[i+1]=nextLevelSum;
    }
    
    //let's try the lookup by binSize here
    /*if(binSize<16){
      
      }*/
    
    //get coordinates of the two bits    
    for(unsigned int i=0; i<levelCounts[numLevels-1]/2; i++){ //loop through 2bits
      unsigned int x=0; //this records the position on the map
      unsigned int temp=i; //use this to track which pair on the level we're currently analyzing so we can get the one above, we're resetting it here to be the bottom bit pair, but as we move through the next for loop we want to update it to always be the order of the pair that was just put into the coords 	
      
      for(unsigned int j=0; j<(numLevels-1); j++){//loop through levels
	//keeps track of which pixel we're on, decrement every time we fill coords
	unsigned int counter=0; 
	unsigned int levelTracker=numLevels-2-j;
	int binCoord = -1;
	if(j<(numLevels-2)){
	  binCoord = levelCountsSum[1-j]-1;
	}
	
	bool flag=0;

	for (unsigned int k=0; k<levelCounts[numLevels-2-j]; k++){
	  binCoord++;
	  
	  if(bin[binCoord]==1){
	    if(counter==temp){//this means it's the pair we're looking for!
	      temp=floor(k/2);  //we update temp to mean the pair we just found
	      if(binCoord%2==1){
		if(levelTracker==0){
		  x=x+4;
		}
		else if(levelTracker==1){
		  x=x+2;
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
      twoBitArr[2*x]=bin[bin.size()-levelCounts[numLevels-1]+2*i]; //fill with twoBits
      twoBitArr[2*x+1]=bin[bin.size()-levelCounts[numLevels-1]+2*i+1];
    }
    
    /*for (unsigned int x=0; x<16; x++){
      std::cout<<twoBitArr[x]<<" ";
    }
    std::cout<<" "<<std::endl;*/
  }
  return twoBitArr;
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
    std::cout<<twoBitArr[x];
  }
  std::cout<<" "<<std::endl;
  return twoBitArr;
}

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



int main () {
  //make lookup table
  int permNum = std::pow(2.0, 16.0);
  std::vector<std::vector<bool> > perms;
  std::vector<std::vector<bool> > permsEncoded;
  perms.reserve(permNum);
  permsEncoded.reserve(permNum);
  std::ofstream table;
  table.open("table.txt");
  table<<"int \t binary \n";

  for(unsigned int i=0; i<permNum; i++){//make map of all possible hit maps with their binary codes
    //int flag =0;
    int temp=i;
    table<<i<<"\t";
    for(unsigned int j=0; j<16; j++){

      perms[i].push_back(temp%2);
      flag+=(temp%2);
      temp=temp/2;
    }
    if(flag<4){
      permsEncoded[i]=encode(perms[i]);
      table<<i<<"\t";
      for(std::vector<bool>::const_iterator j = permsEncoded[i].begin(); j!=permsEncoded[i].end(); ++j){ 
	table<<*j;
      }
      table<<"\n";
    }
  }

  table.close();


  std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

  srand (time(NULL));
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> dist6(0,1);
  
  for(unsigned int i=0; i<10000; i++){
    std::vector<bool> rando={0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0};
    flag = 0;
    for (unsigned int x=0; x<16; x++){
      bool help = dist6(rng);
      if(help){
	flag=1;
	rando[x] = 1;
      }
      //std::cout<<rando[x]<<" ";
    }

    //std::cout<<" "<<std::endl;
    std::vector<bool>bin = encode(rando);
    bool twoBitArr = decode(bin);
  }

  std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds> (t2 - t1).count();
  std::cout<<duration<<std::endl;

  return 0;

}
    /*bins[permsEncoded[i].size()][permsEncoded[i] ]=perms[i];//map of maps of hit maps, hit map key is encoded hit map, encoded hit map key is length of encoded hit map

    if(permsEncoded[i].size()<15&&permsEncoded[i].size()>4){//we only want to record in our lookup table binary strings that are this small, maybe remove, not sure if encoding by string length is a good idea

      //do the multiple sized beginnings lookup table
      for(int beginSize=5; beginSize<permsEncoded[i].size(); beginSize++){
	std::vector<bool>beginning (permsEncoded[i].begin(), permsEncoded[i].begin()+beginSize);
	begins[beginSize][beginning].push_back(permsEncoded[i].size());
      }

      //do beginning length 4 encodings, actually this is really slow...
      //std::cout<<permsEncoded[i].size()<<std::endl;
      std::vector<bool>permEncoded (permsEncoded[i].begin(), permsEncoded[i].end());
      std::vector<bool>begin(permsEncoded[i].begin(),permsEncoded[i].begin()+4);//beginning
      
      end4[begin].push_back(permEncoded);//end of encoded permutation
      end4[begin].push_back(perms[i]);//decoded hit map

      
    }





  
    /*for(int j=5; j<8; j++){//loop through length of binary string that we're going to find length of beginnings for
      if(permsEncoded[i].size()==j){
	bins[j][permsEncoded[i] ]=perms[i];
	for(unsigned int k=4; k<j; k++){//k: length of beginnings
	  std::vector<bool>temp(permsEncoded[i].begin(),permsEncoded[i].begin()+k);
	  //fill all the beginnings of different lengths
	  if(std::find(begins[k][temp].begin(), begins[k][temp].end(),k)==begins[k][temp].end()){//if the beginning is not already there
	    begins[k][temp].push_back(j);
	    std::cout<<j<<": ";
	    std::cout<<temp.size()<<std::endl;
	    std::cout<<"k: "<<k<<std::endl;
	    for(std::vector<bool>::const_iterator l = temp.begin(); l!=temp.end(); ++l){ 
	      std::cout<<*l;
	    }//l
	    std::cout<<" "<<std::endl;
	  }//if
	}//k
      }//if
    }//j*/
