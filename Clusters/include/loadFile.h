#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <math.h>

#include "common_include.h"

using namespace std;

void stringTOnum(string s, double* pdata);
int loadFileFromSift(string filepath, vector<vector<int>>& data);
