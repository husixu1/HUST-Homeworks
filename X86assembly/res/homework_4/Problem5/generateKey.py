import itertools

key='chenzh'
tab=[]

for i in key:
    tab.append((i, i.upper()))

for i in itertools.product(*tab):
    print(''.join(i))

