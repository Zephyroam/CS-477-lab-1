/**
 * BPFNicSkeleton.cpp
 * Author: Ethan Graham
 */
#include <bpfnic.skel.h>

/**
 * @brief RAII class for managing skeleton
 */
template <typename T>
class BPFNicSkeleton {
 private:
  T *skel;
  struct bpf_link *bpfLink;

 public:
  BPFNicSkeleton() : skel(nullptr), bpfLink(nullptr) {}

  ~BPFNicSkeleton() {
    if (skel) {
      T::destroy(skel);
      free(bpfLink);
    }
  }

  int open(const struct bpf_object_open_opts *opts = nullptr) {
    int err;

    if (skel) return -EBUSY;

    skel = T::open(opts);
    err = libbpf_get_error(skel);
    if (err) {
      skel = nullptr;
      return err;
    }

    return 0;
  }

  int load() { return T::load(skel); }

  int attach() { return T::attach(skel); }

  void detach() { return T::detach(skel); }

  const T *operator->() const { return skel; }

  T *operator->() { return skel; }

  const T *get() const { return skel; }
};
