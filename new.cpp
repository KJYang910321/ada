#include "ypglpk.hpp"
#include <iostream>
#include<vector>
using namespace std;

long long m = 20000;
long long n = 7000;

std::vector<std::vector<double>> A(m, std::vector<double>(n));
std::vector<double> b(m), c(n);
std::vector<int> type(n);

struct path{
    int number;
    int weight;
};

struct path table[100];
long long count = 0;
long long check[10000] = {0};

std::pair<double, std::vector<double>> res;

int main(){

    for(int i = 0; i < 10000; i++){
        check[i] = -1;
    }

    int vertex, edge;
    cin >> vertex >> edge;
    for(int e = 0; e < edge; e++){
        int start, end, value;
        cin >> start >> end >> value;
        int num = vertex * start + end;
        if (check[num] == -1){
            check[num] = value;
        }
        else{
            if (value >= check[num]){
                check[num] = value;
            }
        }
        table[e].number = num;
        table[e].weight = value;
    }
    
    int variable = vertex * vertex + vertex;

    //remove not possible edge first
    //clear cycle
    for(int v = 1; v <= vertex; v++){
        for(int w = 1; w <= vertex; w++){
            int key = v * vertex + w;
            if (check[key] == -1){
                A[count][key] = 1;
                b[count] = 0;
            }
            else{
                A[count][variable+v] = 1;
                A[count][variable+w] = -1;
                A[count][key] = 10000;
                b[count] = 9999;
                c[key] = check[key];
            }
            count++;
        }
    }
    
    //each vertex inside - outside = 0
    for(long long v = 1; v <= vertex; v++){
        //inside
        for(long long i = 1; i <= vertex; i++){
            long long num = i * vertex + v;
            A[count][num] = 1;
            A[count+1][num] = -1;
        }
        //outside
        for(long long o = 1; o <= vertex; o++){
            long long num = v * vertex + o;
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
                long long num = vertex * v + o;
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

    for(int v = 0; v <= variable; v++){
        type[v] = GLP_BV;
    }
    for(int v = variable+1; v < 7000; v++){
        type[v] = GLP_CV;
    }

    res = ypglpk::mixed_integer_linear_programming(A,b,c,type);
    if (res.first == -ypglpk::INF) {
        cout << -1 << '\n';
    }
    else {
        cout << (long long)res.first << '\n';
        for(long long i = 0; i < edge; i++){
            int line = table[i].number;
            int vv = table[i].weight;
            if(vv == check[line] && res.second[line] == 1){
                cout << 1;
            }
            else{
                cout << 0;
            }
        }
    }
    cout << '\n';


}