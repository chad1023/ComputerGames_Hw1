#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include<map>
#include<time.h>

using namespace std;


int ** bit_board;
vector<string > guess_board;
map<int,vector<int> >hint_map;

void cleararray(int **array,int row)
{
    if(row!=0)
    {
        for(int i=0;i<row;i++)
        {
            delete [] array[i];
        }
    }
    delete [] array;
    // cout<<"clear success"<<endl;

}

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

}

string bit_present_tab(int n,const int size)
{
    
    char tmp[2*size-1];
    
    int i=2*size-2;
    while(i+2)
    {
        tmp[i--]=((1&n)+'0');
        tmp[i--]='\t';
        n>>=1;
    }
    tmp[2*size-1] = '\0';
    string result(tmp);
    return result;

}

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
}

void generate_patterns(int n,vector<vector<int> >&hint, vector< vector<int> > &patterns)
{
    int limit=(1<<n);
    for(int i=0;i<n;i++)
    {
        vector<int>pattern;
        int tmp=0;
        for(int j=0;j<hint[i].size();j++)
        {
            unsigned int tmp2=(~0);
            tmp2>>=(32-hint[i][j]);
            tmp<<=(hint[i][j]+1);
            tmp+=tmp2;
        }
        if(hint_map.find(tmp)!=hint_map.end())
        {
            pattern=hint_map[tmp];
        }
        else
        {
            expand(tmp,0,0,n,limit,pattern,hint[i]);
            hint_map[tmp]=pattern;
        }
        // cout<<"tmp:"<<tmp<<endl;
        
        patterns.push_back(pattern);
    }

}
//look up board
bool eliminate_row(bool trueboard,int id,int value, vector<vector<int> > &pattern)
{
    bool change=false;
    vector<int>::iterator it=pattern[id].begin();

    if(trueboard)//check value pos = 1=> &=value
    {
        while(it!=pattern[id].end())
        {
            if(((*it)&value)!=value)
            {
                pattern[id].erase(it);
                change=true;
            }
            else
            {
                it++;
            }
        }
    }
    else//check value pos =0 =>  &=0
    {
        while(it!=pattern[id].end())
        {
            if(((*it)&value)!=0)
            {
                pattern[id].erase(it);
                change=true;
            }
            else
            {
                it++;
            }
        }

    }
    return change;
}





bool eliminate(int row_id,int col_id,char value,vector< vector< string > > &row_pattern, vector< vector<string > > &col_pattern)
{
    bool change=false;
    vector<string>::iterator it=row_pattern[row_id].begin();
    while(it!=row_pattern[row_id].end())
    {
        if((*it)[col_id]!=value)
        {
            change=true;
            row_pattern[row_id].erase(it);
            
        }
        else
        {
            it++;
        }
    }
    it=col_pattern[col_id].begin();
    while(it!=col_pattern[col_id].end())
    {
        if((*it)[row_id]!=value)
        {
    
            col_pattern[col_id].erase(it);
            change=true;
        }
        else
        {
            it++;
        }
    }
    return change;
}

bool check(int &n,vector<vector<int> > &col_hint,vector<string > board)//check if the board dismatch the col-hint
{
    int num=board.size();
    int leftspace=n-num;
    for(int i=0;i<n;i++)
    {
        int ptr=0;
        int count=0;
        int idx_hint=0;
        while(ptr<num)
        {
            if(board[ptr][i]=='1')
            {
                count++;
                ptr++;
                if(idx_hint>=col_hint[i].size()){
                    // cout<<"danger"<<endl;
                    return false;
                }
                else if(count>col_hint[i][idx_hint])
                {
                    // cout<<"hereeliminate"<<endl;
                    return false;
                }
                if(ptr==num)
                {
                    
                    if ((num==n)&&(count!=col_hint[i][idx_hint]))
                    {
                        // cout<<"danger"<<endl;
                        return false;
                    }
                    
                    
                }
            }
            else
            {
                if(count!=0)
                {
                    if(idx_hint>=col_hint[i].size()){
                        // cout<<"danger"<<endl;
                        return false;
                    }
                    if(count==col_hint[i][idx_hint])
                    {
                        idx_hint++;
                    }
                    else
                    {
                        // cout<<"danger"<<endl;
                        return false;
                    }
                    count=0;
                }
                ptr++;
            }
            if(ptr==num)
            {
                int needspace=0;
                int betweenspace=0;
                for(int j=idx_hint;j<col_hint[i].size();j++)
                {
                    needspace+=col_hint[i][j];
                    betweenspace++;
                }
                if(count+leftspace<needspace+betweenspace-1)
                {
                    // cout<<"eliminate in check"<<endl;
                    return false;
                }
            }
            
        }
    }
   
    if(num<n){
        return true;
    }
    // cout<<"success!!"<<endl;
    // for(int i=0;i<n;i++)
    // {
    //     cout<<board[i]<<endl;
    // }
    return true;
}

