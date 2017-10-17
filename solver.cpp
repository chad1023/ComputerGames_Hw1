#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include<time.h>

using namespace std;



vector<vector <int> > guess_board;


void eliminate(int row_id,int col_id,int value,vector< vector< vector<int> > > &row_pattern, vector< vector< vector<int> > > &col_pattern)
{
    vector<vector<int> >::iterator it=row_pattern[row_id].begin();
    while(it!=row_pattern[row_id].end())
    {
        if((*it)[col_id]!=value)
        {
            // cout<<"erase"<<endl;
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
            // cout<<"erase"<<endl;
            col_pattern[col_id].erase(it);
        }
        else
        {
            it++;
        }
    }
}

bool check(int n,vector<vector<int> > &col_hint,vector<vector<int> > board)//check if the board dismatch the col-hint
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
            if(board[ptr][i]==1)
            {
                count++;
                ptr++;
                if(idx_hint>=col_hint[i].size()){
                    cout<<"danger"<<endl;
                    return false;
                }
                else if(count>col_hint[i][idx_hint])
                {
                    cout<<"hereeliminate"<<endl;
                    return false;
                }
                if(ptr==num)
                {
                    
                    if ((num==n)&&(count!=col_hint[i][idx_hint]))
                    {
                        return false;
                    }
                    
                    
                }
            }
            else
            {
                if(count!=0)
                {
                    if(idx_hint>=col_hint[i].size()){
                        
                        return false;
                    }
                    if(count==col_hint[i][idx_hint])
                    {
                        idx_hint++;
                    }
                    else
                    {
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
    cout<<"success!!"<<endl;
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            cout<<board[i][j];
        }
        cout<<endl;
    }
    return true;
}

bool preprocess(int idx_row,int n,vector<vector<int> > &boardtmp,vector <vector<vector<int> > > &row_pattern,vector <vector<vector<int> > > &col_pattern)
{
    if(idx_row<0)
    {
        return true;
    }

    vector<vector<int> > board;
    for(int i=0;i<boardtmp.size();i++)
    {
        board.push_back(boardtmp[i]);
    }
    
    vector<int>num;
    for(int i=0;i<n;i++)
    {
        num.push_back(-1);
    }
    for(int i=idx_row+1;i<n;i++)
    {
        board.push_back(num);
    }
    //for the idx_row(painted), remove the unmatched col_pattern
    for(int i=0;i<n;i++)
    {
        vector<vector<int> >::iterator it=col_pattern[i].begin();
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

    int count=0;
    while(count<2)
    {
    // row column guess
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            int tmp=row_pattern[i][0][j];
            board[i][j]=tmp;
            if(row_pattern[i].size()>1)
            {
                for(int k=1;k<row_pattern[i].size();k++)
                {
                    if(tmp!=row_pattern[i][k][j])
                    {
                        board[i][j]=-1;
                        break;
                    }
                }
            }

            tmp=col_pattern[i][0][j];
            board[j][i]=tmp;
            if(col_pattern[i].size()>1)
            {
                
                for(int k=1;k<col_pattern[i].size();k++)
                {
                    if(tmp!=col_pattern[i][k][j])
                    {
                        board[j][i]=-1;
                        break;
                    }
                }
            }
        }
    }
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            if(board[i][j]!=-1)
            {
                eliminate(i,j,board[i][j],row_pattern,col_pattern);
                if(row_pattern[i].empty())
                {
                    return false;
                }
                if(col_pattern[j].empty())
                {
                    return false;
                }
            }
        }
    }
    count++;
    }

    // cout<<"here"<<endl;
    for(int i=0;i<n;i++)
    {
        vector<vector<int> >::iterator it;
        int tmp=col_pattern[i][0][idx_row+1];
        bool guess=true;
        for(it=col_pattern[i].begin();it!=col_pattern[i].end();it++)
        {
            if((*it)[idx_row+1]!=tmp)
            {
                guess=false;
                break;
            }
        }
        if(guess)
        {
            board[idx_row+1][i]=tmp;
        }
    }
    // cout<<"idx:"<<idx_row<<endl;
    // cout<<"board:"<<endl;
    // for(int i=0;i<n;i++)
    // {
    //     cout<<board[idx_row+1][i];
    // }
    // cout<<endl;
    // cout<<"pattern:"<<row_pattern[idx_row+1].size()<<endl;

    // for(int i=0;i<row_pattern[idx_row+1].size();i++)
    // {
    //     for(int j=0;j<n;j++)
    //     {
    //         cout<<row_pattern[idx_row+1][i][j];
    //     }
    //     cout<<endl;
    // }
    
    for(int i=0;i<n;i++)
    {
        if(board[idx_row+1][i]!=-1)
        {
            vector<vector<int> >::iterator it=row_pattern[idx_row+1].begin();
            while(it!=row_pattern[idx_row+1].end())
            {
                if((*it)[i]!=board[idx_row+1][i])
                {
                   
                    row_pattern[idx_row+1].erase(it);
                    // cout<<"erasehere"<<endl;
                    if(row_pattern[idx_row+1].empty())
                    {
                        // cout<<"erasefalse"<<endl;
                        return false;
                    }
                }
                else
                {
                    it++;
                }
                
            }

        }
       
    }
    return true;
    
}


