use ExtUtils::MakeMaker;
# See lib/ExtUtils/MakeMaker.pm for details of how to influence
# the contents of the Makefile that is written.

WriteMakefile
  ('NAME'    => 'cedar',
   'CC'      => 'g++',
   'LD'      => 'g++',
   'DEFINE'  => '-DHAVE_CONFIG_H',
   'CCFLAGS' => $CCFLAGS,
   'INC'     => '-I.. -I../.. -I../../src',
   'OBJECT'  => 'cedar_wrap.o',
);
