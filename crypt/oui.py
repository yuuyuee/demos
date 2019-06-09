#!/usr/bin/env python3

def main(argv):
    if len(argv) != 2:
        print("Usage: %s file" % argv[0])
        return

    print("// X3130 (yuyue) X3130@njsecnet.com")
    print("// Generated from oui.txt by oui.py, DO NOT EDIT !")
    print("")
    print("#ifndef __PIEV2_OUI_H__")
    print("#define __PIEV2_OUI_H__")
    print("#define MAKE_MAC_VENDORS(XXX) \\")

    mac = str()
    vendor = str()

    with open(argv[1], "r") as f:
        while True:
            s = f.readline()
            if len(s) == 0:
                break
            elif len(vendor) > 0:
                print("    XXX(0x{}UL, \"{}\") \\".format(mac, vendor))

            if s.find("(base 16)") < 0:
                continue
            pair = s.split('\t')
            mac = pair[0].split()[0].strip()
            vendor = pair[2].strip()
            if vendor[len(vendor) - 1] == '.':
                vendor = vendor[:len(vendor) - 1]

    print("    XXX(0x{}UL, \"{}\")".format(mac, vendor))
    print("")
    print("#endif // __PIEV2_OUI_H__")

if __name__ == "__main__":
    import sys
    main(sys.argv)
