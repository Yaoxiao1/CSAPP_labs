#bomblab 思路介绍

- [常用指令](#常用指令)
- [BombLab phase_1](#bomblab-phase_1)
- [BombLab phase_2](#bomblab-phase_2)
- [BombLab phase_3](#bomblab-phase_3)
- [BombLab phase_4](#bomblab-phase_4)
- [Bomblab Phase_5](#bomblab-phase_5)
- [Bomblab phase_6](#bomblab-phase_6)
- [Bomblab secret_phase](#bomblab-secret_phase)

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

## Bomblab Phase_5
```assembly
0000000000401062 <phase_5>:
  401062:	53                   	push   %rbx
  401063:	48 83 ec 20          	sub    $0x20,%rsp
  401067:	48 89 fb             	mov    %rdi,%rbx
  40106a:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
  401071:	00 00 
  401073:	48 89 44 24 18       	mov    %rax,0x18(%rsp)
  401078:	31 c0                	xor    %eax,%eax
  40107a:	e8 9c 02 00 00       	callq  40131b <string_length>
  40107f:	83 f8 06             	cmp    $0x6,%eax
  401082:	74 4e                	je     4010d2 <phase_5+0x70>
                                    ----------------------
  401084:	e8 b1 03 00 00       	callq  40143a <explode_bomb>
  401089:	eb 47                	jmp    4010d2 <phase_5+0x70>
                                    ----------------------
  40108b:	0f b6 0c 03          	movzbl (%rbx,%rax,1),%ecx
  40108f:	88 0c 24             	mov    %cl,(%rsp)
  401092:	48 8b 14 24          	mov    (%rsp),%rdx
  401096:	83 e2 0f             	and    $0xf,%edx
  401099:	0f b6 92 b0 24 40 00 	movzbl 0x4024b0(%rdx),%edx
  4010a0:	88 54 04 10          	mov    %dl,0x10(%rsp,%rax,1)
  4010a4:	48 83 c0 01          	add    $0x1,%rax
  4010a8:	48 83 f8 06          	cmp    $0x6,%rax
  4010ac:	75 dd                	jne    40108b <phase_5+0x29>
  4010ae:	c6 44 24 16 00       	movb   $0x0,0x16(%rsp)
  4010b3:	be 5e 24 40 00       	mov    $0x40245e,%esi
  4010b8:	48 8d 7c 24 10       	lea    0x10(%rsp),%rdi
  4010bd:	e8 76 02 00 00       	callq  401338 <strings_not_equal>
                                    ----------------------
  4010c2:	85 c0                	test   %eax,%eax
  4010c4:	74 13                	je     4010d9 <phase_5+0x77>
                                    ----------------------
  4010c6:	e8 6f 03 00 00       	callq  40143a <explode_bomb>
  4010cb:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1) // nop, for alignment
  4010d0:	eb 07                	jmp    4010d9 <phase_5+0x77>
                                    ----------------------
  4010d2:	b8 00 00 00 00       	mov    $0x0,%eax
  4010d7:	eb b2                	jmp    40108b <phase_5+0x29>
                                    ----------------------
  4010d9:	48 8b 44 24 18       	mov    0x18(%rsp),%rax
  4010de:	64 48 33 04 25 28 00 	xor    %fs:0x28,%rax
  4010e5:	00 00 
  4010e7:	74 05                	je     4010ee <phase_5+0x8c>
                                    ----------------------
  4010e9:	e8 42 fa ff ff       	callq  400b30 <__stack_chk_fail@plt>
                                    ----------------------
  4010ee:	48 83 c4 20          	add    $0x20,%rsp
  4010f2:	5b                   	pop    %rbx
  4010f3:	c3                   	retq   
```
phase_5的跳转没有phase_4复杂,但是对于数据的处理比较多,因此要仔细核对,同时这里也出现了前面几个phase里没见过的汇编指令, 比如`0x40106a`把 %fs段偏移0x28的数据移到了%rax寄存器里, 查了下这个叫`金丝雀数`, 为了防止缓冲区溢出的手段, 对当前实验没有太大影响, 可以暂时忽略.
对应的伪代码如下:
```c++
// %rax in a, %rbx in b, %rcx in c, %rdx in d, %rbp in bp
// %rdi in x, %rsi in y, %rsp in p
phase_5();
b = x;
a = 0;      // 0x401078
a = string_length(input_str);    // get length of input string
if(a!=6) explode bomb;           // 0x401084
a = 0;      // 0x4010d2
c = *(b+a); // 0x40108b
d = c;      // 0x40108f~0x401092
d = d%16;   // and& 0xf means get the last 4 bits of %rdx
d = *(d+0x4024b0);  // need to get the data from 0x4024b0
*(p+a+0x10) = d;    // store in stack
// above can simplify as *(p+a+0x10) = *(*(b+a)%16+0x4024b0)
a += 1;
while(a!=6)
{
    c = *(b+a);     // repeat from 0x40108b
    *(p+a+0x10) = *(c%16+0x4024b0);
    ++a;
}
*(rsp+0x16) = 0     // 0x4010ae set end of string , \0
y = 0x40245e;
x = *(rsp+10);      // the string we store from 0x4010d2
if(x!=y) explode bomb;
return;
```
从伪代码来看phase_5的逻辑很简单, 出口就是要对比输入字符串和位于*0x40245e处的字符串, 只要相等即可解除炸弹, 通过`print (char*) 0x40245e`可以知道要比较的字符串是`flyers`, 那么再来根据前面的对应规则来一一解码;
首先肯定是要输入一个长度为6的字符串, 否则直接引爆炸弹, 然后通过`print $rbx`获取对应%rcx上的ASCII码,经过对16取余(and 0xf)获取我们在`0x4024b0`位置上的偏移量, 通过偏移量来一个一个得到最终的`flyers`, 这里的答案有多个, 需要对照ASCII码表来找, 我这里的是`9/.567`

## Bomblab phase_6
``` assembly
00000000004010f4 <phase_6>:
  4010f4:	41 56                	push   %r14
  4010f6:	41 55                	push   %r13
  4010f8:	41 54                	push   %r12
  4010fa:	55                   	push   %rbp
  4010fb:	53                   	push   %rbx
  4010fc:	48 83 ec 50          	sub    $0x50,%rsp
  401100:	49 89 e5             	mov    %rsp,%r13
  401103:	48 89 e6             	mov    %rsp,%rsi
  401106:	e8 51 03 00 00       	callq  40145c <read_six_numbers>
  40110b:	49 89 e6             	mov    %rsp,%r14
  40110e:	41 bc 00 00 00 00    	mov    $0x0,%r12d
  401114:	4c 89 ed             	mov    %r13,%rbp
  401117:	41 8b 45 00          	mov    0x0(%r13),%eax
  40111b:	83 e8 01             	sub    $0x1,%eax
  40111e:	83 f8 05             	cmp    $0x5,%eax
  401121:	76 05                	jbe    401128 <phase_6+0x34>
                                    ----------------------
  401123:	e8 12 03 00 00       	callq  40143a <explode_bomb>
  401128:	41 83 c4 01          	add    $0x1,%r12d
  40112c:	41 83 fc 06          	cmp    $0x6,%r12d
  401130:	74 21                	je     401153 <phase_6+0x5f>
                                    ----------------------
  401132:	44 89 e3             	mov    %r12d,%ebx
  401135:	48 63 c3             	movslq %ebx,%rax
  401138:	8b 04 84             	mov    (%rsp,%rax,4),%eax
  40113b:	39 45 00             	cmp    %eax,0x0(%rbp)
  40113e:	75 05                	jne    401145 <phase_6+0x51>
                                    ----------------------
  401140:	e8 f5 02 00 00       	callq  40143a <explode_bomb>
  401145:	83 c3 01             	add    $0x1,%ebx
  401148:	83 fb 05             	cmp    $0x5,%ebx
  40114b:	7e e8                	jle    401135 <phase_6+0x41>
                                    ----------------------
  40114d:	49 83 c5 04          	add    $0x4,%r13
  401151:	eb c1                	jmp    401114 <phase_6+0x20>
                                    ----------------------
  401153:	48 8d 74 24 18       	lea    0x18(%rsp),%rsi
  401158:	4c 89 f0             	mov    %r14,%rax
  40115b:	b9 07 00 00 00       	mov    $0x7,%ecx
  401160:	89 ca                	mov    %ecx,%edx
  401162:	2b 10                	sub    (%rax),%edx
  401164:	89 10                	mov    %edx,(%rax)
  401166:	48 83 c0 04          	add    $0x4,%rax
  40116a:	48 39 f0             	cmp    %rsi,%rax
  40116d:	75 f1                	jne    401160 <phase_6+0x6c>
                                    ----------------------
  40116f:	be 00 00 00 00       	mov    $0x0,%esi
  401174:	eb 21                	jmp    401197 <phase_6+0xa3>
                                    ----------------------
  401176:	48 8b 52 08          	mov    0x8(%rdx),%rdx
  40117a:	83 c0 01             	add    $0x1,%eax
  40117d:	39 c8                	cmp    %ecx,%eax
  40117f:	75 f5                	jne    401176 <phase_6+0x82>
                                    ----------------------
  401181:	eb 05                	jmp    401188 <phase_6+0x94>
                                    ----------------------
  401183:	ba d0 32 60 00       	mov    $0x6032d0,%edx
  401188:	48 89 54 74 20       	mov    %rdx,0x20(%rsp,%rsi,2)
  40118d:	48 83 c6 04          	add    $0x4,%rsi
  401191:	48 83 fe 18          	cmp    $0x18,%rsi
  401195:	74 14                	je     4011ab <phase_6+0xb7>
                                    ----------------------
  401197:	8b 0c 34             	mov    (%rsp,%rsi,1),%ecx
  40119a:	83 f9 01             	cmp    $0x1,%ecx
  40119d:	7e e4                	jle    401183 <phase_6+0x8f>
                                    ----------------------
  40119f:	b8 01 00 00 00       	mov    $0x1,%eax
  4011a4:	ba d0 32 60 00       	mov    $0x6032d0,%edx
  4011a9:	eb cb                	jmp    401176 <phase_6+0x82>
                                    ----------------------
  4011ab:	48 8b 5c 24 20       	mov    0x20(%rsp),%rbx
  4011b0:	48 8d 44 24 28       	lea    0x28(%rsp),%rax
  4011b5:	48 8d 74 24 50       	lea    0x50(%rsp),%rsi
  4011ba:	48 89 d9             	mov    %rbx,%rcx
  4011bd:	48 8b 10             	mov    (%rax),%rdx
  4011c0:	48 89 51 08          	mov    %rdx,0x8(%rcx)
  4011c4:	48 83 c0 08          	add    $0x8,%rax
  4011c8:	48 39 f0             	cmp    %rsi,%rax
  4011cb:	74 05                	je     4011d2 <phase_6+0xde>
                                    ----------------------
  4011cd:	48 89 d1             	mov    %rdx,%rcx
  4011d0:	eb eb                	jmp    4011bd <phase_6+0xc9>
                                    ----------------------
  4011d2:	48 c7 42 08 00 00 00 	movq   $0x0,0x8(%rdx)
  4011d9:	00 
  4011da:	bd 05 00 00 00       	mov    $0x5,%ebp
  4011df:	48 8b 43 08          	mov    0x8(%rbx),%rax
  4011e3:	8b 00                	mov    (%rax),%eax
  4011e5:	39 03                	cmp    %eax,(%rbx)
  4011e7:	7d 05                	jge    4011ee <phase_6+0xfa>
                                    ----------------------
  4011e9:	e8 4c 02 00 00       	callq  40143a <explode_bomb>
  4011ee:	48 8b 5b 08          	mov    0x8(%rbx),%rbx
  4011f2:	83 ed 01             	sub    $0x1,%ebp
  4011f5:	75 e8                	jne    4011df <phase_6+0xeb>
                                    ----------------------
  4011f7:	48 83 c4 50          	add    $0x50,%rsp
  4011fb:	5b                   	pop    %rbx
  4011fc:	5d                   	pop    %rbp
  4011fd:	41 5c                	pop    %r12
  4011ff:	41 5d                	pop    %r13
  401201:	41 5e                	pop    %r14
  401203:	c3                   	retq   
```
pahse_6不愧是作为压轴的题目, 代码长度跟复杂程度直接提升了两个档次, 这个题一定要把跳转逻辑好好标清楚, 然后写出等价的伪代码, 把不同的功能分块, 等价的伪代码如下:
```c++
// %rax in a, %rbx in b, %rcx in c, %rdx in d, %rbp in bp
// %rdi in x, %rsi in y, %rsp in p, %rbp in bp,
// %r12 in r12, %r13 in r13, %r14 in r14
phase_6();
do{
    y = r13 = p;
    a = read_six_numbers();     // input 6 numbers store from %rsp to %rsp+20
    r14 = p;
    r12 = 0;
    bp = r13;    // here %r13 equals %rsp, stores the first number
    a = *r13-1;
    if(a>5) explode bomb;   // 0x401123
    r12++;
    if(r12==6) break;       // 0x40112c
    b = r12;
    do{
        a = *(p+b*4);       // get next number
        if(a==*bp) explode bomb;    // 0x40113b
        b++;
    } while(b<=5);          // this circle compares each number with numbers after, expolde bomb when equal
    r13 += 4;
} while(r12!=6);
// ---------------------------------------------------------
// part1, check input 6 numbers, make sure they are unique
x = *(p+24);     // 0x401153
a = r14;         // r14 stores the first number
c = 7;
do{
    d = c;       // 0x401160, another circle, d is 7
    d = d - *a;
    *a = d;
    // above can simplify as *a = 7 - *a
    a += 4;
} while(a!=x);    // 0x40116d jump out the circle when traverse all 6 numbers
// ---------------------------------------------------------
// part2, modify the numbers input, num = 7 - num;
y = 0;      // 0x40116f
do{
    c = *(p+y);     // 0x401197
    while(c<=1)
    {
        d = 0x6032d0;   // 0x401183
        *(p+2*y+0x20) = d;  // store numbers
        y += 4;
        if(y==0x18) break 2circle;  // break 2 circles
    }
    a = 1;
    d = 0x6032d0;
    do {
        d = *(d+8);     // 0x401176
        a += 1;
    }while(a!=c);
    *(p+2*y+0x20) = d;
    y += 4;
    if(y==0x18) break;
} while(true);
// ---------------------------------------------------------
// part3, from 0x6032d0 there is an array, so we need to print out the value of each node to next part
b = *(p+0x20);  // 0x4011ab
a = p+0x28;
y = p+0x50;
c = b;
do{
    d = *a;     // 0x4011bd 
    (*c+8) = d;
    a += 8;
    if(a==y) break;     // 0x4011cb
    c = d;
} while(true);      // put numbers from part3 to stack pointer %rsp+0x20
// ---------------------------------------------------------
// part4, in this part we put the encoded number to stack pointer from %rsp+0x20 to %rsp+0x50
bp = 5;
do {
    a = *(b+8);    // pointer to next node
    a = *a;
    if(*b<a) expolde bomb;       // b=*(p+0x20)
    b = *(b+8)
    bp--;
} while(bp!=0)      // 0x4011f5
return;
// ---------------------------------------------------------
// part5, this part checks the encoded number ,make sure it is decending
```
这个phase的代码特别长, 所以要分而治之, 从伪代码来看, 我把它分成了5个部分, 第一个部分从`0x401100`到`0x401151`, 这个部分它首先检查是否输入了6个数, 然后检查每个数都不相等;
第二部分把每个数求对7的补来替代(也就是num = 7 -num);
第三部分涉及到一个数组, 数组从`0x6032d0`开始,储存6个数,依次是`0x14c, 0xa8, 0x39c, 0x2b3, 0x1dd, 0x1bb`, 排列顺序从大到小是`3>4>5>6>1>2`然后根据part3里分支的选择方法,根据%rcx的值决定把哪个节点的值存到%rsp+0x20上面,这个在part4里完成,在part5里对这些数字进一步处理;
part5就是把刚刚存在%rsp+0x20的数字一个个提取出来, 判断他们是降序排列的, 因此我们可以根据这个要求反推出在part3里我们的数据应该如何排列, 最终的结果是`4 3 2 1 6 5`;

## Bomblab secret_phase
正常情况下, 回答完6个phase之后会直接结束本实验, 但是我们在看汇编代码的时候发现在phase_6后面有一个secret_phase, 说明这个实验还有一个彩蛋, 那么第一个要做的就是找到secret_phase的入口, 在反汇编程序里搜索secret_phase,可以发现在`phase_defused`里有一个secret_phase的入口
```assembly
00000000004015c4 <phase_defused>:
  4015c4:	48 83 ec 78          	sub    $0x78,%rsp
  4015c8:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
  4015cf:	00 00 
  4015d1:	48 89 44 24 68       	mov    %rax,0x68(%rsp)
  4015d6:	31 c0                	xor    %eax,%eax
  4015d8:	83 3d 81 21 20 00 06 	cmpl   $0x6,0x202181(%rip)        # 603760 <num_input_strings>
  4015df:	75 5e                	jne    40163f <phase_defused+0x7b>
                                    ----------------------
  4015e1:	4c 8d 44 24 10       	lea    0x10(%rsp),%r8
  4015e6:	48 8d 4c 24 0c       	lea    0xc(%rsp),%rcx
  4015eb:	48 8d 54 24 08       	lea    0x8(%rsp),%rdx
  4015f0:	be 19 26 40 00       	mov    $0x402619,%esi
  4015f5:	bf 70 38 60 00       	mov    $0x603870,%edi
  4015fa:	e8 f1 f5 ff ff       	callq  400bf0 <__isoc99_sscanf@plt>
  4015ff:	83 f8 03             	cmp    $0x3,%eax
  401602:	75 31                	jne    401635 <phase_defused+0x71>
                                    ----------------------
  401604:	be 22 26 40 00       	mov    $0x402622,%esi
  401609:	48 8d 7c 24 10       	lea    0x10(%rsp),%rdi
  40160e:	e8 25 fd ff ff       	callq  401338 <strings_not_equal>
  401613:	85 c0                	test   %eax,%eax
  401615:	75 1e                	jne    401635 <phase_defused+0x71>
                                    ----------------------
  401617:	bf f8 24 40 00       	mov    $0x4024f8,%edi
  40161c:	e8 ef f4 ff ff       	callq  400b10 <puts@plt>
  401621:	bf 20 25 40 00       	mov    $0x402520,%edi
  401626:	e8 e5 f4 ff ff       	callq  400b10 <puts@plt>
  40162b:	b8 00 00 00 00       	mov    $0x0,%eax
  401630:	e8 0d fc ff ff       	callq  401242 <secret_phase>
```
按惯例写成伪代码的形式
```c++
phase_defused();    // 0x4015c4
// %rax in a, %rbx in b, %rcx in c, %rdx in d, %rbp in bp
// %rdi in x, %rsi in y, %rsp in p, %rbp in bp,
// %r12 in r12, %r13 in r13, %r14 in r14, %rip in rp
a = 0;   // 0x4015d6
if(*(rp+0x202181)!=6) return;
r8 = p + 0x10;
c = p + 0xc;
d = p + 0x8;
y = 0x402619;   // *(0x402619) == the input of phase_4
x = 0x603870;   // *(0x603870) == %d %d %s
a = scanf(x, y);    // a is the number of params scanned
if(a!=3) return;    // 0x4015ff
y = 0x402622;       // *(0x402622) == DrEvil
x = p+0x10;         // x is the 3rd input string of phase_4
a = strings_not_equal(x,y);     // compare x and y, return 1 if not equal
if(a!=0) return;
puts(0x4024f8);     // 0x40161c
puts(0x402520);     // 0x401621
a = 0;
call secret_phase();
```
分析phase_defused()的伪代码我们发现, 要进入秘密关卡必须满足一个string_not_equal函数, 对比的string存在0x402622, 打印出来是`DrEvil`,再往上追溯的话可以看到有两个引人注意的地址`0x402619`和`0x603870`,
这两个作为`0x4015fa scanf()`的输入, 第一个打印出来后发现是phase_4的输入, 第二个打印出来是`%d %d %s`表明要检查两个整数和一个字符串, 
正常情况下phase_4的答案只有2个数字, 但是我们前面发现了phase_4在栈上预留的空间远多于2个int的空间, 说明在phase_4可以多加一个字符串的输入, 并且对应的应该是位于`0x462622`的字符串`DrEvil`, 加上这句后, 我们就愉快地进入了秘密关卡, 下面再来分析这个秘密关;
```assembly
0000000000401242 <secret_phase>:
  401242:	53                   	push   %rbx
  401243:	e8 56 02 00 00       	callq  40149e <read_line>
  401248:	ba 0a 00 00 00       	mov    $0xa,%edx
  40124d:	be 00 00 00 00       	mov    $0x0,%esi
  401252:	48 89 c7             	mov    %rax,%rdi
  401255:	e8 76 f9 ff ff       	callq  400bd0 <strtol@plt>
  40125a:	48 89 c3             	mov    %rax,%rbx
  40125d:	8d 40 ff             	lea    -0x1(%rax),%eax
  401260:	3d e8 03 00 00       	cmp    $0x3e8,%eax
  401265:	76 05                	jbe    40126c <secret_phase+0x2a>
                                    ---------------------------
  401267:	e8 ce 01 00 00       	callq  40143a <explode_bomb>
  40126c:	89 de                	mov    %ebx,%esi
  40126e:	bf f0 30 60 00       	mov    $0x6030f0,%edi
  401273:	e8 8c ff ff ff       	callq  401204 <fun7>
  401278:	83 f8 02             	cmp    $0x2,%eax
  40127b:	74 05                	je     401282 <secret_phase+0x40>
                                    ---------------------------
  40127d:	e8 b8 01 00 00       	callq  40143a <explode_bomb>
  401282:	bf 38 24 40 00       	mov    $0x402438,%edi
  401287:	e8 84 f8 ff ff       	callq  400b10 <puts@plt>
  40128c:	e8 33 03 00 00       	callq  4015c4 <phase_defused>
                                    ---------------------------
```
来到secret_phase, 看到不算长的代码, 总算舒了一口气, 如果再出现phase_6的那个量, 估计至少得干1小时, 标记好跳转指令, 写出伪代码如下:
```c++
// %rax in a, %rbx in b, %rcx in c, %rdx in d, %rbp in bp
// %rdi in x, %rsi in y, %rsp in p, %rbp in bp
a = readline();
d = 0xa;
y = 0;
x = a;
a = strtol(x,y);
b = a;
a = a - 1;
if(a>0x3e8) explode bomb;   // 0x401265
y = b;
x = 0x6030f0;   // head of a linked list, value=36, next=0x603110
a = fun7(x,y);  // 0x401273
if(a!=2) explode bomb;  // 0x40127b
x = 0x402438;
puts(x);
return;
```
secret_phase的main部分是比较简单的, 首先判断输入的数不大于0x3e9, 然后调用fun7函数,只有当返回值等于2时才不会引爆炸弹, 所以关键点在于这个fun7;
```assembly
0000000000401204 <fun7>:
  401204:	48 83 ec 08          	sub    $0x8,%rsp
  401208:	48 85 ff             	test   %rdi,%rdi
  40120b:	74 2b                	je     401238 <fun7+0x34>
                                    -------------------------
  40120d:	8b 17                	mov    (%rdi),%edx
  40120f:	39 f2                	cmp    %esi,%edx
  401211:	7e 0d                	jle    401220 <fun7+0x1c>
                                    -------------------------
  401213:	48 8b 7f 08          	mov    0x8(%rdi),%rdi
  401217:	e8 e8 ff ff ff       	callq  401204 <fun7>
  40121c:	01 c0                	add    %eax,%eax
  40121e:	eb 1d                	jmp    40123d <fun7+0x39>
                                    -------------------------
  401220:	b8 00 00 00 00       	mov    $0x0,%eax
  401225:	39 f2                	cmp    %esi,%edx
  401227:	74 14                	je     40123d <fun7+0x39>
                                    -------------------------
  401229:	48 8b 7f 10          	mov    0x10(%rdi),%rdi
  40122d:	e8 d2 ff ff ff       	callq  401204 <fun7>
  401232:	8d 44 00 01          	lea    0x1(%rax,%rax,1),%eax
  401236:	eb 05                	jmp    40123d <fun7+0x39>
                                    -------------------------
  401238:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
  40123d:	48 83 c4 08          	add    $0x8,%rsp
  401241:	c3                   	retq   
```
fun7的长度不算长,但是跳转比较多, 所以作为秘密关卡,还是有它的挑战性所在的
```c++
fun7(x, y);     // 0x401204, x=0x6030f0, y=b(the input number) 
// %rax in a, %rbx in b, %rcx in c, %rdx in d, %rbp in bp
// %rdi in x, %rsi in y, %rsp in p, %rbp in bp
if(x==0) return -1; // 0x401238
d = *x;
if(d>y)
{
    x = *(x+8);
    a = fun7(x, y);
    a = a * 2;      // 0x40121c
    return a;
}
a = 0;      // 0x401220
if(d==y) return a;  // 0x401227
x = *(x+0x10);      // 0x401229
a = fun7(x,y);
a = a*2 + 1;        // 0x401232
return a;
```
fun7的核心在于他的几个跳转, 并且要求最后返回值为2, 这里面涉及到递归和链表, 所以显得复杂了一些, 链表的其实在0x6030f0, 因此要把这些链表的value部分和next指向的位置弄明白, 才能得到要求的返回值, 从`0x6030f0`开始一直到`0x603198`有差不多10个数值, 但是我们用不到那么多;
分析三个分支发现, `*x>y`时返回的是`2*a`是个偶数;
`*x==y`时, 返回`0`;
`*x<y`时, 返回`2a+1`是个奇数;
这么一来我们要得到2的话,首先是a=0, 然后通过2a+1得到1, 然后通过2a返回2, 经历三层递归得到我们要的返回值2, 至此secret_phase就得到了解答, 正确的输入值我这里是`22`