bool DFS(int idx_row,int n,vector<vector<int> > &col_hint,vector<vector<int> > board,vector <vector<vector<int> > > &row_pattern,vector <vector<vector<int> > > &col_pattern)
{
    vector<vector<vector<int> > > new_row_pattern;
    vector<vector<vector<int> > > new_col_pattern;

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
        return true;
    }
    
    if(!preprocess(idx_row-1,n,board,new_row_pattern,new_col_pattern))
    {
        
        // cout<<"false:"<<endl;
        return false;
    }
   

    // cout<<"DFS,idx_row:"<<idx_row<<endl;
    bool ans;
    for(int i=0;i<new_row_pattern[idx_row].size();i++)//i will iterate the possible pattern
    {
        // cout<<"pattern:"<<i<<endl;
        // for(int j=0;j<n;j++)
        // {
        //     cout<<new_row_pattern[idx_row][i][j];
        // }
        // cout<<endl;
        board.push_back(new_row_pattern[idx_row][i]);
        ans=DFS(idx_row+1,n,col_hint,board, new_row_pattern,new_col_pattern);
        if(ans==true)
        {
            break;
        }
        board.pop_back();
    }
    return ans;
}


void generate_row(int idx_row,int current,int n,vector<int>&row_hint, vector<int> pattern, vector< vector< vector<int> > > &row_pattern)//according to the row hint to generate_row
{
    
    if (row_hint.empty())
    {
        // cout<<"empty"<<endl;
        vector<int> tmp;
        for(int i =0;i<n;i++)
        {
            tmp.push_back(0);
        }
        row_pattern[idx_row].push_back(tmp);
        for(int i=0;i<n;i++)
        {
            guess_board[idx_row][i]=0;
        }
        return;
    }
    else if(current>=row_hint.size())
    {
        // cout<<"current<size"<<endl;
        int space=n-pattern.size();
        if(space<0)
        {
            pattern.pop_back();
        }
        else
        {
            for(int i=0;i<space;i++)
            {
                pattern.push_back(0);
            }
        }
        
        row_pattern[idx_row].push_back(pattern);
        return;
    }
    else
    {   
        // cout<<"current:"<<current<<endl;
        int sum=0;//sum: # need to be 1
        for(int i=current;i<row_hint.size();i++)
        {
            sum+=row_hint[i];
        }
        int space=n-pattern.size()-sum+row_hint.size()-current-1;//row_hint-current-1 : necessary space between left pattern 
        // cout<<"space:"<<space<<endl;
        if(space<0){return;}
        for(int i=0;i<=space;i++)
        {
            vector<int>tmp(pattern.begin(),pattern.end());
            for(int j=0;j<row_hint[current];j++)
            {
                tmp.push_back(1);
            }
            tmp.push_back(0);
            generate_row(idx_row,current+1,n,row_hint,tmp,row_pattern);
            pattern.push_back(0);
        }
    }

}



