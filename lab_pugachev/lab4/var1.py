def zapis():
    fil=open("rabvar.bin", "rb+")
    if fil==None:
        print('Vse ploho')
        return 0
    lbadiff=["\x54","\x64","\x94", "\x02","\x05","\x14","\x25","\x45",                   "\x00","\x00","\x10", "\x00"                    , "\xc5", "\x00","\x00","\x00",                    "\x72","\xd6", "\xf9", "\x9c",                   "\x00","\x00","\x00","\x00",            "\x10","\x00","\x00","\x00","\x00","\x00","\x00","\x00",             "\x00","\x05","\x00","\x00","\x00","\x00","\x00","\x00",              "\x22","\x00","\x00","\x00","\x00","\x00","\x00","\x00",                        "\x74","\xd1", "\x00","\x00","\x00","\x00","\x00","\x00",                      "\x00","\x2a", "\xad","\x89", "\xf9", "\x97", "\x0c", "\x10","\x1a","\x4f","\x40","\x26","\xf2","\x5d","\xCE","\xD6",                          "\x20","\x00","\x00","\x00","\x00","\x00","\x00","\x00",               "\x08","\x00","\x00","\x00",                "\x08","\x00","\x00","\x00",                     "\x72","\x3c","\x3f","\x58"]
    for index in lbadiff:
        index=index.encode('utf-8')
        fil.write(index)
    fil.close()
    return 0

if __name__=='__main__':
    zapis()
    exit()
