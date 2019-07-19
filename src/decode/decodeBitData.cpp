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
std::map<std::vector<bool>, std::vector<bool> > bin23;
std::map<std::vector<bool>, std::vector<bool> > bin24;
std::map<std::vector<bool>, std::vector<bool> > bin25;
std::map<std::vector<bool>, std::vector<bool> > bin26;
std::map<std::vector<bool>, std::vector<bool> > bin27;
std::map<std::vector<bool>, std::vector<bool> > bin28;
std::map<std::vector<bool>, std::vector<bool> > bin29;
std::map<std::vector<bool>, std::vector<bool> > bin30;

bool flag=0;

bool decode(std::vector<bool> bin)
{
  unsigned int rows = 2;
  unsigned int cols = 8;
  bool twoBitArr[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  bool flag = 0;
  unsigned int nextLevel = 2;
  std::vector<bool> bin4 = {0,0,0,0};
  if(std::equal(bin.begin(), bin.begin()+3, bin4.begin())){
    twoBitArr[15]=1;
    return twoBitArr;
  }
  else{
    //search binary length 5
    std::map<std::vector<bool>, std::vector<bool> >::iterator it5;
    std::map<std::vector<bool>, std::vector<bool> >::iterator it6;
    std::map<std::vector<bool>, std::vector<bool> >::iterator it7;


    for(it5=bin5.begin(); it5!=bin5.end(); it5++){
      if(std::equal(bin.begin(), bin.begin()+4, it5->first.begin())){
	std::cout<<"help"<<std::endl;
	std::copy(it5->second.begin(), it5->second.end(), twoBitArr); 
	return twoBitArr;	
      }
    }

    for(it6=bin6.begin(); it6!=bin6.end(); it6++){
      if(std::equal(bin.begin(), bin.begin()+5, it6->first.begin())){
	std::copy(it6->second.begin(), it6->second.end(), twoBitArr); 
	return twoBitArr;	
      }
    }

    for(it7=bin7.begin(); it7!=bin7.end(); it7++){
      if(std::equal(bin.begin(), bin.begin()+6, it7->first.begin())){
	std::copy(it7->second.begin(), it7->second.end(), twoBitArr); 
	return twoBitArr;	
      }
    }
    
  /*else if(bin.size()==5){
    //std::map<std::string, int>::iterator it = mapOfWordCount.begin();
    std::copy(bin5.find(bin)->second.begin(), bin5.find(bin)->second.end(), twoBitArr);
    return twoBitArr;
  }
  else if(bin.size()==6){
    std::copy(bin6.find(bin)->second.begin(), bin6.find(bin)->second.end(), twoBitArr);
    return twoBitArr;
  }
  else if(bin.size()==7){
    std::copy(bin7.find(bin)->second.begin(), bin7.find(bin)->second.end(), twoBitArr);
    return twoBitArr;
  }
  else if(bin.size()==8){
    std::copy(bin8.find(bin)->second.begin(), bin8.find(bin)->second.end(), twoBitArr);
    return twoBitArr;
    }*/
  /*else if(bin.size()==9){
    std::copy(bin9.find(bin)->second.begin(), bin9.find(bin)->second.end(), twoBitArr);
    return twoBitArr;
    }*/

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
    unsigned int numLevels = 4;
    int levelCounts[numLevels] = {2, 0, 0, 0};
    unsigned int nextLevel = levelCounts[0];

    for (unsigned int i = 0; i<(numLevels-1); i++){
      //loop through levelCounts, fill with next level before going to next one
      unsigned int dum = 0;

      for (unsigned int k = 0; k<i; k++){//just get the place of current place in bin before looping through this level
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

    unsigned int binSize = levelCounts[0]+levelCounts[1]+levelCounts[2]+levelCounts[3];

    //get coordinates of the two bits    
    for(unsigned int i=0; i<levelCounts[numLevels-1]/2; i++){ //loop through 2bits
      unsigned int x=0;
      unsigned int y=0;
      unsigned int temp=i; //use this to track which pair on the level we're currently analyzing so we can get the one above, we're resetting it here to be the bottom bit pair, but as we move through the next for loop we want to update it to always be the order of the pair that was just put into the coords 	
      
      for(unsigned int j=0; j<(numLevels-1); j++){//loop through levels
	//keeps track of which pixel we're on, decrement every time we fill coords
	unsigned int counter=0; 
	unsigned int levelTracker=numLevels-2-j;
	unsigned int binCoord=binSize-levelCounts[numLevels-1]-levelCounts[numLevels-2]; 
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
		  x=x+4;
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

    //fix this later, it's very slow but it's a temprorary fix while we get other stuff working, this makes it so it's in the order that we divide in
    bool temp4 = twoBitArr[4];
    bool temp5 = twoBitArr[5];
    bool temp6 = twoBitArr[6];
    bool temp7 = twoBitArr[7];
    twoBitArr[4]=twoBitArr[8];
    twoBitArr[5]=twoBitArr[9];
    twoBitArr[6]=twoBitArr[10];
    twoBitArr[7]=twoBitArr[11];
    twoBitArr[8]=temp4;
    twoBitArr[9]=temp5;
    twoBitArr[10]=temp6;
    twoBitArr[11]=temp7;
    
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

  for(int i=0; i<permNum; i++){
    int temp=i;
    for(int j=0; j<16; j++){
      perms[i].push_back(temp%2);
      //std::cout<<temp%2;
      temp=temp/2;
    }
    //std::cout<<""<<std::endl;

    permsEncoded[i]=encode(perms[i]);
    if(permsEncoded[i].size()==5){
      bin5[permsEncoded[i]]=perms[i];
    }
    else if(permsEncoded[i].size()==6){
      bin6[permsEncoded[i]]=perms[i];
    }
    else if(permsEncoded[i].size()==7){
      bin7[permsEncoded[i]]=perms[i];
    }
    else if(permsEncoded[i].size()==8){
      bin8[permsEncoded[i]]=perms[i];
    }
    else if(permsEncoded[i].size()==9){
      bin9[permsEncoded[i]]=perms[i];
    }
    else if(permsEncoded[i].size()==10){
      bin10[permsEncoded[i]]=perms[i];
    }
    else if(permsEncoded[i].size()==11){
      bin11[permsEncoded[i]]=perms[i];
    }
    else if(permsEncoded[i].size()==12){
      bin12[permsEncoded[i]]=perms[i];
    }
    else if(permsEncoded[i].size()==13){
      bin13[permsEncoded[i]]=perms[i];
    }
    else if(permsEncoded[i].size()==14){
      bin14[permsEncoded[i]]=perms[i];
    }
    else if(permsEncoded[i].size()==15){
      bin15[permsEncoded[i]]=perms[i];
    }
    else if(permsEncoded[i].size()==16){
      bin16[permsEncoded[i]]=perms[i];
    }
    else if(permsEncoded[i].size()==17){
      bin17[permsEncoded[i]]=perms[i];
    }
    else if(permsEncoded[i].size()==18){
      bin18[permsEncoded[i]]=perms[i];
    }
    else if(permsEncoded[i].size()==19){
      bin19[permsEncoded[i]]=perms[i];
    }
    else if(permsEncoded[i].size()==20){
      bin20[permsEncoded[i]]=perms[i];
    }
    else if(permsEncoded[i].size()==21){
      bin21[permsEncoded[i]]=perms[i];
    }
    else if(permsEncoded[i].size()==22){
      bin22[permsEncoded[i]]=perms[i];
    }
    else if(permsEncoded[i].size()==23){
      bin23[permsEncoded[i]]=perms[i];
    }
    else if(permsEncoded[i].size()==24){
      bin24[permsEncoded[i]]=perms[i];
    }
    else if(permsEncoded[i].size()==25){
      bin25[permsEncoded[i]]=perms[i];
    }
    else if(permsEncoded[i].size()==26){
      bin26[permsEncoded[i]]=perms[i];
    }
    else if(permsEncoded[i].size()==27){
      bin27[permsEncoded[i]]=perms[i];
    }
    else if(permsEncoded[i].size()==28){
      bin28[permsEncoded[i]]=perms[i];
    }
    else if(permsEncoded[i].size()==29){
      bin29[permsEncoded[i]]=perms[i];
    }
    else if(permsEncoded[i].size()==30){
      bin30[permsEncoded[i]]=perms[i];
    }
    //std::cout<<""<<std::endl;
  }
  std::cout<<bin5.size()<<std::endl;
  std::cout<<bin6.size()<<std::endl;
  std::cout<<bin7.size()<<std::endl;
  std::cout<<bin8.size()<<std::endl;
  std::cout<<bin9.size()<<std::endl;
  std::cout<<bin10.size()<<std::endl;
  std::cout<<bin11.size()<<std::endl;
  std::cout<<bin12.size()<<std::endl;
  std::cout<<bin13.size()<<std::endl;
  std::cout<<bin14.size()<<std::endl;
  std::cout<<bin15.size()<<std::endl;
  std::cout<<bin16.size()<<std::endl;
  std::cout<<bin17.size()<<std::endl;
  std::cout<<bin18.size()<<std::endl;
  std::cout<<bin19.size()<<std::endl;
  std::cout<<bin20.size()<<std::endl;
  std::cout<<bin21.size()<<std::endl;
  std::cout<<bin22.size()<<std::endl;
  std::cout<<bin23.size()<<std::endl;
  std::cout<<bin24.size()<<std::endl;
  std::cout<<bin25.size()<<std::endl;
  std::cout<<bin26.size()<<std::endl;
  std::cout<<bin27.size()<<std::endl;
  std::cout<<bin28.size()<<std::endl;
  std::cout<<bin29.size()<<std::endl;
  std::cout<<bin30.size()<<std::endl;

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
      //std::cout<<rando[x];
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