bool check_bit(int &n,vector<vector<int> > &col_hint,vector<int > board)//check if the board dismatch the col-hint
{
    int num=board.size();
    // cout<<"num:"<<num<<endl;
    int leftspace=n-num;

    
    
    int count[n];
    int idx_hint[n];
    for(int i=0;i<n;i++)
    {
        count[i]=0;
        idx_hint[i]=0;
    }

    int check=(1<<n);
    for(int i=0;i<n;i++)
    {
        check>>=1;
        int ptr=0;
        while(ptr<num)
        {
            if((board[ptr] & check)==check)
            {
                count[i]++;
                ptr++;
                if(idx_hint[i]>=col_hint[i].size())
                {
                    // cout<<"danger1"<<endl;
                    return false;
                }
                else if (count[i]>col_hint[i][idx_hint[i]])
                {
                    // cout<<"danger2"<<endl;
                    return false;
                }
                if(ptr==num)
                {
                    if ((num==n)&&(count[i]!=col_hint[i][idx_hint[i]]))
                    {
                        // cout<<"danger"<<endl;
                        return false;
                    }

                }
            }
            else
            {
                if(count[i]!=0)
                {
                    if(idx_hint[i]>=col_hint[i].size())
                    {
                        // cout<<"false1"<<endl;
                        return false;
                    }
                    if(count[i]==col_hint[i][idx_hint[i]])
                    {
                        idx_hint[i]++;
                    }
                    else
                    {
                        // cout<<"false3"<<endl;
                        return false;
                    }
                    count[i]=0;
                }
                ptr++;
            }
            if(ptr==num)
            {
                int needspace=0;
                int betweenspace=0;
                for(int j=idx_hint[i];j<col_hint[i].size();j++)
                {
                    needspace+=col_hint[i][j];
                    betweenspace++;
                }
                if(count[i]+leftspace<needspace+betweenspace-1)
                {
                    // cout<<"eliminate in check"<<endl;
                    return false;
                }
                
            }
        }

    }
 
    if(num<n)
    {
        return true;
    }
    return true;
    
}

//fromcol:true=> idx後row依據col的生成board



bool guess(bool fromcol,int idx,int n,int **board,vector<vector<int> > &row_pattern,vector<vector<int> > &col_pattern)
{
    bool change=false;
    unsigned int limit=1<<n;
    limit-=1;
    if(fromcol)
    {
        for(int i=0;i<n;i++)
        {
            unsigned int _and = limit;
            unsigned int n_and=limit;
            for(int j=0;j<col_pattern[i].size();j++)
            {
                _and &= col_pattern[i][j];
                n_and &= (~(col_pattern[i][j]));
            }
            int nxor= _and|n_and;
            int bit=0;
            // cout<<"i:"<<i<<",nxor:"<<bit_present(nxor,n)<<endl;
            while(nxor>0)
            {
                
                if(nxor &1)
                {
                   int check=(1<<bit);
                   int row_pos=n-bit-1;
                   if(row_pos<idx)//row_limit
                   {
                       break;
                   }

                   int row_check=(1<<(n-i-1));
                   
                   if(((board[1][row_pos] & row_check)|(board[0][row_pos] & row_check))==row_check)//board has value,sk ip
                   {
                    //    cout<<"hasvalue"<<endl;
                   }
                   else{
                        change=true;
                        if((check & col_pattern[i][0])==check)
                        {
                            
                            board[1][row_pos]|=row_check;
                                // set the 1 table i row check bit to 1
                        }
                        else
                        {
                                // for(int j=0;j<n;j++)
                                // {
                            board[0][row_pos]|=row_check;// set the 0 table i row check bit to 1
                                // }
                                
                        }
                }
                    //bit : set n-bit to col[i] bit 
                }
                bit++;
                nxor>>=1;
            }
        }
    }
    else
    {
        for(int i=0;i<n;i++)
        {
            unsigned int _and = limit;
            unsigned int n_and=limit;
            for(int j=0;j<row_pattern[i].size();j++)
            {
                _and &= row_pattern[i][j];
                n_and &= (~(row_pattern[i][j]));
            }
            int nxor= _and|n_and;
            int bit=0;
            // cout<<"i:"<<i<<",nxor:"<<bit_present(nxor,n)<<endl;
            while(nxor>0)
            {
                
                if(nxor &1)
                {
                   int check=(1<<bit);
                   int col_pos=n-bit-1;
                   if(col_pos<idx)//row_limit
                   {
                       break;
                   }

                   if(((board[1][i] & check)|(board[0][i] & check))==check)//board has value,sk ip
                   {
                    //    cout<<"hasvalue"<<endl;
                   }
                   else
                   {
                       change=true;
                //    int col_check=(1<<(n-i-1)); 
                       if((check & row_pattern[i][0])==check)
                        {
                            board[1][i]|=check;
                                // set the 1 table i row check bit to 1
                        }
                        else
                        {
                                // for(int j=0;j<n;j++)
                                // {
                            board[0][i]|=check;// set the 0 table i row check bit to 1
                                // }
                                
                        }
                    }
                    //bit : set n-bit to col[i] bit 
                }
                bit++;
                nxor>>=1;
            }
        }

    }
    return change;
}

