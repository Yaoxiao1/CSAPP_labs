#datalab 思路介绍 

- [bitXor{#bitxor}](#bitxorbitxor)
- [tmin{#tmin}](#tmintmin)
- [isTmax{#isTmax}](#istmaxistmax)
- [allOddBits{#allOddBits}](#alloddbitsalloddbits)
- [negate{#negate}](#negatenegate)
- [isAsciiDigit{#isAsciiDigit}](#isasciidigitisasciidigit)
- [conditional{#conditional}](#conditionalconditional)
- [isLessOrEqual{#isLessOrEqual}](#islessorequalislessorequal)
- [logicalNeg{#logicalNeg}](#logicalneglogicalneg)
- [howManyBits{#howManyBits}](#howmanybitshowmanybits)
- [floatScale2{#floatScale2}](#floatscale2floatscale2)
- [floatFloat2Int{#floatFloat2Int}](#floatfloat2intfloatfloat2int)
- [floatPower2{#floatPower2}](#floatpower2floatpower2)

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
第一道题目，主要是让大家熟悉一下这个lab，所以难度不是很大，如果之前从来没有写过位运算，那么这道题也会让人卡很久。
难点在于仅使用 ~ & 两个运算来实现异或^, 异或的结果是两个位相同为0，相反为1， 而&运算就是当都为1返回1，那么就可以利用&找到同为1的位，再利用~找到都为0的位，只要让都是1或都是0的位为0，剩余的位为1就可以实现异或，但这里要用到的或|是不被提供的，这里就要用到一个公式~(a|b) => (~a)&(~b)，这样就实现了异或。

## tmin{#tmin}
```
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
	return (1<<31);	// for 32bit system, tmin is 0x80000000
}
```
这道题要求返回二进制补码中的最小值，根据补码的规则，最小值是当最高位为1其余位为0时取到，这里实验环境用的是32位的系统，因此最简单的方法就是把0x1左移31位；
如果不知道系统是多少位的话, 另一种方法是取全1(~0),然后逻辑右移一位(>>1),然后两者相减得到最高位为1, 这种方法步骤比较多, 主要是熟悉了减法的位运算是+(~a+1);

## isTmax{#isTmax}
```
/*
 * isTmax - returns 1 if x is the maximum, two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmax(int x) {
	int m = x+1;	// if x is tmax get tmin
	x = ~(x+m);		// if x==tmax, then get 0, else not 0
	m = !m;			// consider an exception x==-1(all 1)
	x = x+m;		// only x is tmax we get 0
	return !x;		// then return 1
}
```
二进制Tmax的表示形式是符号位为0其余位为1, 它等价于-1(全1)减去Tmin(最高位为1,其余位为0),因此此题有多种解法,目前操作数量最小的应该是代码块的那种,注意要排除-1的干扰


## allOddBits{#allOddBits}
```
/* 
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allOddBits(int x) {
	int A = 0xAA;		//int limit to 0xFF 
	A = (A<<8) + A;		//move to get 0xAAAAAAAA
	A = (A<<16) + A;
	A = (A & x) ^ A;	//to judge if x equals A
	return !A;
}
```
这题其实就是判断x是否等于0xAAAAAAAA, 位运算里判断相等可以用异或,(A^B==0表示两者相等),但是lab要求不能用超过255的数,因此只能通过移位来获得0xAAAAAAAA


## negate{#negate}
```
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
	return ~x + 1;
}
```
应该是最简单的题目之一, 通过这道题可以知道位运算的减法如何实现, ~x + x得到的结果是全1(也就是补码表示的-1), +1之后就等于0, 因此可以用~x+1来表示-x;

## isAsciiDigit{#isAsciiDigit}
```
/* 
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) {
	int upper_bound = (1<<31) + (~0x39); //upperbound should be tmax-0x39;
	int lower_bound = ~0x30+1; 	//lowerbound should be -0x30;
	int sign = 1<<31;	//to get the sign 
	upper_bound = upper_bound + x;
	lower_bound = lower_bound + x;
	return !((upper_bound|lower_bound)&sign); 	// if sign of upper_bound or lower_bound is 1 means x exceed the limit
}
```
判断一个数x是否是ASCII的字符0x30到0x39之间, 这道题给了我们一个判断一个数在某个范围之间不用<>的办法;
最简单能想到的处理就是当x不在范围的时候,我加上一个数的符号位为1, 正常在范围内符号位为0, 那么就要设置一个上界和下界,对于本题来说, 上界应该是tmin(符号位为1其余为0)减去0x39-1(这样+0x39就得到了tmax),这句话翻译成位运算语言就是 1<<31 + (~0x39+1) -1 = 1<<31 + ~0x39;
同理, 下界应该为-0x30, 表示起来就是 ~0x30+1;
然后获取x和上界与下界的和的符号, 出现一个1即返回0; 

## conditional{#conditional}
```
/* 
 * conditional - same as x ? y : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
	int cond = ~(!x); 	// cond = x? -1:-2;
	return ((~(cond+1)&y)|((cond+1)&z));	//~(-1)=0
}
```
按照C语言中的条件三目运算符来返回, x为真返回y,否则返回z, 本题的关键点在于如何利用现有的运算符根据x的值来实现条件转移, 显然可以利用的就是!(非)运算符, 利用~(!x)+1,若x为0,返回的是-1(0xFFFFFFFF), 不为0返回0(0x0), 通过这一系列运算就可以实现判断的效果, -1与任意值做位与(&)运算都能得到改值本身,这样就可以实现题目的要求.

## isLessOrEqual{#isLessOrEqual}
```
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
 // 1. sign different, only x sign 1 and y sign 0 return true
 // 2. sign same, y-x sign 0 return true
int isLessOrEqual(int x, int y) {
	int sign = 1<<31;		// get sign
	int negX = ~x + 1;		// get negtive X
	int subX = y + negX;	// get y-x
	int isDiffSign = ((x&sign)^(y&sign))>>31&1;			// get 1 if different sign, else get 0
	int sameSign = !isDiffSign&(!((subX&sign)>>31&1));	// get 1 if same sign and y-x>=0
	isDiffSign = (x>>31&1)&isDiffSign;	// exclude signx is 0
	return isDiffSign|sameSign;		
		
}
```
本题返回x<=y的结果, 需要考虑两种情况, 两者异号和两者同号,
同号只需相减看符号位, 异号的话满足要求的是x符号位1, y符号位0,
实现这样的多重判断还是需要比较多的操作, 具体思路见上面代码注释, 前三步分别得到符号位、-x、y-x, 第四步isDiffSign为1表示两者异号,0表示同号, 第五步sameSign为1表示y与x同号并且y-x符号位为0, 第六步isDiffSign进一步排除异号时x符号位为0的情况.

## logicalNeg{#logicalNeg}
```
/* 
 * logicalNeg - implement the ! operator, using all of 
 *              the legal operators except !
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int logicalNeg(int x) {
	int negX = ~x+1;			//get -x
	int sign = (negX|x)>>31&1;	//get 0 only x is 0
	return (sign+1)&1;
}
```
本题返回逻辑非, 除了使用!符号其余符号都可以使用, 这里要利用任意值和自己的相反数的符号位不同的规律, 但是0和tmin例外, 但是tmin和tmin相反数的符号位都为1, 因此相反数和自己本身的符号位都是0, 可以利用这个特性得到逻辑非.

## howManyBits{#howManyBits}
```
/* howManyBits - return the minimum number of bits required to represent x in
 *             two's complement
 *  Examples: howManyBits(12) = 5
 *            howManyBits(298) = 10
 *            howManyBits(-5) = 4
 *            howManyBits(0)  = 1
 *            howManyBits(-1) = 1
 *            howManyBits(0x80000000) = 32
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 90
 *  Rating: 4
 */
int howManyBits(int x) {
	int b16, b8, b4, b2, b1;
	int sign = x>>31;			//get sign x
	x = (~sign&x)|(sign&(~x));	//if x>0 get x, else get ~x, so that we can count the position for the highest bit1
	b16 = !!(x>>16)<<4;		// if higer 16bits not none, go count the higher 16bits, b16=16, else go lower 16bits
	x = x>>b16;
	b8 = !!(x>>8)<<3;		// same check higher 8 bits
	x = x>>b8;
	b4 = !!(x>>4)<<2;		// same check higher 4 bits
	x = x>>b4;
	b2 = !!(x>>2)<<1;		// same check higher 2 bits
	x = x>>b2;
	b1 = !!(x>>1);			// check last 2 bits
	x = x>>b1;				// last 1 bits on x itself
	return b16+b8+b4+b2+b1+x+1;	//add all, plus 1(sign)
}
```
这道题应该是datalab整数题目里最难的一道, 需要判断至少要多少位能表达要求值, 作弊偷偷看了一眼该题的test,就发现了做这题的主要思路
```
int test_howManyBits(int x) {
    unsigned int a, cnt;
    x = x<0 ? -x-1 : x;
    a = (unsigned int)x;
    for (cnt=0; a; a>>=1, cnt++)
        ;

    return (int)(cnt + 1);
}
```
可以发现,当x<0时对x取反, ~x = -x-1, 然后数x的最高位的1在第几位,最后加上符号位的1. 如果按照test的思路一步一步去移位的话肯定会超过最大ops限制的90个, 因为最多要移31位, 因此可以使用折半查找的方式减少移位次数, 取log2(32), 最多只需要5次移位即可判断, 因此我们设置了b16,b8,b4,b2,b1来计算每次移动的位数,具体看上面代码注释

## floatScale2{#floatScale2}
```
/* 
 * floatScale2 - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatScale2(unsigned uf) {
	int inf = 0x7f800000;		// get exponent number
	int all1 = 0xff800000;		// get float infinity
	int signf = 0x007fffff;		// get significand
	int spNum = !((uf&inf)^inf);	// special numer NaN or infinity
	int notnormalize = !((uf&inf)^0);	//denormalize number, exponent all 0
	int k = ((uf&all1)>>23)+1;	// k is new exponent 
	int ans = (k<<23) + (uf&signf);	//normal situation, exponent + 1
	int ans2 = (uf&all1) + ((signf&uf)<<1); //denormalize number, significand left move 1bit
	if(spNum) return uf;	// s1: return itself
	else if(notnormalize) return ans2; 	//s2: denormalize
	else return ans;	//s3: regular situation
	
}
```
做浮点类的题目首先要对浮点数的构造熟悉, 这里推荐看一下课本的78页图2-32和2-33, 熟悉32位和64位系统的阶码位数和尾数, 32位浮点数最高1位为符号位, 8位阶码和23位尾数,因此对于unsigned32位数uf,获取阶码要对0x7f800000按位与&,获取尾数要对0x007fffff按位与&, 接下来就是常规的运算,这里是对浮点数*2,分以下几种情况:
1. 最普遍的情况就是给阶码+1,其余部分不变
2. 无穷大(0x7f800000或者0xff800000),返回自身
3. NaN(0x7f8?????),返回自身
因为可以使用if-else条件分支, 所以比上面的整数运算写起来要少了不少限制

## floatFloat2Int{#floatFloat2Int}
```
/* 
 * floatFloat2Int - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
int floatFloat2Int(unsigned uf) {
	int sign =uf>>31;	// get sign
	int exponent = ((uf&0x7f800000)>>23)-127; 	//get exponent
	int significand = (uf&0x007fffff)+0x00800000;	//get significand
	if(exponent>=31) return 0x80000000u; // overflow
	if(exponent<0) return 0;	// small number
	if(exponent>=23&&exponent<31){
		significand<<=exponent-23;	//exponent>=23, significand left move
	}
	if(exponent>=0&&exponent<23) significand>>=23-exponent;
	if(!sign) return significand;	// uf>0, return itself
	else return (~significand+1)|0x80000000;	//uf<0, return opposite
}
```
返回float转化成int的结果, 根据int的限制, 阶码位大于31的都可以返回0x8000000u, 小于1的返回0, 其余情况按照阶码位进行移位运算即可, 这里注意阶码位实际上要减去127的一个偏置(127=2**(8-1)-1)

## floatPower2{#floatPower2}
```
/* 
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 * 
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. Also if, while 
 *   Max ops: 30 
 *   Rating: 4
 */
unsigned floatPower2(int x) {
	int exponent = x + 127;		// get exponent
	if(exponent>=255) return 0x7f800000;	// overflow
	if(exponent<-23) return 0;	// small number
	if(exponent>=-23&&exponent<0) return 1<<(23+exponent);  //denormalize number 
	else return exponent<<23; //regular situation
	
}
```
这题相对于前面两道反而简单一些, 主要是判例较多很多时候通不过, 只要把btest.c文件里的 TIMEOUT_LIMIT改为20秒即可;
x表示指数, 也就是浮点数里的阶码数, 因此只要设置阶码位为x+127(偏置位)即可,超过的部分返回+INF(0x7f800000),
低于尾数可以表示的最低范围(2^-23),返回0
非规格化数的表示(阶码位为0)