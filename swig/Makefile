all: perl-bindings ruby-bindings python-bindings lua-bindings

perl-bindings:
	swig -Wall -perl -outdir perl -c++ cedar.i
	mv -f cedar_wrap.cxx perl

python-bindings: 
	swig -Wall -python -builtin -outdir python -c++ cedar.i
	mv -f cedar_wrap.cxx python

ruby-bindings:
	swig -Wall -ruby -outdir ruby -c++ cedar.i
	mv -f cedar_wrap.cxx ruby/cedar_wrap.cpp

lua-bindings:
	swig -Wall -lua -outdir lua -c++ cedar.i
	mv -f cedar_wrap.cxx lua/cedar_wrap.cc

clean:
	cd perl   && perl Makefile.PL && make distclean && cd ..
	cd python && python setup.py clean --all        && cd ..
	cd ruby   && ruby extconf.rb  && make distclean && cd ..
	cd lua    && make clean && cd ..
	rm -f */cedar*
