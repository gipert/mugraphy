import sys
import os

batch = sys.argv[1]

os.makedirs(batch + '/mac')
os.makedirs(batch + '/out')
os.makedirs(batch + '/log')

with open(batch + '.mac') as f:
    text = f.read()

for i in range(0, 200):
    mtext = text.replace('$OUTPUT', f'out/detector-hits-{i}.root')
    with open(f'{batch}/mac/{batch}-{i}.mac', 'w') as fout:
        fout.write(mtext)
