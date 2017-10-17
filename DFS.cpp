#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#define unsigned int uint

using namespace std;

vector<vector<vector<int> > > col_comb;
vector<vector<vector<int> > > row_comb;

void gencol(int idx, int cur,int n, vector<int> &comb, vector<int> result){
    if(cur >= comb.size()){
        vector<int> vv (result.begin(),result.end());
        if(vv.size()<n){
            int tempn = n-vv.size();
            for(int i=0;i<tempn;i++){
                vv.push_back(0);
            }
        }
        else if(vv.size()>n){
            vv.pop_back();
        }
        col_comb[idx].push_back(vv);
        return;
    }
    int sum=0; //min space needed
    for(int i=cur; i<comb.size();i++){
        sum+=comb[i];
    }
    sum += comb.size()-cur-1; //the space in between
    int space = n-result.size()-sum;
    if(space < 0) return;
    for(int i=0; i<=space; i++){
        vector<int> tempvv(result.begin(),result.end());
        for(int j=0;j<comb[cur];j++){
            tempvv.push_back(1);
        }
        tempvv.push_back(0);
        gencol(idx,cur+1,n,comb,tempvv);
        result.push_back(0);
    }
}

void genrow(int idx, int cur,int n, vector<int> &comb, vector<int> result){
    if(cur >= comb.size()){
        vector<int> vv (result.begin(),result.end());
        if(vv.size()<n){
            int tempn = n-vv.size();
            for(int i=0;i<tempn;i++){
                vv.push_back(0);
            }
        }
        else if(vv.size()>n){
            vv.pop_back();
        }
        row_comb[idx].push_back(vv);
        return;
    }
    int sum=0; //min space needed
    for(int i=cur; i<comb.size();i++){
        sum+=comb[i];
    }
    sum += comb.size()-cur-1; //the space in between
    int space = n-result.size()-sum;
    if(space < 0) return;
    for(int i=0; i<=space; i++){
        vector<int> tempvv(result.begin(),result.end());
        for(int j=0;j<comb[cur];j++){
            tempvv.push_back(1);
        }
        tempvv.push_back(0);
        genrow(idx,cur+1,n,comb,tempvv);
        result.push_back(0);
    }
}


int check(int n, vector<vector<int> > &row, vector<vector<int> > board){
    int num = board.size();
    for(int i=0;i<num;i++){
        int ptr=0;
        int count1=0;
        int count_n=0;
        while(ptr<num){
            if(board[ptr][i] == 1){
                count1++;
                ptr++;
            }
            else{
                if(count1 == 0){
                    ptr++;
                    continue;
                }
                if(count_n >= row[i].size()){
                    //too many black fragment
                    return 0;
                }
                if(count1 == row[i][count_n]){
                    count1 = 0;
                    count_n++;
                    ptr++;
                }
                else return 0;
            }
        }
        if(num<n && count1!=0){
            if(count1>row[i][count_n]) return 0;
            int s=0;
            for(int k=count_n;k<row[i].size();k++){
                s+=row[i][k];
            }
            s+= (row[i].size()-count_n-1);
            if(count1+(n-num)<s ) return 0;
        }
        if(num<n && count1!=0 && (count1+(n-num))<row[i][count_n]) return 0;
        if(num==n && count1!=0){
            if(count_n >= row[i].size()){
                //too many black fragment
                return 0;
            }
            if(count_n < row[i].size()-1){
                return 0;
            }
            if(count1 != row[i][count_n]){
                return 0;
            }
        }
        else if(num==n && count_n != row[i].size()) return 0;
    }
    if(num < n) return 1;
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++)
            cout << board[i][j]<<" ";
        cout <<endl;
    }
    return 1;
}

int DFS(int cur, int n, vector<vector<int> > &row, vector<vector<int> > board){
    if(check(n,row,board) == 0)
        return 0;
    else if(cur >= n)
        return 1;
    int ans = 0;
    for(int i=0;i<col_comb[cur].size();i++){
        board.push_back(col_comb[cur][i]);
        ans = DFS(cur+1,n,row,board);
        if(ans == 1){
            break;
        }
        board.pop_back();
    }
    return ans;
}

int main(int argc, char* argv[]){
    int n;
    string filename;
    stringstream(argv[1]) >> n;
    stringstream(argv[2]) >> filename;
    ifstream infile(filename);
    string line;
    int prob=0;
    int c,r;
    while (getline(infile, line))
    {
        vector<vector<int> > col;
        vector<vector<int> > row;
        vector<vector<int> > board;
        if(line[0] != '$'){
            cout << "error format"<<endl;
            break;
        }
        cout <<line<<endl;
        for(int i = 0; i < n; i++){
            vector<int> temp_row;
            getline(infile, line);
            stringstream ss(line);
            while(ss >> r){
                temp_row.push_back(r);
            }
            row.push_back(temp_row);
        }
        for(int i = 0; i < n; i++){
            vector<int> temp_col;
            getline(infile, line);
            stringstream ss(line);
            while(ss >> c){
                temp_col.push_back(c);
            }
            col.push_back(temp_col);
        }
        vector<int> tempv;
        for(int i=0;i<n;i++){
            vector<vector<int> > tempc;
            vector<vector<int> > tempr;
            col_comb.push_back(tempc);
            row_comb.push_back(tempr);
        }
        for(int i=0;i<n;i++){
            gencol(i,0,n,col[i],tempv);
            genrow(i,0,n,row[i],tempv);
        }
        int tempans=0;
        if(DFS(0,n,row,board) == 0){
            cout << "No solution!!"<<endl;
        }
        col_comb.clear();
        row_comb.clear();

    }

    return 0;
}
