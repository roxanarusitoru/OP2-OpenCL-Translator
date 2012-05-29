#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

using namespace std;

int main(int arc, int** argv) {
  
  string str = "123";
  printf("%d\n", isDigit(str[0]));
  printf("%d\n", strtoi(str));
}
