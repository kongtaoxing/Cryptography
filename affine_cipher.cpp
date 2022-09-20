/*******************************************
 * Name: affine_cipher.cpp
 * Author: kongtaoxing
 * Powered by Beijingjiaotong University
 ******************************************/
#include<iostream>
#include<string>
using namespace std;

int chengfaniyuan(int num){
    for( int i = 0; i < 26; i++){
        if( num * i % 26 == 1){
            return i;
        }
    }
}

string encrypt(string s, int a, int b) {
	for (int i = 0; i < s.length(); i++) {
		s[i] = (a * s[i] - 65 + b) % 26;
		if (s[i] < 0) s[i] += 26;            //C语言-2%26竟然等于-2，不够26需要手动加一下
		s[i] += 65;
	}
	return s;
}

string decrypt(string s, int a, int b) {
	for (int i = 0; i < s.length(); i++) {
		s[i] = (chengfaniyuan(a) * (s[i] - 65 - b)) % 26;
		if (s[i] < 0) s[i] += 26;
		s[i] += 65;
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
	cout << "\n请输入a：";
	int a;
	cin >> a;
	cout << "\n请输入b：";
	int b;
	cin >> b;
	if (c == 'e') cout << "\n加密后的字符串为：" << encrypt(s, a, b) << endl;
	else if (c == 'd') cout << "\n解密后的字符串为：" << decrypt(s, a, b) << endl;
	else cout << "你的输入有误，请重新输入！" << endl;
    system("pause");
	return 0;
}