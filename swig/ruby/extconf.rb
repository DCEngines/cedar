require 'mkmf'

$libs   = '-lstdc++'
$defs   = ['-DHAVE_CONFIG_H']
$INCFLAGS += ' -I.. -I../.. -I../../src'

create_makefile('cedar')