//idx_row後依據col的生成board
void generateguess(bool fromcol,int idx_row,int n,vector<string >&board,vector <vector<string > > &row_pattern,vector <vector<string > > &col_pattern)
{
    if(fromcol)
    {
        for(int i=idx_row+1;i<n;i++)
        {
        size_t j =board[i].find_first_of('*',0);
        while(j!=string::npos)
        {
            bool change=true;
            vector<string >::iterator it=col_pattern[j].begin();
            char tmp=(*it)[i];
            while(it!=col_pattern[j].end())
            {
                
                if(tmp!=(*it)[i])
                {
                    change=false;
                    break;
                }
                it++;
            }
            if(change)
            {
                board[i][j]=tmp;
            }
            j=board[i].find_first_of('*',j+1);
        }
        
        }
    }
    else
    {
        for(int i=idx_row+1;i<n;i++)
        {
        size_t j =board[i].find_first_of('*',0);
        while(j!=string::npos)
        {
            bool change=true;
            vector<string >::iterator it=row_pattern[i].begin();
            char tmp=(*it)[j];
            while(it!=row_pattern[i].end())
            {
                
                if(tmp!=(*it)[j])
                {
                    change=false;
                    break;
                }
                it++;
            }
            if(change)
            {
                board[i][j]=tmp;
            }
            j=board[i].find_first_of('*',j+1);
        }
        
        }

    }

}


