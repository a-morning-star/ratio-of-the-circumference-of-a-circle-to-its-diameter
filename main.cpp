#include<iostream>
#include<vector>
#include<cmath>
using namespace std;

int main() {
    int a;
    cout << "你要获得圆周率的几位小数？：";
    cin >> a;
    a++;
    const int totalDigits = a + 4; // 保留a+4位小数（用于四舍五入）
    vector<int> n(totalDigits + 1, 0); // [0]整数部分 [1]~[totalDigits]小数部分

    int sign = 1; // 符号：1正，-1负
    // 计算级数：i为分母 (1,3,5,...)
    for (int i = 1; ; i += 2) {
        // 终止条件：1/i < 10^(-a-4) 时退出
        if (pow(10, a + 4) < i) break;

        long long c = 1; // 被除数
        // 计算当前项 (1/i) 的小数
        for (int j = 1; j <= totalDigits; j++) {
            int digit = c / i;
            c = (c % i) * 10;

            // 根据符号加减到结果
            if (sign == 1) n[j] += digit;
            else n[j] -= digit;
        }
        sign = -sign; // 符号交替
    }

    // 处理进位/借位（从低位到高位）
    for (int j = totalDigits; j >= 1; j--) {
        // 处理负数的借位
        while (n[j] < 0 && j > 0) {
            n[j - 1]--;
            n[j] += 10;
        }
        // 处理正数的进位
        while (n[j] >= 10 && j > 0) {
            n[j - 1] += n[j] / 10;
            n[j] %= 10;
        }
    }

    // 乘以4 (π = 4 * (π/4))
    int carry = 0;
    for (int j = totalDigits; j >= 0; j--) {
        n[j] = n[j] * 4 + carry;
        carry = n[j] / 10;
        n[j] %= 10;
    }
    n[0] = carry; // 整数部分

    // 四舍五入（使用第a+4位）
    if (n[a + 4] >= 5) {
        n[a + 3]++;
        // 处理进位
        for (int j = a + 3; j >= 1; j--) {
            if (n[j] < 10) break;
            n[j - 1] += n[j] / 10;
            n[j] %= 10;
        }
    }

    // 输出结果
    cout << n[1] << ".";
    for (int i = 2; i <= a; i++) {
        cout << n[i];
    }
    return 0;
}
