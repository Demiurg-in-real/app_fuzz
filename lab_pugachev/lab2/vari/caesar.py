
qemu=open("testing", "rb")
addd=[]
for i in range(20):
    a=(qemu.read(1))
    addd.append(int(a))
print(addd)