bool preprocess_init(int idx_row,int n,vector<string > &boardtmp,vector <vector<string > > &row_pattern,vector <vector<string > > &col_pattern)
{
    // cout<<"prepos"<<endl;
    if(idx_row<0)
    {
        return true;
    }

    vector<string > board;
    for(int i=0;i<boardtmp.size();i++)
    {
        board.push_back(boardtmp[i]);
    }
    
    string num;
    for(int i=0;i<n;i++)
    {
        num.push_back('*');
    }
    for(int i=idx_row+1;i<n;i++)
    {
        board.push_back(num);
    }

    //for the idx_row(painted), remove the unmatched col_pattern
    for(int i=0;i<n;i++)
    {
        vector<string >::iterator it=col_pattern[i].begin();
        // cout<<"board:"<<idx_row<<","<<i<<endl;
        while(it!=col_pattern[i].end())
        {
            if((*it)[idx_row]!=board[idx_row][i])
            {
                col_pattern[i].erase(it);
                if(col_pattern[i].empty())
                {
                    // cout<<"colempty"<<i<<endl;
                    return false;
                }
            }
            else
            {
                it++;
            }
        } 
    }

    bool change=true;
    // cout<<"beforeguess"<<endl;
    while(change)
    {
    change=false;
    generateguess(true,idx_row,n,board,row_pattern,col_pattern);

    // cout<<"afterguess"<<endl;
    // 依據board消除row_pattern
    for(int i=idx_row+1;i<n;i++)
    {
        
        size_t j =board[i].find_first_not_of('*',0);
        while(j!=string::npos)
        {
            vector<string >::iterator it=row_pattern[i].begin();
            while(it!=row_pattern[i].end())
            {

                // cout<<"erasewrong"<<endl;   
                if((*it)[j]!=board[i][j])
                {
                    row_pattern[i].erase(it);
                    change=true;
                    if(row_pattern[i].empty())
                    {
                        return false;
                    }
                    continue;
                }
                it++;
            }
            j=board[i].find_first_not_of('*',j+1);
        }
    }
    generateguess(false,idx_row,n,board,row_pattern,col_pattern);

    for(int i=idx_row+1;i<n;i++)
    {
        
        size_t j =board[i].find_first_not_of('*',0);
        while(j!=string::npos)
        {
            vector<string >::iterator it=col_pattern[j].begin();
            while(it!=col_pattern[j].end())
            {

                // cout<<"erasewrong"<<endl;   
                if((*it)[i]!=board[i][j])
                {
                    col_pattern[j].erase(it);
                    change=true;
                    if(col_pattern[j].empty())
                    {
                        return false;
                    }
                    continue;
                }
                it++;
            }
            j=board[i].find_first_not_of('*',j+1);
        }

    }
    }
    
    return true;
    
}
bool preprocess(int idx_row,int n,vector<int > &boardtmp,vector <vector<int > > &row_pattern,vector <vector<int > > &col_pattern)
{
    // cout<<"prepos:"<<idx_row<<endl;
    if(idx_row<0)
    {
        return true;
    }

    int ** board;
    board=new int*[2];
    board[0]=new int [n];
    board[1]=new int [n];


    for(int i=0;i<boardtmp.size();i++)
    {
        board[0][i]=(~boardtmp[i]);
        board[1][i]=boardtmp[i];
    }
    
    for(int i=idx_row+1;i<n;i++)
    {
        board[0][i]=0;
        board[1][i]=0;
    }

    // cout<<"initboard_0:"<<endl;
    // for(int i=0;i<n;i++)
    // {
    //     cout<<bit_present(board[0][i],n)<<endl;
    // }
    // cout<<"initboard_1:"<<endl;
    // for(int i=0;i<n;i++)
    // {
    //     cout<<bit_present(board[1][i],n)<<endl;
    // }
    // cout<<"row_pattern:"<<endl;
    // for(int i=0;i<n;i++)
    // {
    //     cout<<"row:"<<i<<endl;
    //     for(int j=0;j<row_pattern[i].size();j++)
    //     {
    //         cout<<bit_present(row_pattern[i][j],n)<<endl;
    //     }
    // }

    // cout<<"col_pattern:"<<endl;
    // for(int i=0;i<n;i++)
    // {
    //     cout<<"col:"<<i<<endl;
    //     for(int j=0;j<col_pattern[i].size();j++)
    //     {
    //         cout<<bit_present(col_pattern[i][j],n)<<endl;
    //     }
    // }
    //idx_row
    //board[0][idx_row] check 0 
    //board[1][idx_row] check 1

    

    int col_pos=idx_row;
    int check =1<<n;
    int check_col=check>>(idx_row+1);
    // cout<<"board1_idx:"<<bit_present(board[1][idx_row],n)<<endl;
    //for the idx_row(painted), remove the unmatched col_pattern
    for(int i=0;i<n;i++)
    {
        check>>=1;
        // cout<<"check:"<<bit_present(check,n)<<endl;
        vector<int >::iterator it=col_pattern[i].begin();
        if((board[0][idx_row]&check)==check)//board[idx_row][i]=0
        {
            while(it!=col_pattern[i].end())
            {
                if(((*it)&check_col)!=0)//check 0
                {
                    // cout<<"erase0"<<endl;
                    col_pattern[i].erase(it);
                    if(col_pattern[i].empty())
                    {
                        // cout<<"colempty"<<i<<endl;
                        return false;
                    }
                }
                else
                {
                    it++;
                }
            } 
            
        }
        else if((board[1][idx_row]&check)==check)//board[idx_row][i]=1
        {
            while(it!=col_pattern[i].end())
            {
                if(((*it)&check_col)!=check_col)
                {
                    // cout<<"erase1"<<endl;
                    col_pattern[i].erase(it);
                    if(col_pattern[i].empty())
                    {
                        // cout<<"colempty"<<i<<endl;
                        return false;
                    }
                }
                else
                {
                    it++;
                }
            } 

        }
         // cout<<"board:"<<idx_row<<","<<i<<endl;   
    }
    // cout<<"row_pattern:"<<endl;
    // for(int i=0;i<n;i++)
    // {
    //     cout<<"row:"<<i<<endl;
    //     for(int j=0;j<row_pattern[i].size();j++)
    //     {
    //         cout<<bit_present(row_pattern[i][j],n)<<endl;
    //     }
    // }

    // cout<<"col_pattern:"<<endl;
    // for(int i=0;i<n;i++)
    // {
    //     cout<<"col:"<<i<<endl;
    //     for(int j=0;j<col_pattern[i].size();j++)
    //     {
    //         cout<<bit_present(col_pattern[i][j],n)<<endl;
    //     }
    // }

    bool change=true;
    // cout<<"beforeguess"<<endl;
    while(change)
    {
    change=false;
    guess(true,idx_row,n,board,row_pattern,col_pattern);//guess from col,painted the row

    // cout<<"initboard_0:"<<endl;
    // for(int i=0;i<n;i++)
    // {
    //     cout<<bit_present(board[0][i],n)<<endl;
    // }
    // cout<<"initboard_1:"<<endl;
    // for(int i=0;i<n;i++)
    // {
    //     cout<<bit_present(board[1][i],n)<<endl;
    // }
    // cout<<"afterguess"<<endl;
    // 依據board消除row_pattern
    for(int i=idx_row+1;i<n;i++)
    {
        
        int check = (board[0][i]^board[1][i]);//find not *
        int bit=1;
        while(check>0)
        {
            // cout<<"check:"<<bit_present(check,n)<<endl;
            if(check & 1)
            {
                // cout<<"bit:"<<bit_present(bit,n)<<endl;
                vector<int >::iterator it=row_pattern[i].begin();
                if((board[0][i]&bit)==bit)//board i bit =0;
                {
                    while(it!=row_pattern[i].end())
                    {
        
                        if(((*it)&bit)!=0)//it !=0
                        {
                            row_pattern[i].erase(it);
                            change=true;
                            if(row_pattern[i].empty())
                            {
                                return false;
                            }
                            continue;
                        }

                        it++;
                    }
                    
                }
                else//board i bit =1
                {
                    while(it!=row_pattern[i].end())
                    {
                        if(((*it)&bit)!=bit)//it !=1
                        {
                            row_pattern[i].erase(it);
                            change=true;
                            if(row_pattern[i].empty())
                            {
                                return false;
                            }
                            continue;
                        }

                        it++;
                    }

                }
                
            }
            bit<<=1;
            check>>=1;
        }   
    }
    // cout<<"after row_erase:"<<endl;
    // cout<<"row_pattern:"<<endl;
    // for(int i=0;i<n;i++)
    // {
    //     cout<<"row:"<<i<<endl;
    //     for(int j=0;j<row_pattern[i].size();j++)
    //     {
    //         cout<<bit_present(row_pattern[i][j],n)<<endl;
    //     }
    // }

    // cout<<"col_pattern:"<<endl;
    // for(int i=0;i<n;i++)
    // {
    //     cout<<"col:"<<i<<endl;
    //     for(int j=0;j<col_pattern[i].size();j++)
    //     {
    //         cout<<bit_present(col_pattern[i][j],n)<<endl;
    //     }
    // }
   
    guess(false,idx_row,n,board,row_pattern,col_pattern);//guess from row
     
    // cout<<"initboard_0:"<<endl;
    // for(int i=0;i<n;i++)
    // {
    //     cout<<bit_present(board[0][i],n)<<endl;
    // }
    // cout<<"initboard_1:"<<endl;
    // for(int i=0;i<n;i++)
    // {
    //     cout<<bit_present(board[1][i],n)<<endl;
    // }
    
    
    
    for(int row=idx_row+1;row<n;row++)
    {
    int check =1<<n;
    int check_col=check>>(row+1);
    
    for(int i=0;i<n;i++)
    {
        check>>=1;
        vector<int >::iterator it=col_pattern[i].begin();
        if((board[0][row]&check)==check)//board[idx_row][i]=0
        {
            while(it!=col_pattern[i].end())
            {
                if(((*it)&check_col)!=0)//check 0
                {
                    col_pattern[i].erase(it);
                    change=true;
                    if(col_pattern[i].empty())
                    {
                        // cout<<"colempty"<<i<<endl;
                        return false;
                    }
                }
                else
                {
                    it++;
                }
            } 
            
        }
        else if((board[1][row]&check)==check)//board[idx_row][i]=1
        {
            while(it!=col_pattern[i].end())
            {
                if(((*it)&check_col)!=check_col)
                {
                    col_pattern[i].erase(it);
                    change=true;
                    if(col_pattern[i].empty())
                    {
                        // cout<<"colempty"<<i<<endl;
                        return false;
                    }
                }
                else
                {
                    it++;
                }
            } 

        }
        // cout<<"board:"<<idx_row<<","<<i<<endl;
       
    }
    }
    // change=false;
    }
    cleararray(board,2);
    return true;
    
}



