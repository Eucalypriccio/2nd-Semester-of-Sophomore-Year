#include "xil_io.h"
#include "stdio.h"
#include "xintc_l.h"
#include "xuartlite_l.h"
#include "xparameters.h"
#include <string.h>
#include <ctype.h>

#define MAX_EXPR_LENGTH 128

int precedence(char op); // 运算符优先级
double apply_op(double a, double b, char op); // 计算两个数的运算
double eval_expr(const char* expr); // 表达式求值（支持正负数、小数、括号、四则运算）
void double_to_str(double value, char *buf, int precision);

int main()
{
    int i = 0;
    char expr[MAX_EXPR_LENGTH];
    memset(expr, 0, sizeof(expr));
    int recv_char;
    char buffer[MAX_EXPR_LENGTH] = "";

    Xil_Out32(XPAR_AXI_UARTLITE_3_BASEADDR + XUL_CONTROL_REG_OFFSET,
              XUL_CR_FIFO_RX_RESET | XUL_CR_FIFO_TX_RESET); // 清除RX，TX寄存器

    while (1)
    {
        while((Xil_In32(XPAR_AXI_UARTLITE_3_BASEADDR + XUL_STATUS_REG_OFFSET) &
               XUL_SR_RX_FIFO_VALID_DATA) != XUL_SR_RX_FIFO_VALID_DATA);
        recv_char = Xil_In32(XPAR_AXI_UARTLITE_3_BASEADDR + XUL_RX_FIFO_OFFSET);
        if (recv_char != '=')
        {
            if (i < MAX_EXPR_LENGTH - 1) {
                expr[i] = recv_char;
                i++;
            }
        }
        else
        {
            expr[i] = '\0'; // 结束符
            xil_printf("the expression is: ");
            for (int j = 0; j < i; j++)
            {
                xil_printf("%c", expr[j]);
            }
            xil_printf("\n");

            double result = eval_expr(expr);
//            或手动分离整数和小数部分输出
//            int int_part = (int)result;
//            int frac_part = (int)((result - int_part) * 1000000); // 保留6位小数
//            if (result < 0)
//            {
//            	xil_printf("result: -%d.%06d\n", int_part, frac_part < 0 ? -frac_part : frac_part);
//            }
//            else
//            {
//            	xil_printf("result: %d.%06d\n", int_part, frac_part < 0 ? -frac_part : frac_part);
//            }

            char result_str[32];
			double_to_str(result, result_str, 6); // 保留6位小数

			// snprintf(buffer, MAX_EXPR_LENGTH, "t17.txt=\"%.2f\"\xff\xff\xff", result_str);

			// 通过串口输出字符串
			for (int k = 0; k < strlen(buffer); k++)
			{
				while (!(Xil_In32(XPAR_AXI_UARTLITE_3_BASEADDR + XUL_STATUS_REG_OFFSET) & XUL_SR_TX_FIFO_EMPTY));
				Xil_Out32(XPAR_AXI_UARTLITE_3_BASEADDR + XUL_TX_FIFO_OFFSET, buffer[k]);
			}

            // 清空表达式
            memset(expr, 0, sizeof(expr));
            i = 0;
        }
    }

    return 0;
}

int precedence(char op)
{
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

double apply_op(double a, double b, char op)
{
    switch (op)
    {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return b ? a / b : 0; // 简单处理除零
    }
    return 0;
}

double eval_expr(const char* expr)
{
    double nums[MAX_EXPR_LENGTH]; // 操作数栈
    int num_top = -1;
    char ops[MAX_EXPR_LENGTH]; // 运算符栈
    int op_top = -1;
    int i = 0, n = strlen(expr);

    while (i < n)
    {
        if (expr[i] == '(') // 左括号直接入运算符栈
        {
            ops[++op_top] = expr[i++];
        }
        else if (expr[i] == ')') // 右括号弹出栈，运算直到遇到左括号
        {
            while (op_top >= 0 && ops[op_top] != '(')
            {
                double b = nums[num_top--];
                double a = nums[num_top--];
                nums[++num_top] = apply_op(a, b, ops[op_top--]);
            }
            op_top--; // 弹出'('
            i++;
        }
        else if (isdigit(expr[i]) || (expr[i] == '-' &&
        		   (i == 0 || expr[i-1] == '(' || strchr("+-*/", expr[i-1]))))
        {
        	// 1. 当前位是数字
        	// 2. 当前位是负号且是表达式首位
        	// 3. 当前位是负号且前一位是左括号
        	// 4. 当前位是负号且前一位是操作符
            // 处理正负数和小数
            int sign = 1;
            if (expr[i] == '-')
            {
                sign = -1;
                i++;
            }
            double val = 0;
            while (i < n && isdigit(expr[i])) // 得到实际的操作数
            {
                val = val * 10 + (expr[i++] - '0');
            }
//            if (i < n && expr[i] == '.')
//            {
//                i++;
//                double frac = 0, base = 0.1;
//                while (i < n && isdigit(expr[i]))
//                {
//                    frac += (expr[i++] - '0') * base;
//                    base *= 0.1;
//                }
//                val += frac;
//            }
            nums[++num_top] = sign * val; // 操作数入栈
        }
        else if (strchr("+-*/", expr[i])) // 当前位是操作符
        {
            while (op_top >= 0 && precedence(ops[op_top]) >= precedence(expr[i]))
            {
                double b = nums[num_top--];
                double a = nums[num_top--];
                nums[++num_top] = apply_op(a, b, ops[op_top--]);
            }
            ops[++op_top] = expr[i++];
        }
        else
        {
            // 非法字符
            i++;
        }
    }
    while (op_top >= 0)  // 处理剩余的运算符
    {
        double b = nums[num_top--];
        double a = nums[num_top--];
        nums[++num_top] = apply_op(a, b, ops[op_top--]);
    }
    return nums[num_top];
}

void double_to_str(double value, char *buf, int precision)
{
    if (value < 0)
    {
        *buf++ = '-';
        value = -value;
    }
    int int_part = (int)value;
    double frac = value - int_part;

    // 整数部分
    char int_buf[16];
    int int_i = 0;
    if (int_part == 0)
    {
        int_buf[int_i++] = '0';
    }
    else
    {
        while (int_part > 0)
        {
            int_buf[int_i++] = '0' + int_part % 10;
            int_part /= 10;
        }
    }
    // 反转整数部分
    for (int j = int_i - 1; j >= 0; j--)
    {
        *buf++ = int_buf[j];
    }

    *buf++ = '.';

    // 小数部分
    for (int i = 0; i < precision; i++)
    {
        frac *= 10;
        int digit = (int)frac;
        *buf++ = '0' + digit;
        frac -= digit;
    }
    *buf = '\0';
}
