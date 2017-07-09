%module cedar
%{
#include "trie.h"
%}

%include "exception.i"
%include "std_vector.i"
%template (vectorr) std::vector <result_t>;

#ifdef SWIGPYTHON

#ifdef SWIGPYTHON_BUILTIN
%feature ("python:slot", "mp_subscript", functype="binaryfunc") trie::__getitem__;
%feature ("python:slot", "mp_ass_subscript", functype="objobjargproc") trie::__setitem__;
%feature ("python:slot", "tp_iter", functype="getiterfunc") trie_iterator::__iter__;
%feature ("python:slot", "tp_iternext", functype="iternextfunc") trie_iterator::next;
#endif

%exception trie::__getitem__ {
  try { $action }
  catch (const char* msg) { PyErr_SetString (PyExc_KeyError, msg); return NULL; }
}

%exception trie::__setitem__ {
  try { $action }
  catch (const char* msg) { PyErr_SetString (PyExc_KeyError, msg); return NULL; }
}

%extend trie {
  int  __getitem__ (const char* key) const {
    const int n = self->lookup (key);
    if (n == trie_t::CEDAR_NO_VALUE) throw key;
    return n;
  }
  void __setitem__ (const char* key, const int n) { self->insert (key, n); }
  void __setitem__ (const char* key) { if (self->erase (key) == -1) throw key; }
}

%exception trie_iterator::next {
  $action
  if (! result) {
    PyErr_SetNone (PyExc_StopIteration);
    return NULL;
  }
}
%extend trie_iterator {
  trie_iterator __iter__ () { return *self; }
}
#endif

#ifdef SWIGRUBY
%extend trie {
  VALUE __getitem__ (const char* key) const {
    const int n = self->lookup (key);
    return n == trie_t::CEDAR_NO_VALUE ? Qnil : LONG2FIX (n);
  }
  void  __setitem__ (const char* key, const int n) { self->insert (key, n); }
}
%extend trie_iterator {
  trie_iterator* each () {
    if (! rb_block_given_p ())
      rb_raise (rb_eArgError, "no block given");
    while (const result_t* r = self->next ())
      rb_yield (swig::from <const result_t*> (r));
    return self;
  }
 }
#endif

%include "trie.h"
