a = """
X   0   1   1
1   X   1   1   *
0   0   X   0
0   0   X   1
1   1   X   0   *
1   1   X   1   *
0   0   0   X
0   0   1   X
1   1   0   X   *
1   1   1   X   *


"""

a = a.strip()
a = [''.join(i.split('\t')) for i in a.split('\n')]
joins = []
rawjoins = []
used = [c for c in a]
for index, i in enumerate(a):
    for in2, j in enumerate(a[index+1:]):
        if sum(i[k] != j[k] for k in range(4)) == 1:
            if i in used:
                used.remove(i)
                
            if j in used:
                used.remove(j)
            
            fstr = ""
            for h in range(4):
                if j[h] != i[h]:
                    fstr += 'X'
                    
                else:
                    fstr += i[h]
                    
            fstr += '*' if i[-1] == '*' and j[-1] == '*' else '-'
            joins.append(fstr)
            rawjoins.append(i)
            rawjoins.append(j)
            

joins = set(joins)
joins = sorted([*joins, *used], key=lambda x: (100 * (4 - x.find('X')) - sum(int(c) for c in x[:-1].replace('X', '').replace(' ', '').replace('-', '').replace('*', ''))), reverse=True)
for i in joins:
    print('\t'.join(list(i.replace('-', ''))))
            