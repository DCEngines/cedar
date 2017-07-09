#include <vector>
// #include <cedar.h>
// typedef size_t npos_t;
#include <cedarpp.h>
typedef cedar::npos_t npos_t;
typedef cedar::da <int>  trie_t;

// return type for prefix ()
class result_t {
public:
  result_t (const result_t& r) : _t (r._t), _id (r._id), _len (r._len), _val (r._val), _key (r._key) {}
  result_t (trie_t* t = 0, const npos_t id = 0, const size_t len = 0, const int val = 0) : _t (t), _id (id),  _len (len), _val (val), _key () {}
  ~result_t () {}
  void reset (const npos_t id, const size_t len, const int val)
  {_id = id; _len = len; _val = val; _key.clear (); }
  const char* key () {
    if (_key.empty ())
      { _key.resize (_len + 1); _t->suffix (&_key[0], _len, _id); }
    return &_key[0];
  }
  int value () const { return _val; }
protected:
  trie_t*  _t;
  npos_t  _id;
  size_t  _len;
  int     _val;
private:
  std::vector <char>  _key;
};

// return type (iterator) for predict ()
class trie_iterator : public result_t {
private:
  const npos_t _root;
  result_t     _ret;
public:
  trie_iterator (const trie_iterator& r) : result_t (r), _root (r._root), _ret (r._ret) {}
  trie_iterator (trie_t* t, const npos_t root = 0, const npos_t id = 0, const size_t len = 0, const int val = 0) : result_t (t, id, len, val), _root (root), _ret (t) {}
  ~trie_iterator () {}
  const result_t* next () {
    if (_val == trie_t::CEDAR_NO_PATH) return 0;
    _ret.reset (_id, _len, _val);
    _val = _t->next (_id, _len, _root);
    return &_ret;
  }
};

// interface for script languages; you may want to extend this
class trie {
private:
  trie_t* _t;
  size_t  _num_keys;
public:
  trie  () : _t (new cedar::da <int> ()), _num_keys (0) {}
  ~trie () { delete _t; }
  // read/write
  bool open (const char* fn) { return _t->open (fn, "rb") == 0; }
  bool save (const char* fn) { return _t->save (fn, "wb") == 0; }
  // get statistics
  size_t num_keys () const { return _num_keys; } // O(1)
  // low-level predicates
  int  insert (const char* key, int n = 0) {
    npos_t from = 0;
    size_t pos (0), len (std::strlen (key));
    const int n_ = _t->traverse (key, from, pos, len);
    bool flag = n_ == trie_t::CEDAR_NO_VALUE || n_ == trie_t::CEDAR_NO_PATH;
    if (flag) ++_num_keys;
    _t->update (key, from, pos, len) = n;
    return flag ? 0 : -1;
  }
  int  erase  (const char* key)
  { if (_t->erase (key)) return -1; else { --_num_keys; return 0; } }
  int  lookup (const char* key) const
  { return _t->exactMatchSearch <trie_t::result_type> (key); }
  // high-level (trie-specific) predicates
  std::vector <result_t> prefix (const char* key) const {
    std::vector <result_t> result;
    std::vector <trie_t::result_triple_type> result_;
    const size_t len = std::strlen (key);
    result_.resize (len);
    result.resize  (_t->commonPrefixSearch (key, &result_[0], len, len), _t);
    for (size_t i (0), n (result.size ()); i != n; ++i)
      result[i].reset (result_[i].id, result_[i].length, result_[i].value);
    return result;
  }
  result_t longest_prefix (const char* key) const {
    result_t r (_t, 0, 0, trie_t::CEDAR_NO_VALUE); // result for not found
    npos_t from = 0;
    for (size_t pos (0), len (std::strlen (key)); pos < len; ) {
      const int n = _t->traverse (key, from, pos, pos + 1);
      if (n == trie_t::CEDAR_NO_PATH)  break;
      if (n != trie_t::CEDAR_NO_VALUE) r.reset (from, pos, n);
    }
    return r;
  }
  trie_iterator predict (const char* key) {
    npos_t from = 0;
    size_t pos (0), len (0);
    int n = _t->traverse (key, from, pos);
    npos_t root = from;
    if (n != trie_t::CEDAR_NO_PATH)
      n = _t->begin (from, len);
    return trie_iterator (_t, root, from, len, n);
  }
};
