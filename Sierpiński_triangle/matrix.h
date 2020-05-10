#pragma once
#include <vector>
#include <iostream>
using namespace std;
class matrix
{
public:
    vector< vector<float> > m;
    int r, c;
    matrix(int _r, int _c) {
        this->r = _r;
        this->c = _c;
        m.resize(r);
        for (int i = 0; i < r; i++)
        {
            m[i].resize(c);
        }
    }

    matrix(vector< vector<float> > _m) {
        m = _m;
        this->r = _m.size();
        this->c = _m[0].size();
    }

    matrix mult(matrix n)
    {
        matrix res(this->r, n.c);
        for (int i = 0; i < this->r; i++)
        {
            for (int j = 0; j < n.c; j++)
            {
                res.m[i][j] = 0;
                for (int k = 0; k < this->c; k++)
                {
                    res.m[i][j] = res.m[i][j] + m[i][k] * n.m[k][j];
                }
            }
        }

        return res;
    }

    void print() {

        for (int i = 0; i < this->r; i++)
        {
            for (int j = 0; j < this->c; j++)
            {
                cout << m[i][j] << '\t';
            }
            cout << endl;
        }

        cout << endl;
    }

};





