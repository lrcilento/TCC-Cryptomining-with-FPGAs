// Scoped pointer definitions.

#ifndef AOCL_UTILS_SCOPED_PTRS_H
#define AOCL_UTILS_SCOPED_PTRS_H

namespace aocl_utils {

// Interface is essentially the combination of std::auto_ptr and boost's smart pointers,
// along with some small extensions (auto conversion to T*).

// scoped_ptr: assumes pointer was allocated with operator new; destroys with operator delete
template<typename T>
class scoped_ptr {
public:
  typedef scoped_ptr<T> this_type;

  scoped_ptr() : m_ptr(NULL) {}
  scoped_ptr(T *ptr) : m_ptr(ptr) {}
  ~scoped_ptr() { reset(); }

  T *get() const { return m_ptr; }
  operator T *() const { return m_ptr; }
  T *operator ->() const { return m_ptr; }
  T &operator *() const { return *m_ptr; }

  this_type &operator =(T *ptr) { reset(ptr); return *this; }

  void reset(T *ptr = NULL) { delete m_ptr; m_ptr = ptr; }
  T *release() { T *ptr = m_ptr; m_ptr = NULL; return ptr; }

private:
  T *m_ptr;

  // noncopyable
  scoped_ptr(const this_type &);
  this_type &operator =(const this_type &);
};

// scoped_array: assumes pointer was allocated with operator new[]; destroys with operator delete[]
// Also supports allocation/reset with a number, which is the number of
// elements of type T.
template<typename T>
class scoped_array {
public:
  typedef scoped_array<T> this_type;

  scoped_array() : m_ptr(NULL) {}
  scoped_array(T *ptr) : m_ptr(NULL) { reset(ptr); }
  explicit scoped_array(size_t n) : m_ptr(NULL) { reset(n); }
  ~scoped_array() { reset(); }

  T *get() const { return m_ptr; }
  operator T *() const { return m_ptr; }
  T *operator ->() const { return m_ptr; }
  T &operator *() const { return *m_ptr; }
  T &operator [](int index) const { return m_ptr[index]; }

  this_type &operator =(T *ptr) { reset(ptr); return *this; }

  void reset(T *ptr = NULL) { delete[] m_ptr; m_ptr = ptr; }
  void reset(size_t n) { reset(new T[n]); }
  T *release() { T *ptr = m_ptr; m_ptr = NULL; return ptr; }

private:
  T *m_ptr;

  // noncopyable
  scoped_array(const this_type &);
  this_type &operator =(const this_type &);
};

// scoped_aligned_ptr: assumes pointer was allocated with alignedMalloc; destroys with alignedFree
// Also supports allocation/reset with a number, which is the number of
// elements of type T
template<typename T>
class scoped_aligned_ptr {
public:
  typedef scoped_aligned_ptr<T> this_type;

  scoped_aligned_ptr() : m_ptr(NULL) {}
  scoped_aligned_ptr(T *ptr) : m_ptr(NULL) { reset(ptr); }
  explicit scoped_aligned_ptr(size_t n) : m_ptr(NULL) { reset(n); }
  ~scoped_aligned_ptr() { reset(); }

  T *get() const { return m_ptr; }
  operator T *() const { return m_ptr; }
  T *operator ->() const { return m_ptr; }
  T &operator *() const { return *m_ptr; }
  T &operator [](int index) const { return m_ptr[index]; }

  this_type &operator =(T *ptr) { reset(ptr); return *this; }

  void reset(T *ptr = NULL) { if(m_ptr) alignedFree(m_ptr); m_ptr = ptr; }
  void reset(size_t n) { reset((T*) alignedMalloc(sizeof(T) * n)); }
  T *release() { T *ptr = m_ptr; m_ptr = NULL; return ptr; }

private:
  T *m_ptr;

  // noncopyable
  scoped_aligned_ptr(const this_type &);
  this_type &operator =(const this_type &);
};

#if USE_SVM_API == 1
// scoped_SVM_aligned_ptr: assumes pointer was allocated with clSVMAlloc; destroys with clSVMFree
// Also supports allocation/reset with a number, which is the number of
// elements of type T
template<typename T>
class scoped_SVM_aligned_ptr {
public:
	typedef scoped_SVM_aligned_ptr<T> this_type;

	scoped_SVM_aligned_ptr() : m_ptr(NULL) {}
	scoped_SVM_aligned_ptr(T *ptr) : m_ptr(NULL) { reset(ptr); }
	explicit scoped_SVM_aligned_ptr(cl_context ctx, size_t n) : m_ptr(NULL) { reset(ctx, n); }
	~scoped_SVM_aligned_ptr() { reset(); }

	T *get() const { return m_ptr; }
	operator T *() const { return m_ptr; }
	T *operator ->() const { return m_ptr; }
	T &operator *() const { return *m_ptr; }
	T &operator [](int index) const { return m_ptr[index]; }

	this_type &operator =(T *ptr) { reset(ptr); return *this; }

	void reset(T *ptr = NULL) { if (m_ptr) clSVMFree(m_ctx, m_ptr); m_ptr = ptr; }
	void reset(cl_context ctx, size_t n) { reset((T*)clSVMAlloc(ctx, 0, sizeof(T) * n, 0)); m_ctx = ctx; }
	T *release() { T *ptr = m_ptr; m_ptr = NULL; return ptr; }

private:
	T *m_ptr;
	cl_context m_ctx;

	// noncopyable
	scoped_SVM_aligned_ptr(const this_type &);
	this_type &operator =(const this_type &);
};
#endif /* USE_SVM_API == 1 */

} // ns aocl_utils

#endif