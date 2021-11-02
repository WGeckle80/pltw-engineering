/* Author: Wyatt Geckle
 * Date: 11/2/21
 * 
 * Using a shift register, the arduino generates 8 bits: 4 for the first operand and 4
 * for the second.  User enters the following syntax into the serial monitor: "a + b", "a - b".
 * The variables a and b must be 4-bit signed integers [-8, 7].  The expected output is
 * printed, and physical logic gates perform the operation with a 4-bit output.
 */

#include "intpow.h"

#define OUTPIN 2
#define CLKPIN 3
#define ADDSUBPIN 4

const uint8_t numRegisterBits = 8; //equal number of bits in operands
const int8_t minOpNum = -ipow2((numRegisterBits >> 1) - 1);  //min signed operand num
const uint8_t maxOpNum = -minOpNum - 1;  //max signed operand num

void numOperation(String strOp);
void performOperation(int8_t num1, char op, int8_t num2);
void setNumDigits(uint8_t num);

void setup()
{
    Serial.begin(9600);
    
    pinMode(OUTPIN, OUTPUT);
    pinMode(CLKPIN, OUTPUT);
    pinMode(ADDSUBPIN, OUTPUT);
    
    //clear register
    for (int i = 0; i < numRegisterBits; i++)
    {
        digitalWrite(CLKPIN, HIGH);
        delay(1);
        digitalWrite(CLKPIN, LOW);
        delay(1);
    }
}

void loop()
{
    if (Serial.available())
    {
        numOperation(Serial.readString());
    }
}

//finds operation and operands from serial input
void numOperation(String strOp)
{
    strOp.replace(" ", "");
    strOp.replace("\n", "");
    
    int opLoc = strOp.indexOf("+");
    
    if (opLoc != -1)
    {
        performOperation(strOp.substring(0, opLoc).toInt(),
                         '+',
                         strOp.substring(opLoc + 1).toInt());
        return;
    }
    else
    {
        //finds place of subtraction sign
        
        uint8_t numMinusSigns = 0;
        uint16_t minusSignLocs[3];
        
        for (uint16_t i = 0; i < strOp.length(); i++)
        {
            if (strOp.charAt(i) == '-')
            {
                minusSignLocs[numMinusSigns] = i;
                numMinusSigns++;
            }
        }
        
        switch (numMinusSigns)
        {
            case 0:
                opLoc = -1;
                break;
                
            case 1:
                //if both numbers positive, the minus sign is the first one
                opLoc = minusSignLocs[0];
                break;
            
            case 2:
                //if first number is negative, the minus sign is the second one
                //if second number is negative, the minus sign is the first one
                if (minusSignLocs[0] == 0)
                    opLoc = minusSignLocs[1];
                else
                    opLoc = minusSignLocs[0];
                break;
                
            case 3:
                //if both numbers negative, the minus sign is the second one
                opLoc = minusSignLocs[1];
                break;
                
            default:
                Serial.println("Ambiguous Operation");
                return;
        }
    }
    
    if (opLoc != -1)
    {
        performOperation(strOp.substring(0, opLoc).toInt(),
                         '-',
                         strOp.substring(opLoc + 1).toInt());
        return;
    }
    else
    {
        Serial.println("Missing Allotted Operator");
    }
    
    return;
}

//sends operand bits to shift register, sets addition/subtraction pin, and prints expected output
void performOperation(int8_t num1, char op, int8_t num2)
{
    if (num1 < minOpNum || num1 > maxOpNum ||
        num2 < minOpNum || num2 > maxOpNum)
    {
        Serial.print("Operand(s) Out of Range [");
        Serial.print(-maxOpNum);
        delay(50);  //won't print correctly without small delay
        Serial.print(',');
        Serial.print(maxOpNum);
        Serial.println(']');
        
        return;
    }
        
    setNumDigits(num2);
    setNumDigits(num1);
    
    Serial.print(num1);
    Serial.print(op);
    Serial.print(num2);
    Serial.print('=');
    
    switch (op)
    {
        case '+':
            digitalWrite(ADDSUBPIN, LOW);
            break;
        case '-':
            digitalWrite(ADDSUBPIN, HIGH);
            num2 = ~num2 + 1;  //2s complement
    }
    
    Serial.println(num1 + num2);
    
    return;
}

//sets a single operand's bits in the shift register
void setNumDigits(int8_t num)
{
    //always set num2 digits before num1
      
    for (uint8_t i = 0; i < numRegisterBits >> 1; i++)
    {
        digitalWrite(OUTPIN, num & 1);
        delay(1);
        digitalWrite(CLKPIN, HIGH);
        delay(1);
        digitalWrite(CLKPIN, LOW);
        delay(1);
        
        num >>= 1;
    }
    
    return;
}
