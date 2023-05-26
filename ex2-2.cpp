#include <iostream>
using namespace std;

/*求解最长共同子序列*/
void LCS(string A, string B)
{
    int m = A.size();
    int n = B.size();
    int **C = new int *[m + 1];

    for (int i = 0; i <= m; i++) {
        C[i] = new int[n + 1];
        C[i][0] = 0;
    }
    for (int j = 0; j <= n; j++) {
        C[0][j] = 0;
    }

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (A[i - 1] == B[j - 1]) {
                C[i][j] = C[i - 1][j - 1] + 1;
            } else if (C[i][j - 1] >= C[i - 1][j]) {
                C[i][j] = C[i][j - 1];
            } else {
                C[i][j] = C[i - 1][j];
            }
        }
    }

    int len = C[m][n];
    cout << "共同子序列的长度为：" << len << endl;

    string common;
    common.resize(len);

    int i = m, j = n, k = len - 1;
    while (i > 0 && j > 0) {
        if (A[i - 1] == B[j - 1]) {
            common[k--] = A[i - 1];
            i--;
            j--;
        } else if (C[i][j - 1] >= C[i - 1][j]) {
            j--;
        } else {
            i--;
        }
    }

    cout << "共同子序列为：" << common << endl;

    for (int i = 0; i <= m; i++) {
        delete[] C[i];
    }
    delete[] C;
}

int main()
{
    string A = "ACCGGTCGAGATGCAG";
    //string B = "GTCGTTCGGAATGCAT";
    string B = "ACCGGTCGAGATGCAG";

    LCS(A, B);

    return 0;
}
