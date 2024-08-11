import sys

if len(sys.argv) < 2:
    print("ERROR: no file given")
    exit()

print("here is your texture:")
with open(sys.argv[1], 'r') as f:
    for i in range(32):
        temp = f.readline()
        out = ''
        for c in temp:
            if c == '0':
                out += ' '
            else:
                out += c
        print(out, end='')