bool DFS_init(int idx_row,int n,vector<vector<int> > &col_hint,vector<string > board,vector <vector<string > > &row_pattern,vector <vector<string > > &col_pattern,string &result)
{
    vector<vector<string > > new_row_pattern;
    vector<vector<string > > new_col_pattern;

    // cout<<"DFS_init,idx_row:"<<idx_row<<endl;
    for(int i=0;i<n;i++)
    {
        new_row_pattern.push_back(row_pattern[i]);
        new_col_pattern.push_back(col_pattern[i]);
    }
    
    // cout<<"idx_row:"<<idx_row<<endl;
    if(!check(n,col_hint,board))
    {
        // cout<<"check=true"<<endl;
        return false;
    }
    else if (board.size()==n)//
    {
        cout<<"successfile!!"<<endl;
        for(int i=0;i<n;i++)
        {
            result+=board[i][0];
            for(int j=1;j<n;j++)
            {
                result+="\t";
                result+=board[i][j];
                
            }
            result+="\n";
        }
        return true;
    }
    
    if(!preprocess_init(idx_row-1,n,board,new_row_pattern,new_col_pattern))
    {
        
        // cout<<"false:"<<endl;
        return false;
    }
   

    
    bool ans;
    for(int i=0;i<new_row_pattern[idx_row].size();i++)//i will iterate the possible pattern
    {
        // cout<<"pattern:"<<i<<endl;
        // cout<<new_row_pattern[idx_row][i]<<endl;
        // for(int j=0;j<n;j++)
        // {
        //     cout<<new_row_pattern[idx_row][i][j];
        // }
        // cout<<endl;
        board.push_back(new_row_pattern[idx_row][i]);
        ans=DFS_init(idx_row+1,n,col_hint,board, new_row_pattern,new_col_pattern,result);
        if(ans==true)
        {
            break;
        }
        board.pop_back();
    }
    return ans;
}
bool DFS(int idx_row,int n,vector<vector<int> > &col_hint,vector<int > board,vector <vector<int > > &row_pattern,vector <vector<int > > &col_pattern,string &result)
{
    vector<vector<int > > new_row_pattern;
    vector<vector<int > > new_col_pattern;
    
    // cout<<"DFS,idx_row:"<<idx_row<<endl;
    for(int i=0;i<n;i++)
    {
        new_row_pattern.push_back(row_pattern[i]);
        new_col_pattern.push_back(col_pattern[i]);
    }
    
    // cout<<"idx_row:"<<idx_row<<endl;
    if(!check_bit(n,col_hint,board))
    {
        // cout<<"check=true"<<endl;
        return false;
    }
    else if (board.size()==n)//
    {
        cout<<"successfile!!"<<endl;
        for(int i=0;i<n;i++)
        {
            result+=bit_present_tab(board[i],n);
            // for(int j=1;j<n;j++)
            // {
            //     result+="\t";
            //     result+=board[i][j];
                
            // }
            result+="\n";
        }
        return true;
    }
    
    // if(!preprocess(idx_row-1,n,board,new_row_pattern,new_col_pattern))
    // {
        
    //     // cout<<"false:"<<endl;
    //     return false;
    // }
   

    
    bool ans;
    for(int i=0;i<new_row_pattern[idx_row].size();i++)//i will iterate the possible pattern
    {
        // cout<<"pattern:"<<bit_present(new_row_pattern[idx_row][i],n)<<endl;
        board.push_back(new_row_pattern[idx_row][i]);
        ans=DFS(idx_row+1,n,col_hint,board, new_row_pattern,new_col_pattern,result);
        if(ans==true)
        {
            break;
        }
        board.pop_back();
    }
    return ans;
}

