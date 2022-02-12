import sys
import os

template_macro = sys.argv[1]
batch = sys.argv[2]

os.makedirs(f'runs/{batch}/mac')
os.makedirs(f'runs/{batch}/out')
os.makedirs(f'runs/{batch}/log')

with open(template_macro) as f:
    text = f.read()

for i in range(1, 201):
    mtext = text.replace('$OUTPUT', f'out/detector-hits-{i}.root')
    with open(f'runs/{batch}/mac/{batch}-{i}.mac', 'w') as fout:
        fout.write(mtext)
