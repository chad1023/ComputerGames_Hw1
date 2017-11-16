#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include<time.h>

#include <bitset>

using namespace std;

vector<int> v;
string bit_present(int n,const int size)
{
    
    char tmp[size];
    
    int i=size-1;
    while(i+1)
    {
        tmp[i--]=((1&n)+'0');
        n>>=1;
    }
    tmp[size] = '\0';
    string result(tmp);
    return result;
    // string result;
    // bitset<n>tmp(i);
    // result=tmp.to_string();
   
    
    // cout<<"string:"<<result<<endl;
    // return reverse(result.beign(),result.end()); 

}

// void generate_row(int cu)
// bool 

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
// 1011 limit 1 0000 0000
void expand(int num,int current,int shift,int n,int limit,vector<int>&patterns,vector<int>&hint)
{
    // cout<<"num:"<<bit_present(num,n)<<endl;
    // cout<<"limt>>1:"<<(limit>>1)<<endl;
    if(current<hint.size()&&(num<(limit>>1)))
    {
    
        int space=n;
        int check=limit>>1;
        while((num & (check))!=check)
        {
            space--;
            check>>=1;
        };
        //  cout<<"space:"<<space<<endl;

        if(shift==0)
        {
            shift=space;
        }
        shift-=hint[current];
        int shift_num=((num>>shift)<<(shift));
        int pin=num-shift_num;
        // cout<<"current:"<<current<<endl;
        for(int i=0;i<=n-space;i++)
        {

            int pattern=(shift_num<<i)+pin;
            // cout<<"i:"<<i<<",patern:"<<bit_present(pattern,n)<<endl;
            expand(pattern,current+1,shift-1,n,limit,patterns,hint);
            // while(pattern<limit)
            // {
               
            //     expand(pattern,current+1,n,limit,patterns,hint);
            //     pattern<<=1;
            // }
                    
                    // cout<<"tmp:"<< ((tmp-1)<<i)+1<<endl;
                    // cout<<"string,tmp:"<<bit_present(((tmp-1)<<i)+1,n)<<endl;
        }
    }
    else 
    {
        // cout<<"tmp:"<< (num)<<endl;
        // cout<<"string,tmp:"<<bit_present(num,n)<<endl;
        patterns.push_back(num);
    }
    // if(num & limit-1)
    
    
    
    // if(current<hint.size()-1)//expand and go to the next
    // {
    //     int tmp=
        
    //     // expand current and expand(tmp)
    // }
    // else if (current==hint.size()-1)//on the end
    // {

    // }
   

}

int main()
{
    // int N=5;
    // for (int i=0; i<(1<<N); i++)
    // {
    //     cout<<i;
    // }

    // vector<int> board;
    // for(int i=0;i<N;i++)
    // {
    //     board.push_back(1<<i);
    // }

    // 1 1 1
    // 10101
    // 01010
    // 00101
    // 10010
    // 10001
    // 01001

    
   
   const int n=8;

//    int hint[3]={1,1,1};
   unsigned int tmp=0;
//    cout<<"~0:"<<(~tmp)<<endl;
//    cout<<"sting"<<bit_present(~tmp,32)<<endl;
//    cout<<"~0:"<<((~tmp)>>1)<<endl;
//    cout<<"sting"<<bit_present((~tmp)>>1,32)<<endl;
//    cout<<"~0<<1"<<((~tmp)>>1)<<endl;
   vector<int>patterns;
   vector<int>hint(1,1);
   hint.push_back(2);
   hint.push_back(1);
   
   
   tmp=0;
   int space=0;
   int hint_count=hint.size();
   for(int i=0;i<hint.size();i++)
   {
       unsigned int tmp2=(~0);
       tmp2>>=(32-hint[i]);
       tmp<<=(hint[i]+1);
       tmp+=tmp2;
    //    cout<<"tmp:"<<tmp<<endl;
       space+=hint[i];
   }

   int need=space+hint_count-1;
   cout<<need<<endl;
   cout<<n-need<<endl;
   int limit=(1<<n);
   cout<<"limit:"<<limit<<endl;

   cout<<"tmp_initial:"<<tmp<<",";
   cout<<"string,tmp:"<<bit_present(tmp,n)<<endl;
   
   expand(tmp,0,0,n,limit,patterns,hint);

   cout<<"patterns:"<<endl;
   for(int i=0;i<patterns.size();i++)
   {
       cout<<patterns[i]<<" ";
       cout<<bit_present(patterns[i],n)<<endl;
   }
    
  
//    for(int i=0;i<=n-need;i++)
//    {
        
//         int pattern=tmp;
//         while(pattern<limit)
//         {
           
//             cout<<"tmp:"<< (pattern)<<endl;
//             cout<<"string,tmp:"<<bit_present(pattern,n)<<endl;
//             pattern<<=1;
//             // j++;
//         }
        
//         // cout<<"tmp:"<< ((tmp-1)<<i)+1<<endl;
//         // cout<<"string,tmp:"<<bit_present(((tmp-1)<<i)+1,n)<<endl;
//    }
   cout << "Size of bool = " << sizeof(bool) << endl;
   cout << "Size of int = " << sizeof(int) << endl;
    // cout<<"tmp:"<<tmp;

    
}






