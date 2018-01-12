#!/bin/env python
import sys

def main():
    # check validity
    if len(sys.argv) != 2:
        print("Usage " + sys.argv[0] + " <file to parse>")
        return 1
    file = open(sys.argv[1], 'r')
    outFile = open("out.dat", 'w')

    # read all lines
    count = 0
    for line in file.readlines():
        for word in line.strip('\n').split(' '):
            if '*' in word:
                for i in range(int(word[:word.index('*')])):
                    outFile.write(word[word.index('*') + 1:] + ' ')

                    count = (count + 1) % 8
                    if count == 0:
                        outFile.write('\n')

            else:
                outFile.write(word + ' ')

                count = (count + 1) % 8
                if count == 0:
                    outFile.write('\n')

    # cleanup
    outFile.close()
    file.close()

if __name__ == "__main__":
    main()
