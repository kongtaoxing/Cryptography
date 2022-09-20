/*******************************************
 * Name: caesar.cpp
 * Author: kongtaoxing
 * Powered by Beijingjiaotong University
 ******************************************/
#include<iostream>
#include<string>
using namespace std;

string encrypt(string s, int key) {
	for (int i = 0; i < s.length(); i++) {
		s[i] = (s[i] - 65 + key) % 26;
		if (s[i] < 0) s[i] += 26;            //C语言-2%26竟然等于-2，不够26需要手动加一下
		s[i] += 65;
	}
	return s;
}

string decrypt(string s, int key) {
	for (int i = 0; i < s.length(); i++) {
		s[i] = (s[i] - 65 - key) % 26;
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
	cout << "\n请输入Key：";
	int key;
	cin >> key;
	if (c == 'e') cout << "\n加密后的字符串为：" << encrypt(s, key) << endl;
	else if (c == 'd') cout << "\n解密后的字符串为：" << decrypt(s, key) << endl;
	else cout << "你的输入有误，请重新输入！";
	system("pause");
	return 0;
}