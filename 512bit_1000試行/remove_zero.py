
infile = open('./nsec.csv','r')

for line in infile:
    if line.rsplit('\n')[0] != '0':
        print(line.rsplit('\n')[0])

infile.close()