void generate_row(int idx_row,int current,int n,vector<int>&row_hint, string pattern, vector< vector<string > > &row_pattern)//according to the row hint to generate_row
{
    
    if (row_hint.empty())
    {
        string tmp="";
        for(int i =0;i<n;i++)
        {
            tmp+='0';
        }
        row_pattern[idx_row].push_back(tmp);
        for(int i=0;i<n;i++)
        {
            guess_board[idx_row][i]='0';
        }
        return;
    }
    else if(current>=row_hint.size())
    {
        int space=n-pattern.size();
        if(space<0)
        {
            pattern.pop_back();
        }
        else
        {
            for(int i=0;i<space;i++)
            {
                pattern.push_back('0');
            }
        }
        
        row_pattern[idx_row].push_back(pattern);
        return;
    }
    else
    {   
        
        int sum=0;//sum: # need to be 1
        for(int i=current;i<row_hint.size();i++)
        {
            sum+=row_hint[i];
        }
        int space=n-pattern.size()-sum+row_hint.size()-current-1;//row_hint-current-1 : necessary space between left pattern 
        
        if(space<0){return;}
        for(int i=0;i<=space;i++)
        {
            string tmp=pattern;
            
            for(int j=0;j<row_hint[current];j++)
            {
                tmp.push_back('1');
            }
            tmp.push_back('0');
            generate_row(idx_row,current+1,n,row_hint,tmp,row_pattern);
            pattern.push_back('0');
        }
    }

}



