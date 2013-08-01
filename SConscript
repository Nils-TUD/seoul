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

myenv.Append(CPPFLAGS = ' -DPROFILE')
# turn off these warnings. seoul produces too many of them :(
# -fno-strict-aliasing is needed e.g. for cpu_move
myenv.Append(CXXFLAGS = ' -Wno-unused-parameter -Wno-parentheses -fno-strict-aliasing -Wformat=0')
myenv.Append(CFLAGS = ' -Wno-unused-parameter -Wno-parentheses -fno-strict-aliasing -Wformat=0')

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


# now build vancouver

senv = env.Clone()

# use custom linker script (for params and profile-stuff)
senv['LINKFLAGS'] += ' -Wl,-T,libs/libseoul/nre/linker_' + senv['ARCH'] + '.ld'

# we need to use whole-archive here, because the devices are not used explicitly so that the linker
# thinks he doesn't need to link them
senv['LINKFLAGS'] += ' -Wl,--whole-archive -lseoul -Wl,--no-whole-archive'

# use a fixed link address to maximize the space for guest-memory
linkaddr = '0x60000000' if senv['ARCH'] == 'x86_32' else '0x400000000'
senv['LINKFLAGS'] += ' -Wl,--section-start=.init=' + linkaddr

prog = senv.NREProgram(
    senv, 'vancouver', Glob('nre/src/*.cc'),
    cpppath = ['#libs/libseoul/include', '#libs/libseoul/nre/include'],
    fixedaddr = True
)
senv.Depends(prog, senv['LIBPATH'] + '/libseoul.a')
senv.Depends(prog, 'nre/linker_' + senv['ARCH'] + '.ld')
