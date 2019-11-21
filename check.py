l='>64Sg^}BrbW8J4JX'#str(input('Vvedite chislo: '))
punctuat='!"#$%&\'()*+,-./:;<=>?@[\\]^_`{|}~'
numb=False
lett=False
low=False
upp=False
punct=False
for i in l:
    print(i)
    for j in punctuat:
        if i == j:
            punct=True
            print('punct')
            break
    if i.isupper():
        upp=True
        print('upp')
    if i.islower():
        low=True
        print('low')
    if i.isdigit():
        numb=True
        print('numb')
    if i.isalpha():
        lett=True
        print('lett')
        continue
    if lett and numb and low and upp and punct:
        print('all')
        break
print(str(len(l))+str(len(set(l))))
print(upp)
print(low)
print(numb)
print(punct)
print(lett)
if 'kaspersky' in l.lower():
    print('yes')
if len(l)>=10 and len(set(l))>5 and ('kaspersky' in l.lower()):
    print(1)
    if upp  and low and numb and punct:
        print('GOOD')
else:
    print('BAD')
