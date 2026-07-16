// include/Utils.h
#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <random>
#include <ctime>
#include <memory>

using namespace std;

// 颜色输出辅助
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"

// 工具函数
void clearScreen();
void waitForEnter();
int getRandomInt(int min, int max);
string toLower(const string& str);

#endif