void generate_col(int idx_col,int current,int n,vector<int>&col_hint, vector<int> pattern, vector< vector< vector<int> > > &col_pattern)//according to the row hint to generate_row
{
    
    if (col_hint.empty())
    {
        // cout<<"empty"<<endl;
        vector<int> tmp;
        for(int i =0;i<n;i++)
        {
            tmp.push_back(0);
        }
        col_pattern[idx_col].push_back(tmp);
        for(int i=0;i<n;i++)
        {
            guess_board[i][idx_col]=0;
        }
        return;
    }
    else if(current>=col_hint.size())
    {
        // cout<<"current<size"<<endl;
        int space=n-pattern.size();
        if(space<0)
        {
            pattern.pop_back();
        }
        else
        {
            for(int i=0;i<space;i++)
            {
                pattern.push_back(0);
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
            vector<int>tmp(pattern.begin(),pattern.end());
            for(int j=0;j<col_hint[current];j++)
            {
                tmp.push_back(1);
            }
            tmp.push_back(0);
            generate_col(idx_col,current+1,n,col_hint,tmp,col_pattern);
            pattern.push_back(0);
        }
    }

}



void guess(int &n,vector< vector< vector<int> > > &row_pattern, vector< vector< vector<int> > > &col_pattern)
{
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            int tmp=row_pattern[i][0][j];
            for(int k=1;k<row_pattern[i].size();k++)
            {
                if(tmp!=row_pattern[i][k][j])
                {
                    break;
                }
                else if (k+1>=row_pattern[i].size())
                {
                    guess_board[i][j]=tmp;
                }
            }

            tmp=col_pattern[i][0][j];
            for(int k=1;k<col_pattern[i].size();k++)
            {
                if(tmp!=col_pattern[i][k][j])
                {
                    break;
                }
                else if (k+1>=col_pattern[i].size())
                {
                    guess_board[j][i]=tmp;
                }
            }
        }
    }

    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            if(guess_board[i][j]!=-1)
            {
                eliminate(i,j,guess_board[i][j],row_pattern,col_pattern);
            }
        }
    }
}




int main(int argc, char * argv[])
{
    clock_t t1,t2;
    double totaltime=0.0;;
   
    t1=clock();
    ifstream file;
    
    string line;
    if(argc==1)
    {
        file.open("tcga2016-question.txt",ios::in);
    }
    else
    {
        file.open(argv[1],ios::in);
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
    
    vector<int>tmp;
    for(int i=0;i<n;i++)
    {
        tmp.push_back(-1);
    }
    for(int i=0;i<n;i++)
    {
        guess_board.push_back(tmp);
    }
    
    vector< vector< vector<int> > > hint_cols;
    vector< vector< vector<int> > > hint_rows;

    vector<vector<vector<int> > > row_pattern;
    vector<vector<vector<int> > > col_pattern;
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
            // if(count!=5)
            // {
            //     continue;
            // }
            vector<vector<int> > tmp;
            vector<int>tmpv;
            for (int i=0;i<n;i++)
            {
                
                row_pattern.push_back(tmp);
                col_pattern.push_back(tmp);
                generate_row(i,0,n,row[i],tmpv,row_pattern);
                generate_col(i,0,n,col[i],tmpv,col_pattern);
            }

            
            for(int i=0;i<n;i++)
            {
                guess(n,row_pattern,col_pattern);
            }
            double complex_row=1;
            double complex_col=1;
            for(int i=0;i<n/2;i++)
            {
                complex_row*=row_pattern[i].size();
                complex_col*=col_pattern[i].size();
            }

            
            cout<<"prob:"<<count<<endl;
            cout<<"guessboard:"<<endl;
            for(int i=0;i<n;i++)
            {
                for(int j=0;j<n;j++)
                {
                    if(guess_board[i][j]!=-1)
                    {
                        cout<<guess_board[i][j];
                    }
                    else
                    {
                        cout<<"*";
                    }
                }
                cout<<endl;
            }
            cout<<"complex_row"<<complex_row<<endl;
            cout<<"complex_col"<<complex_col<<endl;
            vector<vector<int> > board;
            
            
           
            DFS(0,n,col,board,row_pattern,col_pattern);
            
            //clear pattern for next problem
            row_pattern.clear();
            col_pattern.clear();
            for(int i=0;i<n;i++)
            {
                for(int j=0;j<n;j++)
                {
                    guess_board[i][j]=-1;
                }
                
            }
            // guess_board.clear();
            t2=clock();
            cout<<"subtime:"<<(t2-t1)/(double)(CLOCKS_PER_SEC)<<endl;
            totaltime+=(t2-t1)/(double)(CLOCKS_PER_SEC);
            t1=t2;
            // break;
        }    
        // cout<<ss;
    }
    cout<<endl<<"time:"<<totaltime<<endl;  
}

 