void generate_col(int idx_col,int current,int n,vector<int>&col_hint, string pattern, vector< vector< string > > &col_pattern)//according to the row hint to generate_row
{   
    if (col_hint.empty())
    {
        string tmp="";
        for(int i =0;i<n;i++)
        {
            tmp.push_back('0');
        }
        col_pattern[idx_col].push_back(tmp);
        for(int i=0;i<n;i++)
        {
            guess_board[i][idx_col]='0';
        }
        return;
    }
    else if(current>=col_hint.size())
    {
        int space=n-pattern.size();
        if(space<0)
        {
            pattern.pop_back();
        }
        else
        {
            for(int i=0;i<space;i++)
            {
                pattern.push_back('0');
            }
        }
        col_pattern[idx_col].push_back(pattern);
        return;
    }
    else
    {   
        // cout<<"current:"<<current<<endl;
        int sum=0;//sum: # need to be 1
        for(int i=current;i<col_hint.size();i++)
        {
            sum+=col_hint[i];
        }
        int space=n-pattern.size()-sum+col_hint.size()-current-1;//row_hint-current-1 : necessary space between left pattern 
        // cout<<"space:"<<space<<endl;
        if(space<0){return;}
        for(int i=0;i<=space;i++)
        {
            string tmp=pattern;
            for(int j=0;j<col_hint[current];j++)
            {
                tmp.push_back('1');
            }
            tmp.push_back('0');
            generate_col(idx_col,current+1,n,col_hint,tmp,col_pattern);
            pattern.push_back('0');
        }
    }

}



bool guess(int &n,vector< vector<string > > &row_pattern, vector< vector<string > > &col_pattern)
{
    bool change=false;
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            char tmp=row_pattern[i][0][j];
            bool diff=false;
            for(int k=1;k<row_pattern[i].size();k++)
            {
                if(tmp!=row_pattern[i][k][j])
                {
                    diff=true;
                    break;
                }
            }
            if(!diff)
            {
                guess_board[i][j]=tmp;
                diff=false;
            }

            tmp=col_pattern[i][0][j];
            for(int k=1;k<col_pattern[i].size();k++)
            {
                if(tmp!=col_pattern[i][k][j])
                {
                    diff=true;
                    break;
                }
            }
            if(!diff)
            {
                guess_board[j][i]=tmp;
                diff=false;
            }
        }
    }

    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            if(guess_board[i][j]!='*')
            {
                if(eliminate(i,j,guess_board[i][j],row_pattern,col_pattern))
                {
                    change=true;
                }
                
            }
        }
    }
    return change;
}




