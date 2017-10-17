#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include<time.h>

using namespace std;

vector<int> v;

void test1()
{
    cout<<"test1"<<endl;
    for(int i=0;i<v.size();i++)
    {
        cout<<v[i];
    }
    cout<<endl;
}

void test2(vector<int> v)
{
    cout<<"test2"<<endl;
    v.push_back(-10);
    for(int i=0;i<v.size();i++)
    {
        cout<<v[i];
    }
    cout<<endl;
    test1();
}


int main()
{
    for(int i=0;i<5;i++)
    {
        v.push_back(i);
    }
    test1();
    
    test2(v);
    cout<<"after"<<endl;
    test1();


}