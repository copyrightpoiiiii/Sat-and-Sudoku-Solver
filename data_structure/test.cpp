#include<cstdio>
#include"vector.hpp"
myVector<int> s;
int main(){
    s.push_back(1);
    for(int i=1;i<=10;i++)
        s.push_back(i);
    for(int i=0;i<s.size();i++)
        printf("%d\n",s[i]);
}