#include "Tool.h"

int Tool::s2i(string str) {
    int num;
    stringstream stream(str);
    stream >> num;
    return num;
}
