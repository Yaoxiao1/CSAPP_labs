#datalab 思路介绍 

1. [bitXor](#bitXor)
2. [tmin](#tmin)
3. [isTmax](#isTmax)
4. [allOddBits](#allOddBits)
5. [negate](#negate)
6. [isAsciiDigit](#isAsciiDigit)
7. [conditional](#conditional)
8. [isLessOrEqual](#isLessOrEqual)
9. [logicalNeg](#logicalNeg)
10. [howManyBits](#howManyBits)
11. [floatScale2](#floatScale2)
12. [floatFloat2Int](#floatFloat2Int)
13. [floatPower2](#floatPower2)

## bitXor{#bitxor}
```
//1
/* 
 * bitXor - x^y using only ~ and & 
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {
	int a = x&y;        // a reveals '1' when x and y both are 1 in the same bit
	int b = (~x)&(~y);  // b reveals '1' when x and y both are 0 in the same bit
	return (~a)&(~b);   // ~(a|b) => (~a)&(~b)
}
```
