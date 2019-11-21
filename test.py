l=[['start',0]]
try:
    while(True):
        r=input()
        for i in l:
            counter=0
            if i[0] != r:
                counter+=1
            if i[0] == r:
                i[1]+=1
            if counter != len(r):
                l.append([r,0])
            
except:
    while(len(l) != 0):
        for i in l:
            check=i[1]
            counter=0
            for j in l:
                if check>j[0]:
                    counter+=1
                if counter == len(l):
                    print(check)
                         
