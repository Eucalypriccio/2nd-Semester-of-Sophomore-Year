#include "xil_io.h"
#include "stdio.h"
#include "xintc_l.h"
#include "xuartlite_l.h"
#include "xparameters.h"
#include <string.h>
#include <ctype.h>

#define MAX_EXPR_LENGTH 128

int precedence(char op); // ��������ȼ�
double apply_op(double a, double b, char op); // ����������������
double eval_expr(const char* expr); // ���ʽ��ֵ��֧����������С�������š��������㣩
void double_to_str(double value, char *buf, int precision);

int main()
{
    int i = 0;
    char expr[MAX_EXPR_LENGTH];
    memset(expr, 0, sizeof(expr));
    int recv_char;
    char buffer[MAX_EXPR_LENGTH] = "";

    Xil_Out32(XPAR_AXI_UARTLITE_3_BASEADDR + XUL_CONTROL_REG_OFFSET,
              XUL_CR_FIFO_RX_RESET | XUL_CR_FIFO_TX_RESET); // ���RX��TX�Ĵ���

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
            expr[i] = '\0'; // ������
            xil_printf("the expression is: ");
            for (int j = 0; j < i; j++)
            {
                xil_printf("%c", expr[j]);
            }
            xil_printf("\n");

            double result = eval_expr(expr);
//            ���ֶ�����������С���������
//            int int_part = (int)result;
//            int frac_part = (int)((result - int_part) * 1000000); // ����6λС��
//            if (result < 0)
//            {
//            	xil_printf("result: -%d.%06d\n", int_part, frac_part < 0 ? -frac_part : frac_part);
//            }
//            else
//            {
//            	xil_printf("result: %d.%06d\n", int_part, frac_part < 0 ? -frac_part : frac_part);
//            }

            char result_str[32];
			double_to_str(result, result_str, 6); // ����6λС��

			// snprintf(buffer, MAX_EXPR_LENGTH, "t17.txt=\"%.2f\"\xff\xff\xff", result_str);

			// ͨ����������ַ���
			for (int k = 0; k < strlen(buffer); k++)
			{
				while (!(Xil_In32(XPAR_AXI_UARTLITE_3_BASEADDR + XUL_STATUS_REG_OFFSET) & XUL_SR_TX_FIFO_EMPTY));
				Xil_Out32(XPAR_AXI_UARTLITE_3_BASEADDR + XUL_TX_FIFO_OFFSET, buffer[k]);
			}

            // ��ձ��ʽ
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
        case '/': return b ? a / b : 0; // �򵥴������
    }
    return 0;
}

double eval_expr(const char* expr)
{
    double nums[MAX_EXPR_LENGTH]; // ������ջ
    int num_top = -1;
    char ops[MAX_EXPR_LENGTH]; // �����ջ
    int op_top = -1;
    int i = 0, n = strlen(expr);

    while (i < n)
    {
        if (expr[i] == '(') // ������ֱ���������ջ
        {
            ops[++op_top] = expr[i++];
        }
        else if (expr[i] == ')') // �����ŵ���ջ������ֱ������������
        {
            while (op_top >= 0 && ops[op_top] != '(')
            {
                double b = nums[num_top--];
                double a = nums[num_top--];
                nums[++num_top] = apply_op(a, b, ops[op_top--]);
            }
            op_top--; // ����'('
            i++;
        }
        else if (isdigit(expr[i]) || (expr[i] == '-' &&
        		   (i == 0 || expr[i-1] == '(' || strchr("+-*/", expr[i-1]))))
        {
        	// 1. ��ǰλ������
        	// 2. ��ǰλ�Ǹ������Ǳ��ʽ��λ
        	// 3. ��ǰλ�Ǹ�����ǰһλ��������
        	// 4. ��ǰλ�Ǹ�����ǰһλ�ǲ�����
            // ������������С��
            int sign = 1;
            if (expr[i] == '-')
            {
                sign = -1;
                i++;
            }
            double val = 0;
            while (i < n && isdigit(expr[i])) // �õ�ʵ�ʵĲ�����
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
            nums[++num_top] = sign * val; // ��������ջ
        }
        else if (strchr("+-*/", expr[i])) // ��ǰλ�ǲ�����
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
            // �Ƿ��ַ�
            i++;
        }
    }
    while (op_top >= 0)  // ����ʣ��������
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

    // ��������
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
    // ��ת��������
    for (int j = int_i - 1; j >= 0; j--)
    {
        *buf++ = int_buf[j];
    }

    *buf++ = '.';

    // С������
    for (int i = 0; i < precision; i++)
    {
        frac *= 10;
        int digit = (int)frac;
        *buf++ = '0' + digit;
        frac -= digit;
    }
    *buf = '\0';
}
