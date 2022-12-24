#include "ypglpk.hpp"
#include <iostream>
#include<vector>
using namespace std;

int m = 20000;
int n = 10000;

std::vector<std::vector<double>> A(m, std::vector<double>(n));
std::vector<double> b(m), c(n);
std::vector<int> type(n);

struct path{
    int begin;
    int end;
};

struct path table[100];
int count = 0;
int variable = 9000;

std::pair<double, std::vector<double>> res;

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
        int num = (table[v].begin) * 100 + table[v].end;
        A[count][v] = 1;
        A[count][num] = -1;
        A[count+1][v] = -1;
        A[count+1][num] = 1;
        b[count] = 0;
        b[count+1] = 0;
        count += 2;
    }

    // //each point inside - outside = 0
    // for(int v = 1; v <= vertex; v++){
    //     //inside
    //     for(int i = 1; i <= vertex; i++){
    //         int num = i * 100 + v;
    //         A[count][num] = 1;
    //         A[count+1][num] = -1;
    //     }
    //     //outside
    //     for(int o = 1; o <= vertex; o++){
    //         int num = v * 100 + o;
    //         A[count][num] = -1;
    //         A[count+1][num] = 1;
    //     }
    //     if (v == 1){
    //         b[count] = -1;
    //         b[count+1] = -1;
    //     }
    //     else if(v == vertex){
    //         b[count] = 1;
    //         b[count+1] = 1;
    //     }
    //     else{
    //         b[count] = 0;
    //         b[count+1] = 0;
    //     }
        
    //     count += 2;
    // }

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
            b[count] = 9999;
            count += 1;
        }
    }

    for(int i = 0; i < 9000; i++){
        type[i] = GLP_BV;
    }
    for(int i = 9000; i < 10000; i++){
        type[i] = GLP_CV;
    }

    res = ypglpk::mixed_integer_linear_programming(A,b,c,type);
    if (res.first == -ypglpk::INF) {
        cout << -1 << '\n';
    }
    else {
        cout << (long long)res.first << '\n';
        for(int i = 0; i < edge; i++){
            cout << res.second[i];
        }
    }
    cout << '\n';
}