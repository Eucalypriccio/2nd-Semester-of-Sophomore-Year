#include "xil_io.h"
#include "stdio.h"
#include "xintc_l.h"
#include "xuartlite_l.h"
#include "xparameters.h"
#include <string.h>
#include <ctype.h>

#define MAX_EXPR_LENGTH 128

void interrupt_hub(void)__attribute__((interrupt_handler));

int precedence(char op);
double apply_op(double a, double b, char op);
double eval_expr(const char* expr);
void double_to_str(double value, char *buf, int precision);

volatile char buffer[MAX_EXPR_LENGTH] = "";
volatile char buffer_expr[MAX_EXPR_LENGTH] = "";
volatile char expr[MAX_EXPR_LENGTH];
volatile int expr_ready;
volatile int expr_idx;


int main()
{
    expr_ready = 0;
    expr_idx = 0;
    memset(expr, 0, sizeof(expr));

    Xil_Out32(XPAR_AXI_UARTLITE_3_BASEADDR + XUL_CONTROL_REG_OFFSET,
              XUL_CR_ENABLE_INTR | XUL_CR_FIFO_RX_RESET | XUL_CR_FIFO_TX_RESET);

    Xil_Out32(XPAR_AXI_INTC_0_BASEADDR + XIN_IER_OFFSET, XPAR_AXI_UARTLITE_3_INTERRUPT_MASK);
    Xil_Out32(XPAR_AXI_INTC_0_BASEADDR + XIN_MER_OFFSET, 0x3);
    microblaze_enable_interrupts();

    while (1)
    {
        if (expr_ready)
        {
            expr[expr_idx] = '\0';
            double result = eval_expr(expr);
            int int_part = (int)result;
            int frac_part = (int)((result - int_part) * 1000000);
            if (result < 0)
            {
            	if (int_part == 0)
            	{
            		xil_printf("result: -%d.%06d\n", int_part, frac_part < 0 ? -frac_part : frac_part);
            	}
            	else
            	{
            		xil_printf("result: %d.%06d\n", int_part, frac_part < 0 ? -frac_part : frac_part);
            	}

            }
            else
            {
            	xil_printf("result: %d.%06d\n", int_part, frac_part < 0 ? -frac_part : frac_part);
            }

            char result_str[32];
			double_to_str(result, result_str, 6);
   		    snprintf(buffer, MAX_EXPR_LENGTH, "t18.txt=\"%s\"\xff\xff\xff", result_str);

			for (int k = 0; k < strlen(buffer); k++)
			{
				while (!(Xil_In32(XPAR_AXI_UARTLITE_3_BASEADDR + XUL_STATUS_REG_OFFSET) & XUL_SR_TX_FIFO_EMPTY));
				Xil_Out32(XPAR_AXI_UARTLITE_3_BASEADDR + XUL_TX_FIFO_OFFSET, buffer[k]);
			}

            expr_ready = 0;
            expr_idx = 0;
            memset(expr, 0, sizeof(expr));
        }
    }

    return 0;
}

void interrupt_hub(void)
{
    u32 status = Xil_In32(XPAR_AXI_UARTLITE_3_BASEADDR + XUL_STATUS_REG_OFFSET);
    if (status & XUL_SR_RX_FIFO_VALID_DATA)
    {
        char recv_char = Xil_In32(XPAR_AXI_UARTLITE_3_BASEADDR + XUL_RX_FIFO_OFFSET);
        if (recv_char != '=' && expr_idx < MAX_EXPR_LENGTH - 1)
        {
            expr[expr_idx] = recv_char;
            snprintf(buffer_expr, MAX_EXPR_LENGTH, "t17.txt=\"%s\"\xff\xff\xff", expr);
            for (int k = 0; k < strlen(buffer_expr); k++)
            {
                while (!(Xil_In32(XPAR_AXI_UARTLITE_3_BASEADDR + XUL_STATUS_REG_OFFSET) & XUL_SR_TX_FIFO_EMPTY));
                Xil_Out32(XPAR_AXI_UARTLITE_3_BASEADDR + XUL_TX_FIFO_OFFSET, buffer_expr[k]);
            }
            expr_idx++;
        }
        else if (recv_char == '=')
        {
            expr[expr_idx] = '\0';
            expr_ready = 1;
        }
    }

    Xil_Out32(XPAR_AXI_INTC_0_BASEADDR + XIN_IAR_OFFSET, status);
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
        case '/': return b ? a / b : 0;
    }
    return 0;
}

double eval_expr(const char* expr)
{
    double nums[MAX_EXPR_LENGTH]; // 数字栈
    int num_top = -1;
    char ops[MAX_EXPR_LENGTH]; // 运算符栈
    int op_top = -1;
    int i = 0, n = strlen(expr);

    while (i < n)
    {
        if (expr[i] == '(')
        {
            ops[++op_top] = expr[i++];
        }
        else if (expr[i] == ')')
        {
            while (op_top >= 0 && ops[op_top] != '(')
            {
                double b = nums[num_top--];
                double a = nums[num_top--];
                nums[++num_top] = apply_op(a, b, ops[op_top--]);
            }
            op_top--;
            i++;
        }
        else if (isdigit(expr[i]) || (expr[i] == '-' &&
                   (i == 0 || expr[i-1] == '(' || strchr("+-*/", expr[i-1]))))
        {
            int sign = 1;
            if (expr[i] == '-')
            {
                sign = -1;
                i++;
            }
            double val = 0;
            while (i < n && isdigit(expr[i]))
            {
                val = val * 10 + (expr[i++] - '0');
            }
            // 处理小数点
            if (i < n && expr[i] == '.')
            {
                i++;
                double frac = 0, base = 0.1;
                while (i < n && isdigit(expr[i]))
                {
                    frac += (expr[i++] - '0') * base;
                    base *= 0.1;
                }
                val += frac;
            }
            nums[++num_top] = (double)sign * val;
        }
        else if (strchr("+-*/", expr[i]))
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
            // 跳过空格等无效字符
            i++;
        }
    }
    while (op_top >= 0)
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

    // 四舍五入处理
    double rounding = 0.5;
    for (int i = 0; i < precision; i++) rounding /= 10;
    frac += rounding;

    // 处理进位
    if (frac >= 1.0) {
        int_part += 1;
        frac -= 1.0;
    }

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