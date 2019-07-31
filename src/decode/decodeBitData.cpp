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

//bool flag=0;
//int counter =0;

bool decode(std::vector<bool> bin, std::vector<std::vector<bool> >sortedTableFinal, int toDec)
{

  bool twoBitArr[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  bool flag = 0;
  unsigned int nextLevel = 2;
  unsigned int sizeLimit = 8;
  
  //look it up
  if(sortedTableFinal[toDec].size()!=1){
  //if(0){
    std::copy(sortedTableFinal[toDec].begin(), sortedTableFinal[toDec].end(), twoBitArr); //convert to array

    return twoBitArr;
    }
  
  //count number of binary digits corresponding to each level	    
  unsigned int numLevels = 4;
  unsigned int levelCounts[numLevels] = {2, 0, 0, 0};
  unsigned int levelCountsSum[numLevels] = {2, 0, 0, 0};//has number in the level, plus all the levels below
  unsigned int nextLevelSum = 2;
  unsigned int binSize = 2;

  for (unsigned int i = 0; i<numLevels; i++){
    //loop through levelCounts, fill with next level before going to next one
    unsigned int dum = 0;

    for (unsigned int j = 0; j<i; j++){
      //just get the place of current place in bin before looping through this level
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
    nextLevelSum+=nextLevel;
    levelCountsSum[i+1]=nextLevelSum;
  }

  binSize-=levelCounts[4];
    
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
	binCoord++;//go through the level
	  
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
    //fill with twoBits
    twoBitArr[2*x]=bin[binSize-levelCounts[numLevels-1]+2*i]; 
    twoBitArr[2*x+1]=bin[binSize-levelCounts[numLevels-1]+2*i+1];
  }
    
  /*for (unsigned int x=0; x<16; x++){
    std::cout<<twoBitArr[x]<<" ";
    }
    std::cout<<" "<<std::endl;*/

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

std::vector<bool> addRandomToT(std::vector<bool>bin, int sizeLimit) {
  //ToT (this only adds 4 digits, not huffman encoded--ask if this is accurate later). totally randomly generated, hmmm where should i put the ToT function?
  int ToTPermNum = std::pow(2.0, sizeLimit-4);//num possible ToT permutations
  int tempToT = rand() % ToTPermNum; //choose random ToT dec value
  for(unsigned int k=0; k<4; k++){
    bin.emplace_back(tempToT%2);
    tempToT=tempToT/2;
  }
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



int main () {
  //make lookup table
  int permNum = std::pow(2.0, 16.0);

  std::vector<std::vector<bool> > perms;
  std::vector<std::vector<bool> > permsEncoded;
  perms.reserve(permNum);
  permsEncoded.reserve(permNum);

  int sizeLimit = 8;

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
  std::vector<std::vector <bool> > sortedTableFinal(256);//maybe array?
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



  std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

  std::random_device dev;
  std::mt19937 rng(dev());
  int oneHundredPercent = 100;
  std::uniform_int_distribution<std::mt19937::result_type> dist6(0, oneHundredPercent-1);
  srand (time(0));

  //generate clumps
  double hitProb = 0.1;
  double twoClumpProb = 0;

  for(unsigned int i=0; i<500000; i++){
    std::vector<bool> rando = {0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0};
    int flag = 0;
    for (unsigned int x=0; x<16; x++){
      if(dist6(rng)<oneHundredPercent*hitProb){//probability of a hit, now we see what kind of cluster it is
	flag = 1;
	rando[x] = 1;
	if(dist6(rng)<twoClumpProb*oneHundredPercent){//two hits
	  if(x==3){
	    rando[8] = 1;
	  }
	  else if(x==7){
	    rando[12] = 1;

	  }
	  else{ 
	    if(x!=11 && x!=15){
	      rando[x+1] = 1;
	    }
	  }
	}
      }
      //std::cout<<rando[x]<<" ";
    }
    //std::cout<<"\n";

    if(!flag){
      i--; //can't have empty one
      continue;
    }

    std::vector<bool>bin = encode(rando);
    std::vector<bool> lookingUp(bin.begin(), bin.begin()+sizeLimit);

    //convert to int
    int toDec = binToDec(lookingUp);    
    bool twoBitArr = decode(bin, sortedTableFinal, toDec);
    
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


/*    std::vector<bool>binBegin(bin.begin(),bin.begin()+5);
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
	  }*/
	/*else{//third level, stop here?
	  if(begins[next].find(binBegin)!=begins[5].end()){
	  }
	  }*/
/*      }
	}*/
    
    

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
