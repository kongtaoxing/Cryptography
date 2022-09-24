/*******************************************
 * Name: myDES.cpp
 * Author: kongtaoxing
 * Powered by Beijingjiaotong University
 ******************************************/
#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<algorithm>
using namespace std;

// IP置换表
int IP[64] =  { 
                57,  49,  41,  33,  25,  17,   9,   1,   
                59,  51,  43,  35,  27,  19,  11,   3,   
                61,  53,  45,  37,  29,  21,  13,   5,   
                63,  55,  47,  39,  31,  23,  15,   7,   
                56,  48,  40,  32,  24,  16,   8,   0,   
                58,  50,  42,  34,  26,  18,  10,   2,   
                60,  52,  44,  36,  28,  20,  12,   4,   
                62,  54,  46,  38,  30,  22,  14,   6
               };

// IP逆置换表
int IPni[64] = {
                39,   7,  47,  15,  55,  23,  63,  31,   
                38,   6,  46,  14,  54,  22,  62,  30,   
                37,   5,  45,  13,  53,  21,  61,  29,   
                36,   4,  44,  12,  52,  20,  60,  28,   
                35,   3,  43,  11,  51,  19,  59,  27,   
                34,   2,  42,  10,  50,  18,  58,  26,   
                33,   1,  41,   9,  49,  17,  57,  25,   
                32,   0,  40,   8,  48,  16,  56,  24
               };  

//S盒  
int S[8][4][16] = {
    //S1
    {
        {14,  4, 13, 1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9, 0,  7},   
        { 0, 15,  7, 4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5, 3,  8},   
        { 4,  1, 14, 8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10, 5,  0},   
        {15, 12,  8, 2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0, 6, 13}
    },   
    //S2  
    {
        {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},   
        {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},   
        {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},   
        {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}
    },   
    //S3  
    {
        {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},   
        {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},   
        {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},   
        {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}
    },   
    //S4  
    {
        {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},   
        {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},   
        {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},   
        {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}
    },   
    //S5  
    {
        {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},   
        {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},   
        {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},   
        {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}
    },   
    //S6  
    {
        {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},   
        {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},   
        {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},   
        {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}
    },   
    //S7  
    {
        {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},   
        {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},   
        {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},   
        {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}
    },   
    //S8  
    {
        {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},   
        {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},   
        {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},   
        {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}
    }
};  

//扩充置换表E，用于把R从32位扩展到48位
int E[48] = {
             31,  0,  1,  2,  3,  4,   
			 3,   4,  5,  6,  7,  8,   
			 7,   8,  9, 10, 11, 12,   
			 11, 12, 13, 14, 15, 16,   
			 15, 16, 17, 18, 19, 20,   
			 19, 20, 21, 22, 23, 24,   
			 23, 24, 25, 26, 27, 28,   
			 27, 28, 29, 30, 31,  0
            };

//置换函数P
int P[32] = {
             15,  6, 19, 20, 28, 11, 27, 16,   
			  0, 14, 22, 25,  4, 17, 30,  9,   
			  1,  7, 23, 13, 31, 26,  2,  8,   
			 18, 12, 29,  5, 21, 10,  3, 24
            };

//置换选择PC1，用以置换密钥
int PC1[56] = {
                56, 48, 40, 32, 24, 16,  8,   
                 0, 57, 49, 41, 33, 25, 17,   
                 9,  1, 58, 50, 42, 34, 26,   
                18, 10,  2, 59, 51, 43, 35,   
                62, 54, 46, 38, 30, 22, 14,   
                 6, 61, 53, 45, 37, 29, 21,   
                13,  5, 60, 52, 44, 36, 28,   
                20, 12,  4, 27, 19, 11,  3
              };

//置换选择PC2，移位之后置换密钥
int PC2[56] = {
                13, 16, 10, 23, 0,  4,  2, 27,   
                14,  5, 20,  9, 22, 18, 11,  3,   
                25,  7, 15,  6, 26, 19, 12,  1,   
                40, 51, 30, 36, 46, 54, 29, 39,   
                50, 44, 32, 47, 43, 48, 38, 55,   
                33, 52, 45, 41, 49, 35, 28, 31
              };

//左移次数，用以移动密钥
int MOVE[16] = {
        1, 1, 2, 2, 2, 2, 2, 2,
        1, 2, 2, 2, 2, 2, 2, 1
        };

string encrypt(string p, string k);  //加密函数主函数
string LowToUpper(string low);  //小写字母转大写字母
string HextoBin(string Hex);  //十六进制转二进制
string BintoHex(string Bin);  //二进制转十六进制
vector<string> subKey(string k);  //子秘钥生成函数
string feistel(string R, string K);   //Feistel函数
string decrypt(string c, string k);   //解密函数

