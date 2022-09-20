/*******************************************
 * Name: Hill.cpp
 * Author: kongtaoxing
 * Powered by Beijingjiaotong University
 ******************************************/
#include<iostream>
#include<string>
#include<vector>
#include<math.h>
using namespace std;

#define mod(a) (((a % 26) + 26) % 26)  //把负数的模计算成正数

int divmod(int a, int b) {   //模26除法
    for(int i = 0; i < 10000; i++) {
        if((a + 26 * i) % b == 0) {
            return mod((a + 26 * i) / b);
        }
    }
    return 0;   //其实是10000以内没有找到🤣
}

string encrypt(string s, vector<int> arr) {    //加密函数
    vector<int> arrs;
    vector<int> ret(s.length(), 0);
    for(int i = 0; i < s.length(); i++)
        arrs.push_back(s[i] - 65);
    int len = sqrt(arr.size());
    for(int i = 0; i < s.length() / len; i++)
        for(int j = 0; j < len; j++)
            for(int k = 0; k < len; k++)
                ret[len * i + j] += (arrs[len * i + k]) * arr[k * len + j];
    for(int i = 0; i < s.length(); i++){
        ret[i] %= 26;
        if(ret[i] < 0) ret[i] += 26;
        s[i] = ret[i] + 65;
        }
    return s;
}

int det(vector<int> arr, int len) {    //行列式求值
    int ans = 0;
    if(len == 1) ans = arr[0];
    else {
        vector<int> yuzi((len - 1) * (len - 1));   //创建余子式矩阵
        int move = 0;
        for(int i = 0; i < len; i++) {
            for(int j = 0; j < len - 1; j++) {
                move = i > j ? 0 : 1;
                for(int k = 0; k < len - 1; k ++) {
                    yuzi[j * (len - 1) + k] = arr[(j + move) * len + k + 1];
                }
            }
            int flag = (i % 2 == 0 ? 1 : -1);
            ans += flag * arr[i * len] * det(yuzi, len - 1);
        }
    }
    return mod(ans);
}

vector<int> inverse(vector<int> arr) {   //求mod26的逆矩阵
    vector<int> inver;
    int len = sqrt(arr.size());
    if(det(arr, len) == 0) cout << "秘钥矩阵的行列式值为0，无法解密" << endl;
    int _det = det(arr, len);
    vector<int> bansui(len * len);   //伴随矩阵
    vector<int> yuzi((len - 1) * (len - 1));    //余子式
    int pi, pj, q;
    for(int ai = 0; ai < len; ai++) {
        for(int aj = 0; aj < len; aj++) {
            for(int bi = 0; bi < len - 1; bi++) {
                for(int bj = 0; bj < len - 1; bj++) {
                    if(ai > bi) pi = 0;
                    else pi = 1;
                    if(aj > bj) pj = 0;
                    else pj = 1;
                    yuzi[bi * (len - 1) + bj] = arr[(bi + pi) * len + bj + pj];
                }
            }
            if((ai + aj) % 2 == 0) q = 1;
            else q = -1;
            bansui[ai * len + aj] = q * det(yuzi, len - 1);
        }
    }
    for(int i = 0; i < len; i++) {
        for(int j = 0; j < len;j++) {
            arr[i * len + j] = divmod(bansui[i * len + j], _det);
            inver.push_back(mod(arr[i * len + j]));
        }
    }
    for(int i = 0; i < len; i++) {   //转置一下输出顺序
        for(int j = 0; j < i; j++) {
            int temp = inver[i * len + j];
            inver[i * len + j] = inver[ j * len + i];
            inver[j * len + i] = temp;
        }
    }
    return inver;
}

string decrypt(string s, vector<int> arr) {     //解密函数
    vector<int> arr_in = inverse(arr);
    vector<int> arrs;
    vector<int> ret(s.length(), 0);
    for(int i = 0; i < s.length(); i++)
        arrs.push_back(s[i] - 65);
    int len = sqrt(arr_in.size());
    for(int i = 0; i < s.length() / len; i++)
        for(int j = 0; j < len; j++)
            for(int k = 0; k < len; k++)
                ret[len * i + j] += (arrs[len * i + k]) * arr_in[k * len + j];
    for(int i = 0; i < s.length(); i++){
        ret[i] %= 26;
        if(ret[i] < 0) ret[i] += 26;
        s[i] = ret[i] + 65;
        }
    return s;
}

int main() {
	cout << "你想加密还是解密，加密输入e，解密输入d：";
	char c;
	cin >> c;
	cout << "\n请输入需要加/解密的字母串：";
	string s;
	cin >> s;
	for (int i = 0; i < s.length(); i++) {
		if (s[i] >= 97 && s[i] <= 122) {
			s[i] -= 32;
		}
	}
	cout << "\n请输入秘钥数组（将矩阵按行序全部输入到本行）：";   //采用一维数组储存二维数组方便计算不同大小的矩阵
	vector<int> arr;
    int n;
	while(cin >> n){
        arr.push_back(n);
        if(cin.get() == '\n')
            break;
    }
    double num=sqrt(arr.size());
    if(det(arr, num) == 0) {
        cout << "秘钥矩阵的行列式值为0，无法解密" << endl;
        return 0;
    }
    if(ceil(num) == num && s.length() % int(num) == 0) {
        //cout << arr.size() << endl;
        if (c == 'e') cout << "\n加密后的字符串为：" << encrypt(s, arr) << endl;
        else if (c == 'd') cout << "\n解密后的字符串为：" << decrypt(s, arr) << endl;
        else cout << "你的输入有误，请重新输入！" << endl;
    }
    else
        cout << "数组长度不正确，请检查后重新输入！" << endl;
    system("pause");
	return 0;
}