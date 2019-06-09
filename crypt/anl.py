#!/usr/bin/env python3

class Statistic:
    def __init__(self, macs, phones):
        self.macs = macs
        self.phones = phones
        self.terminals = []
        self.routers = []
        self.fakes = []
        self.others = []

        for p in self.phones.keys():
            ms = self.phones[p]

            if len(ms) == 1 and len(self.macs[ms[0]]) == 1 and self.macs[ms[0]][0] == p:
                # terminal
                self.terminals.append(ms[0])
            elif len(ms) == 2:
                # router
                for m in ms:
                    if len(self.macs[m]) == 1 and self.macs[m][0] == p:
                        self.terminals.append(m)
                    else:
                        self.routers.append(m)
            elif len(ms) > 2:
                # fake
                for m in ms:
                    self.fakes.append(m)
            else:
                for m in ms:
                    self.others.append(m)

        for m in self.fakes:
            if m in self.routers:
                self.fakes.remove(m)

    def Print(self):
        nb_terminals = len(self.terminals)
        nb_routers = len(self.routers)
        nb_fakes = len(self.fakes)
        nb_others = len(self.others)
        nb_total = nb_terminals + nb_routers + nb_fakes

        print("Terminal %d (%.02f%%) Router %d (%.02f%%) Fake %d (%.02f%%) Others %d (%.02f%%) Total %d"
              % (nb_terminals, nb_terminals / nb_total * 100,
                 nb_routers,   nb_routers / nb_total * 100,
                 nb_fakes,     nb_fakes / nb_total * 100,
                 nb_others,    nb_others / nb_total * 100,
                 nb_total))

    def Terminals(self):
        return tuple(self.terminals)

    def Routers(self):
        return tuple(self.routers)

    def Fakes(self):
        return tuple(self.fakes)

def Display(d, keys):
    for k in keys:
        if k in d:
            l = d[k]
            print("Key {} -> {}".format(k, l))

def main(argv):
    if len(argv) != 2:
        print("Usage: %s file" % argv[0])
        return

    macs = dict()
    phones = dict()

    with open(argv[1], "r") as f:
        while True:
            s = f.readline();
            if len(s) == 0:
                break
            print(s)
            pair = s.split('\t')
            #m, p, t = pair[0], pair[1].strip(), pair[2].strip()
            m, p = pair[0], pair[1].strip()

            # Insert item based the MAC
            if macs.get(m) is None:
                macs[m] = []
            if p not in macs[m]:
                macs[m].append(p)

            # Insert item based the phone number
            if phones.get(p) is None:
                phones[p] = []
            if m not in phones[p]:
                phones[p].append(m)

    s = Statistic(macs, phones)
    s.Print()
    #Display(macs, s.others)

if __name__ == "__main__":
    import sys
    main(sys.argv)