int main()
{
    string PATH_P, PATH_C, PATH_K = "";
    string p, c, k;
    cout << "请输入你想加密还是解密，加密为e，解密为d：";
    string ed;
    cin >> ed;
    cout << "\n请输入需要加解密的文件路径(路径中的单斜杠请输入为双斜杠，以下同)：";
    cin >> PATH_P;
    ifstream pFile(PATH_P, ios::in);
    ifstream kFile;
    if(!pFile.is_open()){
        cout << "\n打开明文/密文文件失败！" << endl;
        system("pause");
        return 0;
    }
    getline(pFile, p);   //读入明文/密文字符串
    if(p.length() % 16 != 0) {
        for(int i = 0; i < 16 - p.length() % 16; i++) {
            p += "0";       //不够加密的话填0补位，方便加密
        }
    }
    cout << "\n请选择准备输入密钥还是选择密钥文件，输入0选择输入密钥， 输入1选择输入密钥文件：";
    int miyao;
    cin >> miyao;
    if(miyao == 0) {
        cout << "\n请输入密钥：";
        cin >> k;
    }
    else {
        cout << "\n请输入密钥文件路径：";
        cin >> PATH_K;
        kFile.open(PATH_K, ios::in);
        if(!kFile.is_open()) {
            cout << "\n打开密钥文件失败！" << endl;
            system("pause");
            return 0;
        }
        getline(kFile, k);   //读入密钥字符串
        kFile.close();
        if(k.length() != 16) {
            cout << "\n请将密钥长度限制为16位！"  << endl;
            system("pause");
            return 0;
        }
    }
    if(ed == "e") {
        c = encrypt(p, k);
    }
    else if(ed == "d") {
        c = decrypt(p, k);
    }
    else {
        cout << "参数错误！" << endl;
        system("pause");
        return 0;
    }
    ofstream cFile("1_decry.txt",ios::out);
    cFile << c;
    pFile.close();
    cFile.close();
    cout << "\n密文/明文已保存至程序目录下的1_decry.txt" << endl;
    vector<string> K = subKey(k);
    // for(int i = 0; i < 16; i++) {
    //     cout << K[i] << endl;
    // }
    system("pause");
    return 0;
}

string encrypt(string p, string k){   //加密函数
    string pBin = HextoBin(p);
    vector<string> Key = subKey(k);
    // for(int i = 0; i < 16; i++) {
    //     cout << "K[" << i << "]: " <<Key[i] << endl;
    // }
    string ans = "";
    for(int i = 0; i < pBin.length() / 64; i++) {    //采用ECB方式
        string _P = pBin.substr(i * 64, 64);
        string pIP(64, '0');   //经IP变换之后的字符串
        for(int j = 0; j < 64; j++) {
            pIP[j] = _P[IP[j]];
        }
        vector<string> L(17, "00000000000000000000000000000000");
        vector<string> R(17, "00000000000000000000000000000000");
        L[0] = pIP.substr(0, 32);
        R[0] = pIP.substr(32, 32);
        cout << "L[0]: " << L[0] << " R[0]: " << R[0] <<endl;
        for(int j = 1; j <= 16; j++) {
            L[j] = R[j - 1];
            string F = feistel(R[j - 1], Key[j - 1]);
            cout << "F: " << F << endl;
            for(int k = 0; k < 32; k++){
                R[j][k] = (L[j - 1][k] - '0') ^ (F[k] - '0') + '0';
            }
            cout << "L[" << j << "]: " << L[j] << " R[" << j << "]: " << R[j] << endl;
        }
        string chaIPni = R[16] + L[16], enc(64, '0');
        // cout << chaIPni << endl;
        for(int j = 0; j < 64; j++) {   //IP逆置换
            enc[j] = chaIPni[IPni[j]];
        }
        ans += BintoHex(enc);
    }
    return ans;
}

string decrypt(string c, string k) {  //解密函数
    string cBin = HextoBin(c);
    vector<string> Key = subKey(k);
    reverse(Key.begin(), Key.end());
    string ans = "";
    for(int i = 0; i < cBin.length() / 64; i++) {   // 采用ECB模式解码
        string _C = cBin.substr(i * 64, 64);
        string cIP(64, '0');   // IP逆置换后的字符串
        for(int j = 0; j < 64; j++) {
            cIP[j] = _C[IPni[j]];
        }
        vector<string> L(17, "00000000000000000000000000000000");
        vector<string> R(17, "00000000000000000000000000000000");
        L[0] = cIP.substr(0, 32);
        R[0] = cIP.substr(32, 32);
        for(int j = 1; j <= 16; j++) {
            R[j] = L[j - 1];
            string F = feistel(L[j - 1], Key[j - 1]);
            for(int k = 0; k < 32; k++) {
                L[j][k] = (R[j - 1][k] - '0') ^ (F[k] - '0') + '0';
            }
        }
        string chaIP = L[16] + R[16], enc(64, '0');
        for(int j = 0; j < 64; j++) {
            enc[j] = chaIP[IP[j]];
        }
        ans += BintoHex(enc);
    }
    return ans;
}

string LowToUpper(string low) {
    for(int i = 0; i < low.length(); i++) {
        if(low[i] >= 'a' && low[i] <= 'z') {
            low[i] -= 32;
        }
    }
    return low;
}

