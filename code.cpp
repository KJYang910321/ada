#include "ypglpk.hpp"
#include <iostream>
#include<vector>
using namespace std;

vector<vector<double>> A;
vector<double> b;
vector<double> c;
vector<int> type;
struct path table[100];
int count = 0;
int variable = 9000;

std::pair<double, std::vector<double>> res;

struct path{
    int begin;
    int end;
};

int main(){

    ypglpk::set_output(true);

    int vertex, edge;
    cin >> vertex >> edge;
    for(int i = 0; i < edge; i++){
        int begin, end, value;
        cin >> begin >> end >> value;
        table[i].begin = begin;
        table[i].end = end;
        c[i] = value;
    }

    //connect path with point
    for(int v = 0; v < edge; v++){
        int num = table[v].begin * 100 + table[v].end;
        A[count][v] = 1;
        A[count][num] = -1;
        A[count+1][v] = -1;
        A[count+1][num] = 1;
        b[count] = 0;
        b[count+1] = 0;
        type[count] = GLP_BV;
        type[count+1] = GLP_BV;
        count += 2;
    }

    //each point inside - outside = 0
    for(int v = 1; v <= vertex; v++){
        //inside
        for(int i = 1; i <= vertex; i++){
            if(v != i){
                int num = i * 100 + v;
                A[count][num] = 1;
                A[count+1][num] = -1;
            }
        }
        //outside
        for(int o = 1; o <= vertex; o++){
            if(v != o){
                int num = v * 100 + o;
                A[count][num] = -1;
                A[count+1][num] = 1;
            }
        }
        if (v == 1){
            b[count] = -1;
            b[count+1] = -1;
        }
        else if(v == vertex){
            b[count] = 1;
            b[count+1] = 1;
        }
        else{
            b[count] = 0;
            b[count+1] = 0;
        }
        type[count] = GLP_BV;
        type[count+1] = GLP_BV;
        
        count += 2;
    }

    //output path <= 1
    for(int v = 1; v <= vertex; v++){
        for(int o = 1; o <= vertex; o++){
            if(v != o){
                int num = 100 * v + o;
                A[count][num] = 1;
            }
        }
        if(v == vertex){
            b[count] = 0;
        }
        else{
            b[count] = 1;
        }
        type[count] = GLP_BV;
        count += 1;

    }
    //variable to avoid cycle
    for(int i = 1; i <= vertex; i++){
        for(int j = 1; j <= vertex; j++){
            if(i != j){
                int num = 100 * i + j;
                A[count][variable+i] = 1;
                A[count][variable+j] = -1;
                A[count][num] = 10000;
            }
            B[count] = 9999;
            type[count] = GLP_CV;
            count += 1;
        }
    }
    res = ypglpk::mixed_integer_linear_programming(A,b,c,type);
    if (res.first == -ypglpk::INF) {
        cout << -1 << '\n';
    }
    else {
        cout << (long long)res.first << '\n';
        for(int i = 0; i < edge; i++){
            cout << res.second[i]
        }
    }

}