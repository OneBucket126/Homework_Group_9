import sm2signature

# 直接采用ecdsa的测试参数
a = 29
b = 18
p = 17
g = [5, 1]
n = 19
k = 2
d = 5

m = '1234'
e = hash(m)
pk = sm2signature.mutipoint(g[0], g[1], d, a, p)
ID_A = "1234567890"
tmp = str(len(ID_A)) + ID_A + str(a) + str(b) + str(g[0]) + str(g[1]) + str(pk[0]) + str(pk[1])
ZA = sm2signature.sm3hash(tmp)
m1 = '1234567890'
m2 = '0123456789'


def leakingk():
    r, s = sm2signature.SM2_signature(m, n, g, d, ZA, k, a, p)
    sk = (sm2signature.modinv(s + r, n) * (k - s)) % n
    print('leeakingk sk:', sk)


def reusingk():
    r1, s1 = sm2signature.SM2_signature(m1, n, g, d, ZA, k, a, p)
    r2, s2 = sm2signature.SM2_signature(m2, n, g, d, ZA, k, a, p)
    sk = ((s2 - s1) * sm2signature.modinv((s1 - s2 + r1 - r2), n)) % n
    print("reusingk sk:", sk)


def diff_reuse():
    r1, s1 = sm2signature.SM2_signature(m1, n, g, d, ZA, k, a, p)
    r2, s2 = sm2signature.SM2_signature(m2, n, g, d, ZA, k, a, p)
    sk1 = ((k - s1) * sm2signature.modinv(s1 + r1, n)) % n
    sk2 = ((k - s2) * sm2signature.modinv(s2 + r2, n)) % n
    print("reusing k by different users ""sk1:", sk1, "sk2", sk2)


def samedk():
    e1 = hash(m)
    r1, s1 = sm2signature.SM2_signature(m1, n, g, d, ZA, k, a, p)
    r2, s2 = sm2signature.SM2_signature(m2, n, g, d, ZA, k, a, p)
    sk = ((s1 * s2 - e1) * sm2signature.modinv((r1 - s1 * s2 - s1 * r2), n)) % n
    print("same dk with ecdsa  sk:", sk)


leakingk()
reusingk()
diff_reuse()
samedk()