string HextoBin(string Hex) {
    string Bin = "";
    for(int i = 0; i < Hex.length(); i++) {
        if(Hex[i] == '0') Bin += "0000";
        else if(Hex[i] == '1') Bin += "0001";
        else if(Hex[i] == '2') Bin += "0010";
        else if(Hex[i] == '3') Bin += "0011";
        else if(Hex[i] == '4') Bin += "0100";
        else if(Hex[i] == '5') Bin += "0101";
        else if(Hex[i] == '6') Bin += "0110";
        else if(Hex[i] == '7') Bin += "0111";
        else if(Hex[i] == '8') Bin += "1000";
        else if(Hex[i] == '9') Bin += "1001";
        else if(Hex[i] == 'A') Bin += "1010";
        else if(Hex[i] == 'B') Bin += "1011";
        else if(Hex[i] == 'C') Bin += "1100";
        else if(Hex[i] == 'D') Bin += "1101";
        else if(Hex[i] == 'E') Bin += "1110";
        else Bin += "1111";
    }
    return Bin;
}

string BintoHex(string Bin) {
    string Hex = "";
    for(int i = 0; i < Bin.length() / 4; i++) {
        int num = (Bin[4 * i] - '0') * 8 + (Bin[4 * i + 1] - '0') * 4 + (Bin[4 * i + 2] - '0') * 2 + (Bin[4 * i + 3] - '0');
        if(0 <= num && num <= 9) Hex += (num + '0');
        else if(num == 10) Hex += "A";
        else if(num == 11) Hex += "B";
        else if(num == 12) Hex += "C";
        else if(num == 13) Hex += "D";
        else if(num == 14) Hex += "E";
        else Hex += "F";
    }
    return Hex;
}

vector<string> subKey(string k) {
    string kBin = HextoBin(k);
    string kBin1(56, '0');
    vector<string> C(16, "0000000000000000000000000000"); 
	vector<string> D(16, "0000000000000000000000000000");
	vector<string> K(16, "000000000000000000000000000000000000000000000000");
	vector<string> K1(16, "000000000000000000000000000000000000000000000000");
	for(int i = 0; i < 56; i++) {
        kBin1[i] = kBin[PC1[i]];  //经过PC1置换丢弃8位
    }
    string C0 = kBin1.substr(0,28);
    string D0 = kBin1.substr(28,28);
    for(int i = 0; i < 27; i++) {
        C[0][i] = C0[i + 1];
        D[0][i] = D0[i + 1];
    }
    C[0][27] = C0[0]; D[0][27] = D0[0];
    K[0] = C[0] + D[0];
    for(int i = 1; i < 16; i++) {  //计算16轮密钥
        string tempC = C[i - 1].substr(0, MOVE[i]);
        string tempD = D[i - 1].substr(0, MOVE[i]);
        for(int j = 0; j < 28 - MOVE[i]; j++) {
            C[i][j] = C[i - 1][j + MOVE[i]];
            D[i][j] = D[i - 1][j + MOVE[i]];
        }
        for(int j = 0; j < MOVE[i]; j++) {
            C[i][28 - MOVE[i] + j] = tempC[j];
            D[i][28 - MOVE[i] + j] = tempD[j];
        }
        K[i] = C[i] + D[i];
    }
	for(int i = 0; i< 16;i++){
		for(int j = 0; j < 48; j++){  //经过PC2丢弃8位
			K1[i][j] = K[i][PC2[j]];
		}
	}
    return K1;
}

string feistel(string R, string K) {   //轮函数
    string _E(48, '0');  //扩展后的R
    string F(48, '0');
    string ret = "";
    cout << "(E)K: " << K << endl;
    for(int i = 0; i < 48; i++) {
        _E[i] = R[E[i]];
        F[i] = (_E[i] - '0') ^ (K[i] - '0') + '0';
    }
    cout << "E(R): " << _E << endl;
    cout << "E(R) XOR K = " << F <<endl;
    for(int i = 0; i < 8; i++) {  // 经过S盒变换
        int x = (F[i * 6] - '0') * 2 + (F[i * 6 + 5] - '0');
        int y = (F[i * 6 + 1] -'0') * 8 + (F[i * 6 + 2] - '0') * 4 + (F[i * 6 + 3] - '0') * 2 + (F[i * 6 + 4] - '0');
        if(S[i][x][y] >= 0 && S[i][x][y] <= 9) ret += (S[i][x][y] + '0');
        else if(S[i][x][y] == 10) ret += "A";
        else if(S[i][x][y] == 11) ret += "B";
        else if(S[i][x][y] == 12) ret += "C";
        else if(S[i][x][y] == 13) ret += "D";
        else if(S[i][x][y] == 14) ret += "E";
        else ret +="F";
    }
    cout << "Right After S-Box: " << ret << endl;
    string chaP = HextoBin(ret);   //还差P变换
    cout << "Before P: " << chaP << endl;
    string ans(32, '0');
    for(int i = 0; i < 32; i++) {
        ans[i] = chaP[P[i]];
    }
    cout << "f: " << ans << endl;
    return ans;
}
