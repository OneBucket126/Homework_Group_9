#用概念验证代码验证上述缺陷 
语言：Python

运行环境：PyCharm on Python 3.10.11

sm2的缺陷，一些攻击

1.leaking k
sk=(s+r)^(-1)*(k-s) mod n

![leaking k](2.png)
2. reusing k
sk=(s2-s1)/(s1-s2+r1-r2)

![reusing k](3.png)
3. reusing k by different users
sk2=(k-s2)/s2+r2
sk1=(k-s1)/(s1+r1)

![reusing k by different users](4.png)
4.same dk with ecdsa
 d=(s1s2-e)/(r1-s1s2-s1r2) mod n

![same dk with ecdsa](5.png)
运行结果：

![运行结果](1.png)