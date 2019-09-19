def chistka():
    fil=open('rabvar.bin', 'rb+')
    if fil==None:
        print('Vse ploho')
        return 0
    x="\x00"
    data=True
    schet=0
    x=x.encode('utf-8')
    while(data):
        data=fil.read(1)
        schet=schet+1
        if data==None:
            break
    fil.seek(0)
    for i in range(schet):
        data=fil.write(x)
    fil.close()
    print(i, '\n')
    return 0

if __name__=='__main__':
    chistka()
    exit() 
