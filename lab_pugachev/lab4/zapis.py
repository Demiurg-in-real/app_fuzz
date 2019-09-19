def zapis():
    fil=open("rabvar.bin", "rb+")
    if fil==None:
        print('Vse ploho')
        return 0
    lbadiff=["\x45","\x46","\x49", "\x20","\x50","\x41","\x52","\x54",                   "\x00","\x00","\x01", "\x00"                    , "\x5c", "\x00","\x00","\x00",                    "\x27","\x6d", "\x9f", "\xc9",                   "\x00","\x00","\x00","\x00",            "\x01","\x00","\x00","\x00","\x00","\x00","\x00","\x00",             "\x00","\x05","\x00","\x00","\x00","\x00","\x00","\x00",              "\x22","\x00","\x00","\x00","\x00","\x00","\x00","\x00",                        "\x47","\x1d", "\x00","\x00","\x00","\x00","\x00","\x00",                      "\x00","\xa2", "\xda","\x98", "\x9f", "\x79", "\xc0", "\x01","\xa1","\xf4","\x04","\x62","\x2f","\xd5","\xEC","\x6D",                          "\x02","\x00","\x00","\x00","\x00","\x00","\x00","\x00",               "\x80","\x00","\x00","\x00",                "\x80","\x00","\x00","\x00",                     "\x27","\xC3","\xF3","\x85"]
    for index in lbadiff:
        index=index.encode('utf-8')
        fil.write(index)
    fil.close()
    return 0

if __name__=='__main__':
    zapis()
    exit()