int main(int argc, char * argv[])
{
    clock_t t1,t2;
    double totaltime=0.0;;
   
    t1=clock();
    ifstream file;
    fstream output;
    
    string line;
    if(argc==1)
    {
        file.open("tcga2016-question.txt",ios::in);
        output.open("result.txt",ios::out);
    }
    else
    {
        file.open(argv[1],ios::in);
        output.open(argv[2],ios::out);
    }

    int line_num=0;

    while(getline(file,line))
    {
        if(line=="$1")
        {
            while(getline(file,line)&&line[0]!='$')
            {
                line_num++;
            }
            cout<<"linenum:"<<line_num<<endl;
            break;
        }
    }
    file.clear();
    file.seekg(0,ios::beg);
    int n=line_num/2;
    
    string init;
    for(int i=0;i<n;i++)
    {
        init.push_back('*');
    }
    for(int i=0;i<n;i++)
    {
        guess_board.push_back(init);
    }
    
    vector< vector< vector<int> > > hint_cols;
    vector< vector< vector<int> > > hint_rows;

    vector<vector<string > > row_pattern;
    vector<vector<string > > col_pattern;
    int count=0;
    file.seekg(0,ios::beg);
    while(getline(file,line))
    {
        if(line[0]=='$')
        {
            vector< vector<int> > col;
            vector< vector<int> > row; 
            for(int i=0;i<n;i++)
            {
                vector<int>tmp;
                getline(file,line);
                stringstream ss(line);
                int num;
                while(ss>>num)
                {
                    
                    tmp.push_back(num);
                }
                col.push_back(tmp);
            }

            for(int i=0;i<n;i++)
            {
                vector<int>tmp;
                getline(file,line);
                stringstream ss(line);
                int num;
                while(ss>>num)
                {
                    tmp.push_back(num);
                   
                }
                row.push_back(tmp);
            }
            count++;
            // if(count<7)
            // {
            //     continue;
            // }
            vector<string> tmp;
            string tmpv="";
            // cout<<'w'<<endl;
            for (int i=0;i<n;i++)
            {            
                row_pattern.push_back(tmp);
                col_pattern.push_back(tmp);
                generate_row(i,0,n,row[i],tmpv,row_pattern);
                generate_col(i,0,n,col[i],tmpv,col_pattern);
                
            }
            vector<vector<int> >row_pattern_tmp;
            vector<vector<int> >col_pattern_tmp;
            generate_patterns(n,row,row_pattern_tmp);
            generate_patterns(n,col,col_pattern_tmp);

            // cout<<"here"<<endl;
            bit_board=new int*[2];
            bit_board[0]=new int[n];
            bit_board[1]=new int[n];

            for(int i=0;i<n;i++)
            {
                bit_board[0][i]=0;
                bit_board[1][i]=0;
            }
            // bool fromcol,int idx,int n,int **board,vector<vector<int> > &row_pattern,vector<vector<int> > &col_pattern
            // guess(false,0,n,bit_board,row_pattern_tmp,col_pattern_tmp);
            // guess(true,0,n,bit_board,row_pattern_tmp,col_pattern_tmp);

            // cout<<"o table:"<<endl;
            // for(int i=0;i<n;i++)
            // {
            //     cout<<bit_present(bit_board[0][i],n)<<endl;
            // }
            // cout<<"1 table:"<<endl;
            // for(int i=0;i<n;i++)
            // {
            //     cout<<bit_present(bit_board[1][i],n)<<endl;
            // }

            // eliminate row_hint acoording bit_board[0] and 1
            bool change=true;
            while(change)
            {
                guess(false,0,n,bit_board,row_pattern_tmp,col_pattern_tmp);
                guess(true,0,n,bit_board,row_pattern_tmp,col_pattern_tmp);
                change=false;
                for(int i=0;i<n;i++)
                {
                    change|=eliminate_row(false,i,bit_board[0][i],row_pattern_tmp);//bool trueboard,int id,int value, vector<vector<int> > &pattern
                    change|=eliminate_row(true,i,bit_board[1][i],row_pattern_tmp);
                }

            }
            
            // cout<<"after eliminate:"<<endl;
            // for(int i=0;i<n;i++)
            // {
            //     cout<<"row:"<<i<<endl;
            //     for(int j=0;j<row_pattern_tmp[i].size();j++)
            //     {
            //         cout<<bit_present(row_pattern_tmp[i][j],n)<<endl;
            //     }
            // }

            // for(int i=0;i<n;i++)
            // {
            //     cout<<"col:"<<i<<endl;
            //     for(int j=0;j<col_pattern_tmp[i].size();j++)
            //     {
            //         cout<<bit_present(col_pattern_tmp[i][j],n)<<endl;
            //     }
            //     cout<<endl;
            // }

            // for(int i=0;i<n;i++)
            // {
            //     cout<<"row:"<<i<<endl;
            //     for(int j=0;j<row_pattern_tmp[i].size();j++)
            //     {
            //         cout<<bit_present(row_pattern_tmp[i][j],n)<<endl;
            //     }
            //     cout<<endl;
            // }
            for(int i=0;i<n;i++)
            {
                if(!guess(n,row_pattern,col_pattern))
                {
                    break;
                }
            }
            
            long long int complex_row=1;
            long long int complex_col=1;
            for(int i=0;i<n;i++)
            {
                
                complex_row*=row_pattern[i].size();
                complex_col*=col_pattern[i].size();
                if(complex_row<0||complex_col<0)
                {
                    break;
                }
            }

            
            cout<<"prob:"<<count<<endl;
          
            vector<int> board;
            
            string result="";
            // DFS_init(0,n,col,board,row_pattern,col_pattern,result);
    
            DFS(0,n,col,board,row_pattern_tmp,col_pattern_tmp,result);
            cout<<"result:"<<endl;
            cout<<result;
            output<<"$"<<count<<endl;
            output<<result;
            for(int i=0;i<n;i++)
            {
                row_pattern[i].clear();
                col_pattern[i].clear();
                guess_board[i]=init;
            }
            cleararray(bit_board,2);
            
            t2=clock();
            cout<<"subtime:"<<(t2-t1)/(double)(CLOCKS_PER_SEC)<<endl;
            totaltime+=(t2-t1)/(double)(CLOCKS_PER_SEC);
            t1=t2;
            // break;
           
        }    
    }
    cout<<endl<<"time:"<<totaltime<<endl;
    file.close();  
    output.close();
}

 