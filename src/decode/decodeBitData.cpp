#include <iostream>

using namespace std;

bool toBoolArr(vector<bool> bin)
{
  bool 2bit[1][2] = {true, true}
  unsigned int 2bitnum = 8;
  unsigned int rows = 2;
  unsigned int cols = 4;
  bool 2bitarr[rows][cols] = {{2bit, 2bit, 2bit, 2bit}, 
			      {2bit, 2bit, 2bit, 2bit}};
  bool flag = false;
  unsigned int numLevels = 3; 
  unsigned int nextLevel = 2;

  //expand from Huffman coding back from 0->01
  for (unsigned int i=0, i<size(bin), i++){
    if(flag && !bin[i]){ //previous one is 0, this one is also 0, need to insert 1
      bin.insert(true, i);
      flag = false;
      
    }
    else if(!flag && !bin[i]){ //this one is 0 but previous one wasn't, need to flag
      flag = true;
    }
  }
  
  
  //  if (size(bin)>1){
  //  if (bin[0] && bin[1]){
  //    nextLevel = 4;
  // }
  //  else{
  //    nextLevel = 2;
  //  }
  //  for (unsigned int i=0, i<numLevels, i++){
  //    for (unsigned int j=0, j<nextLevel, j++){
	
  //      }
  //    if (!bin[i] && !flag){

  //	flag = true;
	
	
  //	continue;
  /*    }
      else if (flag) {
	if
      }
      else {
      }
    }
    }*/

  return 2bitarr;

}
