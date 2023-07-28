import random
import time
import sm3


def Rho_Attack(n):
    a = random.randint(0, 0xffff)
    res = []
    for i in range(0xffff):
        res.append(sm3.sm3(str(a))[:int(n / 4)])
        a = (2 * a + 1)
        if sm3.sm3(str(a))[:int(n / 4)] in res:
            print('Succeed')
            return
    print('Failed')


time_start = time.time()
n = 8
Rho_Attack(n)
time_end = time.time()
time_c = time_end - time_start
print("n=", n)
print('time cost', time_c, 's')
