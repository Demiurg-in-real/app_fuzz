import os

def main():
    with open('image.bin','rb') as f:
        a=f.read()
        c=a.count(b'\xf8\xff\xff\xff')#+a.count(b'\xf8\xff')
        print(c)
    return 0
if __name__=='__main__':
    main()
    exit()
