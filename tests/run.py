#!/usr/bin/env python
# run.py

import os
import string
import sys

def run(fname):
    print 'running: ' + fname
    with open(fname, 'r') as f:
        with open('/tmp/in', 'w') as inputsf:
            with open('/tmp/exp', 'w') as expf:
                for line in f.readlines():
                    tokens = filter(lambda x: x!='', map(string.strip, line.split(':')))
                    if len(tokens) == 2:
                        print >>inputsf, tokens[0]
                        print >>expf, tokens[1]
                    elif len(tokens) != 0:
                        print '*** Non empty test-case not run: %r' % tokens

    os.system('../yall /tmp/in > /tmp/out')
    os.system('diff /tmp/exp /tmp/out')


def drill():
    for _, _, filenames in os.walk('.'):
        for fname in filenames:
            if fname[0:5]=='test_':
                run(fname)

# start here
print "YALL Test Cases"
if(len(sys.argv) == 1):
    drill() # run all test case files we can find
else:
    for f in sys.argv[1:]:
        run(f)
