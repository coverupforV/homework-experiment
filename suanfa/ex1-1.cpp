#include <iostream>
#include <vector>

using namespace std;

typedef vector<vector<int>> matrix;

matrix add(const matrix& A, const matrix& B) {
    int n = A.size(), m = A[0].size();
    matrix C(n, vector<int>(m));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
    return C;
}

matrix subtract(const matrix& A, const matrix& B) {
    int n = A.size(), m = A[0].size();
    matrix C(n, vector<int>(m));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            C[i][j] = A[i][j] - B[i][j];
        }
    }
    return C;
}

matrix multiply(const matrix& A, const matrix& B) {
    int n = A.size();
    matrix C(n, vector<int>(n));
    if (n == 1) {
        C[0][0] = A[0][0] * B[0][0];
    } else {
        int m = n / 2;

        matrix A11(m, vector<int>(m));
        matrix A12(m, vector<int>(m));
        matrix A21(m, vector<int>(m));
        matrix A22(m, vector<int>(m));
        matrix B11(m, vector<int>(m));
        matrix B12(m, vector<int>(m));
        matrix B21(m, vector<int>(m));
        matrix B22(m, vector<int>(m));

        
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < m; j++) {
                A11[i][j] = A[i][j];
                A12[i][j] = A[i][j + m];
                A21[i][j] = A[i + m][j];
                A22[i][j] = A[i + m][j + m];

                B11[i][j] = B[i][j];
                B12[i][j] = B[i][j + m];
                B21[i][j] = B[i + m][j];
                B22[i][j] = B[i + m][j + m];
            }
        }

        
        matrix S1 = subtract(B12, B22);
        matrix S2 = add(A11, A12);
        matrix S3 = add(A21, A22);
        matrix S4 = subtract(B21, B11);
        matrix S5 = add(A11, A22);
        matrix S6 = add(B11, B22);
        matrix S7 = subtract(A12, A22);
        matrix S8 = add(B21, B22);
        matrix S9 = subtract(A11, A21);
        matrix S10 = add(B11, B12);

        
        matrix P1 = multiply(A11, S1);
        matrix P2 = multiply(S2, B22);
        matrix P3 = multiply(S3, B11);
        matrix P4 = multiply(A22, S4);
        matrix P5 = multiply(S5, S6);
        matrix P6 = multiply(S7, S8);
        matrix P7 = multiply(S9, S10);


    matrix C11 = add(subtract(add(P5, P4), P2), P6);
    matrix C12 = add(P1, P2);
    matrix C21 = add(P3, P4);
    matrix C22 = subtract(subtract(add(P5, P1), P3), P7);

   
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            C[i][j] = C11[i][j];
            C[i][j + m] = C12[i][j];
            C[i + m][j] = C21[i][j];
            C[i + m][j + m] = C22[i][j];
        }
    }
    
    
    cout << "P1: " << endl;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            cout << P1[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;

    cout << "P2: " << endl;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            cout << P2[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;

    cout << "P3: " << endl;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            cout << P3[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;

    cout << "P4: " << endl;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            cout << P4[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;

    cout << "P5: " << endl;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            cout << P5[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;

    cout << "P6: " << endl;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            cout << P6[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;

    cout << "P7: " << endl;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            cout << P7[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}
return C;
}

int main(){
    matrix A(16, vector<int>(16, 1));
    matrix B(16, vector<int>(16, 1));

    matrix C = multiply(A, B);

    cout << "Result Matrix C:" << endl;
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++){
            cout << C[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}
