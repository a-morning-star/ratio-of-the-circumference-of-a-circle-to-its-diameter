#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <string>
using namespace std;

// 高精度小数计算类（固定小数位数）
class FixedPrecision {
private:
    vector<int> digits; // 整数部分 + 小数部分
    int integerPart;     // 整数部分
    int totalDigits;     // 总小数位数

public:
    // 构造函数
    FixedPrecision(int totalDigits, double value = 0.0)
        : totalDigits(totalDigits), integerPart(static_cast<int>(value)) {
        digits.resize(totalDigits, 0);
        double fractional = value - integerPart;
        for (int i = 0; i < totalDigits; i++) {
            fractional *= 10;
            digits[i] = static_cast<int>(fractional);
            fractional -= digits[i];
        }
    }

    // 从整数构造
    FixedPrecision(int totalDigits, int integer)
        : totalDigits(totalDigits), integerPart(integer) {
        digits.resize(totalDigits, 0);
    }

    // 获取整数值
    int getInteger() const {
        return integerPart;
    }

    // 获取小数位数
    int getTotalDigits() const {
        return totalDigits;
    }

    // 获取小数部分
    const vector<int>& getFractional() const {
        return digits;
    }

    // 加法
    FixedPrecision operator+(const FixedPrecision& other) const {
        FixedPrecision result(totalDigits);
        int carry = 0;

        // 小数部分相加
        for (int i = totalDigits - 1; i >= 0; i--) {
            int sum = digits[i] + other.digits[i] + carry;
            result.digits[i] = sum % 10;
            carry = sum / 10;
        }

        // 整数部分相加
        result.integerPart = integerPart + other.integerPart + carry;
        return result;
    }

    // 减法
    FixedPrecision operator-(const FixedPrecision& other) const {
        FixedPrecision result(totalDigits);
        int borrow = 0;

        // 小数部分相减
        for (int i = totalDigits - 1; i >= 0; i--) {
            int diff = digits[i] - other.digits[i] - borrow;
            if (diff < 0) {
                diff += 10;
                borrow = 1;
            }
            else {
                borrow = 0;
            }
            result.digits[i] = diff;
        }

        // 整数部分相减
        result.integerPart = integerPart - other.integerPart - borrow;
        return result;
    }

    // 除法（除以整数）
    FixedPrecision operator/(int divisor) const {
        FixedPrecision result(totalDigits);
        int remainder = 0;

        // 整数部分除法
        result.integerPart = integerPart / divisor;
        remainder = integerPart % divisor;

        // 小数部分除法
        for (int i = 0; i < totalDigits; i++) {
            int value = remainder * 10 + digits[i];
            result.digits[i] = value / divisor;
            remainder = value % divisor;
        }
        return result;
    }

    // 输出为字符串
    string toString(int decimalPlaces) const {
        string result;
        result += to_string(integerPart);
        result += '.';
        for (int i = 0; i < decimalPlaces; i++) {
            result += to_string(digits[i]);
        }
        return result;
    }
};

int main() {
    int a;
    cout << "你要获得圆周率的几位小数？：";
    cin >> a;
    if (a < 0) {
        cout << "位数不能为负" << endl;
        return 1;
    }

    // 多保留5位用于四舍五入
    const int totalDigits = a + 5;

    // 初始化π的近似值
    FixedPrecision pi(totalDigits, 0.0);

    // 初始化幂因子（16的k次方）
    FixedPrecision power(totalDigits, 1.0);

    // 最大迭代次数（基于收敛速度）
    int maxIterations = static_cast<int>(a * 2.5) + 20;

    for (int k = 0; k < maxIterations; k++) {
        // 计算分母
        int denom = 8 * k;

        // 计算四项分数
        FixedPrecision term1(totalDigits, 4.0 / (denom + 1));
        FixedPrecision term2(totalDigits, 2.0 / (denom + 4));
        FixedPrecision term3(totalDigits, 1.0 / (denom + 5));
        FixedPrecision term4(totalDigits, 1.0 / (denom + 6));

        // 计算括号内的部分
        FixedPrecision bracket = term1 - term2 - term3 - term4;

        // 计算当前项（乘以幂因子）
        FixedPrecision term = bracket;
        for (int i = 0; i < k; i++) {
            term = term / 16;
        }

        // 累加到π
        pi = pi + term;

        // 更新幂因子
        power = power / 16;

        // 检查是否收敛（幂因子足够小）
        if (power.getInteger() == 0) {
            bool allZero = true;
            for (int digit : power.getFractional()) {
                if (digit != 0) {
                    allZero = false;
                    break;
                }
            }
            if (allZero) break;
        }
    }

    // 四舍五入处理
    string piStr = pi.toString(totalDigits);
    if (piStr.length() > a + 2) { // +2 for "3." 
        int lastDigit = piStr[a + 2] - '0';
        if (lastDigit >= 5) {
            // 处理进位
            int carry = 1;
            for (int i = a + 1; i >= 2; i--) {
                int digit = piStr[i] - '0' + carry;
                if (digit < 10) {
                    piStr[i] = '0' + digit;
                    carry = 0;
                    break;
                }
                else {
                    piStr[i] = '0';
                    carry = 1;
                }
            }
            // 处理整数部分进位
            if (carry) {
                piStr[0] = '4';
            }
        }
        piStr = piStr.substr(0, a + 2);
    }

    // 输出结果
    cout << "圆周率π的近似值：\n";
    cout << piStr << endl;

    return 0;
}
