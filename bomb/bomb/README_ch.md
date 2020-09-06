#bomblab 思路介绍

- [常用指令](#常用指令)
- [BombLab phase_1](#bomblab-phase_1)
- [BombLab phase_2](#bomblab-phase_2)
- [BombLab phase_3](#bomblab-phase_3)
- [BombLab phase_4](#bomblab-phase_4)

## 常用指令
```

```


## BombLab phase_1

```assembly
0000000000400ee0 <phase_1>:
  400ee0:	48 83 ec 08          	sub    $0x8,%rsp        
  400ee4:	be 00 24 40 00       	mov    $0x402400,%esi
  400ee9:	e8 4a 04 00 00       	callq  401338 <strings_not_equal>
  400eee:	85 c0                	test   %eax,%eax
  400ef0:	74 05                	je     400ef7 <phase_1+0x17>
  400ef2:	e8 43 05 00 00       	callq  40143a <explode_bomb>
  400ef7:	48 83 c4 08          	add    $0x8,%rsp
  400efb:	c3                   	retq   
```

phase_1主要是熟悉一些常用的gdb指令, 以及如何把汇编语言改成类C语言的伪代码从而提高代码可读性, 这里我个人改写的伪代码如下

```c++
// %rax in a, %rbx in b, %rcx in c, %rdx in d
// %rdi in x, %rsi in y, %rsp in p
phase_1(string x);  				//x is the input string
y = 0x402400;
a = strings_not_equal(x, y); //compare string x and y, return 1 if not equal
if (a!=0) expolde bomb; 		 
else return
```

所以本题核心在于比较两个字符串,一个是输入的字符串, 一个是位于$0x402400的字符串, 用 `print (char*) 0x402400` 显示出该出的字符串为 `Border relations with Canada have never been better.`,至此phase_1的输入只要与之相等即可通过.





## BombLab phase_2
```assembly
0000000000400efc <phase_2>:
  400efc:	55                   	push   %rbp
  400efd:	53                   	push   %rbx
  400efe:	48 83 ec 28          	sub    $0x28,%rsp
  400f02:	48 89 e6             	mov    %rsp,%rsi
  400f05:	e8 52 05 00 00       	callq  40145c <read_six_numbers>
  400f0a:	83 3c 24 01          	cmpl   $0x1,(%rsp)      
  400f0e:	74 20                	je     400f30 <phase_2+0x34>
                                    ---------------------
  400f10:	e8 25 05 00 00       	callq  40143a <explode_bomb>
  400f15:	eb 19                	jmp    400f30 <phase_2+0x34>
                                    ---------------------
  400f17:	8b 43 fc             	mov    -0x4(%rbx),%eax
  400f1a:	01 c0                	add    %eax,%eax
  400f1c:	39 03                	cmp    %eax,(%rbx)
  400f1e:	74 05                	je     400f25 <phase_2+0x29>
                                    ---------------------
  400f20:	e8 15 05 00 00       	callq  40143a <explode_bomb>
  400f25:	48 83 c3 04          	add    $0x4,%rbx
  400f29:	48 39 eb             	cmp    %rbp,%rbx
  400f2c:	75 e9                	jne    400f17 <phase_2+0x1b>
                                    ---------------------
  400f2e:	eb 0c                	jmp    400f3c <phase_2+0x40>
  400f30:	48 8d 5c 24 04       	lea    0x4(%rsp),%rbx
  400f35:	48 8d 6c 24 18       	lea    0x18(%rsp),%rbp
  400f3a:	eb db                	jmp    400f17 <phase_2 +0x1b>   
                                    ---------------------
  400f3c:	48 83 c4 28          	add    $0x28,%rsp
  400f40:	5b                   	pop    %rbx
  400f41:	5d                   	pop    %rbp
  400f42:	c3                   	retq   
```

做这类汇编实验最好把相关的汇编代码打印出来, 对重要的部分做上标记, 特别是跳转的地方, 否则看起来头昏眼花, 本题关键的汇编部分在`400f0e`到`400f3c`之间, 设计到循环跳转, 大致对应的伪代码如下:
```c++
// %rax in a, %rbx in b, %rcx in c, %rdx in d, %rbp in bp
// %rdi in x, %rsi in y, %rsp in p
phase_2();
int[6]=p;
p=read_six_numbers();         // read 6 numbers, the first number is stored in %rsp, second in $rsp+4, then $rsp+8, till $rsp+20
if(*p!=1) explode bomb;         // 400f0a
b = p+4;                      // second number
bp = p+24;                       // cycle end
a = *(b-4) * 2;                          // previous number*2
if(a!=*b) explode bomb;
b = b+4;                        // third number
while(b!=bp)
{
    a = *(b-4) * 2;                 // a stores previous number
    if(a!=*b) explode bomb;
    b += 4;
}
return;
```
可以发现这里的要求是输入6个数字, `read_six_numbers()`函数还会检查是否是正好6个, 如果不是也会引爆炸弹; 6个数字分别存储在%rsp 到 %rsp+20, 每个数字是32位因此占4个字节;
首先对比第一个数字与1, 如果不相等则引爆炸弹, 因此第一个数字告诉我们要填1, 随后进入一个循环, 后一个数字跟前一个数字的2倍比较, 不相等则引爆炸弹, 因此我们的6个数字也就呼之欲出了, 分别是`1 2 4 8 16 32`, 即为phase_2的答案;

## BombLab phase_3
```assembly
0000000000400f43 <phase_3>:
  400f43:	48 83 ec 18          	sub    $0x18,%rsp
  400f47:	48 8d 4c 24 0c       	lea    0xc(%rsp),%rcx
  400f4c:	48 8d 54 24 08       	lea    0x8(%rsp),%rdx
  400f51:	be cf 25 40 00       	mov    $0x4025cf,%esi
  400f56:	b8 00 00 00 00       	mov    $0x0,%eax
  400f5b:	e8 90 fc ff ff       	callq  400bf0 <__isoc99_sscanf@plt>
  400f60:	83 f8 01             	cmp    $0x1,%eax
  400f63:	7f 05                	jg     400f6a <phase_3+0x27>
                                    ------------------------
  400f65:	e8 d0 04 00 00       	callq  40143a <explode_bomb>
  400f6a:	83 7c 24 08 07       	cmpl   $0x7,0x8(%rsp)
  400f6f:	77 3c                	ja     400fad <phase_3+0x6a>
                                    ------------------------
  400f71:	8b 44 24 08          	mov    0x8(%rsp),%eax
  400f75:	ff 24 c5 70 24 40 00 	jmpq   *0x402470(,%rax,8)
  400f7c:	b8 cf 00 00 00       	mov    $0xcf,%eax
  400f81:	eb 3b                	jmp    400fbe <phase_3+0x7b>
                                    ------------------------
  400f83:	b8 c3 02 00 00       	mov    $0x2c3,%eax
  400f88:	eb 34                	jmp    400fbe <phase_3+0x7b>
                                    ------------------------
  400f8a:	b8 00 01 00 00       	mov    $0x100,%eax
  400f8f:	eb 2d                	jmp    400fbe <phase_3+0x7b>
                                    ------------------------
  400f91:	b8 85 01 00 00       	mov    $0x185,%eax
  400f96:	eb 26                	jmp    400fbe <phase_3+0x7b>
                                    ------------------------
  400f98:	b8 ce 00 00 00       	mov    $0xce,%eax
  400f9d:	eb 1f                	jmp    400fbe <phase_3+0x7b>
                                    ------------------------
  400f9f:	b8 aa 02 00 00       	mov    $0x2aa,%eax
  400fa4:	eb 18                	jmp    400fbe <phase_3+0x7b>
                                    ------------------------
  400fa6:	b8 47 01 00 00       	mov    $0x147,%eax
  400fab:	eb 11                	jmp    400fbe <phase_3+0x7b>
                                    ------------------------
  400fad:	e8 88 04 00 00       	callq  40143a <explode_bomb>
  400fb2:	b8 00 00 00 00       	mov    $0x0,%eax
  400fb7:	eb 05                	jmp    400fbe <phase_3+0x7b>
                                    ------------------------
  400fb9:	b8 37 01 00 00       	mov    $0x137,%eax
  400fbe:	3b 44 24 0c          	cmp    0xc(%rsp),%eax
  400fc2:	74 05                	je     400fc9 <phase_3+0x86>
                                    ------------------------
  400fc4:	e8 71 04 00 00       	callq  40143a <explode_bomb>
  400fc9:	48 83 c4 18          	add    $0x18,%rsp
  400fcd:	c3                   	retq   
```
phase_3的跳转比较多,但是比较单一, 分析过后发现其实是一个switch语句, 伪代码如下:
```assembly
// %rax in a, %rbx in b, %rcx in c, %rdx in d, %rbp in bp
// %rdi in x, %rsi in y, %rsp in p
phase_3();
y = 0x4025cf;
a = 0;
a = scanf();    // readline, store in %rsp+8 ... a stores the first number
if(a<=1) explode bomb;  // 0x400f63
if(*(rsp+8)>7) explode bomb; // 0x400f6f
int sec_num = *(p+12)           // second number
switch(a){
    case 2: 
        if(sec_num!=0x2c3) explode Bomb;    // 0x400f83
        break;
    case 3:
        if(sec_num!=0x100) explode Bomb;    // 0x400f8a
        break;
    case 4:
        if(sec_num!=0x185) explode Bomb;    // 0x400f91
        break;
    case 5:
        if(sec_num!=0xce) explode Bomb;     // 0x400f96
        break;
    case 6:
        if(sec_num!=0x2aa) explode Bomb;    // 0x400f9d
        break;
    case 7:
        if(sec_num!=0x147) explode Bomb;    // 0x400fa4
        break;
}
return;
```
首先检查第一个输入的数据,`400f63`处要求大于1, `400f6f`处要求不大于7, 然后进入关键一步`400f75`, 这里主要就是根据第一个数字(存储在%rax里)来决定第二个值应该是多少, 关键步骤在`400f75`, 可以在gdb里通过 `print /x \*(int\*)(0x402470+8\*num)`(num表示第一个数的值,从2到7) 分别得出跳转的指令目的地, 分别对应的是0x400f83~0x400fa4, 从而根据对应的值输入第二个数即可, 因此此题有6个正确答案;

## BombLab phase_4
```assembly
000000000040100c <phase_4>:
  40100c:	48 83 ec 18          	sub    $0x18,%rsp
  401010:	48 8d 4c 24 0c       	lea    0xc(%rsp),%rcx
  401015:	48 8d 54 24 08       	lea    0x8(%rsp),%rdx
  40101a:	be cf 25 40 00       	mov    $0x4025cf,%esi
  40101f:	b8 00 00 00 00       	mov    $0x0,%eax
  401024:	e8 c7 fb ff ff       	callq  400bf0 <__isoc99_sscanf@plt>
  401029:	83 f8 02             	cmp    $0x2,%eax
  40102c:	75 07                	jne    401035 <phase_4+0x29>
                                    -------------------------
  40102e:	83 7c 24 08 0e       	cmpl   $0xe,0x8(%rsp)
  401033:	76 05                	jbe    40103a <phase_4+0x2e>
                                    -------------------------
  401035:	e8 00 04 00 00       	callq  40143a <explode_bomb>
  40103a:	ba 0e 00 00 00       	mov    $0xe,%edx
  40103f:	be 00 00 00 00       	mov    $0x0,%esi
  401044:	8b 7c 24 08          	mov    0x8(%rsp),%edi
  401048:	e8 81 ff ff ff       	callq  400fce <func4>
                                    -------------------------
  40104d:	85 c0                	test   %eax,%eax
  40104f:	75 07                	jne    401058 <phase_4+0x4c>
                                    -------------------------
  401051:	83 7c 24 0c 00       	cmpl   $0x0,0xc(%rsp)
  401056:	74 05                	je     40105d <phase_4+0x51>
                                    -------------------------
  401058:	e8 dd 03 00 00       	callq  40143a <explode_bomb>
  40105d:	48 83 c4 18          	add    $0x18,%rsp
  401061:	c3                   	retq   
```
这里比较麻烦的地方在于还引用了一个func4
```assembly
0000000000400fce <func4>:
  400fce:	48 83 ec 08          	sub    $0x8,%rsp
  400fd2:	89 d0                	mov    %edx,%eax
  400fd4:	29 f0                	sub    %esi,%eax
  400fd6:	89 c1                	mov    %eax,%ecx
  400fd8:	c1 e9 1f             	shr    $0x1f,%ecx
  400fdb:	01 c8                	add    %ecx,%eax
  400fdd:	d1 f8                	sar    %eax
  400fdf:	8d 0c 30             	lea    (%rax,%rsi,1),%ecx
  400fe2:	39 f9                	cmp    %edi,%ecx
  400fe4:	7e 0c                	jle    400ff2 <func4+0x24>
                                    --------------------------
  400fe6:	8d 51 ff             	lea    -0x1(%rcx),%edx
  400fe9:	e8 e0 ff ff ff       	callq  400fce <func4>
                                    --------------------------
  400fee:	01 c0                	add    %eax,%eax
  400ff0:	eb 15                	jmp    401007 <func4+0x39>
                                    --------------------------
  400ff2:	b8 00 00 00 00       	mov    $0x0,%eax
  400ff7:	39 f9                	cmp    %edi,%ecx
  400ff9:	7d 0c                	jge    401007 <func4+0x39>
                                    --------------------------
  400ffb:	8d 71 01             	lea    0x1(%rcx),%esi
  400ffe:	e8 cb ff ff ff       	callq  400fce <func4>
                                    --------------------------
  401003:	8d 44 00 01          	lea    0x1(%rax,%rax,1),%eax
  401007:	48 83 c4 08          	add    $0x8,%rsp
  40100b:	c3                   	retq   
```
老样子分别分析, 标出所有跳转的部分, 如果是往回跳转就可以写成while语句, 往后跳转就写if语句, 等效伪代码如下:
```c++
// %rax in a, %rbx in b, %rcx in c, %rdx in d, %rbp in bp
// %rdi in x, %rsi in y, %rsp in p
phase_4()
y = 0x4025cf;
a = 0;
a = scanf();        // first num stores in %rsp+8, second stores in %rsp+0xc, a means the number of digit input, should be 2
if(a!=2) explode bomb;
if(*(p+8)>14) explode bomb;  // *(%rsp+8) stores the first number input
d = 14;
y = 0;
x = *(p+8);                 // %rdi, %rsi, %rdx as parameters in func4
a = func4(x,y,d);
if(a!=0) explode bomb;      // 0x401051
if(*(p+12)!=0) explode bomb;    // *(p+12) stores the second number, actually *(p+16) can store a string for secret_phase
return;

func4(x, y, d)
{
    a = d;
    a = a - y;
    c = a;          // c = d-y
    c = c>>31;      // c is the sign of d-y, c=(d-y)>0?0:1
    a += c;
    a = a>>1;       // arithmetic right shift 1
//  above is equal to a = (d+(sign of d-y))/2
    c = a + y;
    if(c>x)         // 0x400fe4
    {
        d = c-1;
        a = func4(x,y,d) * 2;   // 400fe6~400fee
        return a;
    }
    else            // c<=y         
    {
        a = 0;      // 0x400ff2
        if(c>=x) return a;  // 0x400ff9
        y = c+1;
        a = func(x,y,d);    // 0x400ffe
        a = 2*a + 1;
        return a;
    }
}
```
这里phase_4是通往secret_phase的一个关键, 其实分析开头就可以发现一开始为栈预留了0x18的空间, 而实际上我们只输入了2个数字, 因此可以肯定的是多预留了空间, 在后面讲secret_phase的时候会回到这里细讲;
首先从`0x401033`可以得知第一个输入的数不大于0xe(14), 然后把输入的数和两个固定值(0, 14)一起作为func4的参数调用, 要求返回值为0;
为了使func4返回0, 可以看到当进入第一个转移分支(c>x)的时候令c<=x进入`0x400ff2`之后a已经等于0, 只要满足c>=x即可返回0, 那么只有当c==x时可以满足要求,此时的c等于7, 也就是输入的第一个数为7;
第二个数的判断就简单了, 在`0x401051`处只要满足第二个数(存储在%rsp+12)等于0即可, 因此最后的输入为`7 0`;



