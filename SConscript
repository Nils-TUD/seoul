# -*- Mode: Python -*-

import os
Import('env')

myenv = env.Clone()

# instruction emulator
myenv.Precious(myenv.Command(
    'executor/instructions.inc',
    'executor/build_instructions.py',
    '$SOURCE > $TARGET'
))

# Build register handling functions for Intel 82576vf model
for f in ['mmio', 'pci']:
    env.Command(
        'include/model/intel82576vf%s.inc' % f,
        ['model/intel82576vf/genreg.py', 'model/intel82576vf/reg_%s.py' % f],
        '${SOURCES[0]} ${SOURCES[1:]} ${TARGET}'
    )

# turn off these warnings. seoul produces too many of them :(
# -fno-strict-aliasing is needed e.g. for cpu_move
myenv.Append(CXXFLAGS = ' -Wno-unused-parameter -Wno-parentheses -fno-strict-aliasing -Wformat=0 -DPROFILE')
myenv.Append(CFLAGS = ' -Wno-unused-parameter -Wno-parentheses -fno-strict-aliasing -Wformat=0 -DPROFILE')

# somehow we have to compile the instructions.inc with at least -O1. otherwise gcc complains that
# an asm constraint is impossible. strange :/
btype = os.environ.get('NRE_BUILD')
if btype == 'debug':
    halienv = myenv.Clone()
    halienv.Append(CXXFLAGS = ' -O1 -fno-inline')
    halifax = halienv.Object(
    	'executor/halifax.cc', CPPPATH = [myenv['CPPPATH'], 'include', 'nre/include', 'executor']
   	)
else:
    halifax = myenv.Object(
    	'executor/halifax.cc', CPPPATH = [myenv['CPPPATH'], 'include', 'nre/include', 'executor']
    )
myenv.Depends(halifax, 'executor/instructions.inc')

files = [f for f in Glob('*/*.cc') if
         'executor/halifax.cc' not in str(f) and
         'unix/' not in str(f) and 'host/' not in str(f) and
         'model/intel82576vf.cc' not in str(f)]
files += ['host/hostkeyboard.cc', 'host/hostio.cc']

lib = myenv.StaticLibrary(
    'libseoul', [files, halifax], CPPPATH = [myenv['CPPPATH'], 'include', 'nre/include']
)
myenv.Install(myenv['LIBPATH'], lib)
