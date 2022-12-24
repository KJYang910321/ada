#include "ypglpk.hpp"
#include <iostream>
#include<vector>
using namespace std;

long long m = 10000;
long long n = 10000;
long long maxi = -100000000;

std::vector<std::vector<double>> A(m, std::vector<double>(n));
std::vector<double> b(m), c(n);
std::vector<int> type(n);

struct path{
    long long begin;
    long long end;
    long long len;
};

struct path table[100];
long long count = 0;
long long variable = 9000;
long long check[10000] = {0};
long long check2[10000] = {0};

std::pair<double, std::vector<double>> res;

int main(){

    //ypglpk::set_output(true);

    long long vertex, edge;
    cin >> vertex >> edge;
    for(long long i = 0; i < edge; i++){
        long long begin, end, value;
        cin >> begin >> end >> value;
        table[i].begin = begin;
        table[i].end = end;
        long long ck = 100 * begin + end;
        if(check[ck] == 0){
            check[ck] = value;
        }
        else{
            if (value >= check[ck]){
                check[ck] = value;
            }
        }
        table[i].len = value;

        
    }

    //connect path with polong long
    for(long long v = 0; v < edge; v++){
        long long num = (table[v].begin) * 100 + table[v].end;
        long long weight = table[v].len;
        if (weight == check[num] && check2[num] == 0){
            A[count][v] = 1;
            A[count][num] = -1;
            A[count+1][v] = -1;
            A[count+1][num] = 1;
            b[count] = 0;
            b[count+1] = 0;
            c[v] = weight;
            count += 2;
            check2[num] = 1;
        }
        else{
            A[count][v] = 1;
            A[count+1][v] = -1;
            b[count] = 0;
            b[count+1] = 0;
            c[v] = -1;
            count += 2;
        }
    }

    //each polong long inside - outside = 0
    for(long long v = 1; v <= vertex; v++){
        //inside
        for(long long i = 1; i <= vertex; i++){
            long long num = i * 100 + v;
            A[count][num] = 1;
            A[count+1][num] = -1;
        }
        //outside
        for(long long o = 1; o <= vertex; o++){
            long long num = v * 100 + o;
            A[count][num] = -1;
            A[count+1][num] = 1;
        }
        if (v == 1){
            b[count] = -1;
            b[count+1] = 1;
        }
        else if(v == vertex){
            b[count] = 1;
            b[count+1] = -1;
        }
        else{
            b[count] = 0;
            b[count+1] = 0;
        }
        
        count += 2;
    }

    //output path <= 1
    for(long long v = 1; v <= vertex; v++){
        for(long long o = 1; o <= vertex; o++){
            if(v != o){
                long long num = 100 * v + o;
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
    for(long long i = 1; i <= vertex; i++){
        for(long long j = 1; j <= vertex; j++){
            if(i != j){
                long long num = 100 * i + j;
                A[count][variable+i] = 1;
                A[count][variable+j] = -1;
                A[count][num] = 10000;
            }
            b[count] = 9999;
            count += 1;
        }
    }

    for(long long i = 0; i < 9000; i++){
        type[i] = GLP_BV;
    }
    for(long long i = 9000; i < 10000; i++){
        type[i] = GLP_CV;
    }

    for(int cv = 101; cv < 9000; cv++){
        if(check[cv] == 0){
            c[cv] = maxi;
        }
    }

    res = ypglpk::mixed_integer_linear_programming(A,b,c,type);
    if (res.first == -ypglpk::INF) {
        cout << -1 << '\n';
    }
    else {
        cout << (long long)res.first << '\n';
        for(long long i = 0; i < edge; i++){
            cout << res.second[i];
        }
    }
    cout << '\